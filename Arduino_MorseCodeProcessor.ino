/*

  LCD Library Provided By:
  Library originally added 18 Apr 2008
  by David A. Mellis
  library modified 5 Jul 2009
  by Limor Fried (http){//www.ladyada.net)
  example added 9 Jul 2009
  by Tom Igoe
  modified 22 Nov 2010
  by Tom Igoe
  modified 7 Nov 2016
  by Arturo Guadalupi

  This example code is in the public domain.

  http){//www.arduino.cc/en/Tutorial/LiquidCrystalHelloWorld

  Morse Code Translating Device for Arduino Uno
  by Timothy Duong, Jordan Alberico

  last modified 12/12/18

  This code takes any Byte sent over UART and translates it into Alphanumeric. The intended bytes are using Morse Protocol as follows:

  0x00  = dot
  0x01  = dash
  0x02  = end character

*/

// include the library code){
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

byte value[1];            //Byte sent over UART
String uart = "";         //Used to store previous byte values
String letters = "";      //Letter to be printed
boolean printed = false;  //Boolean to print the letter only once
int numLetters = 0;       //Counter to check 1st or 2nd line on LCD

void setup() {

  Serial.begin(9600);
  // set up the LCD's number of columns and rows){
  lcd.begin(16, 2);
  // Print a message to the LCD.
  // print the number of seconds since reset){

}

void loop() {
  // set the cursor to column 0, line 1
  // (note){ line 1 is the second row, since counting begins with 0)){
  formatUART();
  if(!printed){
    lcd.print(letters);
    printed = true;
    numLetters++;
  }

  if(numLetters == 17){
    lcd.setCursor(0,1);
  }
}

//Checks if the Byte is a 0, 1, or 2 and acts accordingly 
void formatUART() {
  if (Serial.available() > 0) {
    Serial.readBytes(value,1);
    delay(5);

    //If byte is 0, then concatenate a 0 to uart
    if (value[0] == 0) {
      uart += "0";
    }
    //If byte is 1, then concatenate a 1 to uart
    else if (value[0] == 1) {
      uart += "1";
    }
    //if byte is 2, then find the corresponding letter
    else if (value[0] == 2) {
      addCharacter();
      uart = "";
      printed = false;
    }
    //if byte is not expected, return error
    else {
      Serial.println("error reading serial");
    }
  }
  
}

//Searches for the letter matching uart, returns error if not found
void addCharacter() {
  delay(100);
  if (uart == "0") {
    letters = "E";
  }
  else if (uart == "00") {
    letters = "I";
  }
  else if (uart == "01") {
    letters = "A";
  }
  else if (uart == "000") {
    letters = "S";
  }
  else if (uart == "001") {
    letters = "U";
  }
  else if (uart == "010") {
    letters = "R";
  }
  else if (uart == "011") {
    letters = "W";
  }
  else if (uart == "0000") {
    letters = "H";
  }
  else if (uart == "00000") {
    letters = "5";
  }
  else if (uart == "0001") {
    letters = "V";
  }
  else if (uart == "0010") {
    letters = "F";
  }
  else if (uart == "0011") {
    letters = " ";
  }
  else if (uart == "0100") {
    letters = "L";
  }
  else if (uart == "00111") {
    letters = "2";
  }
  else if (uart == "00011") {
    letters = "3";
  }
  else if (uart == "00001") {
    letters = "4";
  }
  else if (uart == "011") {
    letters = "W";
  }
  else if (uart == "0110") {
    letters = "P";
  }
  else if (uart == "0111") {
    letters = "J";
  }
  else if (uart == "01111") {
    letters = "1";
  }
  else if (uart == "1") {
    letters = "T";
  }
  else if (uart == "10") {
    letters = "N";
  }
  else if (uart == "11") {
    letters = "M";
  }
  else if (uart == "100") {
    letters = "D";
  }
  else if (uart == "101") {
    letters = "K";
  }
  else if (uart == "110") {
    letters = "G";
  }
  else if (uart == "111") {
    letters = "O";
  }
  else if (uart == "1000") {
    letters = "B";
  }
  else if (uart == "10000") {
    letters = "6";
  }
  else if (uart == "1001") {
    letters = "X";
  }
  else if (uart == "1010") {
    letters = "C";
  }
  else if (uart == "1011") {
    letters = "Y";
  }
  else if (uart == "1100") {
    letters = "Z";
  }
  else if (uart == "1101") {
    letters = "Q";
  }
  else if (uart == "11000") {
    letters = "7";
  }
  else if (uart == "1110") {
    letters = ".";
  }
  else if (uart == "1111") {
    letters = "-";
  }
  else if (uart == "11111") {
    letters = "0";
  }
  else if (uart == "11110") {
    letters = "9";
  }
  else if (uart == "11100") {
    letters = "8";
  }
  else{
    letters = "err";
  }
}
