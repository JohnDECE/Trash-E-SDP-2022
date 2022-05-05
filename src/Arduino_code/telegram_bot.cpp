#include "telegram_bot.h"
#include <ArduinoJson.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

// Replace with your network credentials
const char* ssid = "Waow";
const char* password = "12345678";

// Initialize Telegram BOT
// DO NOT EDIT THIS
#define BOTtoken "2044433568:AAETlhD8CWoHTJ9YXDDlrIBRje7rKkplUQs"  // your Bot Token (Get from Botfather)

// Use @myidbot to find out the chat ID of an individual or a group
// Also note that you need to click "start" on a bot before it can
// message you
#define CHAT_ID "2081345906" // JOHN
//#define CHAT_ID "2067260030" // SMIT
//#define CHAT_ID "2056475085" // Jasmine

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

void teleBotSetup() {

  // Attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  bot.sendMessage(CHAT_ID, "Bot started up", "");
}

void alertUser(const String& message) {
  bot.sendMessage(CHAT_ID, message, "");
}
