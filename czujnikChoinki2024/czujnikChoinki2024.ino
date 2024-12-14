#include <Wire.h>
#include <VL53L0X.h>
#include <ESP8266WiFi.h>

// Definicje pinów
#define BUZZER_PIN D3

// Inicjalizacja czujnika VL53L0X
VL53L0X sensor;

// Sieć Wi-Fi
const char* ssid = "Orange_Internet_B340";     // Nazwa Wi-Fi
const char* password = "DtrGc2Nt6ndC7dN739";     // Hasło Wi-Fi
WiFiServer server(80);                    // Serwer HTTP na porcie 80

int currentDistance = 0;  // Zmienna do przechowywania zmierzonej odległości
int level = 0;            // Zmienna do przechowywania bieżącego poziomu wody (od 1 do 12)
int lastLevel = -1;       // Zmienna do przechowywania ostatniego poziomu wody

// Poziomy wody
int minLevel = 1;  // Stan 1 - 300 mm (0% wody)
int maxLevel = 12; // Stan 12 - 50 mm (100% wody)

// Zakres odległości (w mm)
int minDistance = 300;  // 300 mm - poziom 1 (0% wody)
int maxDistance = 50;   // 50 mm - poziom 12 (100% wody)

// Zmienne czasowe
unsigned long lastSensorReadTime = 0;  // Ostatni czas odczytu z czujnika
unsigned long lastHttpHandleTime = 0; // Ostatni czas obsługi HTTP
const unsigned long sensorInterval = 1; // Interwał odczytu czujnika (w ms)
const unsigned long httpInterval = 10000;   // Interwał obsługi HTTP (w ms)

void setup() {
  // Inicjalizacja komunikacji I2C
  Wire.begin();
  Serial.begin(9600);

  // Inicjalizacja czujnika VL53L0X
  if (!sensor.init()) {
    Serial.println("Błąd inicjalizacji czujnika!");
    while (1);
  }
  sensor.startContinuous();
  
  // Ustawienie pinu buzzera
  pinMode(BUZZER_PIN, OUTPUT);

  // Połączenie z Wi-Fi
  Serial.println("Łączenie z Wi-Fi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nPołączono z Wi-Fi!");
  Serial.print("Adres IP: ");
  Serial.println(WiFi.localIP());

  // Uruchomienie serwera HTTP
  server.begin();
  Serial.println("Serwer HTTP uruchomiony.");
}

void loop() {
  unsigned long currentTime = millis();

  // Odczyt czujnika co 500 ms
  if (currentTime - lastSensorReadTime >= sensorInterval) {
    lastSensorReadTime = currentTime;
    readSensor();
  }

  // Obsługa HTTP co 100 ms
  if (currentTime - lastHttpHandleTime >= httpInterval) {
    lastHttpHandleTime = currentTime;
    handleHttp();
  }

  // Funkcja yield() pozwala ESP8266 obsłużyć wewnętrzne operacje sieciowe
  yield();
}

void readSensor() {
   // Odczyt odległości z czujnika (w mm)
  currentDistance = sensor.readRangeContinuousMillimeters();
  level = map(currentDistance, minDistance, maxDistance, minLevel, maxLevel);
  level = constrain(level, minLevel, maxLevel);  // Ograniczenie poziomu do zakresu 1-12

  // Sprawdzamy, czy zmienił się stan
  if (level != lastLevel) {
    bool increasing = level > lastLevel;  // Czy poziom wody rośnie?
    lastLevel = level;  // Zapisujemy nowy stan
    handleLevelChange(level, increasing);

    // Delay zależny od kierunku zmiany poziomu
    if (increasing) {
      delay(50);  // Krótki delay przy nalewaniu wody
    } else {
      delay(1000);  // Długi delay przy powolnym ubywaniu wody
    }
  }
}

void handleHttp() {
  // Obsługa klienta HTTP
  WiFiClient client = server.available();
  if (client) {
    Serial.println("              Nowy klient połączony.");
    String request = client.readStringUntil('\r');
    client.flush();

    // Obliczanie procentowego poziomu wody
    int waterPercentage = (level - minLevel) * 100 / (maxLevel - minLevel);

    // Tworzenie odpowiedzi HTTP
    String response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
    response += "<!DOCTYPE HTML><html><head>";
    response += "<style>";
    response += "body { font-family: Arial, sans-serif; text-align: center; margin: 0; padding: 0; }";
    response += ".container { width: 100%; max-width: 200px; margin: auto; padding: 20px; }";
    response += ".bar { width: 100px; height: 300px; background-color: #ddd; border-radius: 10px; overflow: hidden; margin: auto; position: relative; }";
    response += ".bar-fill { width: 100%; background-color: #4CAF50; position: absolute; bottom: 0; height: " + String(waterPercentage) + "%; transition: height 0.5s; }";
    response += "</style>";
    response += "</head><body>";
    response += "<div class='container'>";
    response += "<h1>Poziom wody: " + String(waterPercentage) + "%</h1>";
    response += "<p>Odleg&#322;o&#347;&#263; od czujnika: " + String(currentDistance) + " mm</p>";
    response += "<div class='bar'><div class='bar-fill'></div></div>";
    response += "</div>";
    response += "<script>";
    response += "setTimeout(() => { location.reload(); }, 2000);";  // Automatyczne odświeżenie co 2 sekundy
    response += "</script>";
    response += "</body></html>";

    // Wysyłanie odpowiedzi do klienta
    client.print(response);
    client.stop();
    Serial.println("              Klient rozłączony.");
  }
}
void handleLevelChange(int level, bool increasing) {
  switch (level) {
    case 1:
      Serial.println("Stan 1");
      playMerryChristmas();
      break;

    case 2:
      Serial.println("Stan 2");
      playBeep(2);
      break;

    case 3:
      Serial.println("Stan 3");
      playBeep(3);
      break;

    case 4:
      Serial.println("Stan 4");
      playBeep(4);
      break;

    case 5:
      Serial.println("Stan 5");
      playBeep(5);
      break;

    case 6:
      Serial.println("Stan 6");
      playBeep(6);
      break;

    case 7:
      Serial.println("Stan 7");
      playBeep(7);
      break;

    case 8:
      Serial.println("Stan 8");
      playBeep(8);
      break;

    case 9:
      Serial.println("Stan 9");
      playBeep(9);
      break;

    case 10:
      Serial.println("Stan 10");
      playBeep(10);
      break;

    case 11:
      Serial.println("Stan 11");
      playBeep(11);
      break;

    case 12:
      Serial.println("Stan 12");
      playJingleBells();
      break;
  }
}

void playMerryChristmas() {
  tone(BUZZER_PIN, 262, 400);
  delay(400);
  tone(BUZZER_PIN, 294, 400);
  delay(400);
  tone(BUZZER_PIN, 330, 400);
  delay(400);
  tone(BUZZER_PIN, 349, 400);
  delay(400);
  tone(BUZZER_PIN, 392, 400);
  delay(400);
}

void playJingleBells() {
  tone(BUZZER_PIN, 349, 400);
  delay(400);
  tone(BUZZER_PIN, 392, 400);
  delay(400);
  tone(BUZZER_PIN, 440, 400);
  delay(400);
  tone(BUZZER_PIN, 494, 400);
  delay(400);
}

void playBeep(int level) {
  int frequency = map(level, minLevel, maxLevel, 200, 1000);
  tone(BUZZER_PIN, frequency, 200);
  delay(200);
}

