//
// Created by Vittorio Cellucci on 2022-05-02.
//

#include "MeasureText.h"
#include "Helium.h"
#include <include/core/SkFont.h>
#include <include/core/SkFontMgr.h>
#include "UTF8Utils.h"


std::tuple<SkRect, SkFontMetrics> MeasureText::measure(jsi::Runtime& rt, jsi::Object& object) {
   
  std::string text("");
  auto fontFamily = object.getProperty(rt, "fontFamily").asString(rt).utf8(rt);
  auto fontSize = Helium::toPx(object.getProperty(rt, "fontSize").asNumber());
  if(object.hasProperty(rt, "text")) {
    text = object.getProperty(rt, "text").asString(rt).utf8(rt);
  }
  auto fontManager = SkFontMgr::RefDefault();
  auto typeFace = SkTypeface::MakeFromName(fontFamily.c_str(), SkFontStyle::Normal());
  scanForUTF8(text, fontManager, typeFace, fontFamily);
  SkFont font(typeFace, fontSize);
  SkFontMetrics metrics;
  font.getMetrics(&metrics);
  SkRect bounds;
  font.measureText(text.c_str(), text.length(), SkTextEncoding::kUTF8, &bounds);
  SkRect returnBounds = SkRect::MakeXYWH(Helium::toDB(bounds.left()), Helium::toDB(bounds.top()), Helium::toDB(bounds.width()), Helium::toDB(bounds.height()));
  return std::make_tuple(returnBounds, metrics);
}
