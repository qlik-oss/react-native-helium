//
// Created by Vittorio Cellucci on 2022-05-02.
//

#include "MeasureText.h"
#include "Helium.h"
#include <include/core/SkFont.h>
#include <include/core/SkFontMgr.h>
#include <modules/skparagraph/include/ParagraphBuilder.h>
//#include "UTF8Utils.h"


std::tuple<SkRect, SkFontMetrics> MeasureText::measure(jsi::Runtime& rt, jsi::Object& object) {
  std::string text("");
  auto fontFamily = object.getProperty(rt, "fontFamily").asString(rt).utf8(rt);
  auto fontSize = Helium::toPx(object.getProperty(rt, "fontSize").asNumber());
  if(object.hasProperty(rt, "text")) {
    text = object.getProperty(rt, "text").asString(rt).utf8(rt);
  }
  
  auto typeFace = SkTypeface::MakeFromName(fontFamily.c_str(), SkFontStyle::Normal());
  skia::textlayout::ParagraphStyle paragraphStyle;
  sk_sp<skia::textlayout::FontCollection> fontCollection(new skia::textlayout::FontCollection());
  fontCollection->setDefaultFontManager(SkFontMgr::RefDefault());

  skia::textlayout::TextStyle defaultStyle;
  defaultStyle.setFontSize(fontSize);
  defaultStyle.setTypeface(typeFace);
  defaultStyle.setTextBaseline(skia::textlayout::TextBaseline::kAlphabetic);
  paragraphStyle.setTextStyle(defaultStyle);
  paragraphStyle.setTextAlign(skia::textlayout::TextAlign::kStart);
  
  skia::textlayout::ParagraphBuilder::make(paragraphStyle, fontCollection);
  auto paragraphBuilder = skia::textlayout::ParagraphBuilder::make(paragraphStyle, fontCollection);
  paragraphBuilder->addText(text.c_str());
  auto paragraph = paragraphBuilder->Build();
  paragraph->layout(10000);
  SkFontMetrics metrics;
  defaultStyle.getFontMetrics(&metrics);
  
  auto width = paragraph->getLongestLine();
  auto height = paragraph->getHeight();
  SkRect returnBounds = SkRect::MakeXYWH(Helium::toDB(0), Helium::toDB(0), Helium::toDB(width), Helium::toDB(height));
  return std::make_tuple(returnBounds, metrics);
}
