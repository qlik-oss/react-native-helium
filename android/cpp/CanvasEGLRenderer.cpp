//
// Created by Vittorio Cellucci on 2022-04-29.
//

#include "CanvasEGLRenderer.h"
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <android/log.h>
#include <random>
#include "ShapeFactory.h"
#include "CanvasViewManger.h"

#define TAG "Helium"


#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,    TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,     TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,     TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,    TAG, __VA_ARGS__)

CanvasEGLRenderer::CanvasEGLRenderer() {

}

CanvasEGLRenderer::~CanvasEGLRenderer() {
}

bool CanvasEGLRenderer::onDemandEGL() {
  EGLint contextAttribs[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};
  auto canvasEglContext = renderThread->ctx();
  if (canvasEglContext->glDisplay == EGL_NO_DISPLAY) {
    canvasEglContext->glDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (canvasEglContext->glDisplay == EGL_NO_DISPLAY) {
      LOGE("No display");
      return false;
    }

    EGLint major;
    EGLint minor;
    if (!eglInitialize(canvasEglContext->glDisplay, &major, &minor)) {
      LOGE("Not initialized");
      return false;
    }

    EGLint att[] = {
            EGL_RENDERABLE_TYPE,
            EGL_OPENGL_ES2_BIT,
            EGL_SURFACE_TYPE,
            EGL_WINDOW_BIT,
            EGL_ALPHA_SIZE,
            8,
            EGL_BLUE_SIZE,
            8,
            EGL_GREEN_SIZE,
            8,
            EGL_RED_SIZE,
            8,
            EGL_DEPTH_SIZE,
            0,
            EGL_STENCIL_SIZE,
            0,
            EGL_NONE};
    EGLint numConfigs;
    canvasEglContext->glConfig = 0;
    if (!eglChooseConfig(canvasEglContext->glDisplay, att, &canvasEglContext->glConfig, 1,
                         &numConfigs)) {
      LOGE("No config");
      return false;
    }
  }

  if (canvasEglContext->glContext == EGL_NO_CONTEXT) {


    canvasEglContext->glContext = eglCreateContext(canvasEglContext->glDisplay,
                                                   canvasEglContext->glConfig, NULL,
                                                   contextAttribs);


    if (canvasEglContext->glContext == EGL_NO_CONTEXT) {
      EGLint error = eglGetError();
      LOGE("No context %d", error);
      return false;
    }


    LOGD("All good!!!!");
  }

  return true;
}


void CanvasEGLRenderer::surfaceAvailable(JNIEnv *env, jobject surface, int width, int height) {

  surfaceTexture = ANativeWindow_fromSurface(env, surface);

  SkRect bounds = SkRect::MakeWH(width, height);
  skiaRender = std::make_shared<SkiaRenderView>(bounds);
  this->width = width;
  this->height = height;
  renderThread->push([this]{
    if(!onDemandEGL()) {
      return ;
    }
    onDemandSurface();
  });
}

bool CanvasEGLRenderer::onDemandSurface() {
  auto canvasEglContext = renderThread->ctx();
  if (cglSurface == EGL_NO_SURFACE) {

    cglSurface = eglCreateWindowSurface(canvasEglContext->glDisplay, canvasEglContext->glConfig,
                                        surfaceTexture,
                                        nullptr);
    if (cglSurface == EGL_NO_SURFACE) {
      LOGE("Failed to make current surface");
      return false;
    }

    if (!eglMakeCurrent(canvasEglContext->glDisplay, cglSurface, cglSurface,
                        canvasEglContext->glContext)) {
      LOGE("Failed to make current");
      return false;
    }
    return initSkiaEAGL(width, height);
  }

  return true;
}

bool CanvasEGLRenderer::initSkiaEAGL(int width, int height) {
  auto canvasEglContext = renderThread->ctx();
  auto backendInterface = GrGLMakeNativeInterface();
  canvasEglContext->skContext = GrDirectContext::MakeGL(backendInterface);
  if (canvasEglContext->skContext == nullptr) {
    LOGE("There was a problem initialize skia");
    return false;
  }

  return resetSkiaSurface(width, height);
}

