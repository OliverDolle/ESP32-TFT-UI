# Robust Settings System - Technical Documentation

## Overview
The ESP32 TFT UI system now includes a **robust, crash-resistant settings management system** that gracefully handles missing SD cards, corrupted files, and first-time setup scenarios.

---



The system now:
1. Checks if SD card is available
2. Checks if settings.json exists
3. Creates default settings.json if missing
4. Validates existing settings and fills missing keys
5. Handles corrupted JSON gracefully
6. Works even without SD card (RAM-only mode)
7. **Never crashes** - always falls back to safe defaults

---

## System Architecture

### Boot Sequence with Robust Error Handling

```
ESP32 Powers On
    ↓
┌─────────────────────────────────────┐
│ 1. Load Default Settings to RAM    │ ← Always happens first
│    (Safe fallback)                  │
└─────────────────────────────────────┘
    ↓
┌─────────────────────────────────────┐
│ 2. Try to Initialize SD Card       │
└─────────────────────────────────────┘
    ↓
    ├─ SD Card OK? ─────────────┐
    │                            │
   YES                          NO
    │                            │
    ↓                            ↓
┌────────────────────┐    ┌──────────────────────┐
│ Check for          │    │ SD Card Failed!      │
│ settings.json      │    │ • Show error message │
└────────────────────┘    │ • Use RAM defaults   │
    ↓                     │ • Continue running   │
    ├─ File Exists? ─┐   │ • Settings won't save│
    │                │   └──────────────────────┘
   YES              NO           ↓
    │                │           ↓
    ↓                ↓    ┌──────────────────────┐
┌────────────┐  ┌────────────────┐ │  System Ready!       │
│ Read File  │  │ Create Default │ │  (RAM mode)          │
│ Parse JSON │  │ settings.json  │ └──────────────────────┘
└────────────┘  └────────────────┘
    ↓                │
    ├─ Valid? ──┐    │
    │           │    │
   YES         NO    │
    │           │    │
    ↓           ↓    ↓
┌────────────────────────────┐
│ Validate & Fill Missing    │
│ (Add new features)         │
└────────────────────────────┘
    ↓
┌────────────────────────────┐
│ System Ready!              │
│ (Full features)            │
└────────────────────────────┘
```

---

## Key Features

### 1. **Always Safe Defaults**
```cpp
void loadSettings() {
  // Always create defaults FIRST
  createDefaultSettings();
  
  // Then try to load from SD card
  if (!sdCardAvailable) {
    // SD failed - use RAM defaults
    return;
  }
  // ... continue with SD card loading
}
```

### 2. **SD Card Availability Tracking**
```cpp
bool sdCardAvailable = false;  // Global flag

void initSD() {
  if (!SD.begin(SD_CS_PIN)) {
    sdCardAvailable = false;  // Mark as unavailable
    // Show helpful error message
  } else {
    sdCardAvailable = true;   // Mark as available
  }
}

bool isSDCardAvailable() {
  return sdCardAvailable;
}
```

### 3. **Auto-Create Missing settings.json**
```cpp
if (!file_exists) {
  createDefaultSettings();
  saveSettings();  // Auto-create file
}
```

### 4. **Corrupted JSON Recovery**
```cpp
DeserializationError error = deserializeJson(settings, file);
if (error) {
  // JSON corrupted - recreate with defaults
  createDefaultSettings();
  saveSettings();
}
```

### 5. **Missing Key Detection & Fill**
```cpp
void validateAndFillMissingSettings() {
  bool modified = false;
  
  // Check each setting
  if (!settings.containsKey("new_feature")) {
    settings["new_feature"] = defaultValue;
    modified = true;
  }
  
  // Save if we added anything
  if (modified) {
    saveSettings();
  }
}
```

### 6. **Safe Save with Verification**
```cpp
bool saveSettings() {
  if (!sdCardAvailable) {
    return false;  // Can't save - SD not available
  }
  
  // Delete old file first
  if (SD.exists(SETTINGS_FILE)) {
    SD.remove(SETTINGS_FILE);
  }
  
  // Write new file
  File file = SD.open(SETTINGS_FILE, FILE_WRITE);
  if (file) {
    serializeJsonPretty(settings, file);
    file.close();
    
    // Verify it worked
    File verify = SD.open(SETTINGS_FILE, FILE_READ);
    if (verify) {
      verify.close();
      return true;  // Success!
    }
  }
  return false;  // Failed
}
```

---

## Error Scenarios & Handling

