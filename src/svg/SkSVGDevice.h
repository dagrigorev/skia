/*
 * Copyright 2015 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef SkSVGDevice_DEFINED
#define SkSVGDevice_DEFINED

#include "SkClipStackDevice.h"
#include "SkTemplates.h"
#include "SkCustomElement.h"

class SkXMLWriter;

class SkSVGDevice : public SkClipStackDevice {
public:
    static SkBaseDevice* Create(const SkISize& size, SkXMLWriter* writer);

protected:
    void drawPaint(const SkPaint& paint) override;
    void drawPaintWA(const SkPaint& paint, const char* attrName, const char* attrVal) override;
    
    void drawCustomElement(const SkCustomElement &element, const SkPaint &paint) override;
    
    void drawAnnotation(const SkRect& rect, const char key[], SkData* value) override;
    void drawPoints(SkCanvas::PointMode mode, size_t count,
                    const SkPoint[], const SkPaint& paint) override;
    void drawPointsWA(SkCanvas::PointMode mode, size_t count,
                    const SkPoint[], const SkPaint& paint,
                    const char* attrName, const char* attrVal) override;
    
    void drawRect(const SkRect& r, const SkPaint& paint) override;
    void drawRectWA(const SkRect& r, const SkPaint& paint, const char* attrName, const char* attrVal) override;
    void drawOval(const SkRect& oval, const SkPaint& paint) override;
    void drawOvalWA(const SkRect& oval,
                          const SkPaint& paint,
                          const char* attrName,
                          const char* attrVal) override;
    void drawRRect(const SkRRect& rr, const SkPaint& paint) override;
    void drawRRectWA(const SkRRect& rr,
                           const SkPaint& paint, 
                           const char* attrName, 
                           const char* atrtVal) override;
    void drawPath(const SkPath& path,
                  const SkPaint& paint,
                  const SkMatrix* prePathMatrix = nullptr,
                  bool pathIsMutable = false) override;
    void drawPathWA(const SkPath& path,
                          const SkPaint& paint,
                          const char* attrName, 
                          const char* attrVal,
                          const SkMatrix* prePathMatrix = nullptr,
                          bool pathIsMutable = false) override;

    void drawBitmap(const SkBitmap& bitmap, SkScalar x, SkScalar y, const SkPaint& paint) override;
    void drawBitmapWA(const SkBitmap&, SkScalar x, SkScalar y, const SkPaint&, const char* attrName, const char* attrVal) override;
    void drawSprite(const SkBitmap& bitmap,
                    int x, int y, const SkPaint& paint) override;
    void drawSpriteWA(const SkBitmap& bitmap,
                            int x, int y, const SkPaint& paint, 
                            const char* attrName, const char* attrVal) override;
    void drawBitmapRect(const SkBitmap&,
                        const SkRect* srcOrNull, const SkRect& dst,
                        const SkPaint& paint, SkCanvas::SrcRectConstraint) override;
    void drawBitmapRectWA(const SkBitmap&,
                                const SkRect* srcOrNull, const SkRect& dst,
                                const SkPaint& paint,
                                const char* attrName, const char* atrtVal,
                                SkCanvas::SrcRectConstraint) override;
    void drawText(const void* text, size_t len,
                  SkScalar x, SkScalar y, const SkPaint& paint) override;
    void drawTextWA(const void* text, size_t len,
                          SkScalar x, SkScalar y, const SkPaint& paint, 
                          const char* attrName, const char* attrVal) override;
    void drawPosText(const void* text, size_t len,
                     const SkScalar pos[], int scalarsPerPos,
                     const SkPoint& offset, const SkPaint& paint) override;
    void drawTextOnPath(const void* text, size_t len,
                        const SkPath& path, const SkMatrix* matrix,
                        const SkPaint& paint) override;
    void drawVertices(const SkVertices*, SkBlendMode, const SkPaint& paint) override;
    void drawVerticesWA(const SkVertices*, SkBlendMode, const SkPaint&, const char* attrName, const char* attrVal) override;
    void drawDevice(SkBaseDevice*, int x, int y,
                    const SkPaint&) override;
    void drawSpecialWA(SkSpecialImage *image, int x, int y, const SkPaint &paint,
                             SkImage* clipImage, const SkMatrix& clipMatrix,
                             const char* attrName, const char* attrVal) override {
        drawSpecial(image, x, y, paint, clipImage, clipMatrix);
    }
    void drawRegionWA(const SkRegion& r,
                            const SkPaint& paint,
                            const char* attrName,
                            const char* attrVal) override {
        drawRegion(r, paint);
    }

private:
    SkSVGDevice(const SkISize& size, SkXMLWriter* writer);
    ~SkSVGDevice() override;

    struct MxCp;
    void drawBitmapCommon(const MxCp&, const SkBitmap& bm, const SkPaint& paint);

    class AutoElement;
    class ResourceBucket;

    SkXMLWriter*                    fWriter;
    std::unique_ptr<AutoElement>    fRootElement;
    std::unique_ptr<ResourceBucket> fResourceBucket;

    typedef SkClipStackDevice INHERITED;
};

#endif // SkSVGDevice_DEFINED
