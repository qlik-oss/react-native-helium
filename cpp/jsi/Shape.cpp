//
// Created by Vittorio Cellucci on 2022-05-04.
//
#include "Shape.h"
#include "Helium.h"
#include <include/utils/SkParse.h>


SkColor Shape::pareseRGB(const std::string &value) const {
  size_t op = value.find_first_of('('), ep = value.find_first_of(')');
  if (op != std::string::npos && ep + 1 == value.length()) {
    std::string line(value.begin() + op + 1, value.begin() + ep);
    const std::vector<float> params = Helium::split<float>(line);
    if(params.size() == 3){
      return SkColorSetARGB(255, params[0], params[1], params[2]);
    } else if(params.size() == 4){
      return SkColorSetARGB(params[3] * 255, params[0], params[1], params[2]);
    }
  }
  return SK_ColorGREEN;
}


void Shape::setOpacity(const jsi::Object &object, jsi::Runtime &rt) {
  try {
    if (object.hasProperty(rt, "opacity")) {
      auto opacityValue = object.getProperty(rt, "opacity");
      if(!opacityValue.isUndefined()) {
        naturalOpacity = opacityValue.asNumber();
        activeBrush.setAlpha(naturalOpacity * 255);
      } else {
        activeBrush.setAlpha(255);
      }
    } else {
      activeBrush.setAlpha(255);
    }
  }catch(const std::exception& e) {
    activeBrush.setAlpha(255);
  }

}

void Shape::initFillPaint(jsi::Runtime &rt, const jsi::Object &object) {
    activeBrush.setAntiAlias(true);
    activeBrush.setStyle(SkPaint::kFill_Style);

  try {
    auto fill = object.getProperty(rt, "fill").asString(rt).utf8(rt);



    SkColor colorss;
    auto f = fill.find("rgb");
    if(f != std::string::npos) {
      colorss = pareseRGB(fill);
    } else {
      SkParse::FindColor(fill.c_str(), &colorss);
    }

    activeBrush.setColor(colorss);

    setOpacity(object, rt);

    inActiveBrush = SkPaint(activeBrush);
    inActiveBrush.setAlpha(0.5 * 255);
  } catch(const std::exception& e) {
    activeBrush.setColor(SK_ColorBLACK);
  }
  brush = &activeBrush;
}

void Shape::initStrokePaint(jsi::Runtime &rt, const jsi::Object &object) {
  if(object.hasProperty(rt, "stroke")) {
    try {
      auto strokeProp = object.getProperty(rt, "stroke");//.asString(rt).utf8(rt);
      if(!strokeProp.isString()) {
        return;
      }
      auto stroke = strokeProp.asString(rt).utf8(rt);
      auto strokeWidth = Helium::toPx(object.getProperty(rt, "strokeWidth").asNumber());
      SkColor strokeColor;

      auto f = stroke.find("rgb");
      if(f != std::string::npos) {
        strokeColor = pareseRGB(stroke);
      } else {
        SkParse::FindColor(stroke.c_str(), &strokeColor);
      }
      strokePaint.setAntiAlias(true);
      strokePaint.setStyle(SkPaint::kStroke_Style);
      strokePaint.setColor(strokeColor);
      if (object.hasProperty(rt, "opacity")) {
        auto opacity = object.getProperty(rt, "opacity").asNumber();
        strokePaint.setAlpha(opacity * 255);
      } else {
        strokePaint.setAlpha(255);
      }
      strokePaint.setStrokeWidth(strokeWidth);
      hasStroke = true;
    } catch(std::exception& e) {

    }
  }
}

void Shape::initDataPath(jsi::Runtime &rt, const jsi::Object &object) {
  try {
    auto data = object.getProperty(rt, "data").asObject(rt);
    auto select = data.getProperty(rt, "select").asObject(rt);
    auto source = select.getProperty(rt, "source").asObject(rt);
    auto field = source.getProperty(rt, "field");
    auto value = select.getProperty(rt, "value");

    std::stringstream pathStream;
    if(field.isString()) {
      pathStream << field.asString(rt).utf8(rt);
    } else {
      pathStream << field.asNumber();
    }

    pathStream << "/";
    if( value.isString()) {
      pathStream << value.asString(rt).utf8(rt);
    } else {
      pathStream << value.asNumber();
    }
    dataPath = pathStream.str();
    dataShape = std::make_shared<DataShape>(rt, std::move(data));

  } catch (const std::exception& e) {

  }
}

void Shape::inActive() {
  isActive = false;
  isDecorated = false;
  if(naturalOpacity > 0.5) {
    brush = &inActiveBrush;
  }
}

void Shape::active() {
  isActive = true;
  brush = &activeBrush;
}

void Shape::activate(SkPaint cbrush) {
  isActive = true;
  isDecorated = true;
  decoratedBrush = cbrush;
}

bool Shape::toggle() {
  isActive = !isActive;
  if(isActive) {
    active();
  } else {
    inActive();
  }
  return isActive;
}
