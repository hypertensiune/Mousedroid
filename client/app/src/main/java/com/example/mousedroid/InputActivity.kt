package com.example.mousedroid

import android.os.Bundle
import android.util.Log
import android.view.View
import android.view.inputmethod.InputMethodManager
import android.widget.CheckBox
import android.widget.CompoundButton
import android.widget.EditText
import androidx.appcompat.app.AppCompatActivity
import androidx.fragment.app.Fragment

class InputActivity: AppCompatActivity() {
    private lateinit var softinputView: EditText

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_input)

        softinputView = findViewById(R.id.softinput_view)
        val kiw = KeyboardInputWatcher(softinputView)
        softinputView.addTextChangedListener(kiw)

        TcpClient.addOnConnectionLostListener {
            finish()
        }

        findViewById<CheckBox>(R.id.openKeyboard).apply {
            setOnCheckedChangeListener { compoundButton: CompoundButton, checked: Boolean ->
                val imm = getSystemService(INPUT_METHOD_SERVICE) as InputMethodManager
                if(checked){
                    softinputView.requestFocus()
                    imm.showSoftInput(softinputView, InputMethodManager.SHOW_FORCED)
                }
                else{
                    imm.hideSoftInputFromWindow(windowToken, 0)
                }
            }
        }

        findViewById<CheckBox>(R.id.openNumpad).setOnCheckedChangeListener { compoundButton: CompoundButton, checked: Boolean ->
            if (checked)
                changeActiveInputFragment(NumpadFragment())
            else
                changeActiveInputFragment(TouchpadFragment(baseContext))
        }

        changeActiveInputFragment(TouchpadFragment(baseContext))
    }

    private fun changeActiveInputFragment(fragment: Fragment) {
        supportFragmentManager.beginTransaction().replace(R.id.fragment_container, fragment)
            .commit()
    }

    fun numpadButtonClickListener(view: View) = NumpadFragment.onNumpadButtonClickListener(view)
}