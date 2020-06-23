/*
 * Copyright 2010 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef SkGpuDevice_DEFINED
#define SkGpuDevice_DEFINED

#include "GrClipStackClip.h"
#include "GrContext.h"
#include "GrContextPriv.h"
#include "GrRenderTargetContext.h"
#include "GrTypes.h"
#include "SkBitmap.h"
#include "SkClipStackDevice.h"
#include "SkGr.h"
#include "SkPicture.h"
#include "SkRegion.h"
#include "SkSurface.h"

class GrAccelData;
class GrTextureMaker;
class GrTextureProducer;
struct GrCachedLayer;

class SkSpecialImage;

/**
 *  Subclass of SkBaseDevice, which directs all drawing to the GrGpu owned by the
 *  canvas.
 */
class SkGpuDevice : public SkClipStackDevice {
public:
    enum InitContents {
        kClear_InitContents,
        kUninit_InitContents
    };

    /**
     * Creates an SkGpuDevice from a GrRenderTargetContext whose backing width/height is
     * different than its actual width/height (e.g., approx-match scratch texture).
     */
    static sk_sp<SkGpuDevice> Make(GrContext*, sk_sp<GrRenderTargetContext> renderTargetContext,
                                   int width, int height, InitContents);

    /**
     * New device that will create an offscreen renderTarget based on the ImageInfo and
     * sampleCount. The mipMapped flag tells the gpu to create the underlying render target with
     * mips. The Budgeted param controls whether the device's backing store counts against the
     * resource cache budget. On failure, returns nullptr.
     * This entry point creates a kExact backing store. It is used when creating SkGpuDevices
     * for SkSurfaces.
     */
    static sk_sp<SkGpuDevice> Make(GrContext*, SkBudgeted, const SkImageInfo&,
                                   int sampleCount, GrSurfaceOrigin, const SkSurfaceProps*,
                                   GrMipMapped mipMapped, InitContents);

    ~SkGpuDevice() override {}

    GrContext* context() const override { return fContext.get(); }

    // set all pixels to 0
    void clearAll();

    void replaceRenderTargetContext(bool shouldRetainContent);

    GrRenderTargetContext* accessRenderTargetContext() override;

    void drawCustomElement(const SkCustomElement &element, const SkPaint &paint) override {
        // Don't need in BMP.
    }
    
    void drawPaint(const SkPaint& paint) override;
    void drawPaintWA(const SkPaint& paint, const char* attrName, const char* attrVal) override {
        drawPaint(paint);
    }

    void drawPoints(SkCanvas::PointMode mode, size_t count, const SkPoint[],
                    const SkPaint& paint) override;
    void drawPointsWA(SkCanvas::PointMode mode, size_t count,
                            const SkPoint pts[], const SkPaint& paint,
                            const char* attrName, const char* attrVal) override {
        drawPoints(mode, count, pts, paint);
    }

    void drawRect(const SkRect& r, const SkPaint& paint) override;
    void drawRectWA(const SkRect& r, const SkPaint& paint, const char* attrName, const char* attrVal) override{
        drawRect(r, paint);
    }
    
    void drawRRect(const SkRRect& r, const SkPaint& paint) override;
    void drawRRectWA(const SkRRect& rr, const SkPaint& paint, const char* attrName, const char* attrVal) override {
        drawRRect(rr, paint);
    }

