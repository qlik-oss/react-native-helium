package com.qlikossreactnativehelium

import com.facebook.jni.HybridData
import com.facebook.react.bridge.ReactContext
import com.facebook.react.turbomodule.core.CallInvokerHolderImpl

class CanvasViewService {
  constructor(reactContext: ReactContext) {
    this.initializeJNI(reactContext);
  }

  private fun initializeJNI(reactContext: ReactContext) {
    val jsiContextHolder = reactContext.catalystInstance.jsCallInvokerHolder as CallInvokerHolderImpl
    initHybrid(reactContext.javaScriptContextHolder.get(), jsiContextHolder);
  }
  private external fun initHybrid(jsContext: Long, callInvokerHolderImpl: CallInvokerHolderImpl) : HybridData
}
