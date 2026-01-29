# Testing

Different kinds of tests for the framework.

## Structure
```
testing/
├── unit/             # Test small parts
├── integration/      # Test parts working together
├── performance/      # Test speed and memory
└── compatibility/    # Test on different phones
```

## Types
- **Unit**: Test one function at a time
- **Integration**: Test how functions work together
- **Performance**: Check speed, memory, battery
- **Compatibility**: Make sure it works on all phones

## Rules
- Test on CPU only phones (required)
- Test with GPU too (bonus)
- Check different Android versions
- Measure battery usage