//
//  MetalRenderer.hpp
//  qlik-oss-react-native-helium
//
//  Created by Vittorio Cellucci on 2022-05-07.
//

#ifndef MetalRenderer_hpp
#define MetalRenderer_hpp
#include "../cpp/render/SkiaRenderView.h"
#include "../cpp/jsi/Shape.h"
#include "../cpp/jsi/ShapeFactory.h"
#include "../cpp/jsi/SelectionBrush.hpp"
#include <jsi/jsi.h>
#include <mutex>


using namespace facebook;
class MetalLayer;

class MetalRenderer {
public:
  MetalRenderer(void* metalLayer, void* device, void* commandQueue);
  void setNativeId(const std::string& nid);
  void draw();
  void setBounds(const SkRect& bounds);
  void resetBounds(const SkRect& bounds);
  std::shared_ptr<SkiaRenderView> get();
  SkISize getSize();
  void tearDown();
  void clear(const std::string& vid);
  void addShapes(const std::string& vid, jsi::Runtime& rt, const jsi::Value& value);
  void resize(const std::string& vid, const SkRect& bounds);
  void destroy(const std::string& vid);
  void createVirtualRender(const std::string& vid, const SkRect& bounds,  const SkMatrix& matrix);
  void beginSelections(float x, float y);
  bool inSelections() const;
  void setSelectionBrush(jsi::Runtime& rt, const jsi::Value& brush);
  void confirmSelections();
  void startLasso(float x, float y);
  void endLasso(float x, float y);
  void updateLasso(float x, float y);
  void syncBrush();
  void purge();

protected:
  bool tearingDown = false;
  std::mutex drawMutex;
  ShapeFactory shapeFactory;
  std::string nativeId;
  std::shared_ptr<SkiaRenderView> renderView;
  int width = 0;
  int height = 0;
  SkRect clipBounds;
  std::shared_ptr<MetalLayer> metallayer;
  std::shared_ptr<SelectionBrush> selectionBrush;
};
#endif /* MetalRenderer_hpp */
