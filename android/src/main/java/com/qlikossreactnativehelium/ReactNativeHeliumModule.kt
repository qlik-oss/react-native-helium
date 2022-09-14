package com.qlikossreactnativehelium

import com.facebook.react.bridge.ReactApplicationContext
import com.facebook.react.bridge.ReactContextBaseJavaModule
import com.facebook.react.bridge.ReactMethod
import com.facebook.react.turbomodule.core.CallInvokerHolderImpl

class ReactNativeHeliumModule(reactContext: ReactApplicationContext) : ReactContextBaseJavaModule(reactContext) {

  lateinit var canvasViewService: CanvasViewService;
  override fun getName(): String {
    return "ReactNativeHelium"
  }

  external fun install(jsi: Long, scale: Float)

  companion object {
    init {
        System.loadLibrary("helium")
    }
  }
  
  @ReactMethod(isBlockingSynchronousMethod = true)
  fun installModule() {
    this.canvasViewService = CanvasViewService(this.reactApplicationContext)
    val holder = this.reactApplicationContext.javaScriptContextHolder.get();
    val scale = this.reactApplicationContext.resources.displayMetrics.density;
    install(holder, scale) 
  }

}
