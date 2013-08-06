//
//  main.c
//  JSHashmap
//
//  Created by Johan Sundhage on 2013-08-06.
//  Copyright (c) 2013 Johan Sundhage. All rights reserved.
//

#include <stdio.h>
#include "JSHashmap.h"

int main(int argc, const char * argv[])
{
    JSHObject* jsh = JSHCreate(1024);
    JSHSetValueForKey(jsh, "key1", "val1");
    JSHSetValueForKey(jsh, "key2", "val2");
    JSHSetValueForKey(jsh, "key3", "val3");
    JSHSetValueForKey(jsh, "key4", "val4");
    JSHSetValueForKey(jsh, "key5", "val5");
    
    const char* res = JSHGetValueForKey(jsh, "key3");
    const char* res2 = JSHGetValueForKey(jsh, "key6");
    if (res2 == NULL) printf("FUNKAR!\n");
    JSHRemoveValueForKey(jsh, "key3");
    const char* res3 = JSHGetValueForKey(jsh, "key3");
    if (res3 == NULL) printf("FUNKAR IGEN\n");
    
    // insert code here...
    printf("%s\n", res);
    
    
    JSHDebugDiag(jsh);
    
    return 0;
}

