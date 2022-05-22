package com.qlikossreactnativehelium

import com.facebook.react.bridge.ReadableMap
import com.facebook.react.common.MapBuilder
import com.facebook.react.uimanager.BaseViewManager
import com.facebook.react.uimanager.LayoutShadowNode
import com.facebook.react.uimanager.ThemedReactContext
import com.facebook.react.uimanager.annotations.ReactProp

class CanvasViewManager : BaseViewManager<CanvasView, LayoutShadowNode>() {
  var viewMap = HashMap<CanvasView, String>()
  override fun getName(): String {
    return "CanvasView"
  }

  override fun createShadowNodeInstance(): LayoutShadowNode {
    return LayoutShadowNode()
  }

  override fun getShadowNodeClass(): Class<out LayoutShadowNode> {
    return LayoutShadowNode::class.java
  }

  override fun createViewInstance(reactContext: ThemedReactContext): CanvasView {
    return CanvasView(reactContext)
  }

  override fun updateExtraData(root: CanvasView, extraData: Any?) {
  }

  override fun setNativeId(view: CanvasView, nativeId: String?) {
    super.setNativeId(view, nativeId)
    if (nativeId != null) {
      view.nativeId = nativeId
      registerView(nativeId, view)
      viewMap.put(view, nativeId)
    }
  }

  override fun onDropViewInstance(view: CanvasView) {
    super.onDropViewInstance(view)
    var id = viewMap.get(view)
    if(id != null) {
      removeView(id)
      viewMap.remove(view)
    }
  }

  @ReactProp(name = "lasso")
  fun setTheme(view: CanvasView, enabled: Boolean ) {
   view.setLasso(enabled);
  }

  override fun getExportedCustomDirectEventTypeConstants(): Map<String, Any>? {
    return MapBuilder.of(
      "onReady", MapBuilder.of("registrationName", "onReady"),
      "onResized", MapBuilder.of("registrationName", "onResized"),
      "onBeganSelections", MapBuilder.of("registrationName", "onBeganSelections")
    )
  }

  external fun registerView(id: String, view: CanvasView)
  external fun removeView(id: String)
}
