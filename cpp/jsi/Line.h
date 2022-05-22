//
// Created by Vittorio Cellucci on 2022-05-03.
//

#ifndef HELIUM4_LINE_H
#define HELIUM4_LINE_H

#include <jsi/jsi.h>
#include <string>
#include "Shape.h"

using namespace facebook;

/*
 *
stroke:'#e6e6e6'
strokeWidth:1
type:'line'
x1:47.5
x2:47.5
y1:0.5
y2:669.5
 */
class Line : public Shape {
protected:
    float x1, y1;
    float x2, y2;
public:
    Line(jsi::Runtime& rt, const jsi::Object& obj) ;

    void draw(SkCanvas* canvas);

    virtual ~Line() = default;
};
#endif //HELIUM4_LINE_H
