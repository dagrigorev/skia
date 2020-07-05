#include "SkStream.h"
#include "SkXMLWriter.h"
#include "SkCanvas.h"
#include "SkPaint.h"
#include "SkSVGCanvas.h"

void drawEmptyCustomElement(SkCanvas *svgCanvas, const SkPaint& paint);
void drawEmptyCustomWithAttrs(SkCanvas* svgCanvas, const SkPaint& paint);
void drawCustomWithAttrs(SkCanvas* svgCanvas, const SkPaint& paint);

int main(void) {
    SkFILEWStream svgStream("test.svg");
    std::unique_ptr<SkXMLWriter> xmlWriter(new SkXMLStreamWriter(&svgStream));
    SkRect bounds = SkRect::MakeIWH(500, 500);
    std::unique_ptr<SkCanvas> svgCanvas = SkSVGCanvas::Make(bounds, xmlWriter.get());
    auto paint = SkPaint();
    paint.setAntiAlias(true);
    paint.setColor(SkColorSetRGB(255, 0, 0));
    paint.setStyle(SkPaint::kFill_Style);
    paint.setStrokeWidth(2);

    svgCanvas->drawRect(SkRect::MakeWH(100, 100), paint);
    svgCanvas->drawCircle(100, 100, 10, paint);
    svgCanvas->drawCircleWA(200, 200, 50, paint, "hello", "world");

    drawEmptyCustomElement(svgCanvas.get(), paint);
    drawEmptyCustomWithAttrs(svgCanvas.get(), paint);
    drawCustomWithAttrs(svgCanvas.get(), paint);

    return 0;
}

void drawEmptyCustomElement(SkCanvas *svgCanvas, const SkPaint& paint) {
    auto custom = SkCustomElement();
    custom.body = "Sample";
    custom.nextChild = NULL;
    custom.attrs = NULL;
    svgCanvas->drawCustomElement(custom, paint);
}

SkAttr* getRandomAttr() {
    auto attr = new SkAttr();
    attr->jsonData = "{attr1: value1, attr2: value2, attrs3: 12312}";

    return attr;
}

void drawEmptyCustomWithAttrs(SkCanvas* svgCanvas, const SkPaint& paint) {
    auto custom = SkCustomElement();
    custom.body = "Sample";
    custom.nextChild = NULL;
    custom.attrsLength = 3;
    custom.attrs = new SkAttr[3];

    for (int i=0; i < custom.attrsLength; i++) {
        custom.attrs[i] = SkAttr{"{attr1: value1, attr2: value2, attrs3: 12312}"};
    }
    svgCanvas->drawCustomElement(custom, paint);
}

SkCustomElement* getCustom() {
    auto custom = new SkCustomElement();
    custom->body = "Sample";
    custom->nextChild = NULL;
    custom->attrsLength = 3;
    custom->attrs = new SkAttr[3];
    //{*getRandomAttr(), *getRandomAttr(), *getRandomAttr()};
    for (int i = 0; i < custom->attrsLength; i++) {
        custom->attrs[i] = SkAttr{"{attr1: value1, attr2: value2, attrs3: 12312}"};
    }
    return custom;
}

void drawCustomWithAttrs(SkCanvas* svgCanvas, const SkPaint& paint) {
    auto root = getCustom();
    root->nextChild = getCustom();
    root->nextChild->nextChild = getCustom();

    svgCanvas->drawCustomElement(*root, paint);
}
