#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>

#define WIFI_SSID "Galaxy A02c70c"
#define WIFI_PASSWORD "ioch5925"
#define API_KEY "AIzaSyBYxWXNJTcngAMckVMa5hrUlV_RfEXlIY0"
#define DATABASE_URL "control-iot-2ac54-default-rtdb.firebaseio.com"
#define USER_EMAIL "jordicortes459@gmail.com"
#define USER_PASSWORD "123456"

#define PIEZO 2

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long lastMillis = 0;
int lastFrequency = -1;

void setup()
{
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  config.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.database_url = DATABASE_URL;

  Firebase.reconnectNetwork(true);
  fbdo.setBSSLBufferSize(4096, 1024);

  Firebase.begin(&config, &auth);

  Firebase.setDoubleDigits(5);

  pinMode(PIEZO, OUTPUT);
}

void loop()
{
  if (Firebase.ready())
  {
    int frequency = Firebase.getInt(fbdo, "/Coleccion/valor");

    FirebaseJson json;
    Firebase.getJSON(fbdo, "/Coleccion");
    json = fbdo.jsonObject();

    FirebaseJsonData jsonData;

    if (json.get(jsonData, "boton"))
    {
      String boton = jsonData.stringValue;

      if (fbdo.httpCode() != FIREBASE_ERROR_HTTP_CODE_OK)
      {
        Serial.print("Firebase request failed, HTTP code: ");
        Serial.println(fbdo.httpCode());
        Serial.print("Error reason: ");
        Serial.println(fbdo.errorReason());
      }

      Serial.print("Frecuencia Firebase: ");
      Serial.println(frequency);
      Serial.print("Botón Presionado: ");
      Serial.println(boton);

      if (frequency != lastFrequency)
      {
        Serial.print("Frecuencia Registrada");

        if (boton == "A")
        {
          Serial.println(" A");
          tone(PIEZO, 303, 100);
          delay(200);
          noTone(PIEZO);
        }
        else if (boton == "B")
        {
          Serial.println(" B");
          tone(PIEZO, 220, 100);
          delay(200);
          noTone(PIEZO);
        }
        else if (boton == "C")
        {
          Serial.println(" C");
          tone(PIEZO, 330, 100);
          delay(200);
          noTone(PIEZO);
        }
        else if (boton == "D")
        {
          Serial.println(" D");
          tone(PIEZO, 370, 100);
          delay(200);
          noTone(PIEZO);
        }

        Firebase.set(fbdo, "/Coleccion", "null");

        lastFrequency = frequency;
      }
    }
  }

  delay(1000);
}
