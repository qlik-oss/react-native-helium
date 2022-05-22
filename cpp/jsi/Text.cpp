//
// Created by Vittorio Cellucci on 2022-05-03.
//

#include "Text.h"
#include "Helium.h"
#include "TransformFactory.h"
#include <include/core/SkFont.h>
#include <include/core/SkFontMetrics.h>
#include <include/utils/SkTextUtils.h>
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
    auto typeFace = SkTypeface::MakeFromName(fontFamily.c_str(), SkFontStyle::Normal());
    auto baseline = object.getProperty(rt, "baseline").toString(rt).utf8(rt);
    auto anchor = object.getProperty(rt, "anchor").toString(rt).utf8(rt);
    position.fX = (Helium::toPx(object.getProperty(rt, "x").asNumber()));
    position.fY = (Helium::toPx(object.getProperty(rt, "y").asNumber()));
    font = SkFont(typeFace, fontSize);

    font.getMetrics(&fontMetrics);

    calcBaseline(baseline);
    calcAnchor(anchor);

    initFillPaint(rt, object);

    if(object.hasProperty(rt, "transform")) {
        TransformFactory txFactory;
        transform = txFactory.parse(rt, object);
    }
    transform.preTranslate(position.fX, position.fY);
}

void Text::calcBaseline(const std::string& baseline) {
  float emHeight = fontMetrics.fDescent - fontMetrics.fAscent;
  if(baseline == "central") {
      position.fY = position.fY - fontMetrics.fAscent - (emHeight * 0.5f);
  }
  if(baseline == "text-before-edge") {
    position.fY = position.fY - fontMetrics.fAscent;
  }
  if(baseline == "undefined") {
    position.fY = position.fY  + (emHeight);
  }
}

void Text::draw(SkCanvas *canvas) {
    canvas->save();
    canvas->concat(transform);

    SkTextUtils::DrawString(canvas, text.c_str(), 0, 0, font, *brush, textAlign );

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
