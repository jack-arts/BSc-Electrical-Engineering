
int S1 = 2;
int S2 = 3;
int Control = 5;
int POT = A0;
int LDR = A1;
 
void setup() {
  analogWrite(Control,0);
}
 
void loop() {

  analogWrite(Control,160); 
}
