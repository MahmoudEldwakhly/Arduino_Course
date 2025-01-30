#include <Keypad.h>
#include <Wire.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>

Servo servo;  // create servo object to control a servo

int pos = 0;    // variable to store the servo position

int answer;

const byte ROWS = 4; // Four rows
const byte COLS = 4; // Four columns
char keys[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {A0, A1, 11, 10}; // Connect to the row pinouts of the keypad
byte colPins[COLS] = {9, 8, 7, 6};    // Connect to the column pinouts of the keypad

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// LCD address may vary based on your module
LiquidCrystal_I2C lcd(0x27, 16, 2); // Set the LCD address to 0x27 for a 16 chars and 2 line display

int num1 = 0;            // First random number for multiplication
int num2 = 0;            // Second random number for multiplication
int result = 0;          // Store the multiplication result
String userAnswer = "";  // Store the user's answer as a string
bool answered = false;    // Flag to track whether the user answered

void setup()
{
    lcd.init(); // Initialize the LCD
    lcd.backlight(); // Turn on the backlight
    randomSeed(analogRead(0)); // Seed the random number generator
    generateRandomNumbers();
    servo.attach(12);  // Attaches the servo on pin 6 to the servo object
    Wire.begin(10); // Join I2C bus ( slave of address 10)
    Wire.onReceive(receiveEvent); // register event
    Serial.begin(9600);           // start serial for output
    Wire.onRequest(requestEvent); // register event
}

void loop()
{
    char key = keypad.getKey();

    if (!answered)
    {
        if (key)
        {
            if (isdigit(key))
            {
                userAnswer += key;
                lcd.setCursor(8, 0);
                lcd.print(userAnswer);
            }
            else if (key == '#')
            {
                int userValue = userAnswer.toInt();
                lcd.setCursor(0, 1);
                if (userValue == result)
                {
                    lcd.print("Correct  ");
                   // delay(3000);
                    lcd.clear();
                    servo.write(90);
                   // delay(1000);
                    servo.write(120);
                    delay(1000);
                    servo.write(90);
                    delay(1000);
                    answer = 1;
                }
                else
                {
                    lcd.print("Incorrect");
                   // delay(3000);
                    lcd.clear();
                    servo.write(60);
                    delay(1000);
                    servo.write(90);
                    delay(1000);
                    answer = 0;
                }
                userAnswer = "";
                answered = true;
                generateRandomNumbers();
            }
        }
    }
    else
    {
        if (key)
        {
            if (isdigit(key))
            {
                result = num1 * num2;
                lcd.setCursor(0, 0);
                lcd.print(num1);
                lcd.print(" * ");
                lcd.print(num2);
                lcd.print(" = ? ");
                userAnswer = "";
                answered = false;
            }
        }
    }

    Wire.beginTransmission(8); // Transmit to device #8
    Wire.write(answer);        // Sends answer of child true or false
    Wire.endTransmission();    // Stop transmitting
    delay(500);

    Wire.requestFrom(8, 6); // Request 6 bytes from peripheral device #8

    while (Wire.available())
    { // Peripheral may send less than requested
        int lefting = Wire.read(); // Receive a byte as a character
    }

    Wire.requestFrom(9, 6); // Request 6 bytes from peripheral device #9

    while (Wire.available())
    { // Peripheral may send less than requested
        int sorting = Wire.read(); // Receive a byte as a character
    }

    delay(500);
    
}

void generateRandomNumbers()
{
    num1 = random(1, 10); // Random number between 1 and 9
    num2 = random(1, 10); // Random number between 1 and 9

    lcd.clear();  // Clear the LCD before displaying a new problem
    lcd.setCursor(0, 0);
    lcd.print(num1);
    lcd.print(" * ");
    lcd.print(num2);
    lcd.print(" = ? ");
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany)
{


}


 void requestEvent() 
{
  Wire.write(answer); 
}
