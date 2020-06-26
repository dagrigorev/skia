#ifndef SkCustomElement_DEFINED
#define SkCustomElement_DEFINED

#include "SkTypes.h"

/*
 * Simple skia attribute
 */
struct SK_API SkAttr {
    char *name;
    char *value;
};

/*
 *  The custom element definition
 */
struct SK_API SkCustomElement {
    char *body;
    int attrsLength;
    SkAttr* attrs;
    SkCustomElement* root;
    SkCustomElement* nextChild;
};

#endif