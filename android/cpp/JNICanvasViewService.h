//
// Created by Vittorio Cellucci on 2022-05-20.
//

#ifndef GALAXY_JNICANVASVIEWSERVICE_H
#define GALAXY_JNICANVASVIEWSERVICE_H

#include <fbjni/fbjni.h>
#include <ReactCommon/CallInvokerHolder.h>
using namespace facebook;

using JSCallInvokerHolder = jni::alias_ref<facebook::react::CallInvokerHolder::javaobject>;

class JNICanvasViewService : public jni::HybridClass<JNICanvasViewService> {
public:
  static constexpr auto kJavaDescriptor = "Lcom/qlikossreactnativehelium/CanvasViewService;";

  JNICanvasViewService() = default;
  explicit JNICanvasViewService(jni::alias_ref<JNICanvasViewService::jhybridobject> jThis);

  static void registerNatives();

  static  jni::local_ref<jni::HybridClass<JNICanvasViewService>::jhybriddata> initHybrid( jni::alias_ref<jhybridobject> jThis,
                                                                                          jlong jsContext, JSCallInvokerHolder jsCallInvokerHolder);

};


#endif //GALAXY_JNICANVASVIEWSERVICE_H