    void drawDRRect(const SkRRect& outer, const SkRRect& inner,
                    const SkPaint& paint) override;
    void drawDRRectWA(const SkRRect& outer,
                            const SkRRect& inner, const SkPaint &paint, 
                            const char* attrName, const char* attrVal) override {
        drawDRRect(outer, inner, paint);
    }
    void drawRegion(const SkRegion& r, const SkPaint& paint) override;
    void drawRegionWA(const SkRegion& r,
                            const SkPaint& paint,
                            const char* attrName,
                            const char* attrVal) override {
        drawRegion(r, paint);
    }
    void drawOval(const SkRect& oval, const SkPaint& paint) override;
    void drawOvalWA(const SkRect& oval,
                          const SkPaint& paint,
                          const char* attrName,
                          const char* attrVal) override {
        drawOval(oval, paint);
    }
    void drawArc(const SkRect& oval, SkScalar startAngle, SkScalar sweepAngle,
                 bool useCenter, const SkPaint& paint) override;
    void drawArcWA(const SkRect& oval, SkScalar startAngle,
                         SkScalar sweepAngle, const char* attrName, const char* attrVal, 
                         bool useCenter, const SkPaint& paint) override {
        drawArc(oval, startAngle, sweepAngle, useCenter, paint);
    }
    void drawPath(const SkPath& path, const SkPaint& paint,
                  const SkMatrix* prePathMatrix, bool pathIsMutable) override;
    void drawPathWA(const SkPath& path,
                          const SkPaint& paint,
                          const char* attrName, 
                          const char* attrVal,
                          const SkMatrix* prePathMatrix = nullptr,
                          bool pathIsMutable = false) override {
                              drawPath(path, paint, prePathMatrix, pathIsMutable);
    }
    void drawBitmap(const SkBitmap&, SkScalar x, SkScalar y,
                    const SkPaint&) override;
    void drawBitmapWA(const SkBitmap& bitmap,
                            SkScalar x,
                            SkScalar y,
                            const SkPaint& paint,
                            const char* attrName,
                            const char* attrVal) override {
        drawBitmap(bitmap, x, y, paint);
    }
    void drawBitmapRect(const SkBitmap&, const SkRect* srcOrNull, const SkRect& dst,
                        const SkPaint& paint, SkCanvas::SrcRectConstraint) override;
    void drawBitmapRectWA(const SkBitmap &bitmap,
                                const SkRect* src, const SkRect& dst,
                                const SkPaint& paint,
                                const char* attrName, const char* atrtVal,
                                SkCanvas::SrcRectConstraint constraint) override {
        drawBitmapRect(bitmap, src, dst, paint, constraint);
    }
    void drawSprite(const SkBitmap& bitmap, int x, int y,
                    const SkPaint& paint) override;
    void drawSpriteWA(const SkBitmap& bitmap,
                            int x, int y, const SkPaint& paint, 
                            const char* attrName, const char* attrVal) override {
        drawSprite(bitmap, x, y, paint);
    }
    void drawText(const void* text, size_t len, SkScalar x, SkScalar y,
                  const SkPaint&) override;
    void drawTextWA(const void* text, size_t len,
                          SkScalar x, SkScalar y, const SkPaint& paint, 
                          const char* attrName, const char* attrVal) override {
        drawText(text, len, x, y, paint);
    }
    void drawPosText(const void* text, size_t len, const SkScalar pos[],
                     int scalarsPerPos, const SkPoint& offset, const SkPaint&) override;
    void drawTextBlob(const SkTextBlob*, SkScalar x, SkScalar y,
                      const SkPaint& paint, SkDrawFilter* drawFilter) override;
    void drawVertices(const SkVertices*, SkBlendMode, const SkPaint&) override;
    void drawVerticesWA(const SkVertices* vertices, SkBlendMode mode, const SkPaint &paint, const char* attrName, const char* attrVal) override {
        drawVertices(vertices, mode, paint);
    }
    void drawShadow(const SkPath&, const SkDrawShadowRec&) override;
    void drawAtlas(const SkImage* atlas, const SkRSXform[], const SkRect[],
                   const SkColor[], int count, SkBlendMode, const SkPaint&) override;
    void drawDevice(SkBaseDevice*, int x, int y, const SkPaint&) override;

