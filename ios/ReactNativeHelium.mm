#import "ReactNativeHelium.h"
#import <React/RCTBridge.h>
#import <React/RCTBridge+Private.h>
#import <React/RCTUtils.h>
#import <ReactCommon/CallInvoker.h>
#import <ReactCommon/RCTTurboModule.h>

#import <jsi/jsi.h>
#include "CanvasViewManger.h"
#include "../cpp/jsi/CanvasApiHost.h"

namespace facebook {
  namespace react {
    class CallInvoker;
  }
}

using namespace facebook;

@implementation ReactNativeHelium

@synthesize bridge = _bridge;
@synthesize methodQueue = _methodQueue;

RCT_EXPORT_MODULE()

+ (BOOL)requiresMainQueueSetup {
  return YES;
}

-(void)setBridge:(RCTBridge*)bridge {
  _bridge = bridge;
  _setBridgeOnMainQueue = RCTIsMainQueue();
  [self install];
}

-(void)install {
  auto ci = self.bridge.jsCallInvoker;
  RCTCxxBridge *cxxBridge = (RCTCxxBridge*)self.bridge;
  Helium::TheCanvasViewManager::instance()->scale = [[UIScreen mainScreen]scale];
  Helium::TheCanvasViewManager::instance()->platform = std::make_shared<Platform>(*(jsi::Runtime*)cxxBridge.runtime, ci);
  if(cxxBridge.runtime) {
    CanvasApiHost::install(*(jsi::Runtime*)cxxBridge.runtime);
  }
}
@end
