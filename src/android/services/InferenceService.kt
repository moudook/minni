package com.minni.framework.services

import android.app.Notification
import android.app.NotificationChannel
import android.app.NotificationManager
import android.app.Service
import android.content.Context
import android.content.Intent
import android.os.Binder
import android.os.Build
import android.os.IBinder
import android.util.Log
import androidx.core.app.NotificationCompat
import com.minni.framework.core.DSPKernel
import com.minni.framework.core.KnowledgeGraph
import com.minni.framework.core.SatSolver
import com.minni.framework.core.RuleEngine
import com.minni.framework.core.SignalProcessor
import com.minni.framework.core.VectorStore
import com.minni.framework.genai.ModelRunner
import com.minni.framework.genai.Tensor
import java.util.concurrent.Executors

/**
 * A Background Service for running heavy inference tasks.
 * This ensures the logic engine continues running even if the user
 * minimizes the app.
 */
class InferenceService : Service() {

    private val binder = LocalBinder()
    private val executor = Executors.newSingleThreadExecutor()
    // Use quantized storage (int8) for memory optimization
    private val knowledgeGraph by lazy { KnowledgeGraph(true) }
    private val satSolver by lazy { SatSolver() }
    private val ruleEngine by lazy { RuleEngine() }
    private val signalProcessor by lazy { SignalProcessor() }
    private val vectorStore by lazy { VectorStore(true) }
    private val modelRunner by lazy { ModelRunner() }

    inner class LocalBinder : Binder() {
        fun getService(): InferenceService = this@InferenceService
    }

    override fun onBind(intent: Intent): IBinder {
        return binder
    }

    override fun onCreate() {
        super.onCreate()
        Log.i(TAG, "InferenceService created. Memory Optimization (Quantization) Enabled.")
        startForegroundService()
    }

    private fun startForegroundService() {
        val channelId = "MinniInferenceChannel"
        val channelName = "AI Inference Service"

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            val channel = NotificationChannel(
                channelId,
                channelName,
                NotificationManager.IMPORTANCE_LOW
            )
            val manager = getSystemService(Context.NOTIFICATION_SERVICE) as NotificationManager
            manager.createNotificationChannel(channel)
        }

        val notificationBuilder = NotificationCompat.Builder(this, channelId)
            .setContentTitle("Minni AI Engine")
            .setContentText("Running optimized inference in background")
            .setSmallIcon(android.R.drawable.ic_menu_compass) // Placeholder icon
            .setPriority(NotificationCompat.PRIORITY_LOW)

