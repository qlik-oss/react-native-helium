//
// Created by Vittorio Cellucci on 2022-05-03.
//

#include "Text.h"
#include "UTF8Utils.h"
#include "Helium.h"
#include "TransformFactory.h"
#include <include/core/SkFont.h>
#include <include/core/SkFontMetrics.h>
#include <include/utils/SkTextUtils.h>
#include <include/core/SkFontMgr.h>
/*
 *
 * Sample object
 *
 anchor:'end'
 baseline:'central'
 boundingRect:Object
 collider:Object
 fill:'#595959'
 fontFamily:''Source Sans Pro''
 fontSize:12
 maxHeight:8
 maxWidth:26.5
 text:'2000'
 tickValue:2000
 type:'text'
 x:36.5
 y:124.1081560283688
 */

Text::Text(jsi::Runtime &rt, const jsi::Object &object) {
  text = object.getProperty(rt, "text").toString(rt).utf8(rt);
  SkScalar fontSize = static_cast<SkScalar>(Helium::toPx(object.getProperty(rt, "fontSize").asNumber()));
  auto fontFamily = object.getProperty(rt, "fontFamily").toString(rt).utf8(rt);
  auto typeFace = SkTypeface::MakeFromName(fontFamily.c_str(), SkFontStyle::Bold());
  if(!isASCII(text)) {
    auto data = text.c_str();
    auto sk = SkFontMgr::RefDefault();
    const char *bcp47_locale = "";
    SkUnichar  unichar = nextUTF8(&data, data + text.size() );
    auto tp = sk->matchFamilyStyleCharacter(fontFamily.c_str(), SkFontStyle::Normal(),
                                              &bcp47_locale, 1, unichar);
    typeFace.reset(tp);
  }
  auto baseline = object.getProperty(rt, "baseline").toString(rt).utf8(rt);
  auto anchor = object.getProperty(rt, "anchor").toString(rt).utf8(rt);
  position.fX = (Helium::toPx(object.getProperty(rt, "x").asNumber()));
  position.fY = (Helium::toPx(object.getProperty(rt, "y").asNumber()));
  font = SkFont(typeFace, fontSize);
  font.setSubpixel(true);
  font.setEdging(SkFont::Edging::kSubpixelAntiAlias);
  font.getMetrics(&fontMetrics);

  calcBaseline(baseline);
  calcAnchor(anchor);

  initFillPaint(rt, object);

  if(object.hasProperty(rt, "transform")) {
    TransformFactory txFactory;
    transform = txFactory.parse(rt, object);
  }
  transform = SkMatrix::Translate(position.fX, position.fY);

  splitText();
}

void Text::calcBaseline(const std::string& baseline) {
  SkRect bounds;
  font.measureText(text.c_str(), text.length(), SkTextEncoding::kUTF8, &bounds);
  position.fX -= bounds.left();

  float emHeight = fontMetrics.fDescent - fontMetrics.fAscent;

  if(baseline == "central") {
    position.fY = position.fY - fontMetrics.fAscent - (emHeight * 0.5f);
  }
  if(baseline == "text-before-edge") {
    position.fY = position.fY - bounds.top();
  }
  if(baseline == "bottom") {
    position.fY = position.fY - (bounds.height() + bounds.top());
  }

}

void Text::splitText() {
  // this holds all new line
  std::stringstream ss(text);
  std::string line;
  while(std::getline(ss, line)){
    lines.push_back(line);
  }
}

void Text::draw(SkCanvas *canvas) {
  canvas->save();
  canvas->concat(transform);
  float emHeight = fontMetrics.fDescent - fontMetrics.fAscent;
  SkScalar dy = 0;
  SkRect bounds;

  font.measureText(text.c_str(), text.length(), SkTextEncoding::kUTF8, &bounds);
  for(auto&& line: lines) {
    SkTextUtils::DrawString(canvas, line.c_str(), 0, dy, font, *brush, textAlign );
    dy += emHeight + fontMetrics.fDescent;
  };

  canvas->restore();
}

void Text::calcAnchor(const std::string &anchor) {
  if(anchor == "end") {
    textAlign = SkTextUtils::kRight_Align;
  }
  if(anchor == "middle") {
    textAlign = SkTextUtils::kCenter_Align;
  }
}
