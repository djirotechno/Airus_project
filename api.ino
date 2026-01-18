
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h> // Indispensable pour l'analyse JSON
#include <U8x8lib.h>
#include <Wire.h>
 U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);
// Vos identifiants Wi-Fi
const char* ssid = "ECA7";
const char* password = "59RB3YYBF3T";

// L'URL de votre API. Remplacez par une URL qui renvoie du JSON avec temp/hum.
// Si vous testez avec une API réelle (ex: OpenWeatherMap, capteur IoT), adaptez l'URL et les clés.

const char* url = "https://api.airgradient.com/public/api/v1/locations/151804/measures/current?token=77a25676-a9ec-4a99-9137-f33e6776b590"; // Remplacez par l'URL de votre choix

// Si votre API nécessite une clé, décommentez et adaptez selon la méthode (URL ou en-tête)
// const char* apiKey = "VOTRE_CLE_API_ICI";
// const char* url_with_key = "http://api.exemple.com/sensor_data?api_key="; // Exemple clé en URL

void setup() {

  u8x8.begin();
  u8x8.setFlipMode(1); //rotary 180
  u8x8.setFont(u8x8_font_inb21_2x4_r);
  u8x8.setCursor(0, 2);
  u8x8.setCursor(20, 2);
  u8x8.print("AiRUS");
  delay(2000);
  Serial.begin(115200);

  Serial.print("Connexion au WiFi ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connecté !");
  Serial.print("Adresse IP : ");
  Serial.println(WiFi.localIP());

  HTTPClient http;

  Serial.print("[HTTP] Début de la requête GET vers : ");
  Serial.println(url); // Ou url_with_key si vous utilisez une clé API dans l'URL

  http.begin(url); // Ou url_with_key

  // Si votre API utilise une clé en-tête (décommentez et adaptez)
  // http.addHeader("X-API-Key", apiKey);

  int httpResponseCode = http.GET();

  if (httpResponseCode > 0) {
    Serial.print("[HTTP] Code de réponse : ");
    Serial.println(httpResponseCode);

    String payload = http.getString();
    Serial.println("Réponse JSON complète :");
    Serial.println(payload);

    // --- Analyse JSON avec ArduinoJson ---
    // Calculez la taille de capacité nécessaire avec l'assistant ArduinoJson : https://arduinojson.org/v6/assistant/
    // Pour l'exemple JSON donné, une capacité d'environ 256 octets est généralement suffisante.
    const size_t capacity = JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(3) + 200; // Adaptez cette taille!
    StaticJsonDocument<capacity> doc;

    DeserializationError error = deserializeJson(doc, payload);

    if (error) {
      Serial.print(F("Erreur de désérialisation JSON : "));
      Serial.println(error.f_str());
      return;
    }

    // Accéder aux données spécifiques
    // Les données de température et d'humidité sont imbriquées dans l'objet "data"
    //JsonArray list = doc["list"].as<JsonArray>();
    //JsonObject firstItem = list[0];
    //JsonObject components = firstItem["components"];
    float temperature = doc["atmp_corrected"];
    float pm2_5 = doc["pm10_corrected"];
    Serial.println("\n--- Données extraites ---");
    Serial.print("Temperature: ");
    Serial.println(temperature);
    Serial.print("PM10: ");
    Serial.println(pm2_5);
    u8x8.setFont(u8x8_font_chroma48medium8_r);
    u8x8.setCursor(0, 0);
    u8x8.print("Temp : ");
    u8x8.setCursor(20, 0);
    u8x8.print(temperature);  
    u8x8.setCursor(0, 2);
    u8x8.print("PM10 : ");  
    //u8x8.setCursor(20, 0);
    u8x8.print(pm2_5);  
  } else {
    Serial.print("[HTTP] Erreur lors de la requête : ");
    Serial.println(http.errorToString(httpResponseCode));
    u8x8.setFont(u8x8_font_chroma48medium8_r);
    u8x8.setCursor(0, 2);
    u8x8.setCursor(20, 2);
    u8x8.print("Pas de connexion");
    delay(2000);
  }

  http.end();
}

void loop() {
  // Rien dans la boucle pour cet exemple, l'opération se fait une fois au démarrage.
  delay(5000);
}