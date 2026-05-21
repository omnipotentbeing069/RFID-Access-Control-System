/*
 * ============================================================
 *  RFID-Based Access Control System
 *  Author  : Saksham Chamoli
 *  Hardware: ESP32 DevKit v1 + MFRC522 + 16x2 I2C LCD + Relay
 * ============================================================
 *  Libraries required (Arduino Library Manager):
 *    - MFRC522           by GithubCommunity
 *    - LiquidCrystal_I2C by Frank de Brabander
 *
 *  Wiring (MFRC522 via SPI):
 *    MFRC522 SDA  --> GPIO5
 *    MFRC522 SCK  --> GPIO18
 *    MFRC522 MOSI --> GPIO23
 *    MFRC522 MISO --> GPIO19
 *    MFRC522 RST  --> GPIO27
 *    MFRC522 3.3V --> 3.3V
 *    MFRC522 GND  --> GND
 *
 *  Relay / Output:
 *    Relay IN     --> GPIO26
 *    Green LED    --> GPIO25 (Access Granted)
 *    Red LED      --> GPIO33 (Access Denied)
 * ============================================================
 */

#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ── Pin Definitions ──────────────────────────────────────────
#define SS_PIN      5    // MFRC522 SDA/SS
#define RST_PIN     27   // MFRC522 RST
#define RELAY_PIN   26   // Relay (door lock)
#define GREEN_LED   25   // Access granted LED
#define RED_LED     33   // Access denied LED

// ── Objects ──────────────────────────────────────────────────
MFRC522 rfid(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ── Authorised UIDs ──────────────────────────────────────────
// Add your RFID card UIDs here (scan with Serial Monitor first)
// Format: {0xAB, 0xCD, 0xEF, 0x12}
const byte AUTHORISED_UIDS[][4] = {
  {0xDE, 0xAD, 0xBE, 0xEF},   // Card 1 — replace with your UID
  {0x12, 0x34, 0x56, 0x78},   // Card 2 — replace with your UID
};
const int NUM_CARDS = sizeof(AUTHORISED_UIDS) / sizeof(AUTHORISED_UIDS[0]);

// ── Timing ────────────────────────────────────────────────────
#define ACCESS_DELAY  2000   // ms door stays unlocked
#define DENIED_DELAY  1500   // ms denied message duration

// ── Helper: Compare UIDs ──────────────────────────────────────
bool isAuthorised(byte *uid, byte uidSize) {
  if (uidSize != 4) return false;
  for (int i = 0; i < NUM_CARDS; i++) {
    bool match = true;
    for (int j = 0; j < 4; j++) {
      if (uid[j] != AUTHORISED_UIDS[i][j]) {
        match = false;
        break;
      }
    }
    if (match) return true;
  }
  return false;
}

// ── Helper: Print UID to Serial ───────────────────────────────
void printUID(byte *uid, byte size) {
  Serial.print("UID: ");
  for (byte i = 0; i < size; i++) {
    Serial.print(uid[i] < 0x10 ? "0" : "");
    Serial.print(uid[i], HEX);
    if (i < size - 1) Serial.print(":");
  }
  Serial.println();
}

// ── Grant Access ─────────────────────────────────────────────
void grantAccess() {
  Serial.println("✅ Access GRANTED");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" Access GRANTED ");
  lcd.setCursor(0, 1);
  lcd.print("  Door Unlocked ");

  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(RED_LED,   LOW);
  digitalWrite(RELAY_PIN, HIGH);   // Unlock

  delay(ACCESS_DELAY);

  digitalWrite(RELAY_PIN, LOW);    // Lock again
  digitalWrite(GREEN_LED, LOW);
}

// ── Deny Access ──────────────────────────────────────────────
void denyAccess() {
  Serial.println("❌ Access DENIED");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" Access DENIED  ");
  lcd.setCursor(0, 1);
  lcd.print(" Unauthorised!  ");

  digitalWrite(RED_LED,   HIGH);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RELAY_PIN, LOW);

  delay(DENIED_DELAY);

  digitalWrite(RED_LED, LOW);
}

// ── Setup ────────────────────────────────────────────────────
void setup() {
  Serial.begin(9600);

  pinMode(RELAY_PIN, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED,   OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED,   LOW);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print(" RFID  Access   ");
  lcd.setCursor(0, 1);
  lcd.print(" Control System ");
  delay(2000);
  lcd.clear();

  SPI.begin();
  rfid.PCD_Init();

  Serial.println("RFID Access Control System Ready.");
  Serial.println("Scan your card...\n");

  lcd.setCursor(0, 0);
  lcd.print("  Scan  Card    ");
  lcd.setCursor(0, 1);
  lcd.print("                ");
}

// ── Main Loop ────────────────────────────────────────────────
void loop() {
  // Wait for a new card
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    return;
  }

  // Print scanned UID
  printUID(rfid.uid.uidByte, rfid.uid.size);

  // Check authorisation
  if (isAuthorised(rfid.uid.uidByte, rfid.uid.size)) {
    grantAccess();
  } else {
    denyAccess();
  }

  // Halt card to prevent repeated reads
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();

  // Return to idle
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("  Scan  Card    ");
}
