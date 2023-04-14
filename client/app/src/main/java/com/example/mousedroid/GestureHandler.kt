package com.example.mousedroid

import android.content.Context
import android.view.*
import androidx.core.view.GestureDetectorCompat

class GestureHandler(context: Context): GestureDetector.SimpleOnGestureListener() {

    val detector: GestureDetectorCompat = GestureDetectorCompat(context, this)
    private lateinit var view: View

    var scrolled = false
    var isLongPressed = false
    var isMovingAfterLongPress = false

    private val TAG = "Mousedroid"

    companion object {
        const val CLICK = 1
        const val RIGHT_CLICK = 2
        const val DOWN = 3
        const val UP = 4
        const val MOVE = 5
        const val SCROLL = 6
    }

    fun bindView(_view: View){
        view = _view.findViewById(R.id.touchpadFragment)
    }

    override fun onSingleTapUp(e: MotionEvent): Boolean {
        TcpClient.write(CLICK)
        return super.onSingleTapUp(e)
    }

    override fun onDoubleTap(e: MotionEvent): Boolean {
        TcpClient.write(CLICK)
        return true
    }

    override fun onLongPress(e: MotionEvent) {
        TcpClient.write(DOWN)
        isLongPressed = true
        view.performHapticFeedback(HapticFeedbackConstants.LONG_PRESS, HapticFeedbackConstants.FLAG_IGNORE_GLOBAL_SETTING)
    }

    override fun onScroll(
        e1: MotionEvent,
        e2: MotionEvent,
        distanceX: Float,
        distanceY: Float
    ): Boolean {
        if(e1.pointerCount == 1 && e2.pointerCount == 1)
            TcpClient.write(MOVE, distanceX, distanceY)
        else {
            TcpClient.write(SCROLL, -distanceY)
            scrolled = true
        }

        return super.onScroll(e1, e2, distanceX, distanceY)
    }
}