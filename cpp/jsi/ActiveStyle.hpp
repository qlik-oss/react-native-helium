//
//  ActiveStyle.hpp
//  qlik-oss-react-native-helium
//
//  Created by Vittorio Cellucci on 2022-05-17.
//

#ifndef ActiveStyle_hpp
#define ActiveStyle_hpp

#include <jsi/jsi.h>
#include <include/core/SkPaint.h>
#include "Shape.h"

using namespace facebook;

class ActiveStyle {
public:
  ActiveStyle(jsi::Runtime& rt, const jsi::Object& object, std::shared_ptr<DataShape> data);
  SkPaint getBrush() const {
    return activeBrush;
  }
protected:
  int strokeWidthValue = 0;
  SkPaint activeBrush;
};

#endif /* ActiveStyle_hpp */
