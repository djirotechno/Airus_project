
//----------------------------------------------------------------------------#
//                                     AIRUS                                  #
//                         Hackathon Qualitee de l'aire                       #
//----------------------------------------------------------------------------#


#include <WiFi.h>
#include <WiFiManager.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define TOUCH_PIN 1
#define WAKE_PIN 1
#define BUZZER_PIN 6

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// API AIRGRADIENT
const char* apiUrl = "https://api.airgradient.com/public/api/v1/locations/168382/measures/current?token=66e22601-c57f-430b-8858-420dc7016083"; // Remplacez par l'URL de votre choix


// DATA
int pm25 = 0;
int co2 = 0;
float temp = 0;
float hum = 0;

// MODE ECRAN
int screenMode = 0;
bool lastTouch = false;
unsigned long lastPress = 0;

unsigned long lastActivity = 0;
const unsigned long timeout = 60000; // 30 secondes
//WIFI CONFIGURATION AVEC WM
void setupWiFi() {
  WiFiManager wm;

  // wm.resetSettings(); // debug reset

  bool res = wm.autoConnect("Airus-Setup");

  if(!res) {
    ESP.restart();
  }
}

// API DATA
void fetchData() {

  if(WiFi.status() == WL_CONNECTED) {

    HTTPClient http;
    http.begin(apiUrl);

    int code = http.GET();

    if(code == 200) {

      String payload = http.getString();

      StaticJsonDocument<1024> doc;
      deserializeJson(doc, payload);

      pm25 = doc["pm02"];
      co2  = doc["rco2"];
      temp = doc["atmp"];
      hum  = doc["rhum"];
    }

    http.end();
  }
}

void checkBuzzer() {

  if(pm25 > 35) {
    tone(BUZZER_PIN, 2000, 200);
  }
}

//BOUTTON TOUCH
void readTouch() {

  bool touch = digitalRead(TOUCH_PIN);

  //  activité dès qu’on touche
  if(touch == HIGH) {
    lastActivity = millis();
  }

  // détection appui
  if(touch == HIGH && lastTouch == LOW) {

    if(millis() - lastPress > 300) {

      screenMode = (screenMode + 1) % 3;

      lastPress = millis();
    }
  }

  lastTouch = touch;  //  obligatoire
}

//SLEEP MODE
void goToSleep() {

  // message court
  display.clearDisplay();
  display.setCursor(20,20);
  display.println("...");
  display.display();

  delay(800);

  //  couper écran
  display.ssd1306_command(SSD1306_DISPLAYOFF);

  delay(100);
  esp_sleep_enable_ext1_wakeup(
    (1ULL << WAKE_PIN),
    ESP_EXT1_WAKEUP_ANY_HIGH
  );

  delay(100);

  esp_deep_sleep_start();

}


String getLabel(int pm25) {
  if(pm25 <= 15) return "BON";
  else if(pm25 <= 35) return "MOYEN";
  else return "MAUVAIS";
}

String getEmoji(int pm25) {
  if(pm25 <= 15) return ":-)";
  else if(pm25 <= 35) return ":-|";
  else return ":-(";
}

void screenKids() {

  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10,0);
  display.println("QUALITE DE L'AIR");

  display.setTextSize(2);
  display.setCursor(40,18);
  display.print(getEmoji(pm25));

  display.setCursor(30,40);
  display.println(getLabel(pm25));

  display.setTextSize(1);
  display.setCursor(30,56);
  display.print("PM2.5:");
  display.print(pm25);

  display.display();
}

//AFFICHAGE SUR ECRAN
void screenData() {

  display.clearDisplay();
  display.setTextSize(1);

  display.setCursor(0,0);
  display.println("AIR MONITOR");

  display.setCursor(0,16);
  display.print("PM2.5: "); display.println(pm25);

  display.setCursor(0,26);
  display.print("CO2  : "); display.println(co2);

  display.setCursor(0,36);
  display.print("TEMP : "); display.println(temp);

  display.setCursor(0,46);
  display.print("HUM  : "); display.println(hum);

  display.display();
}

void screenEco() {

  display.clearDisplay();
  display.setTextSize(1);

  display.setCursor(10,10);

  if(pm25 <= 15) {
    display.println("Air propre !");
  }
  else if(pm25 <= 35) {
    display.println("Air moyen");
    display.println("Attention pollution");
  }
  else {
    display.println("Air pollue !");
    display.println("Protege-toi !");
  }

  display.display();
}

void updateScreen() {

  if(screenMode == 0) screenKids();
  else if(screenMode == 1) screenData();
  else if(screenMode == 2) screenEco();
}

void bootAnimation() {

  // ===== FRAME 1 =====
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(10, 10);
  display.println("AIRus");
  display.setCursor(30, 30);
  display.println("Starting...");
  display.display();
  delay(1200);

  // ===== FRAME 2 =====
  display.clearDisplay();
  display.setCursor(20, 10);
  display.println("WiFi setup");
  display.setCursor(10, 30);
  display.println("Connecting...");
  //drawProgressBar(20);
  display.display();
  delay(1200);

  // ===== FRAME 3 =====
  display.clearDisplay();
  display.setCursor(20, 10);
  display.println("Sensors");
  display.setCursor(30, 30);
  display.println("Initializing");
 // drawProgressBar(60);
  display.display();
  delay(1200);

  // ===== FRAME 4 =====
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(10, 20);
  display.println("READY");

  display.setTextSize(1);
  display.setCursor(30, 50);
  display.println("SYSTEM OK");

  display.display();
  delay(1000);
}
void setup() {

  Serial.begin(115200);

  pinMode(TOUCH_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
 lastActivity = millis();
  // OLED
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.display();
  delay(2000); // Pause 2 seconds

  // Clear the buffer
  display.clearDisplay();

  // WiFi
  setupWiFi();
}

void loop() {

  readTouch();// bouton tactile
  fetchData();        // API AirGradient
  updateScreen();     // affichage OLED
   if(millis() - lastActivity > timeout) {
    goToSleep();
  }
  checkBuzzer();      // alerte sonore

  delay(2000);
}