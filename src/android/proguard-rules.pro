# ProGuard rules for the Minni library
# Keep JNI native methods from being stripped
-keepclasseswithmembernames class * {
    native <methods>;
}

# Keep the Java wrappers for our C++ classes
-keep class com.minni.framework.core.KnowledgeGraph { *; }
-keep class com.minni.framework.core.DSPKernel { *; }