### Scenario 1: **No SD Card Inserted**

**What Happens:**
```
1. System boots
2. Tries to initialize SD card → FAILS
3. Sets sdCardAvailable = false
4. Shows error message in Serial Monitor
5. Continues with RAM defaults
6. System runs normally
7. Changes NOT saved (RAM only)
```

**Serial Output:**
```
╔════════════════════════════════╗
║ SD CARD INITIALIZATION FAILED  ║
╚════════════════════════════════╝
Please check:
  1. SD card is inserted
  2. SD card is formatted as FAT32
  3. SD_CS pin wiring (default GPIO 4)

WARNING: Settings will NOT be saved!
System will run with defaults from RAM only
```

**User Experience:**
- System works normally
- All features available
- Settings don't persist across reboots
- Status message shown on TFT (optional - can be added)

---

### Scenario 2: **New/Blank SD Card (No settings.json)**

**What Happens:**
```
1. System boots
2. SD card initializes → SUCCESS
3. Looks for settings.json → NOT FOUND
4. Creates default settings in RAM
5. Saves to /settings.json on SD card
6. Verifies file was created
7. System ready with defaults
```

**Serial Output:**
```
✓ SD card initialized successfully
  Card Size: 8192 MB
  Card Type: SDHC
Settings file not found
Creating default settings.json...
✓ Settings saved successfully
  File size: 456 bytes
✓ Default settings.json created
System ready!
```

**Result:**
- New settings.json created automatically
- All defaults applied
- Future changes will be saved
- Works immediately

---

### Scenario 3: **Corrupted settings.json**

**What Happens:**
```
1. System boots
2. SD card initializes → SUCCESS
3. Finds settings.json → EXISTS
4. Tries to parse JSON → FAILS
5. Shows parse error
6. Recreates settings.json with defaults
7. System ready
```

**Serial Output:**
```
✓ Settings file found, reading...
ERROR: JSON parse failed: InvalidInput
Using default settings and creating new file...
✓ Settings saved successfully
System ready!
```

**Result:**
- Corrupted file replaced with good one
- System continues normally
- No crash or hang
- Data lost but system recovers

---

### Scenario 4: **Old settings.json (Missing New Features)**

**What Happens:**
```
1. System boots
2. SD card initializes → SUCCESS
3. Loads settings.json → SUCCESS
4. Checks for new keys (e.g., cc1101_frequency)
5. Adds missing keys with defaults
6. Saves updated settings.json
7. System ready with full features
```

**Serial Output:**
```
✓ Settings loaded successfully
Added missing settings, updating file...
✓ Settings saved successfully
System ready!
```

**Result:**
- Old settings preserved
- New features added automatically
- Seamless upgrade experience
- No user action needed

---

### Scenario 5: **SD Card Write-Protected**

**What Happens:**
```
1. System boots
2. SD card initializes → SUCCESS (read-only)
3. Loads settings.json → SUCCESS
4. User changes setting → saveSettings() called
5. Can't write to SD card → FAILS
6. Shows error message
7. Change remains in RAM until reboot
```

**Serial Output:**
```
Saving settings to SD card...
ERROR: Could not open settings file for writing
Check SD card is not write-protected
```

**Result:**
- System doesn't crash
- Settings not saved to SD
- Will revert on reboot
- User warned via serial

---

## API Reference

### Functions

#### `void initSD()`
Initializes the SD card and sets global `sdCardAvailable` flag.

**Returns:** Nothing  
**Sets:** `sdCardAvailable` (true/false)  
**Serial Output:** Detailed success/error messages

#### `void loadSettings()`
Loads settings from SD card or uses defaults.

**Behavior:**
1. Always creates RAM defaults first
2. If SD available: tries to load settings.json
3. If file missing: creates new one
4. If JSON corrupt: recreates file
5. Validates and fills missing keys
6. Never crashes - always falls back safely

**Returns:** Nothing  
**Updates:** Global `settings` object

#### `bool saveSettings()`
Saves current settings to SD card.

**Returns:**  
- `true` - Settings saved successfully
- `false` - Save failed (no SD or write error)

**Behavior:**
- Checks `sdCardAvailable` first
- Deletes old file before writing new one
- Uses pretty-printed JSON (human-readable)
- Verifies write succeeded

#### `void createDefaultSettings()`
Populates `settings` object with factory defaults.

**Use Cases:**
- First boot (no settings.json)
- Corrupted settings.json
- Manual reset to defaults

