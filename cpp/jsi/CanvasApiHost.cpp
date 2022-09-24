//
// Created by Vittorio Cellucci on 2022-05-01.
//

#include "CanvasApiHost.h"
#include "CanvasViewManger.h"
#include "Rect.h"
#include "MeasureText.h"
#include "Helium.h"

void CanvasApiHost::install(Runtime &runtime) {
  
  auto drawShapes = [](Runtime &runtime, const Value &thisValue, const Value *args, size_t count) {
    Helium::VirtualContext virtualContext(runtime, args[0]);
    auto view = Helium::TheCanvasViewManager::instance()->get(virtualContext.id);
    if(view) {
      view->addShapes(virtualContext.vid, runtime, args[1] );
    }
    return Value::undefined();
  };
  
  auto paint = [](Runtime &runtime, const Value& thisValue, const Value* args, size_t count){
    Helium::VirtualContext virtualContext(runtime, args[0]);
    auto view = Helium::TheCanvasViewManager::instance()->get(virtualContext.id);
    if(view) {
      view->draw();
    }
    return Value::undefined();
  };
  
  auto getSize = [](Runtime &runtime, const Value& thisValue, const Value* args, size_t count){
    Helium::VirtualContext virtualContext(runtime, args[0]);
    auto view = Helium::TheCanvasViewManager::instance()->get(virtualContext.id);
    if(view) {
      SkISize viewSize;
      auto childView = view->get();
      auto renderView = childView->get(virtualContext.vid);
      if(!renderView) {
        viewSize = view->getSize();
      } else {
        viewSize = renderView->size();
      }
      jsi::Object result(runtime);
      result.setProperty(runtime, "width", (Helium::toDB(viewSize.width())));
      result.setProperty(runtime, "height",(Helium::toDB(viewSize.height())));
      return Value(runtime, result);
    }
    return Value::undefined();
  };
  
  auto measureText = [](Runtime &runtime, const Value& thisValue, const Value* args, size_t count){
    auto measureObject = args[0].asObject(runtime);
    jsi::Object result(runtime);
    MeasureText mt;
    auto t = mt.measure(runtime, measureObject);
    auto rect = std::get<0>(t);
    auto metrics = std::get<1>(t);
    result.setProperty(runtime, "width", (rect.width()));
    result.setProperty(runtime, "height", (rect.height()));
    result.setProperty(runtime, "left", (rect.fLeft));
    result.setProperty(runtime, "right", (rect.fRight));
    result.setProperty(runtime, "actualBoundingBoxAscent", (rect.top()));
    result.setProperty(runtime, "fontBoundingBoxAscent", Helium::toDB(metrics.fAscent));
    result.setProperty(runtime, "fontBoundingBoxDescent", Helium::toDB(metrics.fDescent));
    return Value(runtime, result);
  };
  
  auto createVirtualCanvas = [](Runtime& runtime, const Value& thisValue, const Value* args, size_t count){
    Helium::VirtualContext virtualContext(runtime, args[0]);
    auto view = Helium::TheCanvasViewManager::instance()->get(virtualContext.id);
    if(view) {
      HeliumRect rect(runtime, args[2]);
      auto newVid = args[1].toString(runtime).utf8(runtime);
      view->createVirtualRender(newVid, rect.sizedRect(), rect.translation());
    }
    return Value::undefined();
  };
  
  auto resize = [](Runtime& runtime, const Value& thisValue, const Value* args, size_t count){
    Helium::VirtualContext virtualContext(runtime, args[0]);
    auto view = Helium::TheCanvasViewManager::instance()->get(virtualContext.id);
    if(view) {
      HeliumRect rect(runtime, args[1]);
      view->resize(virtualContext.vid, rect.sizedRect());
    }
    return Value::undefined();
  };
  
  auto destroyVirtualCanvas = [](Runtime& runtime, const Value& thisValue, const Value* args, size_t count){
    Helium::VirtualContext virtualContext(runtime, args[0]);
    auto view = Helium::TheCanvasViewManager::instance()->get(virtualContext.id);
    if(view) {
      view->destroy(virtualContext.vid);
    }
    return Value::undefined();
  };
  
  auto clear = [](Runtime& runtime, const Value& thisValue, const Value* args, size_t count){
    Helium::VirtualContext virtualContext(runtime, args[0]);
    auto view = Helium::TheCanvasViewManager::instance()->get(virtualContext.id);
    if(view) {
      view->clear(virtualContext.vid);
    }
    return Value::undefined();
  };
  
  auto setSelectionBrushes = [](Runtime& runtime, const Value& thisValue, const Value* args, size_t count){
    Helium::VirtualContext virtualContext(runtime, args[0]);
    auto view = Helium::TheCanvasViewManager::instance()->get(virtualContext.id);
    if(view) {
      view->setSelectionBrush(runtime, args[1]);
    }
    
    return Value::undefined();
  };
  
  auto confirmSelections = [](Runtime& runtime, const Value& thisValue, const Value* args, size_t count){
    Helium::VirtualContext virtualContext(runtime, args[0]);
    auto view = Helium::TheCanvasViewManager::instance()->get(virtualContext.id);
    if(view) {
      view->confirmSelections();
    }
    return Value::undefined();
  };
  auto module = Object(runtime);
  
  
  module.setProperty(runtime, "paint", Function::createFromHostFunction(
                                                                        runtime,
                                                                        PropNameID::forAscii(runtime, "paint"),
                                                                        1,
                                                                        paint));
  
  module.setProperty(runtime, "getSize",  Function::createFromHostFunction(runtime,
                                                                           PropNameID::forAscii(runtime, "getSize"),
                                                                           1,
                                                                           getSize));
  
  module.setProperty(runtime, "measureText",  Function::createFromHostFunction(runtime,
                                                                               PropNameID::forAscii(runtime, "measureText"),
                                                                               1,
                                                                               measureText));
  
  module.setProperty(runtime, "createVirtualCanvas",  Function::createFromHostFunction(runtime,
                                                                                       PropNameID::forAscii(runtime, "createVirtualCanvas"),
                                                                                       1,
                                                                                       createVirtualCanvas));
  
  module.setProperty(runtime, "drawShapes",  Function::createFromHostFunction(runtime,
                                                                              PropNameID::forAscii(runtime, "drawShapes"),
                                                                              1,
                                                                              drawShapes));
  
  module.setProperty(runtime, "clear",  Function::createFromHostFunction(runtime,
                                                                         PropNameID::forAscii(runtime, "clear"),
                                                                         1,
                                                                         clear));
  
  module.setProperty(runtime, "destroyVirtualCanvas",  Function::createFromHostFunction(runtime,
                                                                                        PropNameID::forAscii(runtime, "destroyVirtualCanvas"),
                                                                                        1,
                                                                                        destroyVirtualCanvas));
  
  module.setProperty(runtime, "resize",  Function::createFromHostFunction(runtime,
                                                                          PropNameID::forAscii(runtime, "resize"),
                                                                          1,
                                                                          resize));
  
  module.setProperty(runtime, "setSelectionBrushes",  Function::createFromHostFunction(runtime,
                                                                                       PropNameID::forAscii(runtime, "setSelectionBrushes"),
                                                                                       1,
                                                                                       setSelectionBrushes));
  
  module.setProperty(runtime, "confirmSelections",  Function::createFromHostFunction(runtime,
                                                                                     PropNameID::forAscii(runtime, "confirmSelections"),
                                                                                     1,
                                                                                     confirmSelections));
  runtime.global().setProperty(runtime, "HeliumCanvasApi", std::move(module));
}
