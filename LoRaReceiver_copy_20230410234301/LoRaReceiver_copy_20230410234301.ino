#include <SPI.h>
#include <LoRa.h>
#include <WiFi.h>

// Replace with your network credentials
const char* ssid = "Telemetria";
const char* password = "udecv16v16";

WiFiServer server(80);

//define the pins used by the LoRa transceiver module
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DIO0 26

//433E6 for Asia
//866E6 for Europe
//915E6 for North America
#define BAND 866E6

void setup() {
  Serial.begin(115200);

  // Create an access point
  Serial.print("Creating access point ");
  Serial.println(ssid);
  WiFi.softAP(ssid, password);

  IPAddress ip = WiFi.softAPIP();
  Serial.print("Access point IP address: ");
  Serial.println(ip);

  // Start the server
  server.begin();
  Serial.println("Server started");

  //initialize LoRa transceiver module
  SPI.begin(SCK, MISO, MOSI, SS);
  LoRa.setPins(SS, RST, DIO0);
  if (!LoRa.begin(BAND)) {
    Serial.println("LoRa Initializing Failed!");
    while (1);
  }
  Serial.println("LoRa Initializing OK!");
}
void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();

  if (client) {
    // Read the first line of the request
    String request = client.readStringUntil('\r');

    // Extract the message from the request
    int messageStartIndex = request.indexOf('?') + 1;
    int messageEndIndex = request.indexOf(' ', messageStartIndex);
    String message = request.substring(messageStartIndex, messageEndIndex);

    // Print the message to the Serial Monitor
    Serial.println(message);

    // Send the message over LoRa
    LoRa.beginPacket();
    LoRa.print(message);
    LoRa.endPacket();
  }
}
