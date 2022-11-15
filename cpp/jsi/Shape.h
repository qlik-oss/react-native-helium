//
// Created by Vittorio Cellucci on 2022-05-03.
//

#ifndef HELIUM4_SHAPE_H
#define HELIUM4_SHAPE_H
#include <memory>
#include <include/core/SkCanvas.h>
#include <jsi/jsi.h>
#include "AABox.h"

using namespace facebook;

struct DataShape {
  jsi::Object data;
  DataShape(jsi::Runtime& rt, jsi::Object&& d):
    data(std::move(d)){}
};

class Shape {
public:
  virtual ~Shape() = default;
  
  virtual void draw(SkCanvas *canvas) {}
  virtual void inActive();
  virtual void active();
  virtual void activate(SkPaint brush);
  virtual bool toggle();
  virtual void clearSelection();
  
  std::string getDataPath() const {
    return dataPath;
  }
  
  std::shared_ptr<AABox> getAAB() const {
    return AAB;
  }
  
  std::shared_ptr<DataShape> getDataShape() {
    return dataShape;
  }

protected:
  void initFillPaint(jsi::Runtime &rt, const jsi::Object &object);

  void initStrokePaint(jsi::Runtime &rt, const jsi::Object &object);

  void initDataPath(jsi::Runtime& rt, const jsi::Object& object);
  
  void setOpacity(const jsi::Object &object, jsi::Runtime &rt);

  SkColor pareseRGB(const std::string &value) const;
protected:
  SkPaint activeBrush;
  SkPaint decoratedBrush;
  SkPaint inActiveBrush;
  SkPaint strokePaint;
  SkPaint* brush;
  std::string dataPath;
  bool hasStroke = false;
  std::shared_ptr<AABox> AAB;
  bool isActive = false;
  bool isDecorated = false;
  std::shared_ptr<DataShape> dataShape;
  double naturalOpacity = 1;
};


#endif //HELIUM4_SHAPE_H
