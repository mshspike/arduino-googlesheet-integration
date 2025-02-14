#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFiS3.h>

// WiFi credentials
const char* ssid = "SSID_HERE";     // Replace with your WiFi name
const char* password = "WIFI_PW_HERE";  // Replace with your WiFi password

// Initialize LCD object
LiquidCrystal_I2C lcd(0x27, 16, 2);

const char* host = "api.pushingbox.com";
const int port = 80;  // PushingBox uses regular HTTP port 80
const char* deviceID = "PUSHBOX_DEVICE_ID_HERE";  // Your PushingBox device ID

void setup() {
  Serial.begin(9600);
  
  // Initialize LCD
  lcd.init();
  lcd.backlight();
  
  // Show WiFi connecting message
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Connecting to");
  lcd.setCursor(0, 1);
  lcd.print(ssid);
  
  // Initialize random seed using analog noise
  randomSeed(analogRead(0));
  
  // Connect to WiFi with status display
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  
  // Show connected message
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WiFi Connected!");
  lcd.setCursor(0, 1);
  lcd.print("IP: ");
  lcd.print(WiFi.localIP());
  delay(2000);  // Show the IP for 2 seconds
}

bool makeHttpRequest() {  
  WiFiClient client;  // Changed to regular WiFiClient
  
  Serial.println("\nStarting connection to server...");
  if (!client.connect(host, port)) {
    Serial.println("Connection failed!");
    return false;
  }
  Serial.println("OK");

  String url = String("/pushingbox?devid=") + deviceID + "&data=122,335";
  String request = String("GET ") + url + " HTTP/1.1\r\n" +
                  "Host: " + host + "\r\n" +
                  "Connection: close\r\n\r\n";
  
  // Print the request to Serial monitor
  Serial.println("\nSending HTTP request:");
  Serial.println("==================");
  Serial.println(request);
  
  // Send the request
  client.print(request);

  unsigned long timeout = millis();
  while (!client.available()) {
    if (millis() - timeout > 5000) {
      Serial.println("Timeout");
      client.stop();
      return false;
    }
  }

  Serial.println("Response:");
  while (client.available()) {
    String line = client.readStringUntil('\n');
    Serial.println(line);
  }

  client.stop();
  Serial.println("Connection closed");
  return true;
}

void loop() {
  lcd.clear();
  Serial.println("\n=== Making new request ===");
  
  if (makeHttpRequest()) {
    // Request successful
    lcd.setCursor(0, 0);
    lcd.print("Request Success!");
    lcd.setCursor(0, 1);
    lcd.print("Data Sent: OK");
    Serial.println("Request completed successfully");
  } else {
    // Request failed
    lcd.setCursor(0, 0);
    lcd.print("Request Failed!");
    lcd.setCursor(0, 1);
    lcd.print("Check Connection");
    Serial.println("Request failed");
  }
  
  Serial.println("Waiting 60 seconds before next request...");
  delay(60000);
} 