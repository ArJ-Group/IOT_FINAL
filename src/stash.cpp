// Soil Moisture Sensor Using LCD
// void setup() {
//     Serial.begin(9600); 
//     lcd.init(); // initialize the lcd
//     lcd.backlight();
//     lcd.clear();
//     lcd.home();
// }

// void scrollText(int row, String message, int delayTime, int lcdColumns)
//  {
//    for (int i = 0; i < lcdColumns; i++)
//    {
//      message = " " + message;
//    }
//    message = message + " ";
//    for (int pos = 0; pos < message.length(); pos++)
//    {
//      lcd.setCursor(0, row);
//      lcd.print(message.substring(pos, pos + lcdColumns));
//      delay(delayTime);
//    }
//  }


// void loop() {
//   lcd.home();

//   moisture_percentage = ( 100.00 - ( (analogRead(sensor_pin)/1023.00) * 100.00 ) );

//   Serial.print("Soil Moisture(in Percentage) = ");
//   Serial.print(moisture_percentage);
//   Serial.println("%");

//   if(moisture_percentage < 45)
//   {
//     lcd.print("kering ");
//   } else if(moisture_percentage > 45 && moisture_percentage < 65)
//   {
//     lcd.print("lembab ");
//   } else 
//   {
//     lcd.print("basah ");
//   } 
//   lcd.println(moisture_percentage);
//   scrollText(1, "Soil Moisture Value", 250, 16);
//   // delay(1000);
// }