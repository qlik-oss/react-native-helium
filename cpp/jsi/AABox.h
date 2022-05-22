//
//  AABox.hpp
//  Pods
//
//  Created by Vittorio Cellucci on 2022-05-16.
//

#ifndef AABox_hpp
#define AABox_hpp
#include <vector>
#include <include/core/SkRect.h>
#include <include/core/SkPath.h>
#include "BoundingVolume.h"

class AABox : public BoundingVolume {
public:
  AABox(const SkRect& rect);
  void buildFrom(const SkRect& rect);
  bool hitTest(const SkRect& rect) override;
  // hit tests a line with points p1 and p2
  bool hitTest(const SkPoint& p1, const SkPoint& p2);
  bool hitTest(std::vector<SkPoint>& verts);
  
  SkRect getRect() const {
    return boundingBox;
  }
  
protected:
  void buildLines();
  bool testPoint(SkPoint& point, std::vector<SkPoint>& verts);
  
protected:
  SkRect boundingBox;
  struct Line {
    SkPoint p1;
    SkPoint p2;
  };
  SkPoint quad[4];
  Line leftLine;
  Line bottomLine;
  Line rightLine;
  Line topLine;
protected:
  bool lineline(const Line& l1, const Line& l2);
};

#endif /* AABox_hpp */
