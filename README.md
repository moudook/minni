# Edge AI Framework for Android

Run AI models directly on Android phones without internet. Works on all devices, even without GPUs.

## Structure
```
├── idea.md                    # Project goals
├── .model/                    # Model configs and data
├── models_capability/         # Model performance tests
│   ├── vision/                # Image models
│   ├── language/              # Text models
│   ├── audio/                 # Audio models
│   └── multimodal/            # Multi type models
├── src/                       # Source code
│   ├── core/                  # Main functions
│   ├── android/               # Android specific
│   ├── optimization/          # Model shrinking
│   └── utils/                 # Helper functions
├── testing/                   # Tests
│   ├── unit/                  # Small tests
│   ├── integration/           # Combined tests
│   ├── performance/           # Speed tests
│   └── compatibility/         # Device compatibility tests
├── research/                  # Research notes
├── docs/                      # Documentation
├── examples/                  # Examples
├── benchmarks/                # Speed benchmarks
├── tools/                     # Development tools
├── config/                    # Settings
└── scripts/                   # Automation
```

## Start Here
1. Read `idea.md` to understand the project
2. Check `research/` for what we have learned
3. Look at `examples/` for how to use

## AI Assistant Guide
AI helpers: read `.qwen/README.md` to understand how to manage this codebase.