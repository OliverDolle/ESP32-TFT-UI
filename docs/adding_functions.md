# Adding New Functions/Modules

## Overview
The code is modular: UI in ui.cpp, menu in menu.cpp, each function in its own file.

## Steps to Add a New Function
1. **Create Files**: Add `include/new_module.h` and `src/new_module.cpp`.
2. **Init Function**: Add `initNewModule()` if needed, call in `setup()`.
3. **Menu Function**: Implement `newModuleMenu()` with UI drawing and touch handling.
4. **Add to Menu**: In `menu.cpp`, add to `menuItems` array: `{"New Module", newModuleMenu},` and update `numItems`.
5. **Settings Integration**: If persistent, add keys to `settings` JSON in `loadSettings()` defaults.
6. **Documentation**: Add `docs/new_module.md` with overview, functions, and extension notes.

For new hardware modules:
- Define pins in `config.h`.
- Include library in `platformio.ini`.
- Initialize in new init function.

This structure keeps code maintainable; new features don't touch core files much.