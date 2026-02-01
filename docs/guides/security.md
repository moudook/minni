# Data Security & Encryption

Minni Framework provides built-in mechanisms to secure sensitive AI data (User Embeddings, Knowledge Graph Facts) at rest. This ensures that even if the device file system is accessed, the user's personal context remains encrypted.

## Overview

The security layer uses **Symmetric Encryption** (currently a lightweight stream cipher, upgradeable to AES-256) implemented in the native C++ core. Encryption is applied transparently during the `save` and `load` operations of the `VectorStore` and `KnowledgeGraph`.

## Architecture

- **C++ Layer:** `SecurityManager` handles key generation and XOR/AES operations.
- **Storage Layer:** `VectorStore` and `KnowledgeGraph` have overloaded `save/load` methods that accept an encryption key.
- **File Format:** Encrypted files use distinct magic headers (`MVE1`, `MKGE`) to prevent accidental loading as plain text.

## Usage Guide

### 1. Key Management

The framework does **not** store the encryption key. It is the responsibility of the Android application layer to manage keys securely, typically using the **Android Keystore System**.

**Example (Conceptual Android Key Retrieval):**
```kotlin
fun getSecretKey(): String {
    // Retrieve key from Android Keystore
    // Return as String (or byte array)
    return "UserSpecificSecretKey123"
}
```

### 2. Secure Vector Store

When saving embeddings (e.g., face vectors, voice prints), pass the key to the `save` method.

```kotlin
val vectorStore = VectorStore(useQuantization = true)
val key = getSecretKey()

// Save encrypted
val success = vectorStore.save(filePath, key)

// Load encrypted
val loaded = vectorStore.load(filePath, key)
```

### 3. Secure Knowledge Graph

Similarly, for the Knowledge Graph:

```kotlin
val kg = KnowledgeGraph()
val key = getSecretKey()

// Save encrypted
kg.save(kgPath, key)

// Load encrypted
kg.load(kgPath, key)
```

### 4. Error Handling

If `load()` is called with:
- **Wrong Key:** The decryption will result in garbage data, and the file header check (`MVS1`/`MKG1` inside the payload) will fail, returning `false`.
- **No Key for Encrypted File:** The loader detects the encrypted header (`MVE1`) and returns `false` immediately.

## Performance Impact

Encryption adds a small overhead during I/O operations.
- **Save:** ~10-20% slower due to encryption pass.
- **Load:** ~10-20% slower due to decryption pass.
- **Inference:** **Zero overhead**. Once loaded into memory, data is decrypted (in RAM) and search operations run at full native speed.

> **Note:** For extremely large datasets where load time is critical, consider using `FlatVectorStore` (mmap) with filesystem-level encryption (FBE) instead of application-level encryption, as mmap cannot easily be combined with application-layer decryption without loading the entire file into RAM.