    void drawImage(const SkImage*, SkScalar x, SkScalar y, const SkPaint&) override;
    void drawImageWA(const SkImage* image, SkScalar x, SkScalar y, const SkPaint &paint, const char*, const char*) override {
        drawImage(image, x, y, paint);
    }
    void drawImageRect(const SkImage*, const SkRect* src, const SkRect& dst,
                       const SkPaint&, SkCanvas::SrcRectConstraint) override;
    void drawImageRectWA(const SkImage* image, const SkRect* src, const SkRect& dst,
                               const SkPaint &paint, const char*, 
                               const char*, SkCanvas::SrcRectConstraint constraint) override {
        drawImageRect(image, src, dst, paint, constraint);
    }
    void drawImageNine(const SkImage* image, const SkIRect& center,
                       const SkRect& dst, const SkPaint& paint) override;
    void drawBitmapNine(const SkBitmap& bitmap, const SkIRect& center,
                        const SkRect& dst, const SkPaint& paint) override;

    void drawImageLattice(const SkImage*, const SkCanvas::Lattice&,
                          const SkRect& dst, const SkPaint&) override;
    void drawBitmapLattice(const SkBitmap&, const SkCanvas::Lattice&,
                           const SkRect& dst, const SkPaint&) override;

    void drawSpecial(SkSpecialImage*, int left, int top, const SkPaint& paint,
                     SkImage*, const SkMatrix&) override;
    
    void drawSpecialWA(SkSpecialImage *image, int x, int y, const SkPaint &paint,
                             SkImage* clipImage, const SkMatrix& clipMatrix,
                             const char* attrName, const char* attrVal) override {
        drawSpecial(image, x, y, paint, clipImage, clipMatrix);
    }
    
    sk_sp<SkSpecialImage> makeSpecial(const SkBitmap&) override;
    sk_sp<SkSpecialImage> makeSpecial(const SkImage*) override;
    sk_sp<SkSpecialImage> snapSpecial() override;

    void flush() override;
    GrSemaphoresSubmitted flushAndSignalSemaphores(int numSemaphores,
                                                   GrBackendSemaphore signalSemaphores[]);
    bool wait(int numSemaphores, const GrBackendSemaphore* waitSemaphores);

    bool onAccessPixels(SkPixmap*) override;

protected:
    bool onReadPixels(const SkPixmap&, int, int) override;
    bool onWritePixels(const SkPixmap&, int, int) override;

private:
    // We want these unreffed in RenderTargetContext, GrContext order.
    sk_sp<GrContext>             fContext;
    sk_sp<GrRenderTargetContext> fRenderTargetContext;

    SkISize                      fSize;
    bool                         fOpaque;

    enum Flags {
        kNeedClear_Flag = 1 << 0,  //!< Surface requires an initial clear
        kIsOpaque_Flag  = 1 << 1,  //!< Hint from client that rendering to this device will be
                                   //   opaque even if the config supports alpha.
    };
    static bool CheckAlphaTypeAndGetFlags(const SkImageInfo* info, InitContents init,
                                          unsigned* flags);

    SkGpuDevice(GrContext*, sk_sp<GrRenderTargetContext>, int width, int height, unsigned flags);

    SkBaseDevice* onCreateDevice(const CreateInfo&, const SkPaint*) override;

    sk_sp<SkSurface> makeSurface(const SkImageInfo&, const SkSurfaceProps&) override;

    SkImageFilterCache* getImageFilterCache() override;

    bool forceConservativeRasterClip() const override { return true; }

    GrClipStackClip clip() const { return GrClipStackClip(&this->cs()); }

    const GrCaps* caps() const { return fContext->contextPriv().caps(); }

    /**
     * Helper functions called by drawBitmapCommon. By the time these are called the SkDraw's
     * matrix, clip, and the device's render target has already been set on GrContext.
     */

