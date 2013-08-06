//
//  JSHashmap.h
//  JSHashmap
//
//  Created by Johan Sundhage on 2013-08-06.
//  Copyright (c) 2013 Johan Sundhage. All rights reserved.
//

#ifndef JSHashmap_JSHashmap_h
#define JSHashmap_JSHashmap_h

typedef struct _JSHTableItem {
    void* obj;
    char* key;
    void* next;
} JSHTableItem;

typedef struct _JSHObject {
    JSHTableItem* buckets;
    unsigned int bucketSize;
} JSHObject;

/*
 Creates a new hashmap object.
 @param bucketSize Number of buckets.
*/
extern JSHObject* JSHCreate(unsigned int bucketSize);

/*
 Destroys the hashmap object. 
 TODO: add value removal callback
*/
extern void JSHDestroy(JSHObject* obj);

/*
 Sets a new value.
 @param obj Hashmap object
 @param key key
 @param value object
 @return old object (or NULL if key was empty)
*/
extern void* JSHSetValueForKey(JSHObject* obj, const char* key, void* value);

/*
 Returns a value.
 @param obj Hashmap object
 @param key key
 @return Value object or NULL (if key is not set)
*/
extern void* JSHGetValueForKey(JSHObject* obj, const char* key);

/*
 Removes a value from hashmap
 @param obj Hashmap object
 @param key key
 @return the value object or NULL
*/
extern void* JSHRemoveValueForKey(JSHObject* obj, const char* key);

/*
 Checks and prints all bucket sizes.
*/
extern void JSHDebugDiag(JSHObject* obj);

#endif
