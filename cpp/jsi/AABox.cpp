//
//  AABox.cpp
//  Pods
//
//  Created by Vittorio Cellucci on 2022-05-16.
//

#include "AABox.h"

AABox::AABox(const SkRect& rect) {
  buildFrom(rect);
}

void AABox::buildFrom(const SkRect& rect) {
  boundingBox = rect;
  buildLines();
}

inline void AABox::buildLines() {
  /** Returns four points in quad that enclose SkRect ordered as: top-left, top-right,
   bottom-right, bottom-left. **/

  boundingBox.toQuad(quad);

  leftLine.p1 = quad[0]; // top left
  leftLine.p2 = quad[3]; // bottom left

  bottomLine.p1 = quad[3]; // bottom left
  bottomLine.p2 = quad[2]; // bottom right

  rightLine.p1 = quad[1]; // top right;
  rightLine.p2 = quad[2]; // bottom right

  topLine.p1 = quad[0]; // top left
  topLine.p2 = quad[1]; // top right
}

bool AABox::hitTest(const SkRect& rect) {
  return rect.intersects(boundingBox);
}

bool AABox::hitTest(const SkPoint &p1, const SkPoint &p2) {
  Line src = {p1, p2};
  bool leftTest = lineline(src, leftLine);
  if( leftTest ) {
    return true;
  }

  bool rightTest = lineline(src, rightLine);
  if(rightTest) {
    return true;
  }

  bool bottomTest = lineline(src, bottomLine);
  if(bottomTest) {
    return true;
  }

  bool topTest = lineline(src, topLine);
  if(topTest) {
    return true;
  }

  return false;
}


bool AABox::hitTest(std::vector<SkPoint>& verts) {

  // check inside first
  for(int i = 0; i < 4; i++) {
    if(testPoint(quad[i], verts)) {
      return true;
    }
  }

  bool collided = false;
  std::vector<SkPoint>::size_type next = 0;
  for(auto i = 0; i < verts.size(); i++ ) {
    next = (next + 1) % verts.size();
    // hit test lines
    if(hitTest(verts[i], verts[next])) {
      return true;
    }
  }


  return false;
}


bool AABox::testPoint(SkPoint &p, std::vector<SkPoint> &verts) {
  bool collided = false;
  std::vector<SkPoint>::size_type next = 0;

  for(auto&& vc: verts) {
    next = (next + 1) % verts.size();
    auto vn = verts[next];

    if (((vc.y() > p.y() && vn.y() < p.y()) || (vc.y() < p.y() && vn.y() > p.y())) &&
        (p.x() < (vn.x()-vc.x())*(p.y()-vc.y()) / (vn.y()-vc.y())+vc.x())) {
      collided = !collided;
    }
  }
  return  collided;
}

bool AABox::lineline(const AABox::Line &l1, const AABox::Line &l2) {

  float x1 = l1.p1.x();
  float x2 = l1.p2.x();
  float y1 = l1.p1.y();
  float y2 = l1.p2.y();
  float x3 = l2.p1.x();
  float x4 = l2.p2.x();
  float y3 = l2.p1.y();
  float y4 = l2.p2.y();

  // find distance to point of intersection
  float uA = ((x4-x3) * (y1-y3) - (y4-y3) * (x1-x3)) / ((y4-y3) * (x2-x1) - (x4-x3) * (y2-y1));
  float uB = ((x2-x1) * (y1-y3) - (y2-y1) * (x1-x3)) / ((y4-y3) * (x2-x1) - (x4-x3) * (y2-y1));

  if(uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1) {
    return true;
  }
  return false;
}