bool CanvasEGLRenderer::resetSkiaSurface(int width, int height) {
  auto canvasEglContext = renderThread->ctx();
  if (!eglMakeCurrent(canvasEglContext->glDisplay, cglSurface, cglSurface,
                      canvasEglContext->glContext)) {
    LOGE("Failed to make current");
    return false;
  }
  glViewport(0, 0, width, height);

  GLint buffer;
  glGetIntegerv(GL_FRAMEBUFFER_BINDING, &buffer);

  GLint stencil;
  glGetIntegerv(GL_STENCIL_BITS, &stencil);

  GLint samples;
  glGetIntegerv(GL_SAMPLES, &samples);

  auto maxSamples = canvasEglContext->skContext->maxSurfaceSampleCountForColorType(
          kRGBA_8888_SkColorType);
  samples = std::max(samples, maxSamples);
  GrGLFramebufferInfo framebufferInfo;
  framebufferInfo.fFBOID = buffer;
  framebufferInfo.fFormat = 0x8058;
  skRenderTarget = GrBackendRenderTarget(width, height, samples, stencil, framebufferInfo);
  skSurface = SkSurface::MakeFromBackendRenderTarget(canvasEglContext->skContext.get(),
                                                     skRenderTarget, kBottomLeft_GrSurfaceOrigin,
                                                     kRGBA_8888_SkColorType,
                                                     nullptr, nullptr);
  if (!skSurface) {
    LOGE("Failed to create skia surface");
    return false;
  }
  return true;
}

void CanvasEGLRenderer::clear(const std::string &vid) {
  renderThread->push([this, vid] {
    auto view = skiaRender->get(vid);
    if (view) {
      view->clear();
    } else {
      skiaRender->clear();
    }
  });
}

void
CanvasEGLRenderer::addShapes(const std::string &vid, jsi::Runtime &rt, const jsi::Value &value) {
  auto shapes = shapeFactory.buildShapes(rt, value);
  renderThread->push([this, vid, shapes] {
    skiaRender->addShapes(vid, std::move(shapes));
  });
}

void CanvasEGLRenderer::draw() {
  renderThread->push([this] {
    auto canvasEglContext = renderThread->ctx();
    if (!onDemandEGL()) {
      return;
    }

    if (!onDemandSurface()) {
      return;
    }

    eglMakeCurrent(canvasEglContext->glDisplay, cglSurface, cglSurface,
                   canvasEglContext->glContext);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    auto canvas = skSurface->getCanvas();
    canvas->clear(SK_AlphaTRANSPARENT);
    skiaRender->draw(canvas);
    canvas->flush();
    canvasEglContext->skContext->flush();
    if (!eglSwapBuffers(canvasEglContext->glDisplay, cglSurface)) {
      int error = eglGetError();
      LOGE("Failed to swap %d", error);
    }
  });
}

void CanvasEGLRenderer::surfaceDestroyed(bool releaseWindow) {
  // don't call this in the thread, since it's not a JNI thread
  if (surfaceTexture && releaseWindow) {
    ANativeWindow_release(surfaceTexture);
    surfaceTexture = nullptr;
  }

  renderThread->push([this] {
    auto canvasEglContext = renderThread->ctx();
    if (canvasEglContext->glDisplay != EGL_NO_DISPLAY) {
      if (cglSurface != EGL_NO_SURFACE) {
        eglDestroySurface(canvasEglContext->glDisplay, cglSurface);
        cglSurface = EGL_NO_SURFACE;
      }
      if(canvasEglContext->glContext != EGL_NO_CONTEXT) {
        eglDestroyContext(canvasEglContext->glDisplay, canvasEglContext->glContext);
        canvasEglContext->glContext = EGL_NO_CONTEXT;
      }
      if(canvasEglContext->glDisplay != EGL_NO_DISPLAY) {
        eglTerminate(canvasEglContext->glDisplay);
        canvasEglContext->glDisplay = EGL_NO_DISPLAY;
      }
      skSurface = nullptr;

    }
  });
}

