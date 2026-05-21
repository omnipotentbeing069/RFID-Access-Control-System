# RFID-Based Access Control System 🔐

A card-scan authentication system built with ESP32 and MFRC522 RFID reader. Grants or denies access based on registered RFID card UIDs, with relay-controlled door lock, LED indicators, and 16x2 LCD feedback.

---

## 📋 Features

- MFRC522 RFID card scanning via SPI protocol
- Multi-card authorisation (add unlimited card UIDs in code)
- Relay output to control door lock / electric strike
- Green LED (Access Granted) and Red LED (Access Denied)
- 16x2 I2C LCD with real-time status messages
- Serial Monitor UID printer to easily register new cards
- Auto-relock after configurable delay

---

## 🔧 Components Required

| Component              | Quantity | Notes                         |
|------------------------|----------|-------------------------------|
| ESP32 DevKit v1        | 1        |                               |
| MFRC522 RFID Module    | 1        | Comes with cards and key fob  |
| 16x2 I2C LCD Display   | 1        | Address: 0x27 or 0x3F        |
| 5V Relay Module        | 1        | For door lock control         |
| Green LED              | 1        | Access granted indicator      |
| Red LED                | 1        | Access denied indicator       |
| 220Ω Resistors         | 2        | For LEDs                      |
| Jumper wires           | —        |                               |

---

## 🔌 Wiring Diagram

```
MFRC522 Module                   ESP32 DevKit v1
──────────────                   ───────────────
SDA (SS)       ──────────────►  GPIO5
SCK            ──────────────►  GPIO18
MOSI           ──────────────►  GPIO23
MISO           ──────────────►  GPIO19
RST            ──────────────►  GPIO27
3.3V           ──────────────►  3.3V
GND            ──────────────►  GND

I2C LCD 16x2                     ESP32 DevKit v1
────────────                     ───────────────
VCC            ──────────────►  5V
GND            ──────────────►  GND
SDA            ──────────────►  GPIO21
SCL            ──────────────►  GPIO22

Relay Module                     ESP32 DevKit v1
────────────                     ───────────────
IN             ──────────────►  GPIO26
VCC            ──────────────►  5V
GND            ──────────────►  GND

LEDs                             ESP32 DevKit v1
────                             ───────────────
Green LED (+)  → 220Ω ────────► GPIO25
Red LED   (+)  → 220Ω ────────► GPIO33
Both LED  (–)  ──────────────►  GND
```

### Pin Summary

| ESP32 Pin | Connected To        |
|-----------|---------------------|
| GPIO5     | MFRC522 SDA/SS      |
| GPIO18    | MFRC522 SCK         |
| GPIO23    | MFRC522 MOSI        |
| GPIO19    | MFRC522 MISO        |
| GPIO27    | MFRC522 RST         |
| GPIO21    | LCD SDA             |
| GPIO22    | LCD SCL             |
| GPIO26    | Relay IN            |
| GPIO25    | Green LED           |
| GPIO33    | Red LED             |

---

## 📁 Repository Structure

```
RFID-Access-Control-System/
├── RFID_Access_Control.ino     ← Main Arduino sketch
├── schematics/
│   └── schematic.png           ← Circuit schematic
├── README.md
└── LICENSE
```

---

## 📦 Libraries Required

| Library              | Author           |
|----------------------|------------------|
| `MFRC522`            | GithubCommunity  |
| `LiquidCrystal_I2C`  | Frank de Brabander |

---

## 🪪 How to Register a New Card

1. Upload the sketch
2. Open Serial Monitor at 9600 baud
3. Scan any RFID card — the UID will print like: `UID: AB:CD:EF:12`
4. Copy the values into the `AUTHORISED_UIDS` array:
   ```cpp
   const byte AUTHORISED_UIDS[][4] = {
     {0xAB, 0xCD, 0xEF, 0x12},  // your card
   };
   ```
5. Re-upload the sketch

---

## 📄 License

MIT License — see [LICENSE](LICENSE)

---

## 👤 Author

**Saksham Chamoli**
B.Tech ECE — College of Technology, GBPUAT Pantnagar
[LinkedIn](https://www.linkedin.com/in/saksham-chamoli-1a7945333/) | [GitHub](https://github.com/your_username)
