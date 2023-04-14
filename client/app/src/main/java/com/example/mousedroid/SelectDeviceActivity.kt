package com.example.mousedroid

import android.app.Activity
import android.content.Context
import android.content.Intent
import android.os.Bundle
import android.view.*
import android.widget.Button
import android.widget.PopupWindow
import androidx.appcompat.app.AppCompatActivity
import androidx.constraintlayout.widget.ConstraintLayout
import androidx.core.widget.addTextChangedListener
import androidx.recyclerview.widget.DefaultItemAnimator
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import com.google.android.material.button.MaterialButton
import com.google.android.material.textfield.TextInputEditText
import com.google.gson.Gson
import com.google.gson.reflect.TypeToken

class SelectDeviceActivity : AppCompatActivity() {

    private lateinit var deviceList: RecyclerView
    private lateinit var addButton: Button
    private lateinit var adapter: DeviceAdapter
    private lateinit var devices: ArrayList<Pair<String, String>>
    private var gson: Gson = Gson()

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.device_list_fragment)

        deviceList = findViewById(R.id.deviceList)
        addButton = findViewById(R.id.addDeviceBtn)

        val res = getPreferences(Context.MODE_PRIVATE)?.getString("DEVICES", "[]")

        res?.let {
            devices = gson.fromJson(res, object: TypeToken<ArrayList<Pair<String, String>>>(){}.type)
        }

        adapter = DeviceAdapter(this, devices, object : DeviceAdapter.OnItemClickListener {
            override fun onItemClick(address: String) {
                val i = Intent()
                i.putExtra("result", address)
                setResult(Activity.RESULT_OK, i)
                finish()
            }

            override fun onItemLongClick(position: Int) {
                showDeleteDialog(position)
            }
        })

        deviceList.layoutManager = LinearLayoutManager(this)
        deviceList.adapter = adapter
        deviceList.itemAnimator = DefaultItemAnimator()

        addButton.setOnClickListener { showAddDeviceDialog() }
    }

    private fun showDeleteDialog(position: Int){
        val pView = layoutInflater.inflate(R.layout.device_delete_fragment, null)
        val popup = PopupWindow(
            pView,
            ConstraintLayout.LayoutParams.MATCH_PARENT,
            ConstraintLayout.LayoutParams.WRAP_CONTENT,
            true
        )

        pView.findViewById<MaterialButton>(R.id.deviceDeleteConfirm).setOnClickListener {
            devices.removeAt(position)
            adapter.notifyItemChanged(position)

            val str = gson.toJson(devices).toString()
            getPreferences(Context.MODE_PRIVATE)?.edit()?.putString("DEVICES", str)?.apply()

            popup.dismiss()
        }

        pView.findViewById<MaterialButton>(R.id.cancelDelete).setOnClickListener {
            popup.dismiss()
        }

        popup.showAtLocation(pView, Gravity.CENTER, 0, 0)
        popup.dim(0.6f)
    }

    private fun showAddDeviceDialog() {
        val pView = layoutInflater.inflate(R.layout.device_add_fragment, null)
        val popup = PopupWindow(
            pView,
            ConstraintLayout.LayoutParams.MATCH_PARENT,
            ConstraintLayout.LayoutParams.WRAP_CONTENT,
            true
        )

        val address: TextInputEditText = pView.findViewById(R.id.textAddress)
        val name: TextInputEditText = pView.findViewById(R.id.textName)
        val deviceAddBtn: MaterialButton = pView.findViewById(R.id.deviceAddConfirm)

        address.addTextChangedListener {
            val ip = it?.toString()
            deviceAddBtn.isEnabled =
                ip?.matches(Regex("^(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\$")) ?: false
        }

        deviceAddBtn.setOnClickListener {
            devices.add(name.text.toString() to address.text.toString())
            adapter.notifyItemInserted(devices.size - 1)

            val str = gson.toJson(devices).toString()
            getPreferences(Context.MODE_PRIVATE)?.edit()?.putString("DEVICES", str)?.apply()

            popup.dismiss()
        }

        pView.findViewById<MaterialButton>(R.id.cancelAdd).setOnClickListener {
            popup.dismiss()
        }

        // !!!
        popup.softInputMode = WindowManager.LayoutParams.SOFT_INPUT_ADJUST_RESIZE
        popup.showAtLocation(pView, Gravity.BOTTOM, 0, 0)
        popup.dim(0.6f)
    }

    // https://stackoverflow.com/questions/35874001/dim-the-background-using-popupwindow-in-android
    private fun PopupWindow.dim(amount: Float) {
        val container = contentView.rootView
        val context = contentView.context
        val wm = context.getSystemService(Context.WINDOW_SERVICE) as WindowManager
        val p = container.layoutParams as WindowManager.LayoutParams
        p.flags = p.flags or WindowManager.LayoutParams.FLAG_DIM_BEHIND
        p.dimAmount = amount
        wm.updateViewLayout(container, p)
    }
}