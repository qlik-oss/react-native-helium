//
//  CanvasView.h
//  Pods
//
//  Created by Vittorio Cellucci on 2022-05-07.
//

#ifndef CanvasView_h
#define CanvasView_h

#import <memory>
#import <React/RCTView.h>

class MetalRenderer;

@interface CanvasView : UIView<CALayerDelegate>

@property (nonatomic, copy) RCTDirectEventBlock onReady;
@property (nonatomic, copy) RCTDirectEventBlock onResized;
@property (nonatomic, copy) RCTDirectEventBlock onLongPressBegan;
@property (nonatomic, copy) RCTDirectEventBlock onLongPressEnded;
@property (nonatomic, copy) RCTDirectEventBlock onBeganSelections;
@property (nonatomic) BOOL lasso;
@property (nonatomic) BOOL disableSelections;
@property (nonatomic) UIPanGestureRecognizer* panGesture;

-(instancetype)initWithMetal:(id<MTLDevice>)devce commandQueue:(id<MTLCommandQueue>)commandQueue;
-(std::shared_ptr<MetalRenderer>)getRenderer;

@end
#endif /* CanvasView_h */
