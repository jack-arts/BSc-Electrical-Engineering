int mic = A0;

int micvalue = 0;
int newvalue=0;
int oldvalue=0;
void setup() {
Serial.begin(9600);
pinMode(mic, INPUT);
}

int microphone(void)
{
  micvalue=0;
long starttime=millis();
  while(millis()-starttime<2000) {
    //Serial.println(analogRead(mic));  
  if(micvalue < analogRead(mic)) {
    micvalue = analogRead(mic);
}
} 
return micvalue;  
}

int comparemic(int *oldvalue){
Serial.println("Old Value");
Serial.println(*oldvalue);
    Serial.println("New Value");
  newvalue=microphone();
  Serial.println(newvalue);
  if(*oldvalue+15 < newvalue){
  *oldvalue=newvalue;
   return 1;
  }
  if(*oldvalue-15 > newvalue){
  *oldvalue=newvalue;
   return -1;
  }
  if(*oldvalue >= newvalue-15 && *oldvalue <= newvalue+15){
  *oldvalue=newvalue;
   return 0;  
  }
}

void loop() {
  Serial.println(comparemic(&oldvalue));
  Serial.println("----------------------------------------------------------------");
    delay(7000);
}
