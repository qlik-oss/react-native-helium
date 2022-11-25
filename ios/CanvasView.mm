//
//  CanvasView.m
//  qlik-oss-react-native-helium
//
//  Created by Vittorio Cellucci on 2022-05-07.
//

#import <Foundation/Foundation.h>
#import "CanvasView.h"
#import "MetalRenderer.h"
#import "../cpp/jsi/Helium.h"

@implementation CanvasView {
  std::shared_ptr<MetalRenderer> renderer;
  BOOL ready;
  CGRect prev;
}

+ (Class) layerClass
{
    return [CAMetalLayer class];
}

- (instancetype)initWithMetal:(id<MTLDevice>)devce commandQueue:(id<MTLCommandQueue>)commandQueue {
  self = [super init];
  ready = NO;
  _lasso = NO;
  _disableSelections = NO;
  if(self) {
    renderer = std::make_shared<MetalRenderer>((__bridge void*)self.layer, (__bridge void*)devce, (__bridge void*)commandQueue);
    self.layer.delegate = self;
    self.clipsToBounds = YES;
    CAMetalLayer* metalLayer = (CAMetalLayer*)self.layer;
    metalLayer.device = devce;
    metalLayer.opaque = false;
    metalLayer.contentsScale = [[UIScreen mainScreen] scale];
    metalLayer.pixelFormat = MTLPixelFormatBGRA8Unorm;
    metalLayer.magnificationFilter = kCAFilterLinear;
    metalLayer.framebufferOnly = NO;
    metalLayer.masksToBounds = YES;
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(handleMemoryWarning:) name: UIApplicationDidReceiveMemoryWarningNotification object:nil];

    
    [self buildGestures];
  }
  return self;
}

-(void)buildGestures {
  UITapGestureRecognizer *tapGestureRecognizer = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(handleTap:)];
  tapGestureRecognizer.numberOfTapsRequired = 1;
  [self addGestureRecognizer:tapGestureRecognizer];
  
  UILongPressGestureRecognizer* longPress = [[UILongPressGestureRecognizer alloc] initWithTarget:self action:@selector(handleLongPress:)];
  [self addGestureRecognizer:longPress];
  
  UIPanGestureRecognizer* pg = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(handlePan:)];
  [self addGestureRecognizer:pg];
  pg.enabled = NO;
  _panGesture = pg;
  
}

- (void) handleTap: (UITapGestureRecognizer *)recognizer {
  
  CGPoint point = [recognizer locationInView:self];
  if(!_disableSelections) {
    renderer->beginSelections(point.x, point.y);
  }
  CGRect frameRect = self.frame;
  CGRect rect = [self convertRect:frameRect toView:[UIApplication sharedApplication].keyWindow.rootViewController.view];
  self.onBeganSelections(@{@"x": @(rect.origin.x),
                           @"y": @(rect.origin.y),
                           @"width":@(rect.size.width),
                           @"height": @(rect.size.height)});
}

- (void) handleLongPress:(UILongPressGestureRecognizer*)recognizer {
  if(recognizer.state == UIGestureRecognizerStateBegan) {
    CGPoint point = [recognizer locationInView:self];
    CGPoint rawPoint = [self convertPoint:point toView:nil];
    
    renderer->onLongPress(point.x, point.y, rawPoint.x, rawPoint.y);
  }
}

-(void) handlePan: (UIPanGestureRecognizer*)recognizer {
  if(_lasso && !_disableSelections) {
    @autoreleasepool {
      CGPoint point = [recognizer locationInView:self];
      switch(recognizer.state) {
        case UIGestureRecognizerStateBegan: {
          renderer->startLasso(point.x, point.y);
          break;
        }
        case UIGestureRecognizerStateChanged: {
          renderer->updateLasso(point.x, point.y);
          break;
        }
        case UIGestureRecognizerStateEnded:{
          renderer->endLasso(point.x, point.y);
          break;
        }
        default:
          break;
      }
      renderer->draw();
    }
  }
}

-(void)layoutSubviews {
  [super layoutSubviews];
  if(!ready) {
    renderer->setBounds(SkRect::MakeWH(Helium::toPx(self.frame.size.width), Helium::toPx(self.frame.size.height)));
    ready = YES;
    prev = self.frame;
    self.onReady(@{});
  } else {
    dispatch_async(dispatch_get_main_queue(), ^{
      renderer->resetBounds(SkRect::MakeWH(Helium::toPx(self.frame.size.width), Helium::toPx(self.frame.size.height)));
      self.onResized(@{});
      self->renderer->draw();
    });
  }
}

-(void)setFrame:(CGRect)frame {
  [super setFrame:frame];
  if(ready) {
    dispatch_async(dispatch_get_main_queue(), ^{
      self->renderer->resetBounds(SkRect::MakeWH(Helium::toPx(self.frame.size.width), Helium::toPx(self.frame.size.height)));
      self.onResized(@{});
      self->renderer->draw();
    });
  }
}

-(std::shared_ptr<MetalRenderer>)getRenderer {
  return renderer;
}

-(void)dealloc {
  renderer->tearDown();
}

-(void)setLasso:(BOOL)lasso {
  _lasso = lasso;
  _panGesture.enabled = _lasso;
}


- (void) handleMemoryWarning:(NSNotification *) notification {
  if(renderer) {
    renderer->purge();
  }

}


@end
