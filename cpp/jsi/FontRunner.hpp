//
//  FontRunner.hpp
//  qlik-oss-react-native-helium
//
//  Created by Vittorio Cellucci on 2022-07-11.
//

#ifndef FontRunner_hpp
#define FontRunner_hpp

#include <include/core/SkFont.h>
#include <include/core/SkFontMetrics.h>
#include <include/utils/SkTextUtils.h>
#include <include/core/SkFontMgr.h>
#include <modules/skshaper/include/SkShaper.h>
#include "UTF8Utils.h"

class FontRunner : public SkShaper::FontRunIterator {
public:
 
  struct Peek {
    const char* p;
    size_t bytes;
  };
  
  FontRunner(const char* utf8, size_t utf8Bytes,
             const SkFont& font, sk_sp<SkFontMgr> fallbackMgr,
             const char* requestName, SkFontStyle requestStyle,
             const SkShaper::LanguageRunIterator* lang);
  
  FontRunner(const char* utf8, size_t utf8Bytes,
             const SkFont& font, sk_sp<SkFontMgr> fallbackMgr, sk_sp<SkTypeface> tf, const std::string& family);
  
  
  void consume() override ;
  Peek peek();
  
  size_t endOfCurrentRun() const override {
    return fCurrent - fBegin;
  }
  bool atEnd() const override {
    return fCurrent == fEnd;
  }
  
  const SkFont& currentFont() const override {
    return fCurrentFont;
  }
  
  const char* current() const {
    return fCurrent;
  }
  
  const char* nextUTF8Char() {
    auto it = fCurrent;
    nextUTF8(&it, fEnd);
    return it;
  }
  
  size_t bytes() const {
    return fEnd - fCurrent;
  }
  
  void reset(const char* utf8, size_t utf8Bytes) {
    fCurrent = utf8;
    fBegin = utf8;
    fEnd = utf8 + utf8Bytes;
  }
  void next() {
    fCurrent = iterator;
  }
  
protected:
  char const * fCurrent;
  char const* fBegin;
  char const* fEnd;
  char const* iterator;
  std::string fontFamily;
  sk_sp<SkFontMgr> const fFallbackMgr;
  SkFont fFont;
  SkFont fOriginalFont;
  SkFont fFallbackFont;
  sk_sp<SkTypeface> originalTypeface;
  SkFont fCurrentFont;
  SkFont* fPeekFont;
  char const * const fRequestName;
  SkFontStyle const fRequestStyle;
  SkShaper::LanguageRunIterator const * const fLanguage;
  
};
#endif /* FontRunner_hpp */
