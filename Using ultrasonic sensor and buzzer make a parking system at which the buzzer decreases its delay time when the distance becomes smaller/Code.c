
// Using ultrasonic sensor and buzzer make a parking system at which the buzzer decreases its delay time when the distance becomes smaller.


 #define ULtrsonic 6	// same pin for ( Trig and ech )
 #define Buzzer 2

long duration , distance ;



void setup()
{


pinMode (Buzzer,OUTPUT);


Serial.begin(9600);


}


void loop()
{
pinMode (ULtrsonic,OUTPUT); digitalWrite ( ULtrsonic , LOW ) ; delayMicroseconds(2); digitalWrite ( ULtrsonic , HIGH ) ;
 
delayMicroseconds(10); digitalWrite ( ULtrsonic , LOW ) ;
 pinMode (ULtrsonic,INPUT); duration = pulseIn (ULtrsonic,1); 
distance = duration / 2*0.0343 ; Serial.println(distance); delay(5);
digitalWrite ( Buzzer , 1 ) ; 
delayMicroseconds(duration/2); 
digitalWrite ( Buzzer , 0 ) ; 
delayMicroseconds(duration/2);



}
