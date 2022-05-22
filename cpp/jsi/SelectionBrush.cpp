//
//  SelectionBrush.cpp
//  qlik-oss-react-native-helium
//
//  Created by Vittorio Cellucci on 2022-05-17.
//

#include "SelectionBrush.hpp"
#include "ActiveStyle.hpp"

SelectionBrush::SelectionBrush(jsi::Runtime& rt, const jsi::Value& value): _rt(&rt) {
  try {
    auto brushObject = value.asObject(rt);
    auto cbrush = brushObject.getProperty(rt, "cbrush").asObject(rt);
    auto cstyle = brushObject.getProperty(rt, "style").asObject(rt);
    brush = std::make_unique<jsi::Object>(std::move(cbrush));
    style = std::make_unique<jsi::Object>(std::move(cstyle));
  } catch (const std::exception& e) {
    
  }
}

void SelectionBrush::add(std::shared_ptr<Shape>& shape) {
  std::lock_guard<std::mutex> lock(guard);
  activeShapes.insert(shape);
}

void SelectionBrush::remove(std::shared_ptr<Shape> &shape) {
  std::lock_guard<std::mutex> lock(guard);
  activeShapes.erase(shape);
}

void SelectionBrush::sync() {
  std::lock_guard<std::mutex> lock(guard);
  try {
    for(auto&& shape : activeShapes) {
      auto dataShape = shape->getDataShape();
      auto& data = dataShape->data;
      ActiveStyle activeStyle(*_rt, *style, data);
      shape->activate(activeStyle.getBrush());
    }
  } catch (const std::exception& e) {
    
  }
}

void SelectionBrush::confirmSelections() {
  std::lock_guard<std::mutex> lg(guard);
  if(activeShapes.empty()) {
    return;
  }
  
  try {
    auto addValuesFunction = brush->getPropertyAsFunction(*_rt, "addValues");
    jsi::Array jsArray(*_rt, activeShapes.size());
    size_t i = 0;
    for(auto&& shape : activeShapes) {
      auto dataShape = shape->getDataShape();
      auto& data = dataShape->data;
      // fn.addValues([{ key, value }]);
      std::string kv("/qHyperCubeDef/");
      auto select = data.getProperty(*_rt, "select").asObject(*_rt);
      auto source = select.getProperty(*_rt, "source").asObject(*_rt);
      auto value = select.getProperty(*_rt, "value");
      auto field = source.getProperty(*_rt, "field").asString(*_rt).utf8(*_rt);
      kv += field;
      auto kvValue = jsi::Value(*_rt, jsi::String::createFromUtf8(*_rt, kv));
      jsi::Object obj(*_rt);
      obj.setProperty(*_rt, "key", kvValue);
      obj.setProperty(*_rt, "value", value);
      jsArray.setValueAtIndex(*_rt, i++, obj);
    }
    addValuesFunction.call(*_rt, jsArray);
  } catch (const std::exception& e) {
    
  }
}

void SelectionBrush::append(const std::unordered_set<std::shared_ptr<Shape>> &data) {
  std::lock_guard<std::mutex> lg(guard);
  activeShapes.insert(data.begin(), data.end());
}
