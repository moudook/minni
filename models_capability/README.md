# Model Capability Tests

Test how well different AI models work on phones.

## Structure
```
models_capability/
├── vision/           # Image models
├── language/         # Text models
├── audio/            # Sound models
└── multimodal/       # Multiple types together
```

## Test These Things
- CPU only performance (required)
- GPU performance (if available)
- Memory usage
- Battery drain
- Accuracy vs original model
- Problems on different phones
- Ways to make it faster

## File Names
`[model_name]_[type]_capabilities.md`
Example: `llama_text_language_capabilities.md`