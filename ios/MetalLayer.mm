//
//  MetalLayer.m
//  qlik-oss-react-native-helium
//
//  Created by Vittorio Cellucci on 2022-05-07.
//

#import <Foundation/Foundation.h>

#import "MetalLayer.h"
#import "../cpp/jsi/Helium.h"
#import "../cpp/render/SkiaRenderView.h"
#import <include/core/SkTypes.h>
#import <include/core/SkPictureRecorder.h>

sk_sp<GrDirectContext> MetalLayer::skContext = nullptr;

MetalLayer::MetalLayer(CAMetalLayer* metalLayer, id<MTLDevice> dvc, id<MTLCommandQueue> cmd) {
  layer = metalLayer;
  device = dvc;
  commandQueue = cmd;
}

void MetalLayer::setSize(int width, int height) {
  dispatch_async(dispatch_get_main_queue(), ^{
    layer.frame = CGRectMake(0, 0, Helium::toDB(width), Helium::toDB(height));
    layer.drawableSize = CGSizeMake(width, height);
   
    if(skContext == nullptr) {
      GrContextOptions grContextOptions;
      skContext = GrDirectContext::MakeMetal((__bridge void*)device, (__bridge void*)commandQueue, grContextOptions);
      if(skContext == nullptr) {
        NSLog(@"Failed to create context");
        return;
      }
    }
  });
}

void MetalLayer::resetSize(int width, int height){
  dispatch_async(dispatch_get_main_queue(), ^{
    layer.frame = CGRectMake(0, 0, Helium::toDB(width), Helium::toDB(height));
    layer.drawableSize = CGSizeMake(width, height);
  });
}

void MetalLayer::draw(std::shared_ptr<SkiaRenderView>& renderView) {
  // wrap it in autorelease pool as per documentation
  // https://developer.apple.com/documentation/quartzcore/cametallayer?language=objc
    @autoreleasepool {
      if(skContext == nullptr) {
        return;
      }
      id<CAMetalDrawable> currentDrawable = [layer nextDrawable];
      if(currentDrawable == nullptr) {
        return;
      }
      
      GrMtlTextureInfo textureInfo;
      textureInfo.fTexture.retain((__bridge void*)currentDrawable.texture);
      GrBackendRenderTarget renderTarget(layer.drawableSize.width, layer.drawableSize.height, textureInfo);
      
      auto surface = SkSurface::MakeFromBackendRenderTarget(skContext.get(), renderTarget, kTopLeft_GrSurfaceOrigin, kBGRA_8888_SkColorType, nullptr, nullptr);
      
      if(surface == nullptr || surface->getCanvas() == nullptr) {
        return;
      }
      
      auto canvas = surface->getCanvas();
      if(canvas == nullptr) {
        return;
      }
      canvas->clear(SK_ColorWHITE);
      renderView->draw(canvas);
      surface->flushAndSubmit();
      
      id<MTLCommandBuffer> commandBuffer([commandQueue commandBuffer]);
      [commandBuffer presentDrawable:currentDrawable];
      [commandBuffer commit];
    }
}
