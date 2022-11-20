//
//  LassoLayer.cpp
//  qlik-oss-react-native-helium
//
//  Created by Vittorio Cellucci on 2022-05-18.
//

#include "LassoLayer.hpp"
#include "Helium.h"
LassoLayer::LassoLayer()
: circleRadius(Helium::toPx(10)) {
  fillPaint.setColor(SK_ColorGREEN);
  fillPaint.setStyle(SkPaint::kFill_Style);
  fillPaint.setBlendMode(SkBlendMode::kSrcOver);
  fillPaint.setAlphaf(0.5);
  strokePaint.setColor(SK_ColorBLACK);
  strokePaint.setStyle(SkPaint::kStroke_Style);
  strokePaint.setStrokeWidth(Helium::toPx(2));

  linePaint.setColor(SK_ColorBLACK);
  linePaint.setStyle(SkPaint::kStroke_Style);
  linePaint.setStrokeWidth(Helium::toPx(4));

  debugBrush.setColor(SK_ColorRED);
  debugBrush.setStyle(SkPaint::kStroke_Style);
  debugBrush.setStrokeWidth(Helium::toPx(4));
}

void LassoLayer::start(float x, float y) {
  active = true;
  linePath.reset();
  circlePoint = SkPoint::Make(x, y);
  lastPoint = circlePoint;
  linePath.moveTo(circlePoint);
}

void LassoLayer::end(float x, float y) {
  active = false;
  linePath.lineTo(circlePoint);
  linePath.close();
}

LassoLayer::Line LassoLayer::update(float x, float y) {
  linePath.lineTo(x, y);
  auto newPoint = SkPoint::Make(x, y);
  auto line = Line{lastPoint, newPoint};
  lastPoint = newPoint;
  return line;
}

void LassoLayer::render(SkCanvas* canvas) {
  if(active) {
    canvas->drawCircle(circlePoint, circleRadius, fillPaint);
    canvas->drawCircle(circlePoint, circleRadius, strokePaint);
    canvas->drawPath(linePath, linePaint);
  }
}
