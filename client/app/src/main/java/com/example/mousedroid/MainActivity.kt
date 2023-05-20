package com.example.mousedroid

import android.Manifest
import android.app.Activity
import android.bluetooth.BluetoothManager
import android.content.Intent
import android.content.IntentFilter
import android.content.pm.PackageManager
import android.os.BatteryManager
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.os.Build
import android.provider.Settings
import android.util.Log
import android.view.View
import android.widget.RelativeLayout
import android.widget.TextView
import androidx.activity.result.contract.ActivityResultContracts
import androidx.constraintlayout.widget.ConstraintLayout
import androidx.core.app.ActivityCompat
import com.google.android.material.button.MaterialButton
import org.w3c.dom.Text

class MainActivity : AppCompatActivity() {

    private val TAG = "Mousedroid"

    private lateinit var inputActivityIntent: Intent

    override fun onRequestPermissionsResult(
        requestCode: Int,
        permissions: Array<out String>,
        grantResults: IntArray
    ) {
        Log.d(TAG, "PERMISSION RESULT")
        if(requestCode == 100) {
            if(!grantResults.contains(PackageManager.PERMISSION_DENIED)) {
                init((baseContext.getSystemService(BLUETOOTH_SERVICE) as BluetoothManager).adapter.name)
            }
            else {
                init("PERMISSION NOT ALLOWED")
            }
        }
        super.onRequestPermissionsResult(requestCode, permissions, grantResults)
    }

    private fun init(deviceName: String) {
        val manufacturer = Build.MANUFACTURER
        val model = Build.MODEL

        val connectionMode = getConnectionMode()
        TcpClient.init(manufacturer, deviceName, model, connectionMode)

        TcpClient.addOnConnectionEstablishedListener {
            inputActivityIntent = Intent(this, InputActivity::class.java)
            startActivity(inputActivityIntent)
        }

        TcpClient.addOnConnectionLostListener {
            runOnUiThread{
                findViewById<ConstraintLayout>(R.id.errorPanel).visibility = View.VISIBLE
                findViewById<RelativeLayout>(R.id.loadingPanel).visibility = View.GONE
                findViewById<TextView>(R.id.errorName).text = getString(R.string.connectionLost)
                findViewById<TextView>(R.id.errorDetail).text = ""
            }
        }

        TcpClient.addOnConnectionFailedListener {
            runOnUiThread {
                findViewById<ConstraintLayout>(R.id.errorPanel).visibility = View.VISIBLE
                findViewById<RelativeLayout>(R.id.loadingPanel).visibility = View.GONE

                if(connectionMode == TcpClient.USB) {
                    findViewById<MaterialButton>(R.id.selectAnotherDeviceButton).visibility = View.GONE
                    findViewById<TextView>(R.id.errorDetail).text = getString(R.string.connectionFailedUSB)
                }
                else{
                    findViewById<TextView>(R.id.errorName).text = getString(R.string.connectionFailed)
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
                findViewById<ConstraintLayout>(R.id.errorPanel).visibility = View.GONE
                findViewById<RelativeLayout>(R.id.loadingPanel).visibility = View.VISIBLE
                TcpClient.connect()
            }
        }

        findViewById<MaterialButton>(R.id.selectAnotherDeviceButton).setOnClickListener {
            findViewById<ConstraintLayout>(R.id.errorPanel).visibility = View.VISIBLE
            findViewById<RelativeLayout>(R.id.loadingPanel).visibility = View.GONE
            resultLauncher.launch(Intent(this, SelectDeviceActivity::class.java))
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.S_V2) {
            if (ActivityCompat.checkSelfPermission(this, Manifest.permission.BLUETOOTH_CONNECT) != PackageManager.PERMISSION_GRANTED) {
                Log.d(TAG, "REQUESTING PERMISSION")
                ActivityCompat.requestPermissions(this, arrayOf(Manifest.permission.BLUETOOTH_CONNECT), 100)
            }
            else {
                init((baseContext.getSystemService(BLUETOOTH_SERVICE) as BluetoothManager).adapter.name)
            }
        } else {
            init(Settings.Secure.getString(contentResolver, "bluetooth_name"))
        }

        //val deviceName = Settings.Global.getString(application.contentResolver, Settings.Global.DEVICE_NAME)
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