#include  <Wire.h>
#include  <LiquidCrystal_I2C.h>
#include "DHT.h"
#define DHTTYPE DHT11   // DHT 11

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

const int DHTPin = 5;
DHT dht(DHTPin, DHTTYPE);

int selector = A0;
int output = 2;

float level;
float h;

byte ctrl = 0;
long Time = 0;
long sensorTime;

const int fiveSeconds = 5000;
const word oneMinute = 60000;


void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(output, OUTPUT);
  lcd.home ();
  lcd.setCursor ( 3, 0 );
  lcd.print("Humidifier");
  delay(3000);
  lcd.clear();
  dht.begin();
}

void loop() {
  level = analogRead(selector);
  level = map(level, 0, 1023, 0, 1000) / 10;

  if (sensorTime < millis()) {
    h = dht.readHumidity();

    if (isnan(h)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }
    sensorTime = millis() + 2000;
  }

  lcd.setCursor (0, 0);
  lcd.print("Current: ");
  lcd.print(h);
  lcd.print("%");
  lcd.setCursor (0, 1);
  lcd.print("Set: ");
  lcd.print(level);
  lcd.print("%");
  delay(300);
  lcd.clear();

  if (h < level) {
    if (ctrl == 0) {
      digitalWrite(output, HIGH);
      ctrl = 1;
      Time = millis() + fiveSeconds;
    }
    if (Time <= millis()) {
      toggleOutpu();
      Time = millis() + oneMinute;
    }
  }

  if (h > level) {
    ctrl = 0;
    digitalWrite(output, LOW);
  }
}

void toggleOutpu() {
  digitalWrite(output, !digitalRead(output));
}

