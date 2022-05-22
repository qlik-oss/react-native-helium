//
// Created by Vittorio Cellucci on 2022-05-03.
//

#ifndef HELIUM4_SHAPEFACTORY_H
#define HELIUM4_SHAPEFACTORY_H
#include "Shape.h"
#include <jsi/jsi.h>
#include "Line.h"
#include "Circle.h"
#include "Text.h"
#include "Rectangle.h"
#include "ObjectPool.h"
using namespace facebook;

class ShapeFactory {
public:
  ShapeFactory() = default;
  std::vector<std::shared_ptr<Shape>> buildShapes(jsi::Runtime& rt, const jsi::Value& value);

protected:

protected:
  ObjectPool<Line> linePool;
  ObjectPool<Circle> circlePool;
  ObjectPool<Text> textPool;
  ObjectPool<Rectangle> rectanglePool;
};


#endif //HELIUM4_SHAPEFACTORY_H
