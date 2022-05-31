//
//  Path.hpp
//  qlik-oss-react-native-helium
//
//  Created by Vittorio Cellucci on 2022-05-30.
//

#ifndef Path_hpp
#define Path_hpp

#include <include/core/SkPath.h>
#include "Shape.h"

using namespace facebook;

class Path : public Shape {
public:
  Path(jsi::Runtime& rt, const jsi::Object& object);
  
  void draw(SkCanvas* canvas) override;
  
protected:
  SkPath path;
  SkMatrix transform;
};

#endif /* Path_hpp */
