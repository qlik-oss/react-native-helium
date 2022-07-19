//
//  FontRunner.cpp
//  qlik-oss-react-native-helium
//
//  Created by Vittorio Cellucci on 2022-07-11.
//

#include "FontRunner.hpp"
FontRunner::FontRunner(const char* utf8, size_t utf8Bytes,
                       const SkFont& font, sk_sp<SkFontMgr> fallbackMgr,
                       const char* requestName, SkFontStyle requestStyle,
                       const SkShaper::LanguageRunIterator* lang)
: fCurrent(utf8), fBegin(utf8), fEnd(utf8 + utf8Bytes)
, fFallbackMgr(std::move(fallbackMgr))
, fFont(font)
, fOriginalFont(font)
, fFallbackFont(fFont)
, fCurrentFont(nullptr)
, fRequestName(requestName)
, fRequestStyle(requestStyle)
, fLanguage(lang)
{
  iterator = fCurrent;
  fCurrentFont = &fFont;
  fPeekFont = &fFont;
}

FontRunner::FontRunner(const char* utf8, size_t utf8Bytes,
                       const SkFont& font, sk_sp<SkFontMgr> fallbackMgr, sk_sp<SkTypeface> tf, const  std::string& family)
: FontRunner(utf8, utf8Bytes, font, std::move(fallbackMgr),
             nullptr, font.refTypefaceOrDefault()->fontStyle(), nullptr)
{
  originalTypeface = tf;
  fontFamily = family;
}

void FontRunner::consume() {
  SkASSERT(fCurrent < fEnd);
  SkASSERT(!fLanguage || this->endOfCurrentRun() <= fLanguage->endOfCurrentRun());
  
  const char *bcp47_locale = "";
  sk_sp<SkTypeface> currentTypeFace(originalTypeface);

  SkUnichar u = nextUTF8(&iterator, fEnd);
  if(fFont.unicharToGlyph(u)) {
    fCurrentFont = &fFont;
  } else if(fFallbackFont.unicharToGlyph(u)) {
    fCurrentFont = &fFallbackFont;
    fPeekFont = fCurrentFont;
  }
  while(iterator < fEnd) {
    const char* prev = iterator;
    SkUnichar u = nextUTF8(&iterator, fEnd);
    // is it a unicode char && does the current font not support it??
    if(fCurrentFont->unicharToGlyph(u) == 0 ) {
      if(fFallbackFont.unicharToGlyph(u) == 0) {
        auto candidate = fFallbackMgr->matchFamilyStyleCharacter(fontFamily.c_str(),
                                                                 SkFontStyle::Normal(),
                                                                 &bcp47_locale, 1, u);
        if (candidate) {
          currentTypeFace.reset(candidate);
          fFallbackFont.setTypeface(currentTypeFace);
          fPeekFont = &fFallbackFont;
          iterator = prev;
        }
      }
      return;
    }
    
    if(iterator != fEnd) {
      if (fFont.getTypeface() != originalTypeface.get()) {
        fFont.setTypeface(originalTypeface);
        iterator = prev;
        return;
      }
    } 
  }
  
  
}

FontRunner::Peek FontRunner::peek() {
  const char* p = iterator;
  while(p < fEnd) {
    const char* prev = p;
    SkUnichar u = nextUTF8(&p, fEnd);
    // is it a unicode char?
    if(fPeekFont->unicharToGlyph(u) == 0) {
      p = prev;
      Peek _peek;
      _peek.p = p;
      _peek.bytes = p - fCurrent;
      return _peek;
    }
  }
  Peek _peek;
  _peek.p = p;
  _peek.bytes = p - fCurrent;
  return _peek;
}
