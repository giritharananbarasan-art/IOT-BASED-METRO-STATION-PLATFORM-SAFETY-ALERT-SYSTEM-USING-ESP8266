#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define PIR_PIN D5
#define LED_PIN D6
#define BUZZER_PIN D7

LiquidCrystal_I2C lcd(0x27, 16, 2);

bool trainPresent = false;

void setup() {
  Serial.begin(9600);

  pinMode(PIR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  Wire.begin(D2, D1);

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Metro Safety");
  delay(2000);
  lcd.clear();
}

void loop() {

  // 📡 RECEIVE TRAIN DATA FROM PYTHON
  if (Serial.available()) {
    char data = Serial.read();

    if (data == '1') trainPresent = true;
    if (data == '0') trainPresent = false;
  }

  int motion = digitalRead(PIR_PIN);

  lcd.setCursor(0, 0);

  if (trainPresent) {
    digitalWrite(LED_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);

    lcd.print("Train Arrived   ");
    lcd.setCursor(0, 1);
    lcd.print("PIR Disabled    ");
  }
  else {
    lcd.print("No Train        ");

    if (motion == HIGH) {
      digitalWrite(LED_PIN, HIGH);
      digitalWrite(BUZZER_PIN, HIGH);

      lcd.setCursor(0, 1);
      lcd.print("Don't Cross Line");

      Serial.println("PIR_ON");   // 🔗 send to Python
    } 
    else {
      digitalWrite(LED_PIN, LOW);
      digitalWrite(BUZZER_PIN, LOW);

      lcd.setCursor(0, 1);
      lcd.print("Safe Zone       ");

      Serial.println("PIR_OFF");
    }
  }

  delay(200);
}