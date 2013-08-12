//
//  JSHashmap.c
//  JSHashmap
//
//  Created by Johan Sundhage on 2013-08-06.
//  Copyright (c) 2013 Johan Sundhage. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "JSHashmap.h"

/*
 
 
 
    hash functions

 
 
*/

int _jsh_hash_for_key_cstring(unsigned int bucket_size, void* key) {
	
	int len = (int)strlen((const char*)key);
	
    unsigned int h = 2166136261;
    for (int i = 0; i < len; i++ )
		h = ( h * 16777619 ) ^ ((const char*)key)[i];
	
	return h%bucket_size;
	
}

int _jsh_hash_for_key_ptr(unsigned int bucket_size, void* key) {
	
    int h = ((int)key)/(sizeof(void*));
	return h%bucket_size;
	
}

jsh_table_item* _jsh_table_item_for_hash_cstring(jshash_t* obj, unsigned int hid, void* key) {
	jsh_table_item* item = &obj->buckets[hid];
	while (item->key != NULL) {
		if (strcmp((const char*)item->key, key) == 0) {
			return item;
		}
		item = item->next;
	}
	return NULL;
}

jsh_table_item* _jsh_table_item_for_hash_ptr(jshash_t* obj, unsigned int hid, void* key) {
	jsh_table_item* item = &obj->buckets[hid];
	while (item->key != NULL) {
        if (item->key == key) return item;
		item = item->next;
	}
	return NULL;
}

void _setup_hash_funcs(jshash_t* obj, enum JSHASH_KEY_TYPE type) {
    obj->type = type;
    if (type == kJSHASH_CSTRING) {
        obj->hash_func = &_jsh_hash_for_key_cstring;
        obj->get_item_func = &_jsh_table_item_for_hash_cstring;
    } else {
        obj->hash_func = &_jsh_hash_for_key_ptr;
        obj->get_item_func = &_jsh_table_item_for_hash_ptr;
    }
    
}


/*
 
 
 
 API
 
 
 
*/


extern jshash_t* jsh_create(enum JSHASH_KEY_TYPE type) {
    jshash_t* obj = malloc(sizeof(jshash_t));
    _setup_hash_funcs(obj, type);
    
    obj->bucket_size = JSHASH_DEFAULT_BUCKET_SIZE;
	obj->buckets = (jsh_table_item*)calloc(JSHASH_DEFAULT_BUCKET_SIZE, sizeof(jsh_table_item));
    return obj;
}

extern jshash_t* jsh_create_num_buckets(enum JSHASH_KEY_TYPE type, unsigned int bucket_size) {
    jshash_t* obj = malloc(sizeof(jshash_t));
    _setup_hash_funcs(obj, type);
    
    obj->bucket_size = bucket_size;
	obj->buckets = (jsh_table_item*)calloc(bucket_size, sizeof(jsh_table_item));
    return obj;
}

void jsh_destroy(jshash_t* obj, void (*value_destroyer_func)(void*)) {
    int i;
	for (i=0; i<obj->bucket_size; i++) {
		if (obj->buckets[i].key) {
            if (obj->type == kJSHASH_CSTRING)
                free(obj->buckets[i].key);
            if (value_destroyer_func) value_destroyer_func(obj->buckets[i].obj);
		}
		
		jsh_table_item* item = (jsh_table_item*)obj->buckets[i].next;
		while (item) {
			if (item->key) {
                if (obj->type == kJSHASH_CSTRING)
                    free(item->key);
                if (value_destroyer_func) value_destroyer_func(item->obj);
			}
			jsh_table_item* temp = item;
			item = (jsh_table_item*)item->next;
			free((void*)temp);
		}
	}
	free((void*)obj->buckets);
    free((void*)obj);
	
}

void* jsh_set_value_for_key(jshash_t* obj, const char* key, void* value) {
 	int hid = obj->hash_func(obj->bucket_size, (void*)key);
	
	jsh_table_item* item;
	item = obj->get_item_func(obj, hid, (void*)key);
	if (item) {
		void* oldObj = item->obj;
		item->obj = value;
		return oldObj;
	}
	
	item = &obj->buckets[hid];
	while (item->next) item = item->next;
	item->obj = value;
	size_t len = strlen(key);
	
    if (obj->type == kJSHASH_CSTRING) {
        item->key = (char*)malloc(len+1);
        strncpy(item->key, key, len);
        item->key[len] = '\0';
	} else {
        item->key = (char*)key;
    }
    
	item->next = (jsh_table_item*)(malloc(sizeof(jsh_table_item)));
    jsh_table_item* nx = item->next;
    nx->key = NULL;
    nx->obj = NULL;
    nx->next = NULL;
	return NULL;
}

void* jsh_get_value_for_key(jshash_t* obj, const char* key) {
	int hid = obj->hash_func(obj->bucket_size, (void*)key);
	
	jsh_table_item* item = obj->get_item_func(obj, hid, (void*)key);
	if (item) return item->obj;
	return NULL;
    
}

void* jsh_remove_value_for_key(jshash_t* obj, const char* key) {
	int hid = obj->hash_func(obj->bucket_size, (void*)key);
	jsh_table_item* item = obj->get_item_func(obj, hid, (void*)key);
	if (item == NULL) return NULL;
	
	if (item == &obj->buckets[hid]) {
        if (obj->type == kJSHASH_CSTRING)
            free(obj->buckets[hid].key);
		void* oldValue = obj->buckets[hid].obj;
        jsh_table_item* nx = item->next;
		obj->buckets[hid] = *nx;
		
		return oldValue;
	} else {
		jsh_table_item* root = &obj->buckets[hid];
		while (root) {
			if (root->next == item) {
				root->next = item->next;
                if (obj->type == kJSHASH_CSTRING)
                    free(item->key);
				void* oldValue = item->obj;
				free((void*) item);
				return oldValue;
			}
			root = root->next;
		}
	}
	return NULL;
}


void jsh_debug_diag(jshash_t* obj) {
	for (int i=0; i<obj->bucket_size; i++) {
		int c = 0;
		jsh_table_item* current = &obj->buckets[i];
		
		while (current->next) {
			c++;
			current = current->next;
		}
		printf("bucket: %i, count: %i\n", i, c);
	}
}

