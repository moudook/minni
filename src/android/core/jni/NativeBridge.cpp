#include <jni.h>
#include <string>
#include <vector>
#include "logic/SatSolver.h"
#include "logic/RuleEngine.h"
#include "logic/VectorStore.h"
#include "signal/DSPKernel.h"
#include "signal/SignalProcessor.h"
#include "signal/KalmanFilter.h"
#include "genai/ModelRunner.h"

// JNI Helper to throw Java exceptions
void throwJavaException(JNIEnv* env, const char* msg) {
    jclass cls = env->FindClass("java/lang/RuntimeException");
    if (cls) {
        env->ThrowNew(cls, msg);
    }
}

extern "C" {

// ========================================================
// Knowledge Graph JNI Bindings
// ========================================================

// Handle to the C++ object stored in Java long
static minni::logic::KnowledgeGraph* getHandleKG(JNIEnv* env, jobject obj) {
    jclass cls = env->GetObjectClass(obj);
    jfieldID fid = env->GetFieldID(cls, "nativeHandle", "J");
    jlong handle = env->GetLongField(obj, fid);
    return reinterpret_cast<minni::logic::KnowledgeGraph*>(handle);
}

JNIEXPORT void JNICALL
Java_com_minni_framework_core_KnowledgeGraph_nativeInit(JNIEnv* env, jobject obj, jboolean useQuantization) {
    auto* kg = new minni::logic::KnowledgeGraph(useQuantization);
    jclass cls = env->GetObjectClass(obj);
    jfieldID fid = env->GetFieldID(cls, "nativeHandle", "J");
    env->SetLongField(obj, fid, reinterpret_cast<jlong>(kg));
}

JNIEXPORT void JNICALL
Java_com_minni_framework_core_KnowledgeGraph_nativeFree(JNIEnv* env, jobject obj) {
    auto* kg = getHandleKG(env, obj);
    if (kg) {
        delete kg;
        jclass cls = env->GetObjectClass(obj);
        jfieldID fid = env->GetFieldID(cls, "nativeHandle", "J");
        env->SetLongField(obj, fid, 0);
    }
}

JNIEXPORT void JNICALL
Java_com_minni_framework_core_KnowledgeGraph_nativeAddFact(JNIEnv* env, jobject obj,
                                                     jstring sub, jstring pred, jstring object) {
    auto* kg = getHandleKG(env, obj);
    if (!kg) return;

    const char* s_sub = env->GetStringUTFChars(sub, nullptr);
    const char* s_pred = env->GetStringUTFChars(pred, nullptr);
    const char* s_obj = env->GetStringUTFChars(object, nullptr);

    kg->add_fact(std::string(s_sub), std::string(s_pred), std::string(s_obj));

    env->ReleaseStringUTFChars(sub, s_sub);
    env->ReleaseStringUTFChars(pred, s_pred);
    env->ReleaseStringUTFChars(object, s_obj);
}

JNIEXPORT jint JNICALL
Java_com_minni_framework_core_KnowledgeGraph_nativeGetNumFacts(JNIEnv* env, jobject obj) {
    auto* kg = getHandleKG(env, obj);
    if (!kg) return 0;
    return static_cast<jint>(kg->num_facts());
}

JNIEXPORT jboolean JNICALL
Java_com_minni_framework_core_KnowledgeGraph_nativeSave(JNIEnv* env, jobject obj, jstring path, jstring encryptionKey) {
    auto* kg = getHandleKG(env, obj);
    if (!kg) return JNI_FALSE;

    const char* s_path = env->GetStringUTFChars(path, nullptr);
    const char* s_key = encryptionKey ? env->GetStringUTFChars(encryptionKey, nullptr) : "";

    bool result = kg->save(std::string(s_path), std::string(s_key));

    env->ReleaseStringUTFChars(path, s_path);
    if (encryptionKey) env->ReleaseStringUTFChars(encryptionKey, s_key);

    return result ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_com_minni_framework_core_KnowledgeGraph_nativeLoad(JNIEnv* env, jobject obj, jstring path, jstring encryptionKey) {
    auto* kg = getHandleKG(env, obj);
    if (!kg) return JNI_FALSE;

    const char* s_path = env->GetStringUTFChars(path, nullptr);
    const char* s_key = encryptionKey ? env->GetStringUTFChars(encryptionKey, nullptr) : "";

    bool result = kg->load(std::string(s_path), std::string(s_key));

    env->ReleaseStringUTFChars(path, s_path);
    if (encryptionKey) env->ReleaseStringUTFChars(encryptionKey, s_key);

    return result ? JNI_TRUE : JNI_FALSE;
}

// ========================================================
// SatSolver JNI Bindings
// ========================================================

static minni::logic::SatSolver* getHandleSat(JNIEnv* env, jobject obj) {
    jclass cls = env->GetObjectClass(obj);
    jfieldID fid = env->GetFieldID(cls, "nativeHandle", "J");
    jlong handle = env->GetLongField(obj, fid);
    return reinterpret_cast<minni::logic::SatSolver*>(handle);
}

JNIEXPORT void JNICALL
Java_com_minni_framework_core_SatSolver_nativeInit(JNIEnv* env, jobject obj) {
    auto* solver = new minni::logic::SatSolver();
    solver->initialize();
    jclass cls = env->GetObjectClass(obj);
    jfieldID fid = env->GetFieldID(cls, "nativeHandle", "J");
    env->SetLongField(obj, fid, reinterpret_cast<jlong>(solver));
}

JNIEXPORT void JNICALL
Java_com_minni_framework_core_SatSolver_nativeFree(JNIEnv* env, jobject obj) {
    auto* solver = getHandleSat(env, obj);
    if (solver) {
        delete solver;
        jclass cls = env->GetObjectClass(obj);
        jfieldID fid = env->GetFieldID(cls, "nativeHandle", "J");
        env->SetLongField(obj, fid, 0);
    }
}

JNIEXPORT jboolean JNICALL
Java_com_minni_framework_core_SatSolver_checkNative(JNIEnv* env, jobject obj, jstring formula) {
    auto* solver = getHandleSat(env, obj);
    if (!solver) return JNI_FALSE;

    const char* s_formula = env->GetStringUTFChars(formula, nullptr);
    minni::logic::SolverStatus status = solver->check(std::string(s_formula));
    env->ReleaseStringUTFChars(formula, s_formula);

    return (status == minni::logic::SolverStatus::SATISFIABLE) ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jobject JNICALL
Java_com_minni_framework_core_SatSolver_getModelNative(JNIEnv* env, jobject obj) {
    auto* solver = getHandleSat(env, obj);
    if (!solver) return nullptr;

    auto model = solver->get_model();

    jclass mapClass = env->FindClass("java/util/HashMap");
    if (mapClass == nullptr) return nullptr;

    jmethodID init = env->GetMethodID(mapClass, "<init>", "()V");
    jobject hashMap = env->NewObject(mapClass, init);

    jmethodID put = env->GetMethodID(mapClass, "put",
                                     "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");

    for (const auto& pair : model) {
        jstring key = env->NewStringUTF(pair.first.c_str());
        jstring value = env->NewStringUTF(pair.second.c_str());
        env->CallObjectMethod(hashMap, put, key, value);
        env->DeleteLocalRef(key);
        env->DeleteLocalRef(value);
    }

    return hashMap;
}

// ========================================================
// RuleEngine JNI Bindings
// ========================================================

static minni::logic::RuleEngine* getHandleRules(JNIEnv* env, jobject obj) {
    jclass cls = env->GetObjectClass(obj);
    jfieldID fid = env->GetFieldID(cls, "nativeHandle", "J");
    jlong handle = env->GetLongField(obj, fid);
    return reinterpret_cast<minni::logic::RuleEngine*>(handle);
}

JNIEXPORT void JNICALL
Java_com_minni_framework_core_RuleEngine_nativeInit(JNIEnv* env, jobject obj) {
    auto* engine = new minni::logic::RuleEngine();
    jclass cls = env->GetObjectClass(obj);
    jfieldID fid = env->GetFieldID(cls, "nativeHandle", "J");
    env->SetLongField(obj, fid, reinterpret_cast<jlong>(engine));
}

JNIEXPORT void JNICALL
Java_com_minni_framework_core_RuleEngine_nativeFree(JNIEnv* env, jobject obj) {
    auto* engine = getHandleRules(env, obj);
    if (engine) {
        delete engine;
        jclass cls = env->GetObjectClass(obj);
        jfieldID fid = env->GetFieldID(cls, "nativeHandle", "J");
        env->SetLongField(obj, fid, 0);
    }
}

JNIEXPORT jboolean JNICALL
Java_com_minni_framework_core_RuleEngine_nativeLoadRules(JNIEnv* env, jobject obj, jstring rulesContent) {
    auto* engine = getHandleRules(env, obj);
    if (!engine) return JNI_FALSE;

    const char* s_rules = env->GetStringUTFChars(rulesContent, nullptr);
    bool result = engine->load_rules(std::string(s_rules));
    env->ReleaseStringUTFChars(rulesContent, s_rules);

    return result ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jstring JNICALL
Java_com_minni_framework_core_RuleEngine_nativeEvaluate(JNIEnv* env, jobject obj, jobject inputsMap) {
    auto* engine = getHandleRules(env, obj);
    if (!engine) return nullptr;

    // Convert Java Map<String, Object> to C++ std::map<std::string, RuleValue>
    std::map<std::string, minni::logic::RuleValue> cpp_inputs;

    jclass mapClass = env->GetObjectClass(inputsMap);
    jmethodID entrySetMethod = env->GetMethodID(mapClass, "entrySet", "()Ljava/util/Set;");
    jobject entrySet = env->CallObjectMethod(inputsMap, entrySetMethod);

    jclass setClass = env->GetObjectClass(entrySet);
    jmethodID iteratorMethod = env->GetMethodID(setClass, "iterator", "()Ljava/util/Iterator;");
    jobject iterator = env->CallObjectMethod(entrySet, iteratorMethod);

    jclass iteratorClass = env->GetObjectClass(iterator);
    jmethodID hasNextMethod = env->GetMethodID(iteratorClass, "hasNext", "()Z");
    jmethodID nextMethod = env->GetMethodID(iteratorClass, "next", "()Ljava/lang/Object;");

    jclass entryClass = env->FindClass("java/util/Map$Entry");
    jmethodID getKeyMethod = env->GetMethodID(entryClass, "getKey", "()Ljava/lang/Object;");
    jmethodID getValueMethod = env->GetMethodID(entryClass, "getValue", "()Ljava/lang/Object;");

    jclass doubleClass = env->FindClass("java/lang/Double");
    jclass stringClass = env->FindClass("java/lang/String");
    jmethodID doubleValueMethod = env->GetMethodID(doubleClass, "doubleValue", "()D");

    while (env->CallBooleanMethod(iterator, hasNextMethod)) {
        jobject entry = env->CallObjectMethod(iterator, nextMethod);
        jstring key = (jstring)env->CallObjectMethod(entry, getKeyMethod);
        jobject value = env->CallObjectMethod(entry, getValueMethod);

        const char* s_key = env->GetStringUTFChars(key, nullptr);
        std::string keyStr(s_key);
        env->ReleaseStringUTFChars(key, s_key);

        if (value != nullptr) {
            if (env->IsInstanceOf(value, doubleClass)) {
                double d_value = env->CallDoubleMethod(value, doubleValueMethod);
                cpp_inputs[keyStr] = d_value;
            } else if (env->IsInstanceOf(value, stringClass)) {
                const char* s_val = env->GetStringUTFChars((jstring)value, nullptr);
                cpp_inputs[keyStr] = std::string(s_val);
                env->ReleaseStringUTFChars((jstring)value, s_val);
            }
        }

        env->DeleteLocalRef(entry);
        env->DeleteLocalRef(key);
        env->DeleteLocalRef(value);
    }

    auto result = engine->evaluate(cpp_inputs);

    if (result.has_value()) {
        return env->NewStringUTF(result.value().c_str());
    } else {
        return nullptr;
    }
}

// ========================================================
// Signal Processing JNI Bindings
// ========================================================

JNIEXPORT void JNICALL
Java_com_minni_framework_core_DSPKernel_complexMagnitude(JNIEnv* env, jclass clazz,
                                                         jfloatArray real, jfloatArray imag, jfloatArray out) {
    jsize len = env->GetArrayLength(real);
    jsize outLen = env->GetArrayLength(out);

    if (len != env->GetArrayLength(imag) || len != outLen) {
        throwJavaException(env, "Array lengths must match");
        return;
    }

    // Get direct pointers if possible, or copy
    jfloat* pReal = env->GetFloatArrayElements(real, nullptr);
    jfloat* pImag = env->GetFloatArrayElements(imag, nullptr);
    jfloat* pOut = env->GetFloatArrayElements(out, nullptr);

    minni::signal::DSPKernel::complex_magnitude(pReal, pImag, pOut, len);

    // Release (0 means copy back to Java array if it was a copy)
    env->ReleaseFloatArrayElements(real, pReal, JNI_ABORT); // No changes to input
    env->ReleaseFloatArrayElements(imag, pImag, JNI_ABORT);
    env->ReleaseFloatArrayElements(out, pOut, 0); // Copy back output
}

JNIEXPORT void JNICALL
Java_com_minni_framework_core_DSPKernel_fft(JNIEnv* env, jclass clazz,
                                            jfloatArray real, jfloatArray imag, jboolean inverse) {
    jsize len = env->GetArrayLength(real);

    if (len != env->GetArrayLength(imag)) {
        throwJavaException(env, "Real and Imaginary arrays must have same length");
        return;
    }

    // Check power of 2
    if ((len & (len - 1)) != 0) {
        throwJavaException(env, "FFT size must be a power of 2");
        return;
    }

    jfloat* pReal = env->GetFloatArrayElements(real, nullptr);
    jfloat* pImag = env->GetFloatArrayElements(imag, nullptr);

    minni::signal::DSPKernel::fft(pReal, pImag, static_cast<size_t>(len), inverse);

    // Copy back both arrays as FFT is in-place
    env->ReleaseFloatArrayElements(real, pReal, 0);
    env->ReleaseFloatArrayElements(imag, pImag, 0);
}

JNIEXPORT jfloat JNICALL
Java_com_minni_framework_core_DSPKernel_dotProduct(JNIEnv* env, jclass clazz,
                                                   jfloatArray a, jfloatArray b) {
    jsize len = env->GetArrayLength(a);
    if (len != env->GetArrayLength(b)) {
        throwJavaException(env, "Array lengths must match");
        return 0.0f;
    }

    jfloat* pA = env->GetFloatArrayElements(a, nullptr);
    jfloat* pB = env->GetFloatArrayElements(b, nullptr);

    float result = minni::signal::DSPKernel::dot_product(pA, pB, static_cast<size_t>(len));

    env->ReleaseFloatArrayElements(a, pA, JNI_ABORT);
    env->ReleaseFloatArrayElements(b, pB, JNI_ABORT);
    return result;
}

JNIEXPORT jfloat JNICALL
Java_com_minni_framework_core_DSPKernel_cosineSimilarity(JNIEnv* env, jclass clazz,
                                                         jfloatArray a, jfloatArray b) {
    jsize len = env->GetArrayLength(a);
    if (len != env->GetArrayLength(b)) {
        throwJavaException(env, "Array lengths must match");
        return 0.0f;
    }

    jfloat* pA = env->GetFloatArrayElements(a, nullptr);
    jfloat* pB = env->GetFloatArrayElements(b, nullptr);

    float result = minni::signal::DSPKernel::cosine_similarity(pA, pB, static_cast<size_t>(len));

    env->ReleaseFloatArrayElements(a, pA, JNI_ABORT);
    env->ReleaseFloatArrayElements(b, pB, JNI_ABORT);
    return result;
}

// ========================================================
// SignalProcessor JNI Bindings
// ========================================================

static minni::signal::SignalProcessor* getHandleSignalProc(JNIEnv* env, jobject obj) {
    jclass cls = env->GetObjectClass(obj);
    jfieldID fid = env->GetFieldID(cls, "nativeHandle", "J");
    jlong handle = env->GetLongField(obj, fid);
    return reinterpret_cast<minni::signal::SignalProcessor*>(handle);
}

JNIEXPORT void JNICALL
Java_com_minni_framework_core_SignalProcessor_nativeInit(JNIEnv* env, jobject obj) {
    auto* proc = new minni::signal::SignalProcessor();
    jclass cls = env->GetObjectClass(obj);
    jfieldID fid = env->GetFieldID(cls, "nativeHandle", "J");
    env->SetLongField(obj, fid, reinterpret_cast<jlong>(proc));
}

JNIEXPORT void JNICALL
Java_com_minni_framework_core_SignalProcessor_nativeFree(JNIEnv* env, jobject obj) {
    auto* proc = getHandleSignalProc(env, obj);
    if (proc) {
        delete proc;
        jclass cls = env->GetObjectClass(obj);
        jfieldID fid = env->GetFieldID(cls, "nativeHandle", "J");
        env->SetLongField(obj, fid, 0);
    }
}

JNIEXPORT void JNICALL
Java_com_minni_framework_core_SignalProcessor_nativeLoad(JNIEnv* env, jobject obj, jfloatArray data) {
    auto* proc = getHandleSignalProc(env, obj);
    if (!proc) return;

    jsize len = env->GetArrayLength(data);
    jfloat* pData = env->GetFloatArrayElements(data, nullptr);

    // Copy to vector
    std::vector<float> vec(pData, pData + len);
    proc->load(vec);

    env->ReleaseFloatArrayElements(data, pData, JNI_ABORT);
}

JNIEXPORT void JNICALL
Java_com_minni_framework_core_SignalProcessor_nativeApplyWindow(JNIEnv* env, jobject obj, jfloatArray window) {
    auto* proc = getHandleSignalProc(env, obj);
    if (!proc) return;

    jsize len = env->GetArrayLength(window);
    jfloat* pWin = env->GetFloatArrayElements(window, nullptr);

    std::vector<float> vec(pWin, pWin + len);
    proc->apply_window(vec);

    env->ReleaseFloatArrayElements(window, pWin, JNI_ABORT);
}

JNIEXPORT void JNICALL
Java_com_minni_framework_core_SignalProcessor_nativeFft(JNIEnv* env, jobject obj, jboolean inverse) {
    auto* proc = getHandleSignalProc(env, obj);
    if (proc) {
        proc->fft(inverse);
    }
}

JNIEXPORT void JNICALL
Java_com_minni_framework_core_SignalProcessor_nativeMagnitude(JNIEnv* env, jobject obj) {
    auto* proc = getHandleSignalProc(env, obj);
    if (proc) {
        proc->magnitude();
    }
}

JNIEXPORT jfloatArray JNICALL
Java_com_minni_framework_core_SignalProcessor_nativeGetOutput(JNIEnv* env, jobject obj) {
    auto* proc = getHandleSignalProc(env, obj);
    if (!proc) return nullptr;

    std::vector<float> output = proc->get_output();

    jfloatArray result = env->NewFloatArray(output.size());
    if (result == nullptr) return nullptr;

    env->SetFloatArrayRegion(result, 0, output.size(), output.data());
    return result;
}

// ========================================================
// KalmanFilter JNI Bindings
// ========================================================

static minni::signal::KalmanFilter* getHandleKalman(JNIEnv* env, jobject obj) {
    jclass cls = env->GetObjectClass(obj);
    jfieldID fid = env->GetFieldID(cls, "nativeHandle", "J");
    jlong handle = env->GetLongField(obj, fid);
    return reinterpret_cast<minni::signal::KalmanFilter*>(handle);
}

JNIEXPORT void JNICALL
Java_com_minni_framework_core_KalmanFilter_nativeInit(JNIEnv* env, jobject obj,
                                                      jfloat measureUncertainty,
                                                      jfloat estUncertainty,
                                                      jfloat processNoise) {
    auto* kf = new minni::signal::KalmanFilter(measureUncertainty, estUncertainty, processNoise);
    jclass cls = env->GetObjectClass(obj);
    jfieldID fid = env->GetFieldID(cls, "nativeHandle", "J");
    env->SetLongField(obj, fid, reinterpret_cast<jlong>(kf));
}

JNIEXPORT void JNICALL
Java_com_minni_framework_core_KalmanFilter_nativeFree(JNIEnv* env, jobject obj) {
    auto* kf = getHandleKalman(env, obj);
    if (kf) {
        delete kf;
        jclass cls = env->GetObjectClass(obj);
        jfieldID fid = env->GetFieldID(cls, "nativeHandle", "J");
        env->SetLongField(obj, fid, 0);
    }
}

JNIEXPORT jfloat JNICALL
Java_com_minni_framework_core_KalmanFilter_nativeUpdate(JNIEnv* env, jobject obj, jfloat measurement) {
    auto* kf = getHandleKalman(env, obj);
    if (!kf) return measurement;
    return kf->update(measurement);
}

JNIEXPORT jfloat JNICALL
Java_com_minni_framework_core_KalmanFilter_nativeGetEstimate(JNIEnv* env, jobject obj) {
    auto* kf = getHandleKalman(env, obj);
    if (!kf) return 0.0f;
    return kf->get_estimate();
}

JNIEXPORT void JNICALL
Java_com_minni_framework_core_KalmanFilter_nativeReset(JNIEnv* env, jobject obj, jfloat initialValue) {
    auto* kf = getHandleKalman(env, obj);
    if (kf) {
        kf->reset(initialValue);
    }
}

// ========================================================
// VectorStore JNI Bindings
// ========================================================

static minni::logic::VectorStore* getHandleVectorStore(JNIEnv* env, jobject obj) {
    jclass cls = env->GetObjectClass(obj);
    jfieldID fid = env->GetFieldID(cls, "nativeHandle", "J");
    jlong handle = env->GetLongField(obj, fid);
    return reinterpret_cast<minni::logic::VectorStore*>(handle);
}

JNIEXPORT void JNICALL
Java_com_minni_framework_core_VectorStore_nativeInit(JNIEnv* env, jobject obj, jboolean useQuantization) {
    auto* vs = new minni::logic::VectorStore(useQuantization);
    jclass cls = env->GetObjectClass(obj);
    jfieldID fid = env->GetFieldID(cls, "nativeHandle", "J");
    env->SetLongField(obj, fid, reinterpret_cast<jlong>(vs));
}

JNIEXPORT void JNICALL
Java_com_minni_framework_core_VectorStore_nativeFree(JNIEnv* env, jobject obj) {
    auto* vs = getHandleVectorStore(env, obj);
    if (vs) {
        delete vs;
        jclass cls = env->GetObjectClass(obj);
        jfieldID fid = env->GetFieldID(cls, "nativeHandle", "J");
        env->SetLongField(obj, fid, 0);
    }
}

JNIEXPORT jboolean JNICALL
Java_com_minni_framework_core_VectorStore_nativeAddVector(JNIEnv* env, jobject obj, jstring id, jfloatArray vector) {
    auto* vs = getHandleVectorStore(env, obj);
    if (!vs) return JNI_FALSE;

    const char* s_id = env->GetStringUTFChars(id, nullptr);
    jsize len = env->GetArrayLength(vector);
    jfloat* pVec = env->GetFloatArrayElements(vector, nullptr);

    std::vector<float> vec(pVec, pVec + len);
    bool result = vs->add_vector(std::string(s_id), vec);

    env->ReleaseFloatArrayElements(vector, pVec, JNI_ABORT);
    env->ReleaseStringUTFChars(id, s_id);

    return result ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jobject JNICALL
Java_com_minni_framework_core_VectorStore_nativeSearch(JNIEnv* env, jobject obj, jfloatArray query, jint limit) {
    auto* vs = getHandleVectorStore(env, obj);
    if (!vs) return nullptr;

    jsize len = env->GetArrayLength(query);
    jfloat* pQuery = env->GetFloatArrayElements(query, nullptr);
    std::vector<float> qVec(pQuery, pQuery + len);
    env->ReleaseFloatArrayElements(query, pQuery, JNI_ABORT);

    auto results = vs->search(qVec, static_cast<size_t>(limit));

    // Create ArrayList
    jclass arrayListClass = env->FindClass("java/util/ArrayList");
    jmethodID arrayListInit = env->GetMethodID(arrayListClass, "<init>", "()V");
    jobject arrayList = env->NewObject(arrayListClass, arrayListInit);
    jmethodID arrayListAdd = env->GetMethodID(arrayListClass, "add", "(Ljava/lang/Object;)Z");

    // Get SearchResult class constructor
    jclass resultClass = env->FindClass("com/minni/framework/core/VectorStore$SearchResult");
    jmethodID resultInit = env->GetMethodID(resultClass, "<init>", "(Ljava/lang/String;F)V");

    for (const auto& pair : results) {
        jstring idStr = env->NewStringUTF(pair.first.c_str());
        jobject resObj = env->NewObject(resultClass, resultInit, idStr, pair.second);
        env->CallBooleanMethod(arrayList, arrayListAdd, resObj);
        env->DeleteLocalRef(idStr);
        env->DeleteLocalRef(resObj);
    }

    return arrayList;
}

JNIEXPORT jint JNICALL
Java_com_minni_framework_core_VectorStore_nativeGetSize(JNIEnv* env, jobject obj) {
    auto* vs = getHandleVectorStore(env, obj);
    return vs ? static_cast<jint>(vs->size()) : 0;
}

JNIEXPORT void JNICALL
Java_com_minni_framework_core_VectorStore_nativeClear(JNIEnv* env, jobject obj) {
    auto* vs = getHandleVectorStore(env, obj);
    if (vs) vs->clear();
}

JNIEXPORT jboolean JNICALL
Java_com_minni_framework_core_VectorStore_nativeSave(JNIEnv* env, jobject obj, jstring path, jstring encryptionKey) {
    auto* vs = getHandleVectorStore(env, obj);
    if (!vs) return JNI_FALSE;

    const char* s_path = env->GetStringUTFChars(path, nullptr);
    const char* s_key = encryptionKey ? env->GetStringUTFChars(encryptionKey, nullptr) : "";

    bool result = vs->save(std::string(s_path), std::string(s_key));

    env->ReleaseStringUTFChars(path, s_path);
    if (encryptionKey) env->ReleaseStringUTFChars(encryptionKey, s_key);

    return result ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_com_minni_framework_core_VectorStore_nativeLoad(JNIEnv* env, jobject obj, jstring path, jstring encryptionKey) {
    auto* vs = getHandleVectorStore(env, obj);
    if (!vs) return JNI_FALSE;

    const char* s_path = env->GetStringUTFChars(path, nullptr);
    const char* s_key = encryptionKey ? env->GetStringUTFChars(encryptionKey, nullptr) : "";

    bool result = vs->load(std::string(s_path), std::string(s_key));

    env->ReleaseStringUTFChars(path, s_path);
    if (encryptionKey) env->ReleaseStringUTFChars(encryptionKey, s_key);

    return result ? JNI_TRUE : JNI_FALSE;
}

// ========================================================
// ModelRunner (GenAI) JNI Bindings
// ========================================================

static minni::genai::ModelRunner* getHandleModelRunner(JNIEnv* env, jobject obj) {
    jclass cls = env->GetObjectClass(obj);
    jfieldID fid = env->GetFieldID(cls, "nativeHandle", "J");
    jlong handle = env->GetLongField(obj, fid);
    return reinterpret_cast<minni::genai::ModelRunner*>(handle);
}

JNIEXPORT void JNICALL
Java_com_minni_framework_genai_ModelRunner_nativeInit(JNIEnv* env, jobject obj) {
    auto* runner = new minni::genai::ModelRunner();
    jclass cls = env->GetObjectClass(obj);
    jfieldID fid = env->GetFieldID(cls, "nativeHandle", "J");
    env->SetLongField(obj, fid, reinterpret_cast<jlong>(runner));
}

JNIEXPORT void JNICALL
Java_com_minni_framework_genai_ModelRunner_nativeFree(JNIEnv* env, jobject obj) {
    auto* runner = getHandleModelRunner(env, obj);
    if (runner) {
        delete runner;
        jclass cls = env->GetObjectClass(obj);
        jfieldID fid = env->GetFieldID(cls, "nativeHandle", "J");
        env->SetLongField(obj, fid, 0);
    }
}

JNIEXPORT jboolean JNICALL
Java_com_minni_framework_genai_ModelRunner_nativeLoadModel(JNIEnv* env, jobject obj, jstring path, jint numThreads) {
    auto* runner = getHandleModelRunner(env, obj);
    if (!runner) return JNI_FALSE;

    const char* s_path = env->GetStringUTFChars(path, nullptr);
    bool result = runner->load_model(std::string(s_path), numThreads);
    env->ReleaseStringUTFChars(path, s_path);

    return result ? JNI_TRUE : JNI_FALSE;
}

// Helper to convert Java Tensor to C++ Tensor
minni::genai::Tensor javaTensorToCpp(JNIEnv* env, jobject jTensor) {
    minni::genai::Tensor cTensor;
    jclass tensorCls = env->GetObjectClass(jTensor);

    // Get Name
    jfieldID nameField = env->GetFieldID(tensorCls, "name", "Ljava/lang/String;");
    jstring jName = (jstring)env->GetObjectField(jTensor, nameField);
    const char* s_name = env->GetStringUTFChars(jName, nullptr);
    cTensor.name = std::string(s_name);
    env->ReleaseStringUTFChars(jName, s_name);

    // Get Shape
    jfieldID shapeField = env->GetFieldID(tensorCls, "shape", "[I");
    jintArray jShape = (jintArray)env->GetObjectField(jTensor, shapeField);
    jint* pShape = env->GetIntArrayElements(jShape, nullptr);
    jsize shapeLen = env->GetArrayLength(jShape);
    cTensor.shape.assign(pShape, pShape + shapeLen);
    env->ReleaseIntArrayElements(jShape, pShape, JNI_ABORT);

    // Get Data
    jfieldID dataField = env->GetFieldID(tensorCls, "data", "[F");
    jfloatArray jData = (jfloatArray)env->GetObjectField(jTensor, dataField);
    jfloat* pData = env->GetFloatArrayElements(jData, nullptr);
    jsize dataLen = env->GetArrayLength(jData);
    cTensor.data.assign(pData, pData + dataLen);
    env->ReleaseFloatArrayElements(jData, pData, JNI_ABORT);

    return cTensor;
}

// Helper to convert C++ Tensor to Java Tensor
jobject cppTensorToJava(JNIEnv* env, const minni::genai::Tensor& cTensor) {
    jclass tensorCls = env->FindClass("com/minni/framework/genai/Tensor");
    jmethodID ctor = env->GetMethodID(tensorCls, "<init>", "(Ljava/lang/String;[I[F)V");

    jstring jName = env->NewStringUTF(cTensor.name.c_str());

    jintArray jShape = env->NewIntArray(cTensor.shape.size());
    env->SetIntArrayRegion(jShape, 0, cTensor.shape.size(), (const jint*)cTensor.shape.data());

    jfloatArray jData = env->NewFloatArray(cTensor.data.size());
    env->SetFloatArrayRegion(jData, 0, cTensor.data.size(), cTensor.data.data());

    return env->NewObject(tensorCls, ctor, jName, jShape, jData);
}

JNIEXPORT jobjectArray JNICALL
Java_com_minni_framework_genai_ModelRunner_nativePredict(JNIEnv* env, jobject obj, jobjectArray inputTensors) {
    auto* runner = getHandleModelRunner(env, obj);
    if (!runner) return nullptr;

    int inputCount = env->GetArrayLength(inputTensors);
    std::vector<minni::genai::Tensor> inputs;
    inputs.reserve(inputCount);

    for (int i = 0; i < inputCount; ++i) {
        jobject jTensor = env->GetObjectArrayElement(inputTensors, i);
        inputs.push_back(javaTensorToCpp(env, jTensor));
        env->DeleteLocalRef(jTensor);
    }

    std::vector<minni::genai::Tensor> outputs = runner->predict(inputs);

    jclass tensorCls = env->FindClass("com/minni/framework/genai/Tensor");
    jobjectArray outputArray = env->NewObjectArray(outputs.size(), tensorCls, nullptr);

    for (size_t i = 0; i < outputs.size(); ++i) {
        jobject jTensor = cppTensorToJava(env, outputs[i]);
        env->SetObjectArrayElement(outputArray, i, jTensor);
        env->DeleteLocalRef(jTensor);
    }

    return outputArray;
}

} // extern "C"
