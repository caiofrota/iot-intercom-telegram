#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

/************************************************/
char *WIFI_SSID = "";
char *WIFI_PASSWORD = "";
const char *BOT_TOKEN = "";
const char *CHAT_ID = "";
const char *MESSAGE = "Ding-dong!";
/************************************************/

#define PIN_INTERCOM A0

const int READ_DELAY = 100;           // Gap between voltage readings.
const int NOTIFICATION_DELAY = 5000;  // Gap between API request callings.

char webhookURL[200];
unsigned long lastRead = 0;
unsigned long lastNotificationSent = 0;
int voltage;

WiFiClientSecure wifiClient;

void setup() {
  Serial.begin(115200);

  strcpy(webhookURL, "/bot");
  strcat(webhookURL, BOT_TOKEN);
  strcat(webhookURL, "/sendMessage?chat_id=");
  strcat(webhookURL, CHAT_ID);
  strcat(webhookURL, "&text=");
  strcat(webhookURL, MESSAGE);
  
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (millis() < lastRead || millis() - lastRead > READ_DELAY) {
    lastRead = millis();
    voltage = analogRead(PIN_INTERCOM) / 1024.0 * 12;
    if (voltage > 6) {
      if (millis() < lastNotificationSent || millis() - lastNotificationSent > NOTIFICATION_DELAY) {
        lastNotificationSent = millis();
        Serial.println("Voltage change identified. Sending notification.");

        wifiClient.setInsecure();
        if (!wifiClient.connect("api.telegram.org", 443)) {
          Serial.println("Connection failed.");
          return;
        }

        wifiClient.print(String("GET ") + webhookURL + " HTTP/1.1\r\n" + "Host: api.telegram.org\r\n" + "User-Agent: ESP8266\r\n" + "Connection: close\r\n\r\n");
        while (wifiClient.available()) {
          String line = wifiClient.readStringUntil('\n');
          if (line == "\r") {
            Serial.println("Headers received");
            break;
          }
        }
        
        String line = wifiClient.readStringUntil('\n');
        if (line.startsWith("{\"state\":\"success\"")) {
          Serial.println("A notification has been sent.");
        } else {
          Serial.println("Sending notification has failed.");
        }
        
        Serial.println("[HTTPS] Received payload telegram: " + line);
      }
    }
  }
}
