//
// Created by Vittorio Cellucci on 2022-05-04.
//
#include "Circle.h"
#include "Helium.h"

Circle::Circle(jsi::Runtime &rt, const jsi::Object &object) {
    cx = Helium::toPx(object.getProperty(rt, "cx").asNumber());
    cy = Helium::toPx(object.getProperty(rt, "cy").asNumber());
    r = Helium::toPx(object.getProperty(rt, "r").asNumber());

    initFillPaint(rt, object);
    initStrokePaint(rt, object);
  
}

void Circle::draw(SkCanvas *canvas) {
    canvas->drawCircle(cx, cy, r, *brush);
//    canvas->drawCircle(cx, cy, r, strokePaint);
}
