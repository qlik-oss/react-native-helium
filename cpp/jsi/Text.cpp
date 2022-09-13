//
// Created by Vittorio Cellucci on 2022-05-03.
//

#include "Text.h"
#include <memory>
#include "UTF8Utils.h"
#include "Helium.h"
#include "TransformFactory.h"
#include <algorithm>
#include <modules/skparagraph/include/ParagraphBuilder.h>

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

const std::string WHITESPACE = " \n\r\t\f\v";


// trim from end (in place)
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

Text::Text(jsi::Runtime &rt, const jsi::Object &object) {
  text = object.getProperty(rt, "text").toString(rt).utf8(rt);
  SkScalar fontSize = static_cast<SkScalar>(Helium::toPx(object.getProperty(rt, "fontSize").asNumber()));
  
  initFillPaint(rt, object);
  auto baseline = object.getProperty(rt, "baseline").toString(rt).utf8(rt);
  auto anchor = object.getProperty(rt, "anchor").toString(rt).utf8(rt);
  fontFamily = object.getProperty(rt, "fontFamily").toString(rt).utf8(rt);
  typeFace = SkTypeface::MakeFromName(fontFamily.c_str(), SkFontStyle::Normal());
  position.fX = (Helium::toPx(object.getProperty(rt, "x").asNumber()));
  position.fY = (Helium::toPx(object.getProperty(rt, "y").asNumber()));
  
  fontCollection.reset(new skia::textlayout::FontCollection());
  fontCollection->setDefaultFontManager(SkFontMgr::RefDefault());
  
  skia::textlayout::TextStyle defaultStyle;
  defaultStyle.setForegroundColor(*brush);
  defaultStyle.setFontSize(fontSize);
  defaultStyle.setTypeface(typeFace);
  defaultStyle.setTextBaseline(skia::textlayout::TextBaseline::kAlphabetic);
  paragraphStyle.setTextStyle(defaultStyle);
  paragraphStyle.setTextAlign(skia::textlayout::TextAlign::kStart);
  
  calcBaseline(baseline);
  calcAnchor(anchor);
  
  auto paragraphBuilder = skia::textlayout::ParagraphBuilder::make(paragraphStyle, fontCollection);
  paragraphBuilder->addText(text.c_str());
  paragraph = paragraphBuilder->Build();
}

void Text::splitText() {
  // this holds all new line
  std::stringstream ss(text);
  std::string line;
  while(std::getline(ss, line)){
    rtrim(line);
    lines.push_back(line);
  }
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

void Text::draw(SkCanvas *canvas) {
  canvas->save();
  if(paragraph) {
    auto clipBounds = canvas->getLocalClipBounds();
    paragraph->layout(clipBounds.width());
    paragraph->paint(canvas, position.fX, position.fY);
  }
  canvas->restore();
}

void Text::calcAnchor(const std::string &anchor) {
  if(anchor == "end") {
    paragraphStyle.setTextAlign(skia::textlayout::TextAlign::kRight);
  }
  if(anchor == "middle") {
    paragraphStyle.setTextAlign(skia::textlayout::TextAlign::kCenter);
  }
}
