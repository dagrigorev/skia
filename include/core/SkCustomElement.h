#ifndef SkCustomElement_DEFINED
#define SkCustomElement_DEFINED

#include "SkTypes.h"

/*
 * Simple skia attribute
 */
struct SK_API SkAttr {
    const char attrName[50];
    const char attrData[255];
};

/*
 *  The custom element definition
 */
struct SK_API SkCustomElement {
    const char *body;
    int attrsLength;
    struct SkAttr* attrs;
    struct SkCustomElement* nextChild;
};

#endif