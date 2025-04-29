#include <SoftwareSerial.h>

// Bluetooth HM-10 pins
#define BT_RX 13
#define BT_TX 12
SoftwareSerial BTSerial(BT_RX, BT_TX);

// Radar pins
#define RADAR_RX 10
#define RADAR_TX 11
SoftwareSerial radarSerial(RADAR_RX, RADAR_TX);

// Alarm or LED output
#define OUTPUT_PIN 4

HardwareSerial& usb_serial = Serial;

String radarBuffer = "";

void setup() {
  pinMode(OUTPUT_PIN, OUTPUT);
  digitalWrite(OUTPUT_PIN, LOW);

  usb_serial.begin(19200);       // For debugging
  BTSerial.begin(9600);          // HM-10 Bluetooth
  radarSerial.begin(19200);      // OPS243 radar

  delay(500);
  radarSerial.write("\r\nOU\r\n");  // Optional radar config
  BTSerial.println("System ready.");
  usb_serial.println("System ready.");
}

void loop() {
  // Only process radar data
  readRadarData();
}

void readRadarData() {
  while (radarSerial.available()) {
    char c = radarSerial.read();

    if (c != '\n' && c != '\r') {
      radarBuffer += c;
    } else {
      processRadarLine(radarBuffer);
      radarBuffer = "";
    }
  }
}

void processRadarLine(String line) {
  line.trim();
  if (line.indexOf("ALERT") != -1) {
    // ALERT detected
    usb_serial.println("🚨 ALERT detected!");
    BTSerial.println(line);

    digitalWrite(OUTPUT_PIN, HIGH);
    delay(3000); // Keep alarm on for 3 full seconds
    digitalWrite(OUTPUT_PIN, LOW);
  }
}