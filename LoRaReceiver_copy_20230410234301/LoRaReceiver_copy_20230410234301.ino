#include <SPI.h>
#include <LoRa.h>

#include <WiFi.h>

// Replace with your network credentials
const char* ssid = "Telemetria";
const char* password = "udecv16v16";

WiFiServer server(80);

//Libraries for OLED Display
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

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

//OLED pins
#define OLED_SDA 4
#define OLED_SCL 15 
#define OLED_RST 16
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

//packet counter
int counter = 0;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);

void setup() {
  //initialize Serial Monitor
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
  //reset OLED display via software
  pinMode(OLED_RST, OUTPUT);
  digitalWrite(OLED_RST, LOW);
  delay(20);
  digitalWrite(OLED_RST, HIGH);

  //initialize OLED
  Wire.begin(OLED_SDA, OLED_SCL);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3c, false, false)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("LORA SENDER ");
  display.display();
  
  
  //SPI LoRa pins
  SPI.begin(SCK, MISO, MOSI, SS);
  //setup LoRa transceiver module
  LoRa.setPins(SS, RST, DIO0);
  
  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  Serial.println("LoRa Initializing OK!");

  delay(2000);
}

void loop() {
   
   // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Wait for data from client
  while(!client.available()){
    delay(1);
  }

  // Read the first line of the request
  String request = client.readStringUntil('\r');




// Find position of first and second space in request
int firstSpace = request.indexOf(' ');
int secondSpace = request.indexOf(' ', firstSpace + 1);

// Extract substring between first and second space
String path = request.substring(firstSpace + 1, secondSpace);

// Print extracted path (should print "casa")
Serial.println(path);


  // Respond to the request
/*  client.flush();
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); // IMPORTANT: this is required to end the HTTP response header
*
  client.println("<html><body><h1>Hello World!</h1></body></html>");
*/

  

  LoRa.beginPacket();
  LoRa.print("Lectura:");
  LoRa.print(path);
  LoRa.endPacket();
  

  
  delay(100);
}
