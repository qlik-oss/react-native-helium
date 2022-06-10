//
//  SelectionsEngine.cpp
//  Pods
//
//  Created by Vittorio Cellucci on 2022-05-16.
//

#include "SelectionsEngine.h"

void SelectionsEngine::resetData(std::vector<std::shared_ptr<Shape>> &shapes) {
  shapeMap.clear();
  inSelections = false;
  for(auto&& shape: shapes) {
    auto path = shape->getDataPath();
    if(!path.empty()) {
      auto it = shapeMap.find(path);
      if(it != shapeMap.end()) {
        it->second.push_back(shape);
      } else {
        shapeMap[path] = std::vector<std::shared_ptr<Shape>>{shape};
      }
    }
  }
  
}

void SelectionsEngine::clearSelections() {
  
}

void SelectionsEngine::beginSelections() {
  if(!inSelections) {
    inSelections = true;
    for(auto&& it : shapeMap) {
      auto& v = it.second;
      for(auto&& shape : v) {
        shape->inActive();
      }
    }
  }
}

void SelectionsEngine::add(std::shared_ptr<Shape> shape) {
  pendingAdd.insert(shape);
}

void SelectionsEngine::remove(std::shared_ptr<Shape> shape) {
  pendingRemoval.insert(shape);
}

void SelectionsEngine::prunePendingAdd(){
  for(auto&& shape : pendingAdd) {
    auto it = shapeMap.find(shape->getDataPath());
    if( it != shapeMap.end()) {
      auto addingShapes = it ->second;
      for(auto&& sa : addingShapes) {
        sa->active();
        selectionBrush->add(sa);
      }
    }
  }
  pendingAdd.clear();
}

void SelectionsEngine::syncSelections() {
  if(selectionBrush) {
    prunePendingAdd();
    prunePendingRemove();
  }
}

void SelectionsEngine::prunePendingRemove() {
  for(auto&& shape : pendingRemoval) {
    auto it = shapeMap.find(shape->getDataPath());
    if( it != shapeMap.end()) {
      auto removingShapes = it ->second;
      for(auto&& sa : removingShapes) {
        sa->inActive();
        selectionBrush->remove(sa);
      }
    }
  }
  pendingRemoval.clear();
}


std::vector<std::shared_ptr<Shape>> SelectionsEngine::getAllFrom(std::shared_ptr<Shape> shape) {
  auto it = shapeMap.find(shape->getDataPath());
  if(it != shapeMap.end()) {
    return it->second;;
  }
  return std::vector<std::shared_ptr<Shape>>();
}

void SelectionsEngine::activate(std::shared_ptr<Shape> shape, std::unordered_set<std::shared_ptr<Shape>> &collect) {
  auto it = shapeMap.find(shape->getDataPath());
  if(it != shapeMap.end()) {
    for(auto&& s : it->second) {
      s->active();
      collect.insert(s);
    }
  }
}
