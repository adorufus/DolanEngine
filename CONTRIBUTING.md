# Contributing to GameEngine

First off, thank you for considering contributing to GameEngine! 🎉

## 🤝 How to Contribute

### Reporting Bugs
- Use the GitHub issue tracker
- Describe the bug clearly with steps to reproduce
- Include system information (OS, GPU, compiler)
- Provide error messages and logs

### Suggesting Features
- Open an issue with the "feature request" label
- Explain the use case and benefits
- Provide examples or mockups if possible

### Pull Requests
1. Fork the repository
2. Create a feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## 📋 Coding Standards

### C++ Style
- Follow existing code style
- Use meaningful variable names
- Add comments for complex logic
- Prefer `const` and `constexpr` where possible
- Use smart pointers (`Ref<T>`, `Scope<T>`)

### Example
```cpp
// Good
Ref<Scene> CreateScene(const std::string& name) {
    auto scene = CreateRef<Scene>(name);
    scene->OnStart();
    return scene;
}

// Avoid
Scene* MakeScene(string n) {
    Scene* s = new Scene(n);
    s->Start();
    return s;
}
```

### Commit Messages
- Use present tense ("Add feature" not "Added feature")
- Use imperative mood ("Move cursor to..." not "Moves cursor to...")
- Limit the first line to 72 characters
- Reference issues and pull requests

Example:
```
Add particle color gradient support

- Implement gradient evaluation
- Add editor UI for gradient editing
- Update ParticleEmitter component

Fixes #123
```

## 🏗️ Architecture Guidelines

### Adding Components
1. Define in `Engine/include/Engine/Scene/Components.h`
2. Add serialization in `SceneSerializer.cpp`
3. Add editor UI in `SceneHierarchyPanel.cpp`
4. Update documentation

### Adding Systems
1. Create in appropriate directory (`Engine/src/`)
2. Follow existing system patterns
3. Integrate with main loop
4. Add profiling markers

## 🧪 Testing

- Test on multiple platforms if possible
- Ensure no memory leaks (use sanitizers)
- Check performance impact
- Update benchmarks if relevant

## 📝 Documentation

- Update relevant `.md` files
- Add code examples for new features
- Document public API functions
- Include usage examples

## ❓ Questions?

Feel free to ask questions in:
- GitHub Discussions
- Issue comments
- Pull Request comments

Thank you for contributing! 🚀

