

// using 7 segments display numbers from 0-9


int b=1; int a=2; int f=3; int g=4; int e=5; int d=6; int c=7;
void setup()
{
pinMode(a,OUTPUT); pinMode(b,OUTPUT);
 pinMode(c,OUTPUT); pinMode(d,OUTPUT);
  pinMode(e,OUTPUT); pinMode(f,OUTPUT);
   pinMode(g,OUTPUT);
}
void zero()
 
{
digitalWrite(a,1); digitalWrite(b,1); digitalWrite(c,1); digitalWrite(d,1);
 digitalWrite(e,1); digitalWrite(f,1); digitalWrite(g,0);
}
void one()
{
digitalWrite(a,0); digitalWrite(b,1); digitalWrite(c,1); digitalWrite(d,0); 
digitalWrite(e,0); digitalWrite(f,0); digitalWrite(g,0);
}
void two()
{
digitalWrite(a,1); digitalWrite(b,1); digitalWrite(c,0); digitalWrite(d,1);
 
digitalWrite(e,1); digitalWrite(f,0); digitalWrite(g,1);
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
digitalWrite(a,1); digitalWrite(b,1); digitalWrite(c,1); digitalWrite(d,0);
 
digitalWrite(e,0); digitalWrite(f,0); digitalWrite(g,0);
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
zero(); delay(1000); one(); delay(1000); two(); delay(1000);
three(); delay(1000); four(); delay(1000); five();
delay(1000); six(); delay(1000); seven(); delay(1000);
eight(); delay(1000); nine(); delay(1000);
}
