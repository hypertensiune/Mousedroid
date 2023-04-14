package com.example.mousedroid

import android.util.Log
import java.io.*
import java.net.*

object TcpClient : Runnable {

    private var PORT = 6969
    private var IP: String? = null

    private const val TAG = "Mousedroid"

    const val USB = 0
    const val WIFI = 1

    private lateinit var manufacturer: String
    private lateinit var model: String
    private lateinit var device_name: String
    private var connectionMode: Int = 0

    private lateinit var socket: Socket
    private lateinit var mPrintWriter: PrintWriter
    private lateinit var mBufferedReader: BufferedReader

    private val connectionEstablishedListeners: MutableList<OnConnectionEstablishedListener> = mutableListOf()
    private val connectionFailedListeners: MutableList<OnConnectionFailedListener> = mutableListOf()
    private val connectionLostListeners: MutableList<OnConnectionLostListener> = mutableListOf()

    fun interface OnConnectionEstablishedListener {
        fun invoke()
    }

    fun interface OnConnectionFailedListener {
        fun invoke()
    }

    fun interface OnConnectionLostListener {
        fun invoke()
    }

    /**
     * Do not call this function. Call connect() method instead
     */
    override fun run() {
        try {
            socket = Socket(IP, PORT)
            socket.tcpNoDelay = true

            socket.trafficClass = 0x10

            mPrintWriter = PrintWriter(socket.getOutputStream())
            socket.soTimeout = 1
            mBufferedReader = BufferedReader(InputStreamReader(socket.getInputStream()))

            OnConnectionEstablished()
            this.write(0, manufacturer, model, device_name, connectionMode)

        } catch (e: IOException) {
            OnConnectionFailed()
            e.printStackTrace()
        }
    }

    fun init(
        manufacturer: String,
        model: String,
        device_name: String,
        connectionMode: Int
    ) {
        this.manufacturer = manufacturer
        this.model = model
        this.device_name = device_name
        this.connectionMode = connectionMode
    }

    fun connect(){
        Thread(this).start()
    }

    fun connect(ip: String){
        IP = ip
        Thread(this).start()
    }

    fun addOnConnectionEstablishedListener(listener: () -> Unit) {
        this.connectionEstablishedListeners.add(OnConnectionEstablishedListener(listener))
    }

    private fun OnConnectionEstablished(){
        this.connectionEstablishedListeners.forEach {
            it.invoke()
        }
    }

    fun addOnConnectionLostListener(listener: () -> Unit) {
        this.connectionLostListeners.add(OnConnectionLostListener(listener))
    }

    private fun OnConnectionLost(){
        this.connectionLostListeners.forEach {
            it.invoke()
        }
    }

    fun addOnConnectionFailedListener(listener: () -> Unit) {
        this.connectionFailedListeners.add(OnConnectionFailedListener(listener))
    }

    private fun OnConnectionFailed(){
        this.connectionFailedListeners.forEach {
            it.invoke()
        }
    }

    fun write(command: Int, vararg args: Any) {
        val msg = args.asList().joinToString(separator = "_")
        val final = "${command}_${msg}_"
        Thread {
            mPrintWriter.write(final)
            mPrintWriter.flush()

            if(mPrintWriter.checkError()){
                OnConnectionLost()
            }
        }.start()
    }
}