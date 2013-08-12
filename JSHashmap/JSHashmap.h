//
//  JSHashmap.h
//  JSHashmap
//
//  Created by Johan Sundhage on 2013-08-06.
//  Copyright (c) 2013 Johan Sundhage. All rights reserved.
//

#ifndef JSHashmap_JSHashmap_h
#define JSHashmap_JSHashmap_h


#define JSHASH_DEFAULT_BUCKET_SIZE 512


enum JSHASH_KEY_TYPE {
    kJSHASH_CSTRING,
    kJSHASH_POINTER
};

typedef struct jsh_table_item {
    void* obj;
    char* key;
    void* next;
} jsh_table_item;




typedef struct jshash_t {
    
    enum JSHASH_KEY_TYPE type;
    
    int (*hash_func)(unsigned int, void*);
    jsh_table_item* (*get_item_func)(struct jshash_t*, unsigned int, void*);
    
    jsh_table_item* buckets;
    unsigned int bucket_size;
} jshash_t;



/*
 Creates a new hashmap object.
 @param bucket_size Number of buckets.
*/
extern jshash_t* jsh_create_num_buckets(enum JSHASH_KEY_TYPE type, unsigned int bucket_size);
extern jshash_t* jsh_create(enum JSHASH_KEY_TYPE type);



/*
 Destroys the hashmap object. 
 @param obj Hashmap object
 @param value_destroyer_func Pointer to function that handles value destroying
*/
extern void jsh_destroy(jshash_t* obj, void (*value_destroyer_func)(void*));

/*
 Sets a new value.
 @param obj Hashmap object
 @param key key
 @param value object
 @return old object (or NULL if key was empty)
*/
extern void* jsh_set_value_for_key(jshash_t* obj, const char* key, void* value);

/*
 Returns a value.
 @param obj Hashmap object
 @param key key
 @return Value object or NULL (if key is not set)
*/
extern void* jsh_get_value_for_key(jshash_t* obj, const char* key);

/*
 Removes a value from hashmap
 @param obj Hashmap object
 @param key key
 @return the value object or NULL
*/
extern void* jsh_remove_value_for_key(jshash_t* obj, const char* key);

/*
 Checks and prints all bucket sizes.
*/
extern void jsh_debug_diag(jshash_t* obj);

#endif
