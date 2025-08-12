#include <ESP8266WiFi.h>

// ssid esp8266
const char* ssid = "Saklar_Lampu"; // Nama WiFi yang dibuat ESP
const char* password = "12345678"; // Password WiFi minimal 8 karakter

const int relayPin = D1; // Pin relay

// port server
WiFiServer server(80);

void setup() {
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);

  Serial.begin(115200);

  // Buat Access Point
  WiFi.softAP(ssid, password);

  Serial.println("Access Point dibuat!");
  Serial.print("Nama WiFi: ");
  Serial.println(ssid);
  Serial.print("Password: ");
  Serial.println(password);

  Serial.println("Buka browser dan akses: http://192.168.4.1");

  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (!client) return;

  String request = client.readStringUntil('\r');
  client.flush();

  if (request.indexOf("/ON") != -1) {
    digitalWrite(relayPin, HIGH);
  } else if (request.indexOf("/OFF") != -1) {
    digitalWrite(relayPin, LOW);
  }

  // HTML tampilan
  String html = "<!DOCTYPE html><html><head><meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<title>Saklar Lampu</title></head><body style='text-align:center;'>";
  html += "<h1>Saklar Lampu</h1>";
  html += "<p><a href='/ON'><button style='width:100px;height:50px;font-size:20px;'>ON</button></a></p>";
  html += "<p><a href='/OFF'><button style='width:100px;height:50px;font-size:20px;'>OFF</button></a></p>";
  html += "</body></html>";

  client.print("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
  client.print(html);
  delay(1);
}