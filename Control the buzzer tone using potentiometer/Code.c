int BUZZER = 3 ; int POT = A0 ;
int POT_READING = 0 ;
int BUZZER_READING = 0 ;
void setup()
{
pinMode (BUZZER,OUTPUT);
pinMode (POT,INPUT);


}

void loop()
{

POT_READING = analogRead(POT) ;
BUZZER_READING = map(POT_READING,0,1023,31,4978);
// analogWrite (BUZZER,BUZZER_READING); tone(BUZZER,BUZZER_READING,1000);
delay(1000); noTone(1000);


}
