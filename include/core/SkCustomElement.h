#ifndef SkCustomElement_DEFINED
#define SkCustomElement_DEFINED

#include <SkTypes.h>

/*
 * Simple skia attribute
 */
struct SK_API SkAttr {
    char name[20];
    char value[255];
};

/*
 *  The custom elemnt definition
 */
struct SK_API SkCustomElement {
    char name[20];
    SkAttr *attrs;
};

#endif