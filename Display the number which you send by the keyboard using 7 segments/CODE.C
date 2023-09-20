

// 3-using 7 segments display the number which you send by the keyboard

int b=1; int a=2; int f=3; int g=4; int e=5; int d=6; int c=7;
char READING = ',' ;
void setup()
{
Serial.begin(9600);
 pinMode(a,OUTPUT);
 pinMode(b,OUTPUT); 
 pinMode(c,OUTPUT);
 pinMode(d,OUTPUT);
 pinMode(e,OUTPUT);
 pinMode(f,OUTPUT);
 pinMode(g,OUTPUT);
}
 
void zero()
{
digitalWrite(a,1); digitalWrite(b,1); digitalWrite(c,1); digitalWrite(d,1); digitalWrite(e,1); digitalWrite(f,1); digitalWrite(g,0);
}
void one()
{
digitalWrite(a,0); digitalWrite(b,1); digitalWrite(c,1); digitalWrite(d,0); digitalWrite(e,0); digitalWrite(f,0); digitalWrite(g,0);
}
void two()
{
digitalWrite(a,1); digitalWrite(b,1); digitalWrite(c,0);
 
digitalWrite(d,1); digitalWrite(e,1); digitalWrite(f,0); digitalWrite(g,1);
}
void three()
{
digitalWrite(a,1); digitalWrite(b,1); digitalWrite(c,1); digitalWrite(d,1); digitalWrite(e,0); digitalWrite(f,0); digitalWrite(g,1);
}
void four()
{
digitalWrite(a,0); digitalWrite(b,1); digitalWrite(c,1); digitalWrite(d,0); digitalWrite(e,0); digitalWrite(f,1); digitalWrite(g,1);
}
 
void five()
{
digitalWrite(a,1); digitalWrite(b,0); digitalWrite(c,1); digitalWrite(d,1); digitalWrite(e,0); digitalWrite(f,1); digitalWrite(g,1);
}
void six()
{
digitalWrite(a,1); digitalWrite(b,0); digitalWrite(c,1); digitalWrite(d,1); digitalWrite(e,1); digitalWrite(f,1); digitalWrite(g,1);
}
void seven()
{
digitalWrite(a,1); digitalWrite(b,1); digitalWrite(c,1);
 
digitalWrite(d,0); digitalWrite(e,0); digitalWrite(f,0); digitalWrite(g,0);
}
void eight()
{
digitalWrite(a,1); digitalWrite(b,1); digitalWrite(c,1); digitalWrite(d,1); digitalWrite(e,1); digitalWrite(f,1); digitalWrite(g,1);
}
void nine()
{
digitalWrite(a,1); digitalWrite(b,1); digitalWrite(c,1); digitalWrite(d,1); digitalWrite(e,0); digitalWrite(f,1); digitalWrite(g,1);
}
 
void loop()
{
if (Serial.available()> 0)
{
READING = Serial.read() ;


switch (READING)
{

case '0' : zero(); break ;
case '1' : one(); break ;
case '2' : two(); break ;
case '3' : three(); break ;
case '4' : four(); break ;
case '5' : five(); break ;
case '6' : six(); break ;
case '7' : seven(); break ;
case '8' : eight(); break ;
case '9' : nine(); break ;
default : Serial.println("Enter a number from 0 to 9 only "); break ;
}



}
 
}
