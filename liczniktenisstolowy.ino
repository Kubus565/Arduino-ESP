#include <TM1638plus.h>

// Definicja pinów ESP8266 (NodeMCU/Wemos D1 Mini)
#define STB_PIN 5  // D1 (GPIO5)
#define CLK_PIN 4  // D2 (GPIO4)
#define DIO_PIN 0  // D3 (GPIO0)

// Inicjalizacja wyświetlacza TM1638
TM1638plus tm(STB_PIN, CLK_PIN, DIO_PIN);

int scoreP1 = 0;  // Wynik gracza 1
int scoreP2 = 0;  // Wynik gracza 2
int serveCounter = 0;  // Licznik serwisów (0-1 dla zmiany serwującego)

void displayScore() {
    char buffer[9];

    // Formatowanie wyniku:  "06-09 P1-1"
    sprintf(buffer, "%02d-%02d P%d-%d", scoreP1, scoreP2, (serveCounter % 2) + 1, (serveCounter % 2) + 1);
    tm.displayText(buffer);
}

// Funkcja resetowania wyniku
void resetGame() {
    scoreP1 = 0;
    scoreP2 = 0;
    serveCounter = 0;
    tm.setLEDs(0x00);
    displayScore();
}

void setup() {
    tm.displayBegin();
    tm.reset();
    resetGame();
}

void loop() {
    uint8_t buttons = tm.readButtons();

    if (buttons & 0x01) {  // Przycisk 1 - Dodaj punkt dla Gracza 1
        scoreP1++;
        serveCounter++;
        displayScore();
    }
    if (buttons & 0x02) {  // Przycisk 2 - Odejmij punkt dla Gracza 1
        if (scoreP1 > 0) scoreP1--;
        serveCounter--;
        displayScore();
    }
    if (buttons & 0x04) {  // Przycisk 3 - Dodaj punkt dla Gracza 2
        scoreP2++;
        serveCounter++;
        displayScore();
    }
    if (buttons & 0x08) {  // Przycisk 4 - Odejmij punkt dla Gracza 2
        if (scoreP2 > 0) scoreP2--;
        serveCounter--;
        displayScore();
    }
    if (buttons & 0x80) {  // Przycisk 8 - Resetowanie wyniku
        resetGame();
    }

    // Wskazywanie serwującego za pomocą diod LED (gracz 1 = LED 0, gracz 2 = LED 1)
    if ((serveCounter / 2) % 2 == 0) {
        tm.setLED(0, 1);  // LED gracza 1
        tm.setLED(1, 0);
    } else {
        tm.setLED(0, 0);
        tm.setLED(1, 1);  // LED gracza 2
    }

    delay(200);  // Małe opóźnienie, aby uniknąć wielokrotnego odczytu przycisku
}
