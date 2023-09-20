/*
Using lcd , pushbutton and 2 leds make a state machine system at which when you push the button the red led on and print "red led" on lcd and if you pushed again the green led on and red led off and print "green led", And if you pushed third it back to red again.
*/

#include <Adafruit_LiquidCrystal.h> 
#define button 2
#define Red_led 3
#define Green_led 4 int button_Reading ; int counter = 0;

Adafruit_LiquidCrystal lcd_1(0);


void setup()
{

pinMode (button,INPUT); pinMode (Red_led,OUTPUT); pinMode (Green_led,OUTPUT); lcd_1.begin(16, 2); lcd_1.setBacklight(1);
 
}


void loop()
{

button_Reading = digitalRead (button) ; if (button_Reading == 1 && counter == 0)
{

digitalWrite(Red_led , 1) ; counter++ ; lcd_1.print("Red led "); digitalWrite(Green_led , 0) ; delay(1000);

}
else if (button_Reading == 1 && counter == 1)
{

digitalWrite(Red_led , 0) ; digitalWrite(Green_led , 1) ;



lcd_1.print("Green led ");



counter = 0 ; delay(1000) ;
}
 
else
{

digitalWrite(Red_led , 0) ; digitalWrite(Green_led , 0) ; lcd_1.clear () ;
}



}