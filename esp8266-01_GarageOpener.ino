#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "YourRouterSSID";
const char* password = "YourRouterPassword";

ESP8266WebServer server(80);

int switchPin = 0;
int switchStateCur;
int relayPin = 2;

int WiFiCon() {
    // Check if we have a WiFi connection, if we don't, connect.
  int xCnt = 0;

  if (WiFi.status() != WL_CONNECTED){

        Serial.println();
        Serial.println();
        Serial.print("Connecting to ");
        Serial.println(ssid);

        WiFi.mode(WIFI_STA);
        
        WiFi.begin(ssid, password);
        
        while (WiFi.status() != WL_CONNECTED  && xCnt < 50) {
          delay(500);
          Serial.print(".");
          xCnt ++;
        }

        if (WiFi.status() != WL_CONNECTED){
          Serial.println("WiFiCon=0");
          return 0; //never connected
        } else {
          Serial.println("WiFiCon=1");
          Serial.println("");
          Serial.println("WiFi connected");  
          Serial.println("IP address: ");
          Serial.println(WiFi.localIP());
          return 1; //1 is initial connection
        }

  } else {
    Serial.println("WiFiCon=2");
    return 2; //2 is already connected
  
  }
}

String htmlServe(int doorAction) {
  String htmStr;

      if (switchStateCur==1 && doorAction ==1){
        digitalWrite(relayPin, 0);
        delay(400);
        digitalWrite(relayPin, 1);
      } else if (switchStateCur==0 && doorAction ==0){
        digitalWrite(relayPin, 0);
        delay(400);
        digitalWrite(relayPin, 1);
      }

  htmStr += "<html>\n<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, maximum-scale=1.0,\">\n";
  htmStr += "<meta http-equiv=\"refresh\" content=\"5; URL=/sdoor\"> \n";
  htmStr += "<body>\n<center>\n<h1>\n";

      if (switchStateCur==1){
        htmStr += "Your Door is Open";
      } else {
        htmStr += "The Door is Closed";
      }
  
  htmStr +="</h1>\n<br><a href=\"/";
  
      if (switchStateCur==0){
        htmStr += "odoor";
      } else {
        htmStr += "cdoor";
      }
      
  htmStr += "\">\n<button>\n";
  
      if (switchStateCur==0){
        htmStr += "Open Door";
      } else {
        htmStr += "Close Door";
      }
  htmStr += "</button></a>\n";

  htmStr += "<br><br><br><a href=\"/sdoor\"><button>Recheck</button></a>";
  
  htmStr += "</center>\n</body>\n</html>\n";

  return htmStr;
  
}

void setup(){

  pinMode(switchPin, INPUT);
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH);
  Serial.begin(115200);

  WiFiCon();

   server.on("/odoor", [](){
      server.send(200, "text/html", htmlServe(0));
   });

   server.on("/cdoor", [](){
      server.send(200, "text/html", htmlServe(1));
   });

   server.on("/sdoor", [](){
      server.send(200, "text/html", htmlServe(2));
   });

  server.begin();

}

void loop(){

  switchStateCur = digitalRead(switchPin);
  
  server.handleClient();

  delay(1000);
}
