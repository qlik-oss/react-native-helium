//
// Created by Vittorio Cellucci on 2022-05-01.
//

#ifndef HELIUM4_RECT_H
#define HELIUM4_RECT_H
#include <jsi/jsi.h>
#include <include/core/SkColor.h>
#include <include/core/SkCanvas.h>
using namespace facebook;
struct RectT {
    int x;
    int y;
    int width;
    int height;
    SkColor color;
};

class HeliumRect {
public:
    HeliumRect(jsi::Runtime &runtime,const jsi::Value &obj);
    void draw(SkCanvas* canvas);
    SkMatrix translation() {
       SkMatrix matrix;
       matrix.setTranslate(rect.x, rect.y);
       return matrix;
    }

    SkRect sizedRect() {
        return SkRect::MakeWH(rect.width, rect.height);
    }
protected:
    RectT rect;
};


#endif //HELIUM4_RECT_H
