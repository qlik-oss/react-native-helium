//
// Created by Vittorio Cellucci on 2022-04-29.
//

#ifndef HELIUM4_CANVASEGLRENDERER_H
#define HELIUM4_CANVASEGLRENDERER_H

#include "android/native_window.h"
#include "EGL/egl.h"
#include "GLES2/gl2.h"
#include <thread>
#include <unordered_map>
#include <jni.h>
#include <jsi/jsi.h>
#include "include/gpu/GrDirectContext.h"
#include "include/gpu/gl/GrGLInterface.h"
#include <include/core/SkColor.h>
#include <include/core/SkCanvas.h>
#include <include/core/SkSurface.h>
#include <include/core/SkSize.h>
#include "SkiaRenderView.h"
#include "Rect.h"
#include "RenderThread.h"
#include "ShapeFactory.h"

using namespace facebook;

class CanvasEGLRenderer {
protected:
  std::shared_ptr<RenderThread> renderThread;
  EGLSurface cglSurface = EGL_NO_SURFACE;
  std::function<void()> removal;

public:
  CanvasEGLRenderer();

  ~CanvasEGLRenderer();

  void surfaceAvailable(JNIEnv *env, jobject surface, int width, int height);

  void surfaceSizeChanged(int width, int height);

  void surfaceDestroyed(bool releaseWindow = true);

  void draw();

  void addShapes(const std::string &vid, jsi::Runtime &rt, const jsi::Value &value);

  void clear(const std::string &vid);

  std::shared_ptr<SkiaRenderView>
  createVirtualRender(const std::string &vid, const SkRect &bounds, const SkMatrix &matrix);

  void destroy(const std::string &vid);

  void resize(const std::string &vide, const SkRect &rect);

  void setSelectionBrush(jsi::Runtime& rt, const jsi::Value& value);

  void confirmSelections();

  void beginSelections(float x, float y);

  void startLasso(float x, float y);
  void updateLasso(float x, float y);
  void endLasso(float x, float y);

  std::shared_ptr<SkiaRenderView> get() {
    return skiaRender;
  }

  SkISize getSize() {
    return SkISize::Make(width, height);
  }

  void stop() {
    stopped = true;
  }

  void setRenderThread(std::shared_ptr<RenderThread> rt) {
    renderThread = rt;
  }

  void markForRemoval(std::function<void()> f) {
    removal = f;
    removeMe();
  }


protected:
  bool initSkiaEAGL(int width, int height);

  bool resetSkiaSurface(int widht, int height);

  bool onDemandSurface();

  bool onDemandEGL();

  void removeMe();

protected:
  ShapeFactory shapeFactory;
  GrBackendRenderTarget skRenderTarget;
  sk_sp<SkSurface> skSurface;
  std::atomic_bool stopped = false;
  ANativeWindow *surfaceTexture = nullptr;
  std::shared_ptr<SkiaRenderView> skiaRender;
  int width = -1;
  int height = -1;
  std::shared_ptr<SelectionBrush> selectionBrush;
};


#endif //HELIUM4_CANVASEGLRENDERER_H
