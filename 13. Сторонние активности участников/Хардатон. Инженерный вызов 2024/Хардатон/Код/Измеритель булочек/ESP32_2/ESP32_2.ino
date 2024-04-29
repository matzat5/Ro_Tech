#include <WiFi.h>
#include <WebServer.h>
#include <WiFiUdp.h>
#include <SoftwareSerial.h>

const char* ssid = "Test";
const char* password = "dkxmss2r";

SoftwareSerial SerialT(13, 12);

WebServer server(80);

float tenza1_data = 0.0;
float tenza2_data = 0.0;
float newData1 = 0.0;

void setup() {
  Serial.begin(9600);
  Serial2.begin(9600);
  SerialT.begin(9600);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, handleRoot);
  server.on("/data", HTTP_GET, handleData);

  server.begin();
}

void loop() {
  while (Serial2.available() > 0) {
    String data = Serial2.readStringUntil('\n');
    int separatorIndex = data.indexOf(',');
    if (separatorIndex != -1) {
      String tenza1_str = data.substring(0, separatorIndex);
      data.remove(0, separatorIndex + 1);
      String tenza2_str = data.substring(0, separatorIndex);
//      int separatorIndex = data.indexOf(',');
//      data.remove(0, separatorIndex + 1);
//      String newData1_str = data;
      tenza1_data = tenza1_str.toFloat();
      tenza2_data = tenza2_str.toFloat();
//      newData1 = newData1_str.toFloat();
    }
  }
  if (SerialT.available() > 0) {
    newData1 = SerialT.parseInt();

  }

  
  server.handleClient();
}

void handleRoot() {
String html = "<!DOCTYPE html><html><head><meta charset='UTF-8'><title>ESP32 Датчики</title>";
html += "<style>body{font-family:Arial, sans-serif;background-color:#36d0ffff;margin:0;padding:0;}.container{max-width:800px;margin:0 auto;padding:20px;background-color:#fff;border-radius:8px;box-shadow:0 0 10px rgba(0, 0, 0, 0.1);}.sensor{font-size:18px;color:#666;}.value{font-size:24px;font-weight:bold;color:#000000;}.sensor p {font-size: 28px;}</style>";
html += "</head><body>";
html += "<div class='container'>";
html += "<h1>Столовая МГТУ УЛК </h1>";
html += "<div class='sensor'>";
html += "<p>Вода: <span id='tenza1' class='value'></span></p>";
html += "<p>Пирожки: <span id='tenza2' class='value'></span></p>";
html += "<p>Количество людей: <span id='newData1' class='value'></span></p>";
html += "</div></div>";
html += "<script>function getData(){var xhttp=new XMLHttpRequest();xhttp.onreadystatechange=function(){if(this.readyState==4&&this.status==200){var data=JSON.parse(this.responseText);document.getElementById('tenza1').innerHTML=data.tenza1;document.getElementById('tenza2').innerHTML=data.tenza2;document.getElementById('newData1').innerHTML=data.newData1;}};xhttp.open('GET','/data',true);xhttp.send();}setInterval(getData,1000);</script>";
html += "</body></html>";

  server.send(200, "text/html", html);
}

void handleData() {
  String data = "{\"tenza1\":" + String(tenza1_data) + ",";
  data += "\"tenza2\":" + String(tenza2_data) + ",";
  data += "\"newData1\":" + String(newData1) + "}";

  server.send(200, "application/json", data);
}
