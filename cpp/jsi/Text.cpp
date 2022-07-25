//
// Created by Vittorio Cellucci on 2022-05-03.
//

#include "Text.h"
#include <memory>
#include "UTF8Utils.h"
#include "Helium.h"
#include "TransformFactory.h"
#include <algorithm>

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

// trim from start (in place)
static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

Text::Text(jsi::Runtime &rt, const jsi::Object &object) {
  fShaper = SkShaper::MakePrimitive();
  text = object.getProperty(rt, "text").toString(rt).utf8(rt);
  SkScalar fontSize = static_cast<SkScalar>(Helium::toPx(object.getProperty(rt, "fontSize").asNumber()));
  
  auto baseline = object.getProperty(rt, "baseline").toString(rt).utf8(rt);
  auto anchor = object.getProperty(rt, "anchor").toString(rt).utf8(rt);
  fontFamily = object.getProperty(rt, "fontFamily").toString(rt).utf8(rt);
  position.fX = (Helium::toPx(object.getProperty(rt, "x").asNumber()));
  position.fY = (Helium::toPx(object.getProperty(rt, "y").asNumber()));
  
  typeFace = SkTypeface::MakeFromName(fontFamily.c_str(), SkFontStyle::Normal());
  fontManager = SkFontMgr::RefDefault();
  
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
  splitText();
  transform = SkMatrix::Translate(position.fX, position.fY);
  textBlobBuilder.reset( new TextRunHandler(text.c_str(), {0, 0}));
  buildText();
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

void Text::buildText()
{
  
  textBlobBuilder->reset();
  for(auto l : lines) {
    const char* utf8 = l.c_str();
    size_t utf8Bytes = l.length();
    textBlobBuilder->setText(utf8);
    if(!bidi) {
      bidi = std::make_unique<CustomBdiRunIterator>(0xfe, utf8Bytes);
      if (!bidi) {
        return;
      }
    }
    
    if(!language) {
      language =  std::unique_ptr<SkShaper::LanguageRunIterator>(SkShaper::MakeStdLanguageRunIterator(utf8, utf8Bytes));
      if (!language) {
        return;
      }
    }
    
    if(!script) {
      SkFourByteTag undeterminedScript = SkSetFourByteTag('Z','y','y','y');
      script =  std::unique_ptr<SkShaper::ScriptRunIterator>(SkShaper::MakeScriptRunIterator(utf8, utf8Bytes, undeterminedScript));
      if (!script) {
        return;
      }
    }
    
    if(!fontRunner) {
      fontRunner = std::make_unique<FontRunner>(utf8, utf8Bytes, font, SkFontMgr::RefDefault(), typeFace, fontFamily);
    } else {
      fontRunner->reset(utf8, utf8Bytes);
    }
    
    while(!fontRunner->atEnd()) {
      auto p = fontRunner->peek();
      auto c = fontRunner->current();
      textBlobBuilder->advance(c);
      textBlobBuilder->peeked(p.bytes);
      fShaper->shape(fontRunner->current(), p.bytes, *fontRunner, *bidi, *script, *language, 2000, textBlobBuilder.get() );
      fontRunner->next();
    }
    textBlobBuilder->newLine();
  }
  textBlob = textBlobBuilder->makeBlob();
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
  canvas->concat(transform);
//  buildText();
  canvas->drawTextBlob(textBlob, 0, 0, *brush);
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
