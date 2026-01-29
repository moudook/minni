# Config Files

Settings for the framework.

## Structure
```
config/
├── default_config.json    # Default settings
├── model_configs/         # Per model settings
├── device_configs/        # Per phone settings
└── optimization_profiles/ # Different modes
```

## Types
- **Default**: Works on all phones
- **Model**: Specific to each AI model
- **Device**: For different phone specs
- **Profiles**: Different modes (fast, battery saving, etc.)