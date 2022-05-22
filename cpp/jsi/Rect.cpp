//
// Created by Vittorio Cellucci on 2022-05-01.
//

#include <include/core/SkColor.h>
#include <include/utils/SkParse.h>
#include "Rect.h"
#include "Helium.h"

HeliumRect::HeliumRect(jsi::Runtime &runtime, const jsi::Value &obj) {
    auto object = obj.asObject(runtime);
    rect.x = Helium::toPx(object.getProperty(runtime, "x").asNumber());
    rect.y = Helium::toPx(object.getProperty(runtime, "y").asNumber());
    rect.width = Helium::toPx(object.getProperty(runtime, "width").asNumber());
    rect.height = Helium::toPx(object.getProperty(runtime, "height").asNumber());

    std::string color = object.getProperty(runtime, "fillColor").toString(runtime).utf8(runtime);
    SkColor colors;
    SkParse::FindColor("red", &colors);
    rect.color = colors; //SkColorSetARGB(a, c.r, c.g, c.b);
}

void HeliumRect::draw(SkCanvas *canvas) {
    SkPaint paint;
    paint.setStyle(SkPaint::kFill_Style);
    paint.setColor(rect.color);
    SkRect rect = SkRect::MakeXYWH(this->rect.x, this->rect.y, this->rect.width, this->rect.height);
    canvas->drawRect(rect, paint);
}
