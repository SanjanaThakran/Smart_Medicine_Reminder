# 💊 Smart Medicine Reminder & Patient Adherence System (ESP32)

An IoT-enabled, automated medicine dosage reminder and adherence tracking system powered by an **ESP32 microcontroller**. It features real-time schedule management via a local Web Dashboard, IR sensor-based dose verification, multi-channel caregiver alert dispatch (Twilio WhatsApp & SMTP Email), and non-volatile flash storage logging (LittleFS).

---

## 🌟 Key Features

- 🕒 **NTP Time Synchronization**: Connects to `pool.ntp.org` for accurate real-time clock synchronization (GMT+5:30).
- 🌐 **Web Management Dashboard**: Embedded HTTP web server on port 80 allowing dynamic schedule updates and real-time adherence status monitoring without re-flashing.
- 🖐️ **IR Sensor Take-Verification**: Optical infrared sensors (`IR1` & `IR2`) detect physical container removal to verify dose consumption.
- 🚨 **Multi-Level Alarms**: LED indicators (Blue & Red) paired with a piezoceramic audio buzzer alert the patient during schedule windows.
- 📲 **Instant Caregiver Alerts**: Automatically dispatches **WhatsApp notifications (Twilio API)** and **SMTP Emails (Gmail)** if a dose remains unverified after 30 seconds.
- 📁 **Flash Adherence Logs**: Historical adherence record (`/log.csv`) saved in LittleFS for medical reviews.

---

## 📐 Hardware Pinout Mapping

| Component | Pin (ESP32) | Function |
| :--- | :--- | :--- |
| **IR Sensor 1** | `GPIO 21` | Optical verification for Medicine 1 container |
| **IR Sensor 2** | `GPIO 4` | Optical verification for Medicine 2 container |
| **Blue LED** | `GPIO 5` | Visual alarm for Medicine 1 |
| **Red LED** | `GPIO 15` | Visual alarm for Medicine 2 |
| **Audio Buzzer** | `GPIO 18` | Acoustic alarm output |

---

## 📂 Project Architecture

The code is modularized into dedicated header and C++ implementation files for clean code segregation:

```
SmartMedicineReminder/
├── SmartMedicineReminder.ino   # Main setup() and loop() entry point
├── config.h                    # Pinouts, WiFi, SMTP, and Twilio credentials
├── globals.h                   # Shared state variable declarations
├── globals.cpp                 # Shared state variable definitions
├── alarm_manager.h             # Alarm & IR sensor monitoring interface
├── alarm_manager.cpp           # Hardware pin logic & 30-sec timeout check
├── notifications.h             # Multi-channel alert dispatch interface
├── notifications.cpp           # Twilio WhatsApp & Gmail SMTP implementation
├── logger.h                    # LittleFS file logging & web history UI interface
├── logger.cpp                  # LittleFS adherence log operations
├── web_server.h                # Web dashboard route binding interface
├── web_server.cpp              # Embedded web UI & JSON status endpoint
├── .gitignore                  # Git tracking rules
└── README.md                   # Project documentation
```

---

## 🛠️ Software Dependencies

Before flashing the ESP32 via Arduino IDE or PlatformIO, ensure the following libraries are installed:

- `ESP_Mail_Client` (by Mobizt)
- `LittleFS` (Built-in ESP32 core package)
- `WebServer` (Built-in ESP32 core package)
- `Preferences` (Built-in ESP32 core package)
- `WiFiClientSecure` & `HTTPClient`

---

## 🚀 Setup & Flashing Instructions

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/YOUR_USERNAME/Smart-Medicine-Reminder.git
   ```
2. **Configure Parameters**:
   Open `config.h` and configure your:
   - WiFi SSID and Password
   - SMTP Email & Gmail App Password
   - Twilio Account SID, Auth Token, and WhatsApp numbers
3. **Flash to ESP32**:
   - Open `SmartMedicineReminder.ino` in Arduino IDE.
   - Select **Board**: `ESP32 Dev Module`.
   - Select **Partition Scheme**: `Default 4MB with spiffs/littlefs`.
   - Upload the sketch.
4. **Access Dashboard**:
   - Open Serial Monitor (`115200` baud) to find the assigned IP address.
   - Open `http://<ESP32-IP-Address>` in any browser.

---

## 👤 Author

**Sanjana Thakran**  
GitHub: [@SanjanaThakran](https://github.com/SanjanaThakran)
