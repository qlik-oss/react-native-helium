//
//  ActiveStyle.cpp
//  qlik-oss-react-native-helium
//
//  Created by Vittorio Cellucci on 2022-05-17.
//

#include "ActiveStyle.hpp"
#include "Helium.h"

ActiveStyle::ActiveStyle(jsi::Runtime& rt, const jsi::Object& obj,  std::shared_ptr<DataShape> data) {
  auto activeStyle = obj.getProperty(rt, "active").asObject(rt);
  auto strokeWidth = activeStyle.getProperty(rt, "strokeWidth");
  if(strokeWidth.isObject()) {
    activeBrush.setStyle(SkPaint::kStroke_Style);
    auto strokeWidthObject = strokeWidth.asObject(rt);
    if(strokeWidthObject.isFunction(rt)) {
      auto cc = strokeWidthObject.asFunction(rt);
      jsi::Object d = jsi::Object(rt);
      if(data) {
        d.setProperty(rt, "data", data->data);
        
        auto sw = cc.call(rt, d);
        if(sw.isNumber()) {
          strokeWidthValue = Helium::toPx(sw.asNumber());
        }
      }
    }
  } else if(strokeWidth.isNumber()) {
    activeBrush.setStyle(SkPaint::kStroke_Style);
    strokeWidthValue = Helium::toPx(strokeWidth.asNumber());
  }
  
  if(strokeWidthValue > 0) {
    activeBrush.setColor(SK_ColorBLACK);
    activeBrush.setStrokeWidth(strokeWidthValue);
  } else {
    activeBrush.setColor(SK_ColorTRANSPARENT);
  }
}
