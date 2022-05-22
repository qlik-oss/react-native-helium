//
// Created by Vittorio Cellucci on 2022-05-04.
//

#include "Rectangle.h"
#include "Helium.h"
#include "AABox.h"

/*
  "collider": {
    "type": null
  },
  "fill": "rgb(96, 142, 142)",
  "height": 151.02342064714946,
  "minHeightPx": 1,
  "minWidthPx": 1,
  "show": true,
  "stroke": "rgba(255, 255, 255, 0.2)",
  "strokeLinejoin": "miter",
  "strokeWidth": 1,
  "type": "rect",
  "width": 18.666666666666668,
  "x": 274.6666666666667,
  "y": 109.97657935285054
 */

Rectangle::Rectangle(jsi::Runtime &rt, const jsi::Object &object) {
  initDataPath(rt, object);
  initFillPaint(rt, object);
  initStrokePaint(rt, object);
  rect = SkRect::MakeXYWH(
          Helium::toPx(object.getProperty(rt, "x").asNumber()),
          Helium::toPx(object.getProperty(rt, "y").asNumber()),
          Helium::toPx(object.getProperty(rt, "width").asNumber()),
          Helium::toPx(object.getProperty(rt, "height").asNumber()));

  AAB = std::make_shared<AABox>(rect);
}

void Rectangle::draw(SkCanvas *canvas) {
  canvas->drawRect(rect, *brush);
  if(hasStroke) {
   canvas->drawRect(rect, strokePaint);
  }
  if(isDecorated) {
    canvas->drawRect(rect, decoratedBrush);
  }
}

