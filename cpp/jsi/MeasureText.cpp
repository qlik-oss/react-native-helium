//
// Created by Vittorio Cellucci on 2022-05-02.
//

#include "MeasureText.h"
#include "Helium.h"
#include <include/core/SkFont.h>

SkISize MeasureText::measure(jsi::Runtime& rt, jsi::Object& object) {
  std::string text("");
  auto fontFamily = object.getProperty(rt, "fontFamily").asString(rt).utf8(rt);
  auto fontSize = Helium::toPx(object.getProperty(rt, "fontSize").asNumber());
  if(object.hasProperty(rt, "text")) {
    text = object.getProperty(rt, "text").asString(rt).utf8(rt);
  }

  auto typeFace = SkTypeface::MakeFromName(fontFamily.c_str(), SkFontStyle::Normal());
  SkFont font(typeFace, fontSize);
  SkRect bounds;
  font.measureText(text.c_str(), text.length(), SkTextEncoding::kUTF8, &bounds);
  return SkISize::Make(Helium::toDB(bounds.width()), Helium::toDB(bounds.height()));
}
