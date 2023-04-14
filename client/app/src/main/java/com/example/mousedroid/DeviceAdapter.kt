package com.example.mousedroid

import android.content.Context
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.TextView
import androidx.constraintlayout.widget.ConstraintLayout
import androidx.recyclerview.widget.RecyclerView
import java.util.ArrayList

class DeviceAdapter(
    context: Context?,
    devices: ArrayList<Pair<String, String>>,
    listener: OnItemClickListener
) :
    RecyclerView.Adapter<DeviceAdapter.ViewHolder>() {

    private var devices: ArrayList<Pair<String, String>>
    private var context: Context?
    private var listener: OnItemClickListener

    class ViewHolder(view: View) : RecyclerView.ViewHolder(view) {
        var wrapper: ConstraintLayout = view.findViewById(R.id.deviceWrapper)
        var device_name: TextView = view.findViewById(R.id.device_name)
        var device_address: TextView = view.findViewById(R.id.device_address)
    }

    interface OnItemClickListener {
        fun onItemClick(address: String)
        fun onItemLongClick(position: Int)
    }

    init {
        this.devices = devices
        this.context = context
        this.listener = listener
    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): ViewHolder {
        val view: View =
            LayoutInflater.from(parent.context).inflate(R.layout.devicelistitem, parent, false)
        return ViewHolder(view)
    }

    override fun onBindViewHolder(vh: ViewHolder, position: Int) {
        val name = devices[position].first
        val address = devices[position].second

        vh.device_name.text = name
        vh.device_address.text = address

        vh.wrapper.setOnClickListener {
            listener.onItemClick(address)
        }

        vh.wrapper.setOnLongClickListener {
            listener.onItemLongClick(position)
            true
        }
    }

    override fun getItemCount(): Int {
        return devices.size
    }
}