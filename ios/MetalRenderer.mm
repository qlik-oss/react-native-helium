//
//  MetalRenderer.cpp
//  qlik-oss-react-native-helium
//
//  Created by Vittorio Cellucci on 2022-05-07.
//
#include "RenderThread.hpp"
#include "MetalLayer.h"
#include "MetalRenderer.h"
#include "CanvasViewManger.h"
#import "../cpp/jsi/Helium.h"

MetalRenderer::MetalRenderer(void* metalLayer, void* device, void* commandQueue) {
  CAMetalLayer* layer = (__bridge CAMetalLayer*)metalLayer;
  id<MTLDevice> _device = (__bridge id<MTLDevice>)device;
  id<MTLCommandQueue> _commandQueue = (__bridge id<MTLCommandQueue>)commandQueue;
  metallayer = std::make_shared<MetalLayer>(layer, _device, _commandQueue);
}

void MetalRenderer::setNativeId(const std::string &nid) {
  nativeId = nid;
}

void MetalRenderer::draw() {
  std::lock_guard<std::mutex> lg(drawMutex);
  if(!tearingDown) {
    if(!tearingDown) {
      metallayer->draw(renderView);
    }
  }
}

void MetalRenderer::setBounds(const SkRect &bounds) {
  width = bounds.width();
  height = bounds.height();
  clipBounds = bounds;
  metallayer->setSize(bounds.width(), bounds.height());
  if(!renderView) {
    renderView = std::make_shared<SkiaRenderView>(bounds);
  } else {
    renderView->resetBound(bounds);
  }
}

void MetalRenderer::resetBounds(const SkRect &bounds) {
  width = bounds.width();
  height = bounds.height();
  clipBounds = bounds;
  metallayer->resetSize(bounds.width(), bounds.height());
  renderView->resetBound(bounds);
}

std::shared_ptr<SkiaRenderView> MetalRenderer::get() {
  return renderView;
}

void MetalRenderer::addShapes(const std::string &vid, jsi::Runtime &rt, const jsi::Value &value) {
  auto shapes = shapeFactory.buildShapes(rt, value);
  if(renderView) {
    renderView->addShapes(vid, shapes);
  }
}

SkISize MetalRenderer::getSize() {
  return SkISize::Make(width, height);
}

void MetalRenderer::tearDown(){
  std::lock_guard<std::mutex> lg(drawMutex);
  tearingDown = true;
  dispatch_async(dispatch_get_main_queue(), ^{
    Helium::TheCanvasViewManager::instance()->remove(nativeId);
  });
}

void MetalRenderer::clear(const std::string& vid) {
  if(renderView) {
    auto view = renderView->get(vid);
    if(view) {
      view->clear();
    } else {
      renderView->clear();
    }
  }
}

void MetalRenderer::resize(const std::string& vid, const SkRect& bounds) {
  if(renderView) {
    renderView->resize(vid, bounds);
  }
}

void MetalRenderer::destroy(const std::string& vid) {
  if(renderView) {
    renderView->destroy(vid);
  }
}

void MetalRenderer::createVirtualRender(const std::string& vid, const SkRect& bounds,  const SkMatrix& matrix) {
  if(renderView) {
    renderView->createVirtualRender(vid, bounds, matrix);
  }
}

void MetalRenderer::beginSelections(float x, float y) {
  if(renderView) {
    renderView->beginSelections(SkPoint::Make(Helium::toPx(x), Helium::toPx(y)));
    Helium::TheCanvasViewManager::instance()->runOnJS([this]{
      renderView->syncBrush();
      draw();
    });
  }
}

bool MetalRenderer::inSelections() const {
  return renderView->inSelections();
}

void MetalRenderer::setSelectionBrush(jsi::Runtime& rt, const jsi::Value& brush) {
  selectionBrush = std::make_shared<SelectionBrush>(rt, brush);
  renderView->setSelectionBrush(selectionBrush);
}

void MetalRenderer::confirmSelections() {
  if(selectionBrush) {
    selectionBrush->confirmSelections();
  }
}

void MetalRenderer::clearSelections() {
  renderView->clearSelections();
  draw();
}

void MetalRenderer::startLasso(float x, float y){
  renderView->startLasso(Helium::toPx(x), Helium::toPx(y));
}

void MetalRenderer::endLasso(float x, float y) {
  renderView->endLasso(Helium::toPx(x), Helium::toPx(y));
}

void MetalRenderer::updateLasso(float x, float y) {
  renderView->updateLasso(Helium::toPx(x), Helium::toPx(y));
}

void MetalRenderer::syncBrush() {
  if(renderView) {
    Helium::TheCanvasViewManager::instance()->runOnJS([this]{
      renderView->syncBrush();
      draw();
    });
  }
}

void MetalRenderer::purge() {
  if(metallayer) {
    metallayer->purge();
  }
}
