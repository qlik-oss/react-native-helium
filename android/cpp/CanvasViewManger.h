//
// Created by Vittorio Cellucci on 2022-05-01.
//

#ifndef HELIUM4_CANVASVIEWMANGER_H
#define HELIUM4_CANVASVIEWMANGER_H

#include <unordered_map>
#include <memory>
#include <string>
#include <thread>
#include <algorithm>
#include <ReactCommon/CallInvoker.h>
#include "Singleton.h"

#ifdef ANDROID_HELIUM

# include "CanvasEGLRenderer.h"

#else
# include "MetalRenderer.h"
#endif
namespace Helium {
  template<typename T>
  class CanvasViewManagerT {
  public:
    float scale = 1;
    int currentThreadIndex = 0;

    CanvasViewManagerT() {
      int maxThreads = std::max<unsigned int>(std::thread::hardware_concurrency(), 2);
      for (int i = 0; i < maxThreads; i++) {
        renderThreads.template emplace_back(std::make_shared<RenderThread>());
      }
    }

    void add(const std::string &id, std::shared_ptr<T> view) {
      std::unique_lock<std::mutex> lk(mk);

      view->setRenderThread(renderThreads[currentThreadIndex]);
      currentThreadIndex += (currentThreadIndex + 1) % renderThreads.size();
      views[id] = view;
    }

    void remove(const std::string &id) {
      std::unique_lock<std::mutex> lk(mk);
      auto it = views.find(id);
      if (it != views.end()) {
        auto view = it->second;
        view->markForRemoval([this, id] {
          std::unique_lock<std::mutex> lk(mk);
          auto dit = views.find(id);
          if (dit != views.end()) {
            views.erase(dit);
          }
        });
      }
    }

    std::shared_ptr<T> get(const std::string &id) {
      std::unique_lock<std::mutex> lk(mk);
      auto it = views.find(id);
      if (it != views.end()) {
        return it->second;
      }
      return std::shared_ptr<T>();
    }

    void captureView(const std::string &id) {
      std::unique_lock<std::mutex> lk(mk);
      auto it = views.find(id);
      if (it != views.end()) {
        capturedView = it->second;
      }
    }

    void releaseCapture() {
      std::unique_lock<std::mutex> lk(mk);
      capturedView = nullptr;
    }

    void beginSelections(float x, float y) {
      if(capturedView) {
        capturedView->beginSelections(x, y);
      }
    }

    void starLasso(float x, float y) {
      if(capturedView) {
        capturedView->startLasso(x, y);
      }
    }

    void updateLasso(float x, float y) {
      if(capturedView) {
        capturedView->updateLasso(x, y);
      }
    }

    void endLasso(float x, float y) {
      if(capturedView) {
        capturedView->endLasso(x, y);
      }
    }

    void setCallInvoker(std::shared_ptr<facebook::react::CallInvoker> ci) {
      jsCallInvoker = ci;
    }

    void runOnJS(std::function<void()> &&runner) {
      jsCallInvoker->invokeAsync(std::move(runner));
    }

  protected:
    std::mutex mk;
    std::unordered_map<std::string, std::shared_ptr<T>> views;
    std::vector<std::shared_ptr<RenderThread>> renderThreads;
    std::shared_ptr<facebook::react::CallInvoker> jsCallInvoker;
    std::shared_ptr<T> capturedView;
  };

#ifdef ANDROID_HELIUM
  using CanvasViewManager = CanvasViewManagerT<CanvasEGLRenderer>;
#else
  using CanvasViewManager = CanvasViewManagerT<MetalRenderer>;
#endif

  using TheCanvasViewManager = Singleton<CanvasViewManager>;
}


#endif //HELIUM4_CANVASVIEWMANGER_H
