#include <WiFiNINA.h>

char ssid[] = "eduroam";             
char username[] = "zyuhan"; 
char pass[] = "ndedu24fall04170014";      

int status = WL_IDLE_STATUS;      

int enA = 3;
int in1 = 1;
int in2 = 2;
int speed;
//  initial motor positions
bool pos_1=HIGH;
bool pos_2=LOW;

WiFiServer server(80);            

void setup() {
  Serial.begin(9600);
  
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  speed=50;
  enable_WiFi();
  connect_WiFi();
  
  server.begin(); 
  printWifiStatus();

  //Initial motor direction
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);  
  //Initialize speed  
  analogWrite(enA,speed);

}

void loop() {
  WiFiClient client = server.available(); // Accept new client connections

  if (client) {
    Serial.println("New client connected");

    String request = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        request += c;
        Serial.write(c);
        
        if (c == '\n') {
          if (request.indexOf("GET /H") >= 0) {
            pos_1=!pos_1;
            pos_2=!pos_2;
    

            digitalWrite(in1, pos_1);
            digitalWrite(in2, pos_2); 
            analogWrite(enA, speed);
          }

          if (request.indexOf("GET /I") >= 0) {
          
            if(speed<=250 && speed!=255)
              speed=speed+5;
            else
              speed=speed;
            if(speed==0)
            {
              digitalWrite(in1, 0);
              digitalWrite(in2, 0); 
              analogWrite(enA, speed); 
            }
            else
            {
              digitalWrite(in1, pos_1);
              digitalWrite(in2, pos_2); 
              analogWrite(enA, speed);
            }
          }   

          if (request.indexOf("GET /J") >= 0) {
            if(speed>=5 && speed!=0)
              speed=speed-5;
            else
              speed=speed;
            delay(100);
            digitalWrite(in1, pos_1);
            digitalWrite(in2, pos_2); 
            analogWrite(enA, speed);
          }  

          if (request.indexOf("GET /K") >= 0) {
            speed=0;
            delay(100);
            digitalWrite(in1, LOW);
            digitalWrite(in2, LOW);             
            analogWrite(enA, speed);
          }                                   

          // Send HTTP response  // ## Some form and font
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println();
          client.println("<!DOCTYPE html>");
          client.println("<html lang='en'>");
          client.println("<head>");
          client.println("<meta name='viewport' content='width=device-width, initial-scale=1.0'>");
          client.println("<style>");
          client.println("body { display: flex; flex-direction: column; align-items: center; justify-content: center; height: 100vh; background-color: #f4f4f4; font-family: Arial, sans-serif; }");
          client.println("h1 { text-align: center; color: #333; }");
          client.println(".button-container { display: flex; flex-direction: column; align-items: center; gap: 15px; }");
          client.println("button { width: 200px; height: 60px; font-size: 18px; border: none; border-radius: 10px; background-color: #007BFF; color: white; cursor: pointer; transition: 0.3s; }");
          client.println("button:hover { background-color: #0056b3; }");
          client.println("</style>");
          client.println("</head>");
          client.println("<body>");
          client.println("<h1>Control Panel</h1>");
          client.println("<div class='button-container'>");
          client.println("<button onclick=\"window.location.href='/H';\">Change Direction</button>");
          client.println("<button onclick=\"window.location.href='/I';\">Increase Speed</button>");
          client.println("<button onclick=\"window.location.href='/J';\">Decrease Speed</button>");
          client.println("<button onclick=\"window.location.href='/K';\">Turn Off</button>");   // ## How to make it like the cloud panel
          client.println("</div>");
          client.println("</body>");
          client.println("</html>");
          client.println();




          client.println();
          
          break; 
        }
      }
    }

    delay(100);  // Allow some time before disconnecting
    client.stop();
    Serial.println("Client disconnected");
  }
}

void enable_WiFi() {
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < "1.0.0") {
    Serial.println("Please upgrade the firmware");
  }
}

void connect_WiFi() {
  while (status != WL_CONNECTED) {
    Serial.print("Connecting to SSID: ");
    Serial.println(ssid);
    status = WiFi.beginEnterprise(ssid, username, pass);
    delay(5000);
  }
}

void printWifiStatus() {
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Signal Strength (RSSI): ");
  Serial.print(WiFi.RSSI());
  Serial.println(" dBm");
}
