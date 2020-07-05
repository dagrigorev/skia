#ifndef SkCustomElement_DEFINED
#define SkCustomElement_DEFINED

#include "SkTypes.h"
#include <string>

/*
 * Simple skia attribute
 */
struct SK_API SkAttr {
    std::string attrName;
    std::string attrData;
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