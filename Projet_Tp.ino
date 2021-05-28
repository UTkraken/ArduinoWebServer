#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#define FASTLED_ESP8266_RAW_PIN_ORDER
#include <FastLED.h>

#define NUM_LEDS 8
#define DATA_PIN D8

CRGB leds[NUM_LEDS];
ESP8266WebServer server(80);
int red, green, blue ;
CRGB grb;

const char *ssid = "GEWIFITP1";
const char *password = "geiigeii";

void handleIndex() {
  int test;
  for (int i =0; i < server.args(); i++) {
    if ( server.argName(i) == "red" ) {
      test = server.arg(i).toInt();
      if (  test >=  0 && test <= 255 ) {
        red = test;  
      }
    }
    if ( server.argName(i) == "green" ) {
      test = server.arg(i).toInt();
      if (  test >=  0 && test <= 255 ) {
        green = test;  
      }
    }
    if ( server.argName(i) == "blue" ) {
      test = server.arg(i).toInt();
      if (  test >=  0 && test <= 255 ) {
        blue = test;  
      }
    }
  }

  for(int i=0; i<NUM_LEDS ; i++) {
    leds[i].setRGB(red,green,blue);
    FastLED.show();
  }
  String res = "All good";
  server.send(200, "text/plain", res); 
}

void setup() {
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS); 
  grb.red = 255;
  grb.green = 125;
  grb.blue = 25;
  for(int i=0; i<NUM_LEDS ; i++) {
    leds[i] = grb ;
    FastLED.show();
  }
  SPIFFS.begin(); // Start the SPI Flash Files System
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("webExample")) {
    Serial.println("MDNS responder started");
  }

    server.on("/led", handleIndex);
  
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
  MDNS.update();
}

void handleNotFound() {
  bool found = handleFileRead(server.uri()) ;
  if ( found ) {
      return ;
  }
}

String getContentType(String filename) { // convert the file extension to the MIME type
  if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  else if ( filename.endsWith(".jpg")  or filename.endsWith(".JPG") \
         or filename.endsWith(".JPEG") or filename.endsWith(".jpeg") ) return "image/jpeg";
  return "text/plain";
}

bool handleFileRead(String path) { 
  Serial.println("handleFileRead: " + path);
  if (path.endsWith("/")) path += "index.html";        
  String contentType = getContentType(path);           
  if (SPIFFS.exists(path)) {                            
    File file = SPIFFS.open(path, "r");                
    size_t sent = server.streamFile(file, contentType); 
    file.close();                                       
    return true;
  }
  Serial.println("\tFile Not Found");
  return false;            
}
