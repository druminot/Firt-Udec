#include <SoftwareSerial.h>  //incluimos SoftwareSerial
#include <TinyGPS.h>         //incluimos TinyGPS
#include <ESP8266WiFi.h>


//se demora la primera vez quizas es la pila , recomiendo usar dcdc
TinyGPS gps;                     //Declaramos el objeto gps
SoftwareSerial serialgps(4,5);  //Declaramos el pin 4 Tx y 3 Rx

//Declaramos la variables para la obtención de datos
int year;
byte month, day, hour, minute, second, hundredths;
unsigned long chars;
unsigned short sentences, failed_checksum;
const char* ssid = "Telemetria";
const char* password = "udecv16v16";

void setup() {
  Serial.begin(115200);   //Iniciamos el puerto serie
  serialgps.begin(9600);  //Iniciamos el puerto serie del gps
 WiFi.setSleepMode(WIFI_NONE_SLEEP);
   WiFi.persistent(true);
WiFi.setAutoReconnect(true);

  WiFi.begin(ssid, password);

   while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }

  //Imprimimos:
  Serial.println("");
  Serial.println("GPS GY-GPS6MV2 Leantec");
  Serial.println(" ---Buscando senal--- ");
  Serial.println("");
}

void loop() {

  //accel.getEvent(&event);


  while (serialgps.available()) {





    int c = serialgps.read();

    if (gps.encode(c)) {
      float latitude, longitude;
      gps.f_get_position(&latitude, &longitude);
      Serial.print("Latitud/Longitud: ");
      Serial.print(latitude, 5);
      Serial.print(", ");
      Serial.println(longitude, 5);




      gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths);
      Serial.print("Fecha: ");
      Serial.print(day, DEC);
      Serial.print("/");
      Serial.print(month, DEC);
      Serial.print("/");
      Serial.print(year);
      Serial.print(" Hora: ");
      Serial.print(hour, DEC);
      Serial.print(":");
      Serial.print(minute, DEC);
      Serial.print(":");
      Serial.print(second, DEC);
      Serial.print(".");
      Serial.println(hundredths, DEC);
      Serial.print("Altitud (metros): ");
      Serial.println(gps.f_altitude());
      Serial.print("Rumbo (grados): ");
      Serial.println(gps.f_course());
      Serial.print("Velocidad(kmph): ");
      Serial.println(gps.f_speed_kmph());
      Serial.print("Satelites: ");
      Serial.println(gps.satellites());
      Serial.println();


  String mensaje ="Altitud(m):" +String(gps.f_altitude()) + "Rumbo(grados):" + String(gps.f_course()) + "Velocidad(kmph):" + String(gps.f_speed_kmph())+"Satelites"+String(gps.satellites())+"Fecha:"+day+"-"+month+"-"+year+"Hora:"+hour+":"+minute+":"+second+".";
  
  // Conexión al servidor web del ESP8266
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect("192.168.4.1", httpPort)) {
    Serial.println("Error de conexión al servidor web del ESP8266");
    return;
  }

   client.print("GET /enviar?message=");
  client.print(mensaje);
    client.print(" HTTP/1.1\r\n");
  client.print("Host: 192.168.4.1\r\n");
  client.print("Connection: close\r\n\r\n");



    }
    
  }
}