        val notification = notificationBuilder.build()
        val notificationId = 1001

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.Q) {
            // Must match manifest declaration
            startForeground(
                notificationId,
                notification,
                android.content.pm.ServiceInfo.FOREGROUND_SERVICE_TYPE_DATA_SYNC
            )
        } else {
            startForeground(notificationId, notification)
        }
    }

    override fun onDestroy() {
        super.onDestroy()
        knowledgeGraph.close()
        satSolver.close()
        ruleEngine.close()
        signalProcessor.close()
        vectorStore.close()
        modelRunner.close()
        executor.shutdown()
        Log.i(TAG, "InferenceService destroyed")
    }

    /**
     * Run a logic query in the background.
     */
    fun runLogicQuery(subject: String, predicate: String, callback: (Int) -> Unit) {
        executor.submit {
            Log.d(TAG, "Running logic query: $subject $predicate ?")

            // Example usage of the native graph
            // In a real app, we might load this from a file first
            if (knowledgeGraph.numFacts == 0) {
                knowledgeGraph.addFact("Alice", "knows", "Bob")
                knowledgeGraph.addFact("Bob", "knows", "Charlie")
            }

            // Mock query overhead
            Thread.sleep(500)

            val resultCount = knowledgeGraph.numFacts
            Log.d(TAG, "Query complete. Facts count: $resultCount")

            // Callback to UI thread would require a Handler,
            // here we just invoke for simplicity (caller handles thread)
            callback(resultCount)
        }
    }

    /**
     * Run a SAT solver check in the background.
     */
    fun runSatCheck(formula: String, callback: (Boolean, Map<String, String>) -> Unit) {
        executor.submit {
            Log.d(TAG, "Running SAT check for: $formula")
            val isSat = satSolver.check(formula)
            val model = if (isSat) satSolver.model else emptyMap()
            Log.d(TAG, "SAT result: $isSat, Model: $model")
            callback(isSat, model)
        }
    }

    /**
     * Run a rule engine evaluation in the background.
     */
    fun runRuleEvaluation(rules: String, inputs: Map<String, Double>, callback: (String?) -> Unit) {
        executor.submit {
            Log.d(TAG, "Loading rules...")
            val success = ruleEngine.loadRules(rules)
            if (!success) {
                Log.e(TAG, "Failed to load rules")
                callback("Error: Failed to load rules")
                return@submit
            }

            Log.d(TAG, "Evaluating inputs: $inputs")
            val result = ruleEngine.evaluate(inputs)
            Log.d(TAG, "Rule result: $result")
            callback(result)
        }
    }

    /**
     * Run a signal processing benchmark in the background.
     */
    fun runSignalBenchmark(size: Int, callback: (Float) -> Unit) {
        executor.submit {
            Log.d(TAG, "Running DSP benchmark on size $size")

            // Adjust size to power of 2 for FFT
            var fftSize = 1
            while (fftSize < size) fftSize *= 2
            if (fftSize > size) fftSize /= 2 // Keep it within bounds or use strict power of 2

            val real = FloatArray(fftSize) { it.toFloat() * 0.01f }
            val imag = FloatArray(fftSize) { 0f }
            val output = FloatArray(fftSize)

            val start = System.nanoTime()

            // 1. Run FFT
            DSPKernel.fft(real, imag, false)

            // 2. Compute Magnitude
            DSPKernel.complexMagnitude(real, imag, output)

            val end = System.nanoTime()

            val ms = (end - start) / 1_000_000.0f
            Log.d(TAG, "Benchmark complete (FFT+Mag on $fftSize): ${ms}ms")
            callback(ms)
        }
    }

    /**
     * Run a signal processing benchmark using the high-level SignalProcessor chain.
     */
    fun runSignalChainBenchmark(size: Int, callback: (Float) -> Unit) {
        executor.submit {
            Log.d(TAG, "Running SignalProcessor Chain benchmark on size $size")

            // Adjust size to power of 2
            var fftSize = 1
            while (fftSize < size) fftSize *= 2
            if (fftSize > size) fftSize /= 2

            val input = FloatArray(fftSize) { it.toFloat() * 0.01f }
            val window = FloatArray(fftSize) { 0.5f } // Simple window

            val start = System.nanoTime()

            // Chain: Load -> Window -> FFT -> Magnitude
            signalProcessor.load(input)
            signalProcessor.applyWindow(window)
            signalProcessor.fft(false)
            signalProcessor.magnitude()
            val output = signalProcessor.getOutput() // Retrieve result

            val end = System.nanoTime()

            val ms = (end - start) / 1_000_000.0f
            Log.d(TAG, "Chain complete (Load+Win+FFT+Mag on $fftSize): ${ms}ms")
            callback(ms)
        }
    }

    /**
     * Run a vector similarity search demonstration.
     */
    fun runVectorSearch(callback: (String) -> Unit) {
        executor.submit {
            Log.d(TAG, "Running VectorStore Search Demo...")

            // Populate if empty
            if (vectorStore.size() == 0) {
                // Concepts as vectors (Mocking 3 dimensions: [Tech, Food, Outdoors])
                vectorStore.addVector("Coding", floatArrayOf(0.9f, 0.1f, 0.1f))
                vectorStore.addVector("Hiking", floatArrayOf(0.1f, 0.1f, 0.9f))
                vectorStore.addVector("Burger", floatArrayOf(0.1f, 0.9f, 0.1f))
                vectorStore.addVector("Laptop", floatArrayOf(0.8f, 0.1f, 0.0f))
            }

            // Query: "Programming outside" -> High Tech, Low Food, Med Outdoors
            val query = floatArrayOf(0.7f, 0.0f, 0.5f)

            val results = vectorStore.search(query, 2)

            val resultStr = results.joinToString(", ") { it.toString() }
            Log.d(TAG, "Vector Search Result: $resultStr")
            callback(resultStr)
        }
    }

    /**
     * Run a Generative AI inference (LiteRT) in the background.
     */
    fun runGenAIInference(modelPath: String, callback: (String) -> Unit) {
        executor.submit {
            Log.d(TAG, "Running GenAI Inference on model: $modelPath")

            // Load Model
            // In a real app, modelPath would be an absolute path to a file in app storage
            val loaded = modelRunner.loadModel(modelPath, 4)
            if (!loaded) {
                Log.e(TAG, "Failed to load GenAI model")
                callback("Error: Failed to load model")
                return@submit
            }

            // Prepare Input Tensor (Dummy input for demo)
            // Shape: [1, 224, 224, 3] (Standard ImageNet)
            val inputShape = intArrayOf(1, 224, 224, 3)
            val inputData = FloatArray(1 * 224 * 224 * 3) { 0.5f }
            val inputTensor = Tensor("input_0", inputShape, inputData)

            val inputs = arrayOf(inputTensor)

            // Run Inference
            val start = System.nanoTime()
            val outputs = modelRunner.predict(inputs)
            val end = System.nanoTime()

            val ms = (end - start) / 1_000_000.0f

            if (outputs != null && outputs.isNotEmpty()) {
                val output = outputs[0]
                val resultInfo = "Inference done in ${ms}ms. Output shape: ${output.shape.contentToString()}"
                Log.d(TAG, resultInfo)
                callback(resultInfo)
            } else {
                Log.e(TAG, "Inference returned no output")
                callback("Error: Inference failed")
            }
        }
    }

    /**
     * Test Encrypted Persistence.
     * Saves data with a key, clears it, then loads it back.
     */
    fun testSecureStorage(callback: (String) -> Unit) {
        executor.submit {
            Log.d(TAG, "Testing Encrypted Storage...")
            val key = "SuperSecretKey123"
            val file = "${filesDir.absolutePath}/secure_vectors.bin"

            // 1. Add Data
            val id = "secret_agent"
            val vec = FloatArray(128) { 0.99f }
            vectorStore.addVector(id, vec)

            // 2. Save Encrypted
            val saved = vectorStore.save(file, key)
            if (!saved) {
                callback("Failed to save encrypted file")
                return@submit
            }

            // 3. Clear Memory
            vectorStore.clear()
            if (vectorStore.size() != 0) {
                callback("Failed to clear store")
                return@submit
            }

            // 4. Load Encrypted
            val loaded = vectorStore.load(file, key)
            if (!loaded) {
                callback("Failed to load encrypted file")
                return@submit
            }

            // 5. Verify
            val results = vectorStore.search(vec, 1)
            if (results.isNotEmpty() && results[0].id == id) {
                val msg = "Success! Saved & Loaded encrypted data. Found: ${results[0].id}"
                Log.d(TAG, msg)
                callback(msg)
            } else {
                callback("Data verification failed after load.")
            }
        }
    }

    companion object {
        private const val TAG = "MinniInferenceService"
    }
}
