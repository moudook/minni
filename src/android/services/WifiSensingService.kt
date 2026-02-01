package com.minni.framework.services

import android.Manifest
import android.annotation.SuppressLint
import android.app.Service
import android.content.Context
import android.content.Intent
import android.content.pm.PackageManager
import android.net.wifi.rtt.RangingRequest
import android.net.wifi.rtt.RangingResult
import android.net.wifi.rtt.RangingResultCallback
import android.net.wifi.rtt.WifiRttManager
import android.net.wifi.ScanResult
import android.net.wifi.WifiManager
import android.os.Binder
import android.os.IBinder
import android.os.Handler
import android.os.Looper
import android.util.Log
import androidx.core.app.ActivityCompat
import com.minni.framework.core.RuleEngine
import com.minni.framework.core.KalmanFilter
import kotlin.random.Random

/**
 * Service for handling WiFi Sensing research (RTT/CSI).
 * Currently implements WiFi Round-Trip-Time (RTT) ranging for indoor positioning.
 */
class WifiSensingService : Service() {

    private val binder = LocalBinder()
    private var wifiRttManager: WifiRttManager? = null
    private var wifiManager: WifiManager? = null
    private val handler = Handler(Looper.getMainLooper())
    private val ruleEngine by lazy { RuleEngine() }

    // Kalman filters for 3 APs (mocked)
    // measurement_err=0.5m, est_err=1.0m, process_noise=0.01
    private val kf1 by lazy { KalmanFilter(0.5f, 1.0f, 0.01f) }
    private val kf2 by lazy { KalmanFilter(0.5f, 1.0f, 0.01f) }
    private val kf3 by lazy { KalmanFilter(0.5f, 1.0f, 0.01f) }

    // True distances (simulated)
    private var dist1 = 1.5f
    private var dist2 = 4.2f
    private var dist3 = 12.0f

    // Sample rules for context inference
    // In a real app, these would be trained on collected data
    private val inferenceRules = """
        IF (ap1_dist <= 5.0) & (ap2_dist <= 5.0) THEN Context=Home_LivingRoom
        IF (ap1_dist > 10.0) & (ap3_dist <= 3.0) THEN Context=Home_Bedroom
        IF (ap2_dist > 20.0) THEN Context=Outside
    """.trimIndent()

    inner class LocalBinder : Binder() {
        fun getService(): WifiSensingService = this@WifiSensingService
    }

    override fun onBind(intent: Intent): IBinder {
        return binder
    }

    override fun onCreate() {
        super.onCreate()
        wifiRttManager = getSystemService(Context.WIFI_RTT_RAGING_SERVICE) as? WifiRttManager
        wifiManager = getSystemService(Context.WIFI_SERVICE) as? WifiManager

        // Load rules
        ruleEngine.loadRules(inferenceRules)

        // Initialize filters
        kf1.reset(dist1)
        kf2.reset(dist2)
        kf3.reset(dist3)

        Log.i(TAG, "WifiSensingService created. RTT Available: ${wifiRttManager?.isAvailable}")
    }

    override fun onDestroy() {
        super.onDestroy()
        ruleEngine.close()
        kf1.close()
        kf2.close()
        kf3.close()
    }

    @SuppressLint("MissingPermission")
    fun startRanging(callback: (List<String>, String?) -> Unit) {
        if (wifiRttManager == null || !packageManager.hasSystemFeature(PackageManager.FEATURE_WIFI_RTT)) {
            Log.e(TAG, "WiFi RTT not supported on this device.")
            callback(listOf("Error: RTT not supported"), null)
            return
        }

        // In a real app, we would scan for APs first.
        // Here we mock the behavior or assume known APs if we had ScanResults.
        // Since we can't easily get real ScanResults without a full scan and permissions,
        // we will implement the logic flow but note the limitation.

        Log.d(TAG, "Starting Ranging sequence...")

        // Pseudo-implementation:
        // 1. WifiManager.startScan()
        // 2. On results, filter for 802.11mc (RTT) capable APs
        // 3. Build RangingRequest
        // 4. wifiRttManager.startRanging(...)

        // For research template purposes, we log the intent.
        // Real implementation requires ACCESS_FINE_LOCATION permission at runtime.

        if (ActivityCompat.checkSelfPermission(this, Manifest.permission.ACCESS_FINE_LOCATION) != PackageManager.PERMISSION_GRANTED) {
            callback(listOf("Error: Missing Location Permission"), null)
            return
        }

        // Mock result for demonstration
        handler.postDelayed({
            // Add Gaussian noise to ground truth
            val noise1 = (Random.nextFloat() - 0.5f) * 1.0f // +/- 0.5m noise
            val noise2 = (Random.nextFloat() - 0.5f) * 1.5f
            val noise3 = (Random.nextFloat() - 0.5f) * 0.8f

            val raw1 = dist1 + noise1
            val raw2 = dist2 + noise2
            val raw3 = dist3 + noise3

            // Apply Kalman Filter
            val smooth1 = kf1.update(raw1)
            val smooth2 = kf2.update(raw2)
            val smooth3 = kf3.update(raw3)

            val distances = listOf(
                "AP_1: Raw=%.2fm Smooth=%.2fm".format(raw1, smooth1),
                "AP_2: Raw=%.2fm Smooth=%.2fm".format(raw2, smooth2),
                "AP_3: Raw=%.2fm Smooth=%.2fm".format(raw3, smooth3)
            )

            // Infer context using smoothed values
            val inputs = mapOf(
                "ap1_dist" to smooth1.toDouble(),
                "ap2_dist" to smooth2.toDouble(),
                "ap3_dist" to smooth3.toDouble()
            )

            val context = ruleEngine.evaluate(inputs) ?: "Unknown"

            callback(distances, context)
        }, 1000)
    }

    companion object {
        private const val TAG = "MinniWifiService"
    }
}
