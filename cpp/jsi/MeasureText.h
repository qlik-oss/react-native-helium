//
// Created by Vittorio Cellucci on 2022-05-02.
//

#ifndef HELIUM4_MEASURETEXT_H
#define HELIUM4_MEASURETEXT_H

#include <jsi/jsi.h>
#include <include/core/SkSize.h>
#include <include/core/SkFont.h>


using namespace facebook;

class MeasureText {
public:
    MeasureText() = default;
    std::tuple<SkSize, SkFontMetrics> measure(jsi::Runtime& rt,  jsi::Object& object);
protected:

};


#endif //HELIUM4_MEASURETEXT_H
