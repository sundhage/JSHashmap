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

JSHObject* JSHCreate(unsigned int bucketSize) {
    
    char* o = malloc(sizeof(JSHObject));
    
    JSHObject* obj = (JSHObject*)o;
    
    obj->bucketSize = bucketSize;
	obj->buckets = (JSHTableItem*)malloc(sizeof(JSHTableItem)*bucketSize);
	int i;
	for (i=0; i<bucketSize; i++) {
		obj->buckets[i].key = NULL;
		obj->buckets[i].obj = NULL;
		obj->buckets[i].next = NULL;
	}
    
    return obj;
}

void JSHDestroy(JSHObject* obj) {
    int i;
	for (i=0; i<obj->bucketSize; i++) {
		if (obj->buckets[i].key) {
			free(obj->buckets[i].key);
			//onRemoval(buckets[i].obj);
		}
		
		JSHTableItem* item = (JSHTableItem*)obj->buckets[i].next;
		while (item) {
			if (item->key) {
				free(item->key);
				//onRemoval(item->obj);
			}
			JSHTableItem* temp = item;
			item = (JSHTableItem*)item->next;
			free((void*)temp);
		}
	}
	free((void*)obj->buckets);
    free((void*)obj);
	
}

// private

int _JSHHashForKey(unsigned int bucketSize, const char* key) {
	
	int len = (int)strlen(key);
	
    unsigned int h = 2166136261;
    for (int i = 0; i < len; i++ )
		h = ( h * 16777619 ) ^ key[i];
	
	return h%bucketSize;
	
}

JSHTableItem* _JSHTableItemForHash(JSHObject* obj, int hid, const char* key) {
	JSHTableItem* item = &obj->buckets[hid];
	while (item->key != NULL) {
		if (strcmp((const char*)item->key, key) == 0) {
			return item;
		}
		item = item->next;
	}
	return NULL;
}

// API

void* JSHSetValueForKey(JSHObject* obj, const char* key, void* value) {
 	int hid = _JSHHashForKey(obj->bucketSize, key);
	
	JSHTableItem* item;
	item = _JSHTableItemForHash(obj, hid, key);
	if (item) {
		void* oldObj = item->obj;
		item->obj = value;
		return oldObj;
	}
	
	item = &obj->buckets[hid];
	while (item->next) item = item->next;
	item->obj = value;
	size_t len = strlen(key);
	
	item->key = (char*)malloc(len+1);
	strncpy(item->key, key, len);
	item->key[len] = '\0';
	
	item->next = (JSHTableItem*)(malloc(sizeof(JSHTableItem)));
    JSHTableItem* nx = item->next;
    nx->key = NULL;
    nx->obj = NULL;
    nx->next = NULL;
	return NULL;
}

void* JSHGetValueForKey(JSHObject* obj, const char* key) {
	int hid = _JSHHashForKey(obj->bucketSize, key);
	
	JSHTableItem* item = _JSHTableItemForHash(obj, hid, key);
	if (item) return item->obj;
	return NULL;
    
}

void* JSHRemoveValueForKey(JSHObject* obj, const char* key) {
	int hid = _JSHHashForKey(obj->bucketSize, key);
	JSHTableItem* item = _JSHTableItemForHash(obj, hid, key);
	if (item == NULL) return NULL;
	
	if (item == &obj->buckets[hid]) {
        
		free(obj->buckets[hid].key);
		void* oldValue = obj->buckets[hid].obj;
        JSHTableItem* nx = item->next;
		obj->buckets[hid] = *nx;
		
		return oldValue;
	} else {
		JSHTableItem* root = &obj->buckets[hid];
		while (root) {
			if (root->next == item) {
				root->next = item->next;
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
void JSHDebugDiag(JSHObject* obj) {
	for (int i=0; i<obj->bucketSize; i++) {
		int c = 0;
		JSHTableItem* current = &obj->buckets[i];
		
		while (current->next) {
			c++;
			current = current->next;
		}
		printf("bucket: %i, count: %i\n", i, c);
	}
}

