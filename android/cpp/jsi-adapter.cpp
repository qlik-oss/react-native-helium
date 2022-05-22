#include <jni.h>
#include <jsi/jsi.h>
#include <fbjni/fbjni.h>
#include <ReactCommon/CallInvoker.h>
#include <ReactCommon/CallInvokerHolder.h>
#include "CanvasViewManger.h"
#include "CanvasApiHost.h"
#include "JNICanvasViewService.h"

extern "C"
JNIEXPORT void JNICALL
Java_com_qlikossreactnativehelium_CanvasViewManager_registerView(JNIEnv *env, jobject thiz,
                                                                 jstring id, jobject view) {
  const char *utf_string;
  jboolean isCopy;
  utf_string = env->GetStringUTFChars(id, &isCopy);
/* ... use string ... */
  std::string cid(utf_string);
  Helium::TheCanvasViewManager::instance()->add(cid, std::make_shared<CanvasEGLRenderer>());
  if (isCopy == JNI_TRUE) {
    env->ReleaseStringUTFChars(id, utf_string);
  }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_qlikossreactnativehelium_CanvasViewManager_removeView(JNIEnv *env, jobject thiz,
                                                               jstring id) {
  const char *utf_string;
  jboolean isCopy;
  utf_string = env->GetStringUTFChars(id, &isCopy);
/* ... use string ... */
  std::string cid(utf_string);
  Helium::TheCanvasViewManager::instance()->remove(cid);
  if (isCopy == JNI_TRUE) {
    env->ReleaseStringUTFChars(id, utf_string);
  }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_qlikossreactnativehelium_CanvasView_surfaceAvailable(JNIEnv *env, jobject thiz, jstring id,
                                                              jobject surface, jint width,
                                                              jint height) {

  const char *utf_string;
  jboolean isCopy;
  utf_string = env->GetStringUTFChars(id, &isCopy);
  std::string cid(utf_string);
  auto view = Helium::TheCanvasViewManager::instance()->get(cid);
  if (view) {
    view->surfaceAvailable(env, surface, width, height);
  }
  if (isCopy == JNI_TRUE) {
    env->ReleaseStringUTFChars(id, utf_string);
  }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_qlikossreactnativehelium_CanvasView_surfaceDestroyed(JNIEnv *env, jobject thiz,
                                                              jstring id) {
  const char *utf_string;
  jboolean isCopy;
  utf_string = env->GetStringUTFChars(id, &isCopy);
  std::string cid(utf_string);
  auto view = Helium::TheCanvasViewManager::instance()->get(cid);
  if (view) {
    view->surfaceDestroyed();
  }
  if (isCopy == JNI_TRUE) {
    env->ReleaseStringUTFChars(id, utf_string);
  }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_qlikossreactnativehelium_CanvasView_surfaceSizedChanged(JNIEnv *env, jobject thiz,
                                                                 jstring id, jint width,
                                                                 jint height) {
  const char *utf_string;
  jboolean isCopy;
  utf_string = env->GetStringUTFChars(id, &isCopy);
  std::string cid(utf_string);
  auto view = Helium::TheCanvasViewManager::instance()->get(cid);
  if (view) {
    view->surfaceSizeChanged(width, height);
  }
  if (isCopy == JNI_TRUE) {
    env->ReleaseStringUTFChars(id, utf_string);
  }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_qlikossreactnativehelium_ReactNativeHeliumModule_install(JNIEnv *env, jobject thiz,
                                                                  jlong jsi, jfloat scale) {
  using namespace facebook;
  auto jsRuntime = reinterpret_cast<jsi::Runtime *>(jsi);
  Helium::TheCanvasViewManager::instance()->scale = scale;
  CanvasApiHost::install(*jsRuntime);
}

extern "C"
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *) {
  return jni::initialize(vm, [] {
    JNICanvasViewService::registerNatives();
  });
}

extern "C"
JNIEXPORT void JNICALL
Java_com_qlikossreactnativehelium_CanvasView_captureView(JNIEnv *env, jobject thiz, jstring id) {
  const char *utf_string;
  jboolean isCopy;
  utf_string = env->GetStringUTFChars(id, &isCopy);
  std::string cid(utf_string);
  Helium::TheCanvasViewManager::instance()->captureView(cid);
  if (isCopy == JNI_TRUE) {
    env->ReleaseStringUTFChars(id, utf_string);
  }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_qlikossreactnativehelium_CanvasView_beginSelections(JNIEnv *env, jobject thiz, jfloat x,
                                                             jfloat y) {
  Helium::TheCanvasViewManager::instance()->beginSelections(x, y);
}


extern "C"
JNIEXPORT void JNICALL
Java_com_qlikossreactnativehelium_CanvasView_startLasso(JNIEnv *env, jobject thiz, jfloat x,
                                                        jfloat y) {
  Helium::TheCanvasViewManager::instance()->starLasso(x, y);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_qlikossreactnativehelium_CanvasView_updateLasso(JNIEnv *env, jobject thiz, jfloat x,
                                                         jfloat y) {
  Helium::TheCanvasViewManager::instance()->updateLasso(x, y);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_qlikossreactnativehelium_CanvasView_endLasso(JNIEnv *env, jobject thiz, jfloat x,
                                                      jfloat y) {
  Helium::TheCanvasViewManager::instance()->endLasso(x, y);
}
