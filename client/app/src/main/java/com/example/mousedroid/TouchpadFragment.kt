package com.example.mousedroid

import android.content.Context
import android.content.pm.ActivityInfo
import android.os.Bundle
import android.view.*
import androidx.fragment.app.Fragment

class TouchpadFragment(context: Context) : Fragment(), View.OnTouchListener {

    private var gestureHandler: GestureHandler = GestureHandler(context)
    private val TAG = "Mousedroid"


    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
    }

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        activity?.requestedOrientation = ActivityInfo.SCREEN_ORIENTATION_SENSOR;

        val view = inflater.inflate(R.layout.touchpad_fragment, container, false)

        view.setOnTouchListener(this)

        gestureHandler.bindView(view)

        return view
    }

    override fun onTouch(p0: View?, p1: MotionEvent): Boolean {
        if(p1.actionMasked == MotionEvent.ACTION_POINTER_UP && p1.pointerCount == 2){
            if(!gestureHandler.scrolled){
                TcpClient.write(GestureHandler.RIGHT_CLICK)
            }
            else{
                gestureHandler.scrolled = false
            }
            return false
        }

        if(gestureHandler.detector.onTouchEvent(p1)){
            return true
        }

        when(p1.action) {
            MotionEvent.ACTION_MOVE -> {
                if(gestureHandler.isLongPressed){
                    gestureHandler.isLongPressed = false
                    gestureHandler.isMovingAfterLongPress = true
                    val cancelEvent = MotionEvent.obtain(p1)
                    cancelEvent.action = MotionEvent.ACTION_CANCEL
                    gestureHandler.detector.onTouchEvent(cancelEvent)
                }
            }
            MotionEvent.ACTION_UP -> {
                if(gestureHandler.isLongPressed || gestureHandler.isMovingAfterLongPress){
                    gestureHandler.isMovingAfterLongPress = false
                    gestureHandler.isLongPressed = false
                    TcpClient.write(GestureHandler.UP)
                }
            }
        }

        return true
    }
}