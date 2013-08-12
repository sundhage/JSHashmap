//
//  main.c
//  JSHashmap
//
//  Created by Johan Sundhage on 2013-08-06.
//  Copyright (c) 2013 Johan Sundhage. All rights reserved.
//

#include <stdio.h>
#include "JSHashmap.h"
#include <stdlib.h>


void releaser(void* obj) {
    printf("Should release: %p\n", obj);
    //free(obj);
}


int main(int argc, const char * argv[])
{
    jshash_t* jsh = jsh_create_num_buckets(kJSHASH_CSTRING, 16);
    jsh_set_value_for_key(jsh, "key1", "val1");
    jsh_set_value_for_key(jsh, "key2", "val2");
    jsh_set_value_for_key(jsh, "key3", "val3");
    jsh_set_value_for_key(jsh, "key4", "val4");
    jsh_set_value_for_key(jsh, "key5", "val5");
    
    const char* res = jsh_get_value_for_key(jsh, "key3");
    const char* res2 = jsh_get_value_for_key(jsh, "key6");
    if (res2 == NULL) printf("FUNKAR!\n");
    jsh_remove_value_for_key(jsh, "key3");
    const char* res3 = jsh_get_value_for_key(jsh, "key3");
    if (res3 == NULL) printf("FUNKAR IGEN\n");
    
    // insert code here...
    printf("%s\n", res);
    
    
    jsh_debug_diag(jsh);
    
    jsh_destroy(jsh, &releaser);
    
    return 0;
}

