//
// Created by Vittorio Cellucci on 2022-05-03.
//

#include "ShapeFactory.h"

std::vector<std::shared_ptr<Shape>> ShapeFactory::buildShapes(jsi::Runtime &rt, const jsi::Value &value) {
  std::vector<std::shared_ptr<Shape>> shapes;
  auto object = value.asObject(rt);
  auto inputArray = object.asArray(rt);
  for(size_t i = 0; i < inputArray.length(rt); i++) {
    auto shape = inputArray.getValueAtIndex(rt, i).asObject(rt);
    auto shapeType = shape.getProperty(rt, "type").asString(rt).utf8(rt);
    if(shapeType == "line") {
      auto item = linePool.create(rt, shape);
      shapes.push_back(item);
    }

    if(shapeType == "circle") {
      auto item = circlePool.create(rt, shape);
      shapes.push_back(item);
    }

    if(shapeType == "text") {
      auto text = shape.getProperty(rt, "text").asString(rt).utf8(rt);
      if(text.length() > 0) {
        shapes.emplace_back(textPool.create(rt, shape));
      }
    }

    if(shapeType == "rect") {
      shapes.emplace_back(rectanglePool.create(rt, shape));
    }
  }
  return shapes;
}




