//
//  BoundingVolume.h
//  Pods
//
//  Created by Vittorio Cellucci on 2022-05-16.
//

#ifndef BoundingVolume_h
#define BoundingVolume_h

class BoundingVolume {
public:
  virtual ~BoundingVolume() = default;
  
  virtual bool hitTest(const SkRect& rect) {
    return false;
  }
};

#endif /* BoundingVolume_h */
