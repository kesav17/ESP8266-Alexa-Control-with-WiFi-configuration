# 🌐 Sinric Pro Integration Guide

## 1. Create a Device
- Go to [Sinric Pro Dashboard](https://portal.sinric.pro/)
- Create a new device (e.g., "My LED")
- Choose type: `Switch`
- Note the `Device ID` and `API Key`

## 2. Add Credentials in Code
In your code (main.cpp), include:
```cpp
#include <SinricPro.h>
#include <SinricProSwitch.h>

#define APP_KEY   "YourAPIKey"
#define APP_SECRET "YourAppSecret"
#define DEVICE_ID "YourDeviceID"
```

Done! 🎉
