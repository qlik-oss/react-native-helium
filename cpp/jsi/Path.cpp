//
//  Path.cpp
//  qlik-oss-react-native-helium
//
//  Created by Vittorio Cellucci on 2022-05-30.
//

#include "Path.h"
#include "Helium.h"
#include <include/utils/SkParsePath.h>
#include "CanvasViewManger.h"
#include "TransformFactory.h"



using namespace facebook;

Path::Path(jsi::Runtime& rt, const jsi::Object& object) {
  try {
    initFillPaint(rt, object);
    auto pathString = object.getProperty(rt, "d").toString(rt).utf8(rt);
    SkParsePath::FromSVGString(pathString.c_str(), &path);
    auto deviceScale = SkMatrix::Scale(Helium::TheCanvasViewManager::instance()->scale, Helium::TheCanvasViewManager::instance()->scale);
    if(object.hasProperty(rt, "transform")) {
      TransformFactory txFactory;
      transform = txFactory.parse(rt, object);
    }
    auto count = path.countPoints();
    auto b = path.getBounds();
  }catch(const std::exception& e) {
    
  }
}

void Path::draw(SkCanvas *canvas) {
  canvas->save();
  canvas->setMatrix(transform);
  canvas->drawPath(path, activeBrush);
  canvas->restore();
}
