#include "GyverMotor.h"
GMotor motorR(DRIVER2WIRE, 4, 5);
GMotor motorL(DRIVER2WIRE, 7, 6);

void setup() {
  Serial.begin(115200);
  delay(200);
  motorR.setMode(FORWARD);
  motorR.setMinDuty(30); // Установите минимальную скважность ШИМ для мотор R
  motorL.setMode(FORWARD);
  motorL.setMinDuty(30); // Установите минимальную скважность ШИМ для мотор L

  // Выводим сообщение о старте программы
  Serial.println("Program started");
}

void loop() {
  static String inputString = "";  // строка для хранения данных из Serial
  static bool stringComplete = false;  // состояние: получены ли все данные

  // Чтение данных из Serial
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar == '\n') {
      stringComplete = true;
    } else {
      inputString += inChar;
    }
  }

  // Если получены все данные, обрабатываем их
  if (stringComplete) {
    Serial.print("Received data: ");
    Serial.println(inputString);

    int commaIndex = inputString.indexOf(',');
    if (commaIndex > 0) {
      String xString = inputString.substring(0, commaIndex);
      String yString = inputString.substring(commaIndex + 1);

      int signalX = xString.toInt();
      int signalY = yString.toInt();

      if (signalX < 13 && signalX > 0 ) {
         signalX = 0;
      }

      if (signalY < 13 && signalY > 0) {
         signalY = 0;
      }

      if (signalX > -13 && signalX < 0 ) {
         signalX = 0;
      }

      if (signalY > -13 && signalY < 0) {
         signalY = 0;
      }

      Serial.print("Parsed X: ");
      Serial.println(signalX);
      Serial.print("Parsed Y: ");
      Serial.println(signalY);

      // Преобразуем по танковой схеме
      int dutyR = signalY + signalX;
      int dutyL = signalY - signalX;

      Serial.print("Calculated dutyR: ");
      Serial.println(dutyR);
      Serial.print("Calculated dutyL: ");
      Serial.println(dutyL);

      motorR.setSpeed(dutyR/2);
      motorL.setSpeed(dutyL/2);
    } else {
      Serial.println("Invalid input format");
    }

    // Очищаем строку и сбрасываем флаг
    inputString = "";
    stringComplete = false;
  }

  delay(10);  // задержка просто для "стабильности"
}
