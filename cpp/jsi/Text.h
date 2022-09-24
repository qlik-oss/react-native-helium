//
// Created by Vittorio Cellucci on 2022-05-03.
//

#ifndef HELIUM4_TEXT_H
#define HELIUM4_TEXT_H
#define SK_UNICODE_AVAILABLE
#define SK_SHAPER_HARFBUZZ_AVAILABLE
#include <jsi/jsi.h>
#include "Shape.h"
#include <memory>
#include <include/core/SkFont.h>
#include <include/core/SkFontMetrics.h>
#include <include/utils/SkTextUtils.h>
#include <include/core/SkFontMgr.h>
#include <modules/skshaper/include/SkShaper.h>
#include <modules/skparagraph/include/Paragraph.h>
#include "TextRunHundler.hpp"
#include "FontRunner.hpp"
#include "RunIterators.h"

using namespace facebook;

class Text : public Shape{
protected:
  SkPoint position;
  SkFont font;
  SkMatrix transform;
  SkFontMetrics fontMetrics{};
  SkRect measuredBounds;
  std::string text;
  std::vector<std::string> lines;
  std::string fontFamily;
  sk_sp<SkTypeface> typeFace;
  sk_sp<skia::textlayout::FontCollection> fontCollection;
  std::unique_ptr<skia::textlayout::Paragraph> paragraph;
  skia::textlayout::ParagraphStyle paragraphStyle;

public:
  Text(jsi::Runtime& rt, const jsi::Object& object);
  virtual ~Text() = default;
  void draw(SkCanvas* canvas) override;

protected:
  void calcBaseline(const std::string& baseline);
  void calcAnchor(const std::string& anchor);
};


#endif //HELIUM4_TEXT_H
