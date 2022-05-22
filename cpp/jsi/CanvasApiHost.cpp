//
// Created by Vittorio Cellucci on 2022-05-01.
//

#include "CanvasApiHost.h"
#include "CanvasViewManger.h"
#include "Rect.h"
#include "MeasureText.h"
#include "Helium.h"

Value CanvasApiHost::get(Runtime &runtime, const PropNameID &name) {
  auto methodName = name.utf8(runtime);

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
      result.setProperty(runtime, "width", jsi::Value(Helium::toDB(viewSize.width())));
      result.setProperty(runtime, "height", jsi::Value(Helium::toDB(viewSize.height())));
      return Value(runtime, result);
    }
    return Value::undefined();
  };

  auto measureText = [](Runtime &runtime, const Value& thisValue, const Value* args, size_t count){
    auto measureObject = args[0].asObject(runtime);
    jsi::Object result(runtime);
    MeasureText mt;
    auto size = mt.measure(runtime, measureObject);
    result.setProperty(runtime, "width", Value(size.width()));
    result.setProperty(runtime, "height", Value(size.height()));
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

  if(methodName == "paint") {
    return Function::createFromHostFunction(
                                            runtime,
                                            PropNameID::forAscii(runtime, "paint"),
                                            1,
                                            paint);
  }

  if(methodName == "getSize") {
    return Function::createFromHostFunction(runtime,
                                            PropNameID::forAscii(runtime, "getSize"),
                                            1,
                                            getSize);
  }

  if(methodName == "measureText") {
    return Function::createFromHostFunction(runtime,
                                            PropNameID::forAscii(runtime, "measureText"),
                                            1,
                                            measureText);
  }


  if(methodName == "createVirtualCanvas") {
    return Function::createFromHostFunction(runtime,
                                            PropNameID::forAscii(runtime, "createVirtualCanvas"),
                                            3,
                                            createVirtualCanvas);
  }

  if(methodName == "drawShapes") {
    return Function::createFromHostFunction(runtime,
                                            PropNameID::forAscii(runtime, "drawShapes"),
                                            2,
                                            drawShapes);
  }

  if(methodName == "clear") {
    return Function::createFromHostFunction(runtime,
                                            PropNameID::forAscii(runtime, "clear"),
                                            1,
                                            clear);
  }

  if(methodName == "destroyVirtualCanvas") {
    return Function::createFromHostFunction(runtime,
                                            PropNameID::forAscii(runtime, "destroyVirtualCanvas"),
                                            1,
                                            destroyVirtualCanvas);
  }


  if(methodName == "resize") {
    return Function::createFromHostFunction(runtime,
                                            PropNameID::forAscii(runtime, "resize"),
                                            2,
                                            resize);
  }

  if(methodName == "setSelectionBrushes") {
    return Function::createFromHostFunction(runtime,
                                            PropNameID::forAscii(runtime, "setSelectionBrushes"),
                                            2,
                                            setSelectionBrushes);
  }

  if(methodName == "confirmSelections") {
    return Function::createFromHostFunction(runtime,
                                            PropNameID::forAscii(runtime, "confirmSelections"),
                                            1,
                                            confirmSelections);
  }

  return Value::undefined();
}

void CanvasApiHost::set(Runtime &runtime, const PropNameID &name, const Value &value) {
}
