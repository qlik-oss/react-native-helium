//
//  LassoLayer.hpp
//  qlik-oss-react-native-helium
//
//  Created by Vittorio Cellucci on 2022-05-18.
//

#ifndef LassoLayer_hpp
#define LassoLayer_hpp

#include <include/core/SkCanvas.h>
#include <include/core/SkPoint.h>
#include <include/core/SkPaint.h>
#include <include/core/SkPath.h>

class LassoLayer {
public:
struct Line {
  SkPoint p1;
  SkPoint p2;
  float length() const {
    float dx = p2.x() - p1.x();
    float dy = p2.y() - p1.y();
    return sqrt(((dx*dx) + (dy*dy)));
  }
};

public:
  LassoLayer();
  void start(float x, float y);
  void end(float x, float y);
  Line update(float x, float y);
  void render(SkCanvas* canvas);

  SkPath getPath() const {
    return linePath;
  }

protected:
  SkPaint fillPaint;
  SkPaint strokePaint;
  SkPaint linePaint;
  SkPoint circlePoint;
  SkPoint lastPoint;
  SkPath linePath;
  SkPaint debugBrush;
  float circleRadius;
  bool active = false;
};

#endif /* LassoLayer_hpp */
