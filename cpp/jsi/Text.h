//
// Created by Vittorio Cellucci on 2022-05-03.
//

#ifndef HELIUM4_TEXT_H
#define HELIUM4_TEXT_H
#include <jsi/jsi.h>
#include "Shape.h"
#include <include/core/SkFont.h>
#include <include/core/SkFontMetrics.h>
#include <include/utils/SkTextUtils.h>

using namespace facebook;

class Text : public Shape{
protected:
  SkPoint position;
  SkFont font;
  SkMatrix transform;
  SkFontMetrics fontMetrics{};
  std::string text;
  SkTextUtils::Align textAlign = SkTextUtils::kLeft_Align;
  std::vector<std::string> lines;
public:
  Text(jsi::Runtime& rt, const jsi::Object& object);
  virtual ~Text() = default;
  void draw(SkCanvas* canvas) override;
  
protected:
  void calcBaseline(const std::string& baseline);
  void calcAnchor(const std::string& anchor);
  void splitText();
  
};


#endif //HELIUM4_TEXT_H
