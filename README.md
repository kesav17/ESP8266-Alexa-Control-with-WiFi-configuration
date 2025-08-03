# ESP8266 WiFi Configuration with Web Portal

This project sets up an ESP8266 (e.g., NodeMCU) as a WiFi manager. If the device cannot connect to a saved WiFi network, it starts in AP mode and serves a configuration page.


## 🔧 Features
- Captive portal (AP mode)
- SPIFFS for HTML storage
- Manual SSID entry
- Sinric Pro compatible
- Voice control from Alexa device

## 📂 Folder Structure
```
ESP8266_WiFi_Config/
├── data/               
│   └── index.html               # Contains HTML files for SPIFFS
│   └── jquery-3.6.0.min.js      # Contains jquery files for SPIFFS
├── src/                
│   └── main.cpp                 # Main source code
├── spiffs_upload.md             # SPIFFS uploading guide
├── sinric_setup.md              # Sinric Pro instructions
├── alexa_integration.md         # Alexa App configuration instructions
└── platformio.ini               # PlatformIO configuration
```


## 🚀 Getting Started

### 1. Requirements
- PlatformIO (VSCode Extension)
- ESP8266 board (NodeMCU, Wemos, etc.)
- USB cable and driver
- Python 3 (for `mkspiffs`, optional)
- See `sinric_setup.md` for Sinric Pro integration.

### 2. Upload HTML & jquery to SPIFFS

See `spiffs_upload.md` for web file uploads.

### 3. Upload Firmware

```bash
pio run --target upload
```

### 4. Connect to ESP

- After reset, it will start in AP mode `ESP_Config`.
- Connect your mobile/laptop to `ESP_Config`.
- Visit `http://192.168.4.1` in browser.
- Submit WiFi credentials.

### 5. Extend Project

You can integrate Sinric Pro commands to control relays, LEDs, etc., once the WiFi setup is done.

### 6. Extend Project

See `alexa_integration.md` to control relays, LEDs, etc., using voice command with Alexa device

---

## 🔗 Resources

- jQuery: https://jquery.com/
- PlatformIO: https://platformio.org/

---

## 📃 License

MIT License. Use at your own risk.

