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
int aft_isrtl(int c){
  if (
    (c==0x05BE)||(c==0x05C0)||(c==0x05C3)||(c==0x05C6)||
    ((c>=0x05D0)&&(c<=0x05F4))||
    (c==0x0608)||(c==0x060B)||(c==0x060D)||
    ((c>=0x061B)&&(c<=0x064A))||
    ((c>=0x066D)&&(c<=0x066F))||
    ((c>=0x0671)&&(c<=0x06D5))||
    ((c>=0x06E5)&&(c<=0x06E6))||
    ((c>=0x06EE)&&(c<=0x06EF))||
    ((c>=0x06FA)&&(c<=0x0710))||
    ((c>=0x0712)&&(c<=0x072F))||
    ((c>=0x074D)&&(c<=0x07A5))||
    ((c>=0x07B1)&&(c<=0x07EA))||
    ((c>=0x07F4)&&(c<=0x07F5))||
    ((c>=0x07FA)&&(c<=0x0815))||
    (c==0x081A)||(c==0x0824)||(c==0x0828)||
    ((c>=0x0830)&&(c<=0x0858))||
    ((c>=0x085E)&&(c<=0x08AC))||
    (c==0x200F)||(c==0xFB1D)||
    ((c>=0xFB1F)&&(c<=0xFB28))||
    ((c>=0xFB2A)&&(c<=0xFD3D))||
    ((c>=0xFD50)&&(c<=0xFDFC))||
    ((c>=0xFE70)&&(c<=0xFEFC))||
    ((c>=0x10800)&&(c<=0x1091B))||
    ((c>=0x10920)&&(c<=0x10A00))||
    ((c>=0x10A10)&&(c<=0x10A33))||
    ((c>=0x10A40)&&(c<=0x10B35))||
    ((c>=0x10B40)&&(c<=0x10C48))||
    ((c>=0x1EE00)&&(c<=0x1EEBB))
  ) return 1;
  return 0;
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
  prepareText();
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

void Text::prepareText() {
  SkString temp;
  const char* begin = text.c_str();
  const char* end = text.c_str() + text.length();
  size_t index = 0;
  while(begin != end) {
    SkUnichar u = nextUTF8(&begin, end);
    
    if(aft_isrtl(u) == 1) {
      temp.insertUnichar(index, u);
    } else {
      temp.appendUnichar(u);
      index = temp.size();
    }
  }
  text = temp.c_str();
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
