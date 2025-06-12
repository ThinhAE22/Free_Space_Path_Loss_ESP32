#include <WiFi.h>
#include <cmath>

const char* targetSSID = "TP-Link_CBDC";

float calculateFrequencyMHz(int channel) {
  // Source of Wi-Fi frequency logic: IEEE 802.11 standard
  // For 2.4 GHz: freq = 2407 + 5 * channel
  // For 5 GHz: freq = 5000 + 5 * channel (approximation)
  if (channel >= 1 && channel <= 14) {
    return 2407 + channel * 5;
  } else {
    return 5000 + channel * 5;
  }
}

float calculateDistance(int rssi, float freqMHz) {
  // Log-distance path loss model (ITU approximation)
  float exp = (27.55 - (20.0 * log10(freqMHz)) + abs(rssi)) / 20.0;
  //20*log((4*pi/c) *10**6)  10**6 is taken from freq MHz 
  return pow(10.0, exp); // distance in meters
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect(); // Ensure not connected
  delay(100);

  Serial.println("Scanning for WiFi networks...");
  int n = WiFi.scanNetworks();

  if (n == 0) {
    Serial.println("No networks found.");
    return;
  }

  for (int i = 0; i < n; ++i) {
    String ssid = WiFi.SSID(i);
    int rssi = WiFi.RSSI(i);
    int channel = WiFi.channel(i);

    if (ssid == targetSSID) {
      Serial.println("Target network found!");
      Serial.print("SSID: "); Serial.println(ssid);
      Serial.print("RSSI: "); Serial.print(rssi); Serial.println(" dBm");
      Serial.print("Channel: "); Serial.println(channel);

      float freqMHz = calculateFrequencyMHz(channel);
      Serial.print("Frequency: "); Serial.print(freqMHz); Serial.println(" MHz");

      float distance = calculateDistance(rssi, freqMHz);
      Serial.print("Estimated Distance: ");
      Serial.print(distance, 2); // 2 decimal places
      Serial.println(" meters");

      break;
    }
  }
}

void loop() {
  // Nothing in loop
}
