//
// Created by Vittorio Cellucci on 2022-05-02.
//

#ifndef HELIUM4_MEASURETEXT_H
#define HELIUM4_MEASURETEXT_H

#include <jsi/jsi.h>
#include <include/core/SkSize.h>

using namespace facebook;

class MeasureText {
public:
    MeasureText() = default;
    SkISize measure(jsi::Runtime& rt,  jsi::Object& object);
protected:

};


#endif //HELIUM4_MEASURETEXT_H
