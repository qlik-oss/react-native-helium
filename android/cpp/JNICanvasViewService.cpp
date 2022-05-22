//
// Created by Vittorio Cellucci on 2022-05-20.
//

#include "JNICanvasViewService.h"
#include "CanvasViewManger.h"

void JNICanvasViewService::registerNatives() {
  javaClassStatic()->registerNatives({
                                             makeNativeMethod("initHybrid",
                                                              JNICanvasViewService::initHybrid),
                                     });
}


JNICanvasViewService::JNICanvasViewService(jni::alias_ref<JNICanvasViewService::jhybridobject> jThis) {

}


jni::local_ref<jni::HybridClass<JNICanvasViewService>::jhybriddata> JNICanvasViewService::initHybrid( jni::alias_ref<jhybridobject> jThis,
                                                                                                      jlong jsContext, JSCallInvokerHolder jsCallInvokerHolder) {

  Helium::TheCanvasViewManager::instance()->setCallInvoker(jsCallInvokerHolder->cthis()->getCallInvoker());
  return makeCxxInstance();
}
