//
// Created by Vittorio Cellucci on 2022-05-04.
//

#ifndef HELIUM4_RECTANGLE_H
#define HELIUM4_RECTANGLE_H

#include <include/core/SkRect.h>
#include "Shape.h"

class Rectangle : public Shape {
public:
    Rectangle(jsi::Runtime& rt, const jsi::Object& object);
    virtual ~Rectangle() = default;
    void draw(SkCanvas* canvas) override;

protected:
    SkRect rect;
};


#endif //HELIUM4_RECTANGLE_H
