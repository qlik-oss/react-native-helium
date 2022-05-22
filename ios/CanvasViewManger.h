//
// Created by Vittorio Cellucci on 2022-05-01.
//

#ifndef HELIUM4_CANVASVIEWMANGER_H
#define HELIUM4_CANVASVIEWMANGER_H
#include <map>
#include <memory>
#include <string>
#include <thread>
#include <algorithm>
#include "Singleton.h"
#include "MetalRenderer.h"
#include "Platform.h"

namespace Helium {
template<typename T>
class CanvasViewManagerT {
public:
  float scale = 1;
  std::shared_ptr<Platform> platform;
  
  void add(const std::string& id,  std::shared_ptr<T> view) {
    view->setNativeId(id);
    views[id] = view;
  }
  
  void remove(const std::string& id) {
    auto it = views.find(id);
    if(it != views.end()) {
      views.erase(it);
    }
  }
  
  std::shared_ptr<T> get(const std::string& id) {
    auto  it = views.find(id);
    if(it != views.end()) {
      return it->second;
    }
    return std::shared_ptr<T>();
  }
  
  void runOnJS(std::function<void()>&& f) {
    platform->jsCallInvoker->invokeAsync(std::move(f));
  }
  
protected:
  std::map<std::string, std::shared_ptr<T>> views;
};

using CanvasViewManager = CanvasViewManagerT<MetalRenderer>;
using TheCanvasViewManager = Singleton<CanvasViewManager>;
}


#endif //HELIUM4_CANVASVIEWMANGER_H