    // The tileSize and clippedSrcRect will be valid only if true is returned.
    bool shouldTileImageID(uint32_t imageID,
                           const SkIRect& imageRect,
                           const SkMatrix& viewMatrix,
                           const SkMatrix& srcToDstRectMatrix,
                           const GrSamplerState& params,
                           const SkRect* srcRectPtr,
                           int maxTileSize,
                           int* tileSize,
                           SkIRect* clippedSubset) const;
    // Just returns the predicate, not the out-tileSize or out-clippedSubset, as they are not
    // needed at the moment.
    bool shouldTileImage(const SkImage* image, const SkRect* srcRectPtr,
                         SkCanvas::SrcRectConstraint constraint, SkFilterQuality quality,
                         const SkMatrix& viewMatrix, const SkMatrix& srcToDstRect) const;

    sk_sp<SkSpecialImage> filterTexture(SkSpecialImage*,
                                        int left, int top,
                                        SkIPoint* offset,
                                        const SkImageFilter* filter);

    // Splits bitmap into tiles of tileSize and draws them using separate textures for each tile.
    void drawTiledBitmap(const SkBitmap& bitmap,
                         const SkMatrix& viewMatrix,
                         const SkMatrix& srcToDstMatrix,
                         const SkRect& srcRect,
                         const SkIRect& clippedSrcRect,
                         const GrSamplerState& params,
                         const SkPaint& paint,
                         SkCanvas::SrcRectConstraint,
                         int tileSize,
                         bool bicubic);

    // Used by drawTiledBitmap to draw each tile.
    void drawBitmapTile(const SkBitmap&,
                        const SkMatrix& viewMatrix,
                        const SkRect& dstRect,
                        const SkRect& srcRect,
                        const GrSamplerState& samplerState,
                        const SkPaint& paint,
                        SkCanvas::SrcRectConstraint,
                        bool bicubic,
                        bool needsTextureDomain);

    void drawPinnedTextureProxy(sk_sp<GrTextureProxy>,
                                uint32_t pinnedUniqueID,
                                SkColorSpace*,
                                SkAlphaType alphaType,
                                const SkRect* srcRect,
                                const SkRect* dstRect,
                                SkCanvas::SrcRectConstraint,
                                const SkMatrix& viewMatrix,
                                const SkPaint&);

    void drawTextureMaker(GrTextureMaker* maker,
                          int imageW,
                          int imageH,
                          const SkRect* srcRect,
                          const SkRect* dstRect,
                          SkCanvas::SrcRectConstraint,
                          const SkMatrix& viewMatrix,
                          const SkPaint&);

    void drawTextureProducer(GrTextureProducer*,
                             const SkRect* srcRect,
                             const SkRect* dstRect,
                             SkCanvas::SrcRectConstraint,
                             const SkMatrix& viewMatrix,
                             const SkPaint&);

    void drawTextureProducerImpl(GrTextureProducer*,
                                 const SkRect& clippedSrcRect,
                                 const SkRect& clippedDstRect,
                                 SkCanvas::SrcRectConstraint,
                                 const SkMatrix& viewMatrix,
                                 const SkMatrix& srcToDstMatrix,
                                 const SkPaint&);

    void drawProducerLattice(GrTextureProducer*, std::unique_ptr<SkLatticeIter>, const SkRect& dst,
                             const SkPaint&);

    void drawStrokedLine(const SkPoint pts[2], const SkPaint&);

    void wireframeVertices(SkVertices::VertexMode, int vertexCount, const SkPoint verts[],
                           SkBlendMode, const uint16_t indices[], int indexCount, const SkPaint&);

    static sk_sp<GrRenderTargetContext> MakeRenderTargetContext(GrContext*,
                                                                SkBudgeted,
                                                                const SkImageInfo&,
                                                                int sampleCount,
                                                                GrSurfaceOrigin,
                                                                const SkSurfaceProps*,
                                                                GrMipMapped);

    friend class GrAtlasTextContext;
    friend class SkSurface_Gpu;      // for access to surfaceProps
    typedef SkClipStackDevice INHERITED;
};

#endif
