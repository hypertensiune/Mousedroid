package com.example.mousedroid

import android.app.Activity
import android.content.Intent
import android.content.IntentFilter
import android.os.BatteryManager
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.os.Build
import android.provider.Settings
import android.view.View
import android.widget.TextView
import androidx.activity.result.contract.ActivityResultContracts
import androidx.constraintlayout.widget.ConstraintLayout
import com.google.android.material.button.MaterialButton

class MainActivity : AppCompatActivity() {

    private val TAG = "Mousedroid"

    private lateinit var inputActivityIntent: Intent

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        val manufacturer = Build.MANUFACTURER
        val model = Build.MODEL
        val deviceName = Settings.Secure.getString(contentResolver, "bluetooth_name")

        val connectionMode = getConnectionMode()

        TcpClient.init(manufacturer, model, deviceName, connectionMode)

        TcpClient.addOnConnectionEstablishedListener {
            inputActivityIntent = Intent(this, InputActivity::class.java)
            startActivity(inputActivityIntent)
        }

        TcpClient.addOnConnectionLostListener {
            runOnUiThread{
                findViewById<ConstraintLayout>(R.id.main).visibility = View.VISIBLE
                findViewById<TextView>(R.id.errorName).text = "Connection lost!"
                findViewById<TextView>(R.id.errorDetail).text = getString(R.string.connectionLost)
            }
        }

        TcpClient.addOnConnectionFailedListener {
            runOnUiThread {
                findViewById<ConstraintLayout>(R.id.main).visibility = View.VISIBLE

                if(connectionMode == TcpClient.USB) {
                    findViewById<MaterialButton>(R.id.selectAnotherDeviceButton).visibility = View.GONE
                    findViewById<TextView>(R.id.errorDetail).text = getString(R.string.connectionFailedUSB)
                }
                else{
                    findViewById<TextView>(R.id.errorDetail).text = getString(R.string.connectionFailedWIFI)
                }
            }
        }

        when(connectionMode) {
            TcpClient.USB -> TcpClient.connect("localhost")
            TcpClient.WIFI -> resultLauncher.launch(Intent(this, SelectDeviceActivity::class.java))
        }

        findViewById<MaterialButton>(R.id.retryButton).setOnClickListener {
            if(connectionMode == TcpClient.USB){
                finish()
                startActivity(intent)
            }
            else{
                findViewById<ConstraintLayout>(R.id.main).visibility = View.GONE
                TcpClient.connect()
            }
        }

        findViewById<MaterialButton>(R.id.selectAnotherDeviceButton).setOnClickListener {
            findViewById<ConstraintLayout>(R.id.main).visibility = View.VISIBLE
            resultLauncher.launch(Intent(this, SelectDeviceActivity::class.java))
        }
    }

    private var resultLauncher = registerForActivityResult(ActivityResultContracts.StartActivityForResult()) { result ->
        if(result.resultCode == Activity.RESULT_OK){
            val ip = result.data?.getStringExtra("result").toString()
            TcpClient.connect(ip)
        }
    }

    private fun getConnectionMode(): Int {
        val ifilter = IntentFilter(Intent.ACTION_BATTERY_CHANGED)
        val batteryStatus = baseContext.registerReceiver(null, ifilter)

        val isPluggedIn = batteryStatus?.getIntExtra(
            BatteryManager.EXTRA_PLUGGED,
            -1
        ) == BatteryManager.BATTERY_PLUGGED_USB

        return if(isPluggedIn) TcpClient.USB else TcpClient.WIFI
    }
}