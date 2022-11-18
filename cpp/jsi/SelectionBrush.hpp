//
//  SelectionBrush.hpp
//  qlik-oss-react-native-helium
//
//  Created by Vittorio Cellucci on 2022-05-17.
//

#ifndef SelectionBrush_hpp
#define SelectionBrush_hpp
#include "Shape.h"
#include <unordered_set>
#include <jsi/jsi.h>
#include <mutex>

using namespace facebook;

class SelectionBrush {
public:
  SelectionBrush(jsi::Runtime& rt, const jsi::Value& brush);
  void add(std::shared_ptr<Shape>& shape);
  void append(const std::unordered_set<std::shared_ptr<Shape>>& data);
  void remove(std::shared_ptr<Shape>& shape);
  void sync();
  void confirmSelections();
  void clear();
protected:
  std::mutex guard;
  std::unordered_set<std::shared_ptr<Shape>> activeShapes;
  jsi::Runtime* _rt;
  std::unique_ptr<jsi::Object> brush;
  std::unique_ptr<jsi::Object> style;
};

#endif /* SelectionBrush_hpp */
