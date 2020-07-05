#include "SkStream.h"
#include "SkXMLWriter.h"
#include "SkCanvas.h"
#include "SkPaint.h"
#include "SkSVGCanvas.h"

void drawEmptyCustomElement(SkCanvas *svgCanvas, const SkPaint& paint);
void drawEmptyCustomWithAttrs(SkCanvas* svgCanvas, const SkPaint& paint);

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
    attr->attrName = "TestName";
    attr->attrData = "TestValue";

    return attr;
}

void drawEmptyCustomWithAttrs(SkCanvas* svgCanvas, const SkPaint& paint) {
    auto custom = SkCustomElement();
    custom.body = "Sample";
    custom.nextChild = NULL;
    custom.attrsLength = 3;
    custom.attrs = new SkAttr[3]
    {
        *getRandomAttr(),
        *getRandomAttr(),
        *getRandomAttr()
    };

    svgCanvas->drawCustomElement(custom, paint);
}