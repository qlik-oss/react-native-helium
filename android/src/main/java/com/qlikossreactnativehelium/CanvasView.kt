package com.qlikossreactnativehelium

import android.content.Context
import android.content.res.Resources
import android.graphics.Matrix
import android.graphics.PointF
import android.graphics.Rect
import android.graphics.SurfaceTexture
import android.util.Log
import android.view.*
import android.widget.TextView
import com.facebook.react.bridge.Arguments
import com.facebook.react.bridge.ReactContext
import com.facebook.react.uimanager.events.RCTEventEmitter
import com.facebook.react.uimanager.events.RCTModernEventEmitter

class CanvasView : TextureView, TextureView.SurfaceTextureListener {

  private lateinit var surface: Surface
  lateinit var nativeId: String
  private var gestureDetector: GestureDetector
  private var lasso = false
  private var disabledSelections = false;

  private inner class GestureListener : GestureDetector.SimpleOnGestureListener() {
    @Suppress("DEPRECATION")
    override fun onSingleTapUp(e: MotionEvent?): Boolean {
      if (e != null) {
        if(!disabledSelections) {
          captureView(nativeId)
          beginSelections(e.x, e.y)
        }
        val reactContext = context as ReactContext
        val event = Arguments.createMap()
        reactContext.getJSModule(RCTEventEmitter::class.java).receiveEvent(id, "onBeganSelections", event);
      }
      return false;
    }
    
    override fun onLongPress(e: MotionEvent?) {
      if(e != null) {
        captureView(nativeId)
        val loc = IntArray(2)
        getLocationInWindow(loc)
        var ox = loc[0] 
        var oy = loc[1] 
        var rx = ox + e.x
        var ry = oy + e.y
        rx = pxToDp(context, rx)
        ry = pxToDp(context, ry)
        var x = e.x
        var y = e.y
         
        handleLongPress(x, y, rx, ry)
       
      }
    }
  }

  constructor(context: Context) : super(context) {
    // add coment
    surfaceTextureListener = this
    this.isOpaque = false
    gestureDetector = GestureDetector(context, GestureListener())

  }

  override fun onTouchEvent(event: MotionEvent?): Boolean {
    gestureDetector.onTouchEvent(event)
    if (event != null && lasso && !disabledSelections) {
      when (event.action) {
        MotionEvent.ACTION_DOWN -> {
          Log.d("MotionEVENT", "Started")
          startLasso(event.x, event.y)
        }
        MotionEvent.ACTION_MOVE -> {
          Log.d("MotionEVENT", "Moving")
          updateLasso(event.x, event.y)
        }
        MotionEvent.ACTION_CANCEL -> {
          Log.d("MotionEVENT", "Canceled")
          endLasso(event.x, event.y)
        }
        MotionEvent.ACTION_UP -> {
          Log.d("MotionEVENT", "Up")
          endLasso(event.x, event.y)
        }
      }
    }
    return true
  }

  @Suppress("DEPRECATION")
  override fun onSurfaceTextureAvailable(surface: SurfaceTexture, width: Int, height: Int) {

    this.surface = Surface(surface)
    surfaceAvailable(nativeId, this.surface, width, height);
    val reactContext = context as ReactContext
    val event = Arguments.createMap()
    reactContext.getJSModule(RCTEventEmitter::class.java).receiveEvent(id, "onReady", event);
  }

  @Suppress("DEPRECATION")
  override fun onSurfaceTextureSizeChanged(surface: SurfaceTexture, width: Int, height: Int) {
    surfaceSizedChanged(nativeId, width, height)
    val reactContext = context as ReactContext
    val event = Arguments.createMap()
    reactContext.getJSModule(RCTEventEmitter::class.java).receiveEvent(id, "onResized", event);
  }

  override fun onSurfaceTextureDestroyed(surface: SurfaceTexture): Boolean {
    surfaceDestroyed(nativeId);
    this.surface.release()
    return false
  }

  override fun onSurfaceTextureUpdated(surface: SurfaceTexture) {
  }

  fun setLasso(newVal: Boolean) {
    lasso = newVal
  }
  
  fun setDisableSelections(newVal: Boolean) {
    disabledSelections = newVal;
  }

  fun pxToDp(ctx: Context, px: Float): Float {
    return px / ctx.resources.displayMetrics.density
  }

  external fun surfaceAvailable(id: String, surface: Surface, width: Int, height: Int)
  external fun surfaceDestroyed(id: String)
  external fun surfaceSizedChanged(id: String, width: Int, height: Int);
  external fun captureView(id: String)
  external fun beginSelections(x: Float, y: Float)
  external fun startLasso(x: Float, y: Float)
  external fun updateLasso(x: Float, y: Float)
  external fun endLasso(x: Float, y: Float)
  external fun handleLongPress(x: Float, y: Float, rx: Float, ry: Float)
}
