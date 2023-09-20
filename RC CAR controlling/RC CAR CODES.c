

// Car Direction Test :


#define speedL 10
#define IN1 9
#define IN2 8
#define IN3 7
#define IN4 6
#define speedR 5
void setup()
{
Serial.begin (9600);
for(int i=5 ; i<=10 ; i++)
{
pinMode(i, OUTPUT);
}
}
void forword()
{
digitalWrite(IN1, HIGH);
digitalWrite(IN2, LOW);
digitalWrite(IN3, HIGH);
digitalWrite(IN4, LOW);
analogWrite(speedL,150);
analogWrite(speedR,150);
}
void backword()
{
digitalWrite(IN1, LOW);
digitalWrite(IN2, HIGH);
digitalWrite(IN3, LOW);
digitalWrite(IN4, HIGH);
analogWrite(speedL,150);
analogWrite(speedR,150);
}
void left()
{
digitalWrite(IN1, LOW);
digitalWrite(IN2, LOW);
digitalWrite(IN3, HIGH);
digitalWrite(IN4, LOW);
analogWrite(speedL,0);
analogWrite(speedR,150);
}
void right()
{
digitalWrite(IN1, HIGH);
digitalWrite(IN2, LOW);
digitalWrite(IN3, LOW);
digitalWrite(IN4, LOW);
analogWrite(speedL,150);
analogWrite(speedR,0);
}
void stopp() {
digitalWrite(IN1, LOW);
digitalWrite(IN2, LOW);
digitalWrite(IN3, LOW);
digitalWrite(IN4, LOW);
analogWrite(speedL,0);
analogWrite(speedR,0); }
void loop() {
forword(); delay(2000);
backword(); delay(2000);
right(); delay(2000);
left(); delay(2000);
stopp(); delay(2000);
}

//---------------------------------------------------------
// Controlled robot by Bluetooth : 

#define speedL 10
#define IN1 9
#define IN2 8
#define IN3 7
#define IN4 6
#define speedR 5
char Reading;
void setup()
{
Serial.begin (9600);
for(int i=5 ; i<=10 ; i++)
{
pinMode(i, OUTPUT);
}
}
void forword()
{
digitalWrite(IN1, HIGH);
digitalWrite(IN2, LOW);
digitalWrite(IN3, HIGH);
digitalWrite(IN4, LOW);
analogWrite(speedL,150);
analogWrite(speedR,150);
}
void backword()
{
digitalWrite(IN1, LOW);
digitalWrite(IN2, HIGH);
digitalWrite(IN3, LOW);
digitalWrite(IN4, HIGH);
analogWrite(speedL,150);
analogWrite(speedR,150);
}
void left()
{
digitalWrite(IN1, LOW);
digitalWrite(IN2, LOW);
digitalWrite(IN3, HIGH);
digitalWrite(IN4, LOW);
analogWrite(speedL,0);
analogWrite(speedR,150);
}
void right()
{
digitalWrite(IN1, HIGH);
digitalWrite(IN2, LOW);
digitalWrite(IN3, LOW);
digitalWrite(IN4, LOW);
analogWrite(speedL,150);
analogWrite(speedR,0);
}
void stopp() {
digitalWrite(IN1, LOW);
digitalWrite(IN2, LOW);
digitalWrite(IN3, LOW);
digitalWrite(IN4, LOW);
analogWrite(speedL,0);
analogWrite(speedR,0); }
void loop() {
if(Serial.available()>0){
Reading=Serial.read();
switch(Reading){
case 'F': forword(); break;
case 'B': backword(); break;
case 'R': right(); break;
case 'L' : left(); break;
case 'S' :stopp(); break;
}
}
}

//-----------------------------------------------------------------------------
// Line Follower Code :


#define speedL 10
#define IN1 9
#define IN2 8
#define IN3 7
#define IN4 6
#define speedR 5
#define sensorL 4
#define sensorR 3
int sl=0;
int sr=0;
void setup() {
for(int i=5;i<=10;i++)
{
pinMode(i, OUTPUT);
}
pinMode(sensorR, INPUT);
pinMode(sensorL, INPUT);
}
void forword()
{
digitalWrite(IN1, HIGH);
digitalWrite(IN2, LOW);
digitalWrite(IN3, HIGH);
digitalWrite(IN4, LOW);
analogWrite(speedL,150);
analogWrite(speedR,150);
}
void backword()
{
digitalWrite(IN1, LOW);
digitalWrite(IN2, HIGH);
digitalWrite(IN3, LOW);
digitalWrite(IN4, HIGH);
analogWrite(speedL,150);
analogWrite(speedR,150);
}
void left()
{
digitalWrite(IN1, LOW);
digitalWrite(IN2, LOW);
digitalWrite(IN3, HIGH);
digitalWrite(IN4, LOW);
analogWrite(speedL,0);
analogWrite(speedR,150);
}
void right()
{
digitalWrite(IN1, HIGH);
digitalWrite(IN2, LOW);
digitalWrite(IN3, LOW);
digitalWrite(IN4, LOW);
analogWrite(speedL,150);
analogWrite(speedR,0);
}
void stopp(){
digitalWrite(IN1, LOW);
digitalWrite(IN2, LOW);
digitalWrite(IN3, LOW);
digitalWrite(IN4, LOW);
analogWrite(speedL,0);
analogWrite(speedR,0);
}
void loop(){
sl=digitalRead(sensorL);
sr=digitalRead(sensorR);
if (sl==0&&sr==0)
forword();
else if (sl==0&&sr==1)
right();
else if (sl==1&&sr==0)
left();
else if (sl==1&&sr==1)
stopp(); }
//----------------------------------------------------------------------------------
// Obstacle Avoiding Code :

#define speedL 10
#define IN1 9
#define IN2 8
#define IN3 7
#define IN4 6
#define speedR 5
#define trig 3
#define echo 4
long duration,distance;
void setup() 
{
for(int i=5 ; i<=11 ; i++)
{
pinMode(i, OUTPUT);
}
pinMode(echo, INPUT);
}
void Ultrasonic(){
digitalWrite(trig, LOW);
delayMicroseconds(2);
digitalWrite(trig, HIGH);
delayMicroseconds(10);
digitalWrite(trig, LOW);
duration = pulseIn(echo, HIGH);
distance = (duration/2) * 0.0343;
}
void forword()
{
digitalWrite(IN1, HIGH);
digitalWrite(IN2, LOW);
digitalWrite(IN3, HIGH);
digitalWrite(IN4, LOW);
analogWrite(speedL,150);
analogWrite(speedR,150);
}
void backword()
{
digitalWrite(IN1, LOW);
digitalWrite(IN2, HIGH);
digitalWrite(IN3, LOW);
digitalWrite(IN4, HIGH);
analogWrite(speedL,150);
analogWrite(speedR,150);
}
void left()
{
digitalWrite(IN1, LOW);
digitalWrite(IN2, LOW);
digitalWrite(IN3, HIGH);
digitalWrite(IN4, LOW);
analogWrite(speedL,0);
analogWrite(speedR,150);
}
void right()
{
digitalWrite(IN1, HIGH);
digitalWrite(IN2, LOW);
digitalWrite(IN3, LOW);
digitalWrite(IN4, LOW);
analogWrite(speedL,150);
analogWrite(speedR,0);
}
void stopp(){
digitalWrite(speedL, LOW);
digitalWrite(speedR, LOW);
}
void loop(){
Ultrasonic();
if(distance<20){
stopp(); delay(250);
backword();delay(500);
right(); delay(1000);
}
else{ forword(); }
}