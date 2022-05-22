//
//  MetalLayer.h
//  Pods
//
//  Created by Vittorio Cellucci on 2022-05-07.
//

#ifndef MetalLayer_h
#define MetalLayer_h

#import <CoreFoundation/CoreFoundation.h>
#import <UIKit/UIKit.h>

#import <MetalKit/MetalKit.h>
#import <QuartzCore/CAMetalLayer.h>

#import <include/gpu/mtl/GrMtlBackendContext.h>
#import <include/gpu/GrDirectContext.h>
#import <include/core/SkSurface.h>

class SkiaRenderView;
class MetalLayer {
public:
  MetalLayer(CAMetalLayer* metalLayer, id<MTLDevice> dvc, id<MTLCommandQueue> cmd);
  void setSize(int width, int height);
  void resetSize(int width, int height);
  void draw(std::shared_ptr<SkiaRenderView>& renderView);
  void clearScreen();
private:
  sk_sp<SkSurface> offscreenTarget;
  CAMetalLayer* layer;
  bool needsResize = false;
  id<MTLCommandQueue> commandQueue;
  id<MTLDevice> device;
  static sk_sp<GrDirectContext> skContext;

};

#endif /* MetalLayer_h */