void CanvasEGLRenderer::surfaceSizeChanged(int width, int height) {
  // clear all
  this->width = width;
  this->height = height;
  renderThread->push([this, width, height] {
    skiaRender->resetBound(SkRect::MakeWH(width, height));
    auto canvasEglContext = renderThread->ctx();
    if(canvasEglContext->glDisplay != EGL_NO_DISPLAY) {

      if(cglSurface != EGL_NO_SURFACE) {
        eglMakeCurrent(canvasEglContext->glDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        eglDestroySurface(canvasEglContext->glDisplay, cglSurface);
        cglSurface = EGL_NO_SURFACE;
      }

      if(!surfaceTexture) {
        return;
      }
      cglSurface = eglCreateWindowSurface(canvasEglContext->glDisplay, canvasEglContext->glConfig,
                                          surfaceTexture,
                                          nullptr);
      if (cglSurface == EGL_NO_SURFACE) {
        LOGE("Failed to create new surface %d", eglGetError());
        return;
      }
      if (!eglMakeCurrent(canvasEglContext->glDisplay, cglSurface, cglSurface,
                          canvasEglContext->glContext)) {
        LOGE("Failed to make current %d", eglGetError());
        return;
      }
      if (canvasEglContext->skContext) {
        canvasEglContext->skContext->resetContext();
      }

      resetSkiaSurface(width, height);
    }
  });
}

std::shared_ptr<SkiaRenderView>
CanvasEGLRenderer::createVirtualRender(const std::string &vid, const SkRect &bounds,
                                       const SkMatrix &matrix) {
  renderThread->push([this, vid, bounds, matrix] {
    skiaRender->createVirtualRender(vid, bounds, matrix);
  });
  return std::shared_ptr<SkiaRenderView>();
}

void CanvasEGLRenderer::destroy(const std::string &vid) {
  renderThread->push([this, vid] {
    skiaRender->destroy(vid);
  });
}

void CanvasEGLRenderer::resize(const std::string &vid, const SkRect &rect) {
  renderThread->push([this, vid, rect] {
    skiaRender->resize(vid, rect);
  });
}

void CanvasEGLRenderer::setSelectionBrush(jsi::Runtime &rt, const jsi::Value &value) {
  selectionBrush = std::make_shared<SelectionBrush>(rt, value);
  renderThread->push([this]{
    skiaRender->setSelectionBrush(selectionBrush);
  });
}

void CanvasEGLRenderer::confirmSelections() {
  Helium::TheCanvasViewManager::instance()->releaseCapture();
  if(selectionBrush) {
    selectionBrush->confirmSelections();
  }
}

void CanvasEGLRenderer::beginSelections(float x, float y) {
  auto point = SkPoint::Make(x, y);
  skiaRender->beginSelections(point);
  Helium::TheCanvasViewManager::instance()->runOnJS([this]{
    skiaRender->syncBrush();
    draw();
  });
}

void CanvasEGLRenderer::clearSelections() {
  skiaRender->clearSelections();
  draw();
}

void CanvasEGLRenderer::startLasso(float x, float y) {
  if(skiaRender) {
    skiaRender->startLasso(x, y);
    draw();
  }
}

void CanvasEGLRenderer::updateLasso(float x, float y) {
  skiaRender->updateLasso(x, y);
  draw();
}

void CanvasEGLRenderer::endLasso(float x, float y) {
  skiaRender->endLasso(x, y);
  // endLasso does some work on the JS thread so
  // we defer the draw call until after the endLasso has completed
  Helium::TheCanvasViewManager::instance()->runOnJS([this]{
    draw();
  });
}