#### `void validateAndFillMissingSettings()`
Checks for missing keys and adds them with defaults.

**Purpose:**  
Handles version upgrades where new features add new settings keys.

**Example:**
```cpp
// Old settings.json doesn't have CC1101 settings
// This function adds them automatically:
if (!settings.containsKey("cc1101_frequency")) {
  settings["cc1101_frequency"] = 433.92;
  modified = true;
}
```

#### `bool isSDCardAvailable()`
Check if SD card is currently available.

**Returns:**  
- `true` - SD card working
- `false` - SD card failed or missing

**Use Case:**
```cpp
if (isSDCardAvailable()) {
  // Safe to save
  saveSettings();
} else {
  // Warn user
  showSDCardError();
}
```

---

## Default Settings

```json
{
  "nrf_address1": "\u00e7\u00e7\u00e7\u00e7\u00e7",
  "nrf_address2": "\u00d7\u00d7\u00d7\u00d7\u00d7",
  "nrf_data_rate": "2MBPS",
  "cc1101_frequency": 433.92,
  "cc1101_modulation": "ASK",
  "cc1101_power": 10,
  "wifi_ssid": "ESP32_Extender",
  "wifi_pass": "password",
  "extender_mode": false,
  "sta_ssid": "",
  "sta_pass": "",
  "tft_cs": 5,
  "tft_dc": 17,
  "tft_rst": 16,
  "touch_cs": 21,
  "touch_irq": 22,
  "sd_cs": 4,
  "nrf1_ce": 25,
  "nrf1_csn": 26,
  "nrf2_ce": 27,
  "nrf2_csn": 32,
  "cc1101_csn": 33,
  "cc1101_gdo0": 14,
  "cc1101_gdo2": 12,
  "splash_file": "/splash.bmp",
  "splash_duration": 3000
}
```

---

## User-Facing Error Messages

### On TFT Display (Optional Feature)
When SD card fails, can show:

```
╔═══════════════════════════╗
║     SD CARD ERROR         ║
╠═══════════════════════════╣
║ Please check:             ║
║ 1. SD card is inserted    ║
║ 2. Formatted as FAT32     ║
║ 3. SD_CS pin (GPIO 4)     ║
║                           ║
║ Using defaults...         ║
║ Settings won't be saved   ║
╚═══════════════════════════╝
```

### On Serial Monitor
Detailed technical information for debugging.

---

## Testing Scenarios

### Test 1: Fresh Install
1. Format SD card as FAT32
2. Insert into module (empty, no files)
3. Power on ESP32
4. **Expected:** settings.json created automatically
5. **Verify:** Check SD card - file should exist

### Test 2: No SD Card
1. Remove SD card completely
2. Power on ESP32
3. **Expected:** System runs with defaults
4. **Verify:** Serial shows SD error, system continues
5. **Note:** Settings won't persist

### Test 3: Corrupted File
1. Create invalid settings.json: `{corrupt data!!!}`
2. Power on ESP32
3. **Expected:** File replaced with good defaults
4. **Verify:** settings.json is now valid JSON

### Test 4: Old Version Upgrade
1. Use settings.json without `cc1101_frequency`
2. Power on ESP32
3. **Expected:** Missing keys added automatically
4. **Verify:** settings.json now has all keys

### Test 5: Write-Protected Card
1. Lock SD card (write-protect switch)
2. Change a setting via UI
3. **Expected:** Error message, change not saved
4. **Verify:** Reboot shows old value

---

## Advantages

**Never Crashes** - All error paths handled  
**Self-Healing** - Recreates missing/corrupt files  
**Graceful Degradation** - Works without SD (RAM mode)  
**Forward Compatible** - Auto-adds new settings  
**User-Friendly** - Clear error messages  
**Developer-Friendly** - Easy to add new settings  
**Production-Ready** - Handles all edge cases  

---

## Adding New Settings (For Developers)

### Step 1: Add to `createDefaultSettings()`
```cpp
void createDefaultSettings() {
  // ...existing settings...
  settings["my_new_setting"] = defaultValue;
}
```

### Step 2: Add to `validateAndFillMissingSettings()`
```cpp
void validateAndFillMissingSettings() {
  // ...existing checks...
  if (!settings.containsKey("my_new_setting")) {
    settings["my_new_setting"] = defaultValue;
    modified = true;
  }
}
```

### Step 3: Use in Code
```cpp
int myValue = settings["my_new_setting"] | defaultValue;
```

**Done!** Old installations will automatically get the new setting.

---


