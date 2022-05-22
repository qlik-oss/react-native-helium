//
//  SelectionsEngine.hpp
//  Pods
//
//  Created by Vittorio Cellucci on 2022-05-16.
//

#ifndef SelectionsEngine_hpp
#define SelectionsEngine_hpp

#include <unordered_map>
#include <vector>
#include "Shape.h"
#include "SelectionBrush.hpp"

class SelectionsEngine {
public:
  SelectionsEngine() = default;
  void resetData(std::vector<std::shared_ptr<Shape>>& shapes);
  void select(const std::string& key);
  void clearSelections();
  void beginSelections();
  
  void syncSelections();
  void add(std::shared_ptr<Shape> shape);
  void remove(std::shared_ptr<Shape> shape);
  std::vector<std::shared_ptr<Shape>> getAllFrom(std::shared_ptr<Shape> shape);
  void activate(std::shared_ptr<Shape> shape, std::unordered_set<std::shared_ptr<Shape>>& collect);
  
  void sync() {
    if(selectionBrush) {
      selectionBrush->sync();
    }
    inSelections = false;
  }
  
  bool isInSelections() const {
    return inSelections;
  }
  
  void setSelectionBrush(std::shared_ptr<SelectionBrush>& brush) {
    selectionBrush = brush;
  }
  
  std::shared_ptr<SelectionBrush> getSelectionBrush() {
    return selectionBrush;
  }
  
protected:
  void prunePendingAdd();
  void prunePendingRemove();
  
protected:
  std::unordered_map<std::string, std::vector<std::shared_ptr<Shape>>> shapeMap;
  std::unordered_set<std::shared_ptr<Shape>> pendingAdd;
  std::unordered_set<std::shared_ptr<Shape>> pendingRemoval;
  std::shared_ptr<SelectionBrush> selectionBrush;
  bool inSelections = false;
  
};

#endif /* SelectionsEngine_hpp */
