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

Text::Text(jsi::Runtime &rt, const jsi::Object &object) {
  text = object.getProperty(rt, "text").toString(rt).utf8(rt);
  SkScalar fontSize = static_cast<SkScalar>(Helium::toPx(object.getProperty(rt, "fontSize").asNumber()));

  initFillPaint(rt, object);
  initStrokePaint(rt, object);
  
  auto baseline = object.getProperty(rt, "baseline").toString(rt).utf8(rt);
  auto anchor = object.getProperty(rt, "anchor").toString(rt).utf8(rt);
  fontFamily = object.getProperty(rt, "fontFamily").toString(rt).utf8(rt);
  typeFace = SkTypeface::MakeFromName(fontFamily.c_str(), SkFontStyle::Normal());
  position.fX = (Helium::toPx(object.getProperty(rt, "x").asNumber()));
  position.fY = (Helium::toPx(object.getProperty(rt, "y").asNumber()));

  fontCollection.reset(new skia::textlayout::FontCollection());
  fontCollection->setDefaultFontManager(SkFontMgr::RefDefault());

  skia::textlayout::TextStyle defaultStyle;
  defaultStyle.setForegroundColor(SkPaint(*brush));
  defaultStyle.setFontSize(fontSize);
  defaultStyle.setTypeface(typeFace);
  defaultStyle.setTextBaseline(skia::textlayout::TextBaseline::kAlphabetic);
  defaultStyle.setFontFamilies({SkString(fontFamily.c_str())});
  defaultStyle.getFontMetrics(&fontMetrics);

  paragraphStyle.setTextAlign(skia::textlayout::TextAlign::kJustify);
  
  font = SkFont(typeFace, fontSize);
  
  auto paragraphBuilder = skia::textlayout::ParagraphBuilder::make(paragraphStyle, fontCollection);
  paragraphBuilder->pushStyle(defaultStyle);
  paragraphBuilder->addText(text.c_str());
  paragraphBuilder->pop();
  paragraph = paragraphBuilder->Build();
  
  buildStroke(defaultStyle);
  calcBaseline(baseline);
  calcAnchor(anchor);

  if(object.hasProperty(rt, "transform")) {
    TransformFactory txFactory;
    transform = txFactory.parse(rt, object);
  }
   
  transform = SkMatrix::Translate(position.fX, position.fY);
}

void Text::calcBaseline(const std::string& baseline) {
  font.measureText(text.c_str(), text.length(), SkTextEncoding::kUTF8, &measuredBounds, brush);
  font.getMetrics(&fontMetrics);
  SkScalar actualFontSize = fontMetrics.fDescent - fontMetrics.fAscent;
  SkScalar fontBaseline = actualFontSize - fontMetrics.fDescent;
  SkScalar fCorrectDescent = fontBaseline - measuredBounds.height() + fontMetrics.fLeading ;
  position.fY -= fCorrectDescent;
   
  // things are from the bottom
  if(baseline == "text-before-edge") {
    position.fY -= measuredBounds.bottom();
  }
  
  if(baseline == "center" || baseline == "central") {
    position.fY -= measuredBounds.bottom() + measuredBounds.height() / 2.0 ;
  }
}

void Text::buildStroke(skia::textlayout::TextStyle& style) {
  if(hasStroke) {
    // theres a bug in skia and metal blending, so have to build a seperate paragraph
    skia::textlayout::ParagraphStyle strokeParagraphStyle;
    strokeParagraphStyle.setTextAlign(skia::textlayout::TextAlign::kJustify);

    style.setForegroundColor(SkPaint(strokePaint));

    auto strokeBuilder = skia::textlayout::ParagraphBuilder::make(strokeParagraphStyle, fontCollection);
    strokeBuilder->pushStyle(style);
    strokeBuilder->addText(text.c_str());
    strokeBuilder->pop();
    strokeParagraph = strokeBuilder->Build();
  }
}

void Text::draw(SkCanvas *canvas) {
  canvas->save();
  canvas->concat(transform);
  if(paragraph) {
    auto clipBounds = canvas->getLocalClipBounds();
    paragraph->layout(clipBounds.width());
   
    if(hasStroke) {
      strokeParagraph->layout(clipBounds.width());
      strokeParagraph->paint(canvas, 0, 0);
    }
    paragraph->paint(canvas, 0, 0);
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
