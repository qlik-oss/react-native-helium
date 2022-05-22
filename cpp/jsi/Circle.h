//
// Created by Vittorio Cellucci on 2022-05-03.
//

#ifndef HELIUM4_CIRCLE_H
#define HELIUM4_CIRCLE_H
#include <jsi/jsi.h>
#include "Shape.h"
/*
 * cx:205.5211267605634
cy:NaN
fill:'#3F8AB3'
opacity:1
r:12.147440221172292
stroke:'rgba(255, 255, 255, 0.2)'
strokeDasharray:''
strokeWidth:1
type:'circle'
 */

using namespace facebook;

class Circle : public Shape {
protected:
    int cx = 0;
    int cy = 0;
    int r = 0;
public:
    Circle(jsi::Runtime& rt, const jsi::Object& object) ;
    virtual ~Circle() = default;
    void draw(SkCanvas *canvas) override ;
};

#endif //HELIUM4_CIRCLE_H
