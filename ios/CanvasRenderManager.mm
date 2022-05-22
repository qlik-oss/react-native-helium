//
//  CanvasViewManager.m
//  qlik-oss-react-native-helium
//
//  Created by Vittorio Cellucci on 2022-05-07.
//

#import <Foundation/Foundation.h>
#import <React/RCTViewManager.h>

#import "CanvasView.h"
#import "CanvasViewManger.h"

#import <MetalKit/MetalKit.h>
#import <QuartzCore/CAMetalLayer.h>

static id<MTLDevice> device = MTLCreateSystemDefaultDevice();
static id<MTLCommandQueue> commandQueue = id<MTLCommandQueue>([device newCommandQueue]);

@interface CanvasRenderManager : RCTViewManager
@end

@implementation CanvasRenderManager

RCT_EXPORT_MODULE(CanvasView)
RCT_EXPORT_VIEW_PROPERTY(onReady, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(onResized, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(onBeganSelections, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(onLongPressBegan, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(onLongPressEnded, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(lasso, BOOL);

RCT_CUSTOM_VIEW_PROPERTY(nativeID, NSString, CanvasView) {
  // Get parameter
  std::string nativeId([json UTF8String]);
  std::shared_ptr<MetalRenderer> renderView = [view getRenderer];
  Helium::TheCanvasViewManager::instance()->add(nativeId, renderView);
}

-(UIView*)view {
  auto canvasView = [[CanvasView alloc]initWithMetal:device commandQueue:commandQueue];
  return canvasView;
}

@end
