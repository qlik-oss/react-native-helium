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
  auto fallBackTypFace = SkTypeface::MakeFromName(fontFamily.c_str(), SkFontStyle::Normal());
  SkFont font(typeFace, fontSize);
  SkFont fallBackFont = font;
  SkFont currentFont = font;
  SkFontMetrics metrics;
  const char* begin = text.c_str();
  const char* end = text.c_str() + text.length();
  const char* it = begin;
  SkScalar runningWidth = 0;
  const char* bcp47_locale = "";
  const char* c = begin;
  SkUnichar u = nextUTF8(&c, end);
  if(!currentFont.unicharToGlyph(u)) {
    auto candidate = fontManager->matchFamilyStyleCharacter(fontFamily.c_str(),
                                                            SkFontStyle::Normal(),
                                                            &bcp47_locale, 1, u);
    if (candidate) {
      fallBackTypFace.reset(candidate);
      fallBackFont.setTypeface(fallBackTypFace);
      fallBackFont.setSize(fontSize);

    }
    currentFont = fallBackFont;
  }
  while (it != end) {
    const char* prev = it;
    SkUnichar u = nextUTF8(&it, end);
    if(!currentFont.unicharToGlyph(u)) {
      size_t bytes = prev - begin;
      auto w = currentFont.measureText(begin, bytes, SkTextEncoding::kUTF8);
      runningWidth += w;
      begin += bytes;
      if(font.unicharToGlyph(u)) {
        currentFont = font;
      }
      else if(!fallBackFont.unicharToGlyph(u)) {
        auto candidate = fontManager->matchFamilyStyleCharacter(fontFamily.c_str(),
                                                                SkFontStyle::Normal(),
                                                                &bcp47_locale, 1, u);
        if (candidate) {
          fallBackTypFace.reset(candidate);
          fallBackFont.setTypeface(fallBackTypFace);
          fallBackFont.setSize(fontSize);
        }
        
        currentFont = fallBackFont;
      }
    }
  }
  auto bytes = it - begin;
  SkRect bounds;
  auto w = currentFont.measureText(begin, bytes, SkTextEncoding::kUTF8, &bounds);
  runningWidth += w;
  currentFont.getMetrics(&metrics);
  auto width = runningWidth;
  auto height = bounds.height();
  SkRect returnBounds = SkRect::MakeXYWH(Helium::toDB(0), Helium::toDB(0), Helium::toDB(width), Helium::toDB(height));
  return std::make_tuple(returnBounds, metrics);
}
