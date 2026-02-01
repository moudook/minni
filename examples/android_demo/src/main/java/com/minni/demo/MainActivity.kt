package com.minni.demo

import android.content.ComponentName
import android.content.Context
import android.content.Intent
import android.content.ServiceConnection
import android.os.Bundle
import android.os.IBinder
import android.widget.Button
import android.widget.TextView
import androidx.appcompat.app.AppCompatActivity
import com.minni.framework.services.InferenceService
import com.minni.framework.services.WifiSensingService
import android.Manifest
import android.content.pm.PackageManager
import androidx.core.app.ActivityCompat

class MainActivity : AppCompatActivity() {

    private var inferenceService: InferenceService? = null
    private var wifiService: WifiSensingService? = null
    private var isBoundInference = false
    private var isBoundWifi = false
    private lateinit var statusText: TextView

    private val inferenceConnection = object : ServiceConnection {
        override fun onServiceConnected(className: ComponentName, service: IBinder) {
            val binder = service as InferenceService.LocalBinder
            inferenceService = binder.getService()
            isBoundInference = true
        }
        override fun onServiceDisconnected(arg0: ComponentName) {
            isBoundInference = false
        }
    }

    private val wifiConnection = object : ServiceConnection {
        override fun onServiceConnected(className: ComponentName, service: IBinder) {
            val binder = service as WifiSensingService.LocalBinder
            wifiService = binder.getService()
            isBoundWifi = true
        }
        override fun onServiceDisconnected(arg0: ComponentName) {
            isBoundWifi = false
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        statusText = findViewById(R.id.text_status)
        val btnLogic = findViewById<Button>(R.id.btn_run_logic)
        val btnSat = findViewById<Button>(R.id.btn_run_sat)
        val btnRules = findViewById<Button>(R.id.btn_run_rules)
        val btnSignal = findViewById<Button>(R.id.btn_run_signal)
        val btnWifi = findViewById<Button>(R.id.btn_run_wifi)
        val btnVector = findViewById<Button>(R.id.btn_run_vector)
        val btnGenAi = findViewById<Button>(R.id.btn_run_genai)
        val btnSecurity = findViewById<Button>(R.id.btn_test_security)

        // Request Permissions for WiFi/Location
        if (checkSelfPermission(Manifest.permission.ACCESS_FINE_LOCATION) != PackageManager.PERMISSION_GRANTED) {
            requestPermissions(arrayOf(Manifest.permission.ACCESS_FINE_LOCATION), 1001)
        }

        // Bind Services
        bindService(Intent(this, InferenceService::class.java), inferenceConnection, Context.BIND_AUTO_CREATE)
        bindService(Intent(this, WifiSensingService::class.java), wifiConnection, Context.BIND_AUTO_CREATE)

        btnLogic.setOnClickListener {
            if (isBoundInference) {
                statusText.text = "Running Logic Query..."
                inferenceService?.runLogicQuery("Alice", "knows") { factCount ->
                    runOnUiThread {
                        statusText.text = "Logic Result: Graph has $factCount facts"
                    }
                }
            }
        }

        btnSat.setOnClickListener {
            if (isBoundInference) {
                statusText.text = "Running SAT Check..."
                // (A or B) & (!A) => B must be true
                val formula = "A | B & !A"
                inferenceService?.runSatCheck(formula) { isSat, model ->
                    runOnUiThread {
                        statusText.text = "SAT Result: $isSat, Model: $model"
                    }
                }
            }
        }

        btnRules.setOnClickListener {
            if (isBoundInference) {
                statusText.text = "Running Rule Engine..."
                // Simple rule: IF (temp > 37.5) THEN Diagnosis=Fever
                val rules = "IF (temp > 37.5) THEN Diagnosis=Fever\n" +
                            "IF (temp <= 37.5) THEN Diagnosis=Healthy"
                val inputs = mapOf("temp" to 38.0)

                inferenceService?.runRuleEvaluation(rules, inputs) { result ->
                    runOnUiThread {
                        statusText.text = "Rule Result: $result"
                    }
                }
            }
        }

        btnSignal.setOnClickListener {
            if (isBoundInference) {
                statusText.text = "Running Signal Processor Chain..."
                // Use power of 2 for FFT
                val size = 16384 // 2^14
                inferenceService?.runSignalChainBenchmark(size) { ms ->
                    runOnUiThread {
                        statusText.text = "Result: Chain (Win+FFT+Mag) on $size floats in ${ms}ms"
                    }
                }
            }
        }

        btnWifi.setOnClickListener {
            if (isBoundWifi) {
                statusText.text = "Running WiFi Ranging..."
                wifiService?.startRanging { distances, context ->
                    runOnUiThread {
                        statusText.text = "WiFi: $context\nDetails: $distances"
                    }
                }
            } else {
                statusText.text = "WiFi Service not bound yet."
            }
        }

        btnVector.setOnClickListener {
            if (isBoundInference) {
                statusText.text = "Running Vector Search..."
                inferenceService?.runVectorSearch { result ->
                    runOnUiThread {
                        statusText.text = "Vector Result: $result"
                    }
                }
            }
        }

        btnGenAi.setOnClickListener {
            if (isBoundInference) {
                statusText.text = "Running GenAI Inference..."
                // Using a dummy path for demo purposes (would be a real file in assets/storage)
                val dummyPath = filesDir.absolutePath + "/mobilenet.tflite"
                inferenceService?.runGenAIInference(dummyPath) { result ->
                    runOnUiThread {
                        statusText.text = "GenAI: $result"
                    }
                }
            }
        }

        btnSecurity.setOnClickListener {
            if (isBoundInference) {
                statusText.text = "Testing Secure Storage..."
                inferenceService?.testSecureStorage { result ->
                    runOnUiThread {
                        statusText.text = "Security: $result"
                    }
                }
            }
        }
    }

    override fun onDestroy() {
        super.onDestroy()
        if (isBoundInference) {
            unbindService(inferenceConnection)
            isBoundInference = false
        }
        if (isBoundWifi) {
            unbindService(wifiConnection)
            isBoundWifi = false
        }
    }
}
