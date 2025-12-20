# Creating Splash Screen Images - Quick Guide

## Image Requirements
- **Size**: 320x240 pixels (landscape) or 240x320 (portrait)
- **Format**: 24-bit BMP (bitmap)
- **Filename**: `splash.bmp` (or custom via settings)
- **Location**: Root of SD card

## Method 1: Windows Paint (Easiest)

### Step-by-Step:
1. **Find or create your image** (logo, picture, etc.)
2. **Open in Paint** (Right-click → Edit with Paint)
3. **Resize**:
   - Click "Resize" in Home tab
   - Select "Pixels"
   - Uncheck "Maintain aspect ratio"
   - Set: **320 width**, **240 height**
   - Click OK
4. **Save**:
   - File → Save As → BMP Picture
   - Name: `splash.bmp`
   - Save to SD card root

## Method 2: GIMP (Free, Professional)

### Step-by-Step:
1. **Open GIMP** (download from gimp.org)
2. **Open your image** (File → Open)
3. **Resize**:
   - Image → Scale Image
   - Width: **320**, Height: **240**
   - Click "Scale"
4. **Convert to RGB**:
   - Image → Mode → RGB
5. **Export**:
   - File → Export As
   - Name: `splash.bmp`
   - Click "Export"
   - Choose: 24-bit, no compression
   - Export to SD card

## Method 3: Online Converter (No software needed)

### Step-by-Step:
1. **Go to**: https://www.img2go.com/resize-image
2. **Upload your image**
3. **Set dimensions**: 320 x 240 pixels
4. **Download resized image**
5. **Convert to BMP**:
   - Go to: https://www.img2go.com/convert-to-bmp
   - Upload resized image
   - Download as BMP
6. **Rename to**: `splash.bmp`
7. **Copy to SD card root**

## Method 4: Using ImageMagick (Command Line)

```bash
# Install ImageMagick first
# Then convert any image:
convert input.jpg -resize 320x240! -type TrueColor -depth 24 splash.bmp
```

## Creating a Custom Splash from Scratch

### Using Paint:
1. **Open Paint**
2. **Click Resize** → Pixels
3. **Set**: 320 x 240
4. **Draw your design**:
   - Add text (use Text tool)
   - Add shapes
   - Add colors
5. **Save as** `splash.bmp` (BMP format)

### Design Tips:
- **Keep it simple**: Too much detail may not show well
- **High contrast**: Use bright colors on dark backgrounds
- **Centered text**: Easier to read
- **Test it**: Preview before final save
- **Landscape**: 320 (width) x 240 (height) works best

## Example Designs

### Option 1: Text Logo
```
╔═══════════════════════════════╗
║                               ║
║       ESP32 TFT SYSTEM        ║
║         Version 1.0           ║
║                               ║
╚═══════════════════════════════╝
```

### Option 2: Project Name
```
╔═══════════════════════════════╗
║                               ║
║         [YOUR LOGO]           ║
║      IoT Control Center       ║
║                               ║
╚═══════════════════════════════╝
```

## Installing the Splash Image

1. **Format SD card** as FAT32
2. **Copy `splash.bmp`** to root (not in a folder!)
3. **Insert SD card** into TFT display module
4. **Power on ESP32**
5. **Watch splash screen** appear for 3 seconds!

## Customizing via Settings Menu

You can now customize splash settings directly on the device:

1. **Main Menu** → Settings → Splash Screen
2. **Options**:
   - **Set Duration**: 1-10 seconds
   - **Default Filename**: Reset to /splash.bmp
   - **Custom Filename**: Use different file (e.g., logo2.bmp)
   - **Test Splash Now**: Preview without rebooting!

## Multiple Splash Images

Want to switch between different splash screens?

1. **Create multiple BMP files**:
   - `logo1.bmp`
   - `logo2.bmp`
   - `startup.bmp`
2. **Copy all to SD card root**
3. **Go to**: Settings → Splash Screen → Custom Filename
4. **Enter**: logo1 (or logo2, startup, etc.)
5. **Save and test!**

## Troubleshooting

### Splash doesn't show
- ✅ Check filename is exactly `splash.bmp` (lowercase)
- ✅ Verify size is exactly 320x240 pixels
- ✅ Ensure format is 24-bit BMP (not PNG/JPG)
- ✅ Confirm SD card is FAT32
- ✅ File is in root, not in a folder

### "BMP size mismatch" error
- Image must be **exactly** 320x240 pixels
- Resize using one of the methods above

### "BMP file not found" error
- Check SD card is inserted
- Verify filename matches settings
- Ensure file is on SD card root

### Image looks wrong
- Make sure it's 24-bit BMP (not 8-bit indexed)
- Try recreating with Paint or GIMP
- Test with a simple solid color image first

### Default screen shows instead
- System shows default when splash.bmp is missing
- This is normal if you haven't added an image yet
- Just add splash.bmp to enable custom splash

## Sample Splash Images

Want ready-made splash screens? Create these simple designs:

### Design 1: Welcome Screen
- Black background
- White text: "ESP32 SYSTEM"
- Blue text: "Initializing..."
- Save as 320x240 BMP

### Design 2: Logo Style
- Gradient background
- Your project name in large font
- Small "Powered by ESP32" at bottom
- Save as 320x240 BMP

### Design 3: Info Screen
- Dark blue background
- Device name at top
- Version number
- Your name/company
- Save as 320x240 BMP

## Advanced: Animated Splash (Future Feature)

Currently shows static image. To add animation:
- Would need multiple BMP files
- Code modification to cycle through them
- Each frame displayed briefly
- Possible future enhancement!

## Quick Test

To verify your splash works:
1. Go to **Settings → Splash Screen**
2. Click **"Test Splash Now"**
3. Your image will display immediately
4. No reboot needed!

This lets you test different images quickly until you find the perfect one!

---

**Remember**: The splash image is optional. If no splash.bmp exists, the system shows a default startup screen and works perfectly fine!
