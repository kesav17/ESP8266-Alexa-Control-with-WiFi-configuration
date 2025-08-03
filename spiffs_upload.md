# 📦 How to Upload SPIFFS Files (Web Files)

1. Place HTML and jquery files inside the `data/` directory.
2. Open PlatformIO terminal and run:
   ```
   pio run --target buildfs
   pio run --target uploadfs
   ```

This creates and uploads the SPIFFS file system to your ESP8266.
