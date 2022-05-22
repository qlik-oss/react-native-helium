//
// Created by Vittorio Cellucci on 2022-05-13.
//
#include "Line.h"
#include "Helium.h"

Line::Line(jsi::Runtime &rt, const jsi::Object &obj) {
  std::string stroke = obj.getProperty(rt, "stroke").asString(rt).utf8(rt);

  initStrokePaint(rt, obj);

  x1 = Helium::toPx(obj.getProperty(rt, "x1").asNumber());
  y1 = Helium::toPx(obj.getProperty(rt, "y1").asNumber());
  x2 = Helium::toPx(obj.getProperty(rt, "x2").asNumber());
  y2 = Helium::toPx(obj.getProperty(rt, "y2").asNumber());
}

void Line::draw(SkCanvas *canvas) {
  canvas->drawLine(x1, y1, x2, y2, strokePaint);
}

