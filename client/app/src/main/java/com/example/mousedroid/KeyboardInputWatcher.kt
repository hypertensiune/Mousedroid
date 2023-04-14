package com.example.mousedroid

import android.text.Editable
import android.text.TextWatcher
import android.widget.EditText

class KeyboardInputWatcher(editText: EditText): TextWatcher {

    private val TAG = "Mousedroid"

    private var ignoreChange = false

    private val KEY_PRESS = 7

    private val edittext: EditText = editText

    init {
        editText.reset()
    }

    override fun beforeTextChanged(p0: CharSequence?, p1: Int, p2: Int, p3: Int) { }

    override fun onTextChanged(text: CharSequence, start: Int, lengthBefore: Int, lengthAfter: Int) {
        if(!ignoreChange){
            if(lengthAfter < lengthBefore){
                TcpClient.write(KEY_PRESS, 127) // DEL
                if(start == 1 && lengthAfter == 0){
                    edittext.reset()
                }
            }
            else {
                val c = if(text.isNotEmpty()) text[text.length - 1] else null
                if(c == '\n'){
                    TcpClient.write(KEY_PRESS, 10) // \n
                    edittext.reset()
                }
                else{
                    if(lengthAfter == lengthBefore + 1){
                        sendCharBuffer(text[text.length - 1].toString())
                    }
                    else{
                        sendCharBuffer(text.substring(start))
                    }
                }
            }
        }
    }

    override fun afterTextChanged(p0: Editable?) { }

    private fun EditText.reset(){
        ignoreChange = true
        edittext.setText("//")
        edittext.setSelection(edittext.length())
        ignoreChange = false
    }

    private fun sendCharBuffer(string: String){
        val out = string.map { it.code }.toIntArray()
        TcpClient.write(KEY_PRESS, *out.toTypedArray())
    }
}