#include <ESP8266WiFi.h>
//#include <OneWire.h>
//#include <DallasTemperature.h>
IPAddress ip(192,168,1,1);        // ip
IPAddress shluz(192,168,1,1);     // шлюз
IPAddress podset(255,255,255,0);  // задаем для нашего телефона подсеть   
WiFiServer server(80);
const char* ssid = "ESP";
const char* password = "12345678";
int a=0;
bool led1IsOn = false;
bool led2IsOn = false;
bool led3IsOn = false;
const int LED_0 = 16;
const int LED_1 = 5;
const int LED_2 = 2;
const int oneWireBus = 4; 

float temperatureC = 0.0f;
// Create an instance of the server
// specify the port to listen on as an argument

//OneWire oneWire(oneWireBus);
//DallasTemperature sensors(&oneWire);
 String k;
unsigned long counts; //variable for GM Tube events
unsigned long previousMillis; //variable for time measurement
IRAM_ATTR void impulse() { // dipanggil setiap ada sinyal FALLING di pin 2
    counts++;
}
#define LOG_PERIOD 600 
void setup() {
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(ip, shluz, podset);  // ip и shluz должны быть одинаковы 192,168,1,1
  WiFi.softAP(ssid, password);   
 
  Serial.begin(115200);
 
  delay(10);
  //sensors.begin();
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  
  
  while ((WiFi.softAPgetStationNum() == 0) && (a < 58)) {
    delay(500);
    Serial.print(".");
  }
   if(WiFi.softAPgetStationNum() != 0) // проверка после 50 попыток (подкл или нет) 
    { 
        Serial.print("\n\r");                            // переход на строку ниже
        Serial.println("WiFi - ПОДКЛЮЧИЛСЯ к ТЕЛЕФОНУ");
  
  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
}
 counts = 0;
    Serial.begin(115200);
    pinMode(4, INPUT);   
    attachInterrupt(digitalPinToInterrupt(4), impulse, FALLING); //define external interrupts 
    Serial.println("Start counter");
}

void loop() {
  // Check if a client has connected
 // sensors.requestTemperatures(); 
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  unsigned long currentMillis = millis();
    if (currentMillis - previousMillis > LOG_PERIOD) {
        previousMillis = currentMillis;
        Serial.println(counts);
        k=counts;
        counts = 0;
    }
  
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
   // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  // Match the request
 // controller(req, client);
  client.flush(); 
  // Prepare the response
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
  s += k;
  // Send the response to the client
  client.print(s);
  delay(1);
  Serial.println("Client disonnected");

  // The client will actually be disconnected 
  // when the function returns and 'client' object is detroyed
}
void controller(String req,  WiFiClient client){
   if (req.indexOf("/temperature") != -1){
    // temperatureC = sensors.getTempCByIndex(0);
     Serial.println("Showing temperature");
    } else if(req.indexOf("/led1") != -1){
     setLedState(LED_0, led1IsOn);
     led1IsOn = !led1IsOn;
    } else if(req.indexOf("/led2") != -1){
      setLedState(LED_1, led2IsOn);
      led2IsOn = !led2IsOn;
    } else if(req.indexOf("/led3") != -1){
      setLedState(LED_2, led3IsOn);
      led3IsOn = !led3IsOn;
    } else {
    Serial.println("invalid request");
    client.stop();
    return;
  }
  }
void setLedState(int led, bool state){
  if(!state){
    digitalWrite(led, HIGH);
  } else {
    digitalWrite(led, LOW);}
  }
