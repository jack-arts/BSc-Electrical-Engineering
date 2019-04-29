//MPPT pins
int currentRead = A1;
int voltageRead = A2;
int VoltageWrite = 11;
int current=50;

//driver pins
int highWrite = 5;
int lowWrite = 6;

//load detection pins
int enableWrite = 7;
int enableWriteB = 8;
 int oldrealTimeVoltage = 0;
//local parameters
double realTimeCurrent;
double realTimeVoltage = 0;
int enableTreshold = 800;
int oldInstantPower = 0;
int newInstantPower = 0;
int powerOutput = 13;
int PWM = 0;
int dir =1;
double oldPower;
double newPower;
double maxPower;

int count=0;
//int frequency = 50;
void setup() {
  Serial.begin(9600);
  TCCR0B = TCCR0B & B11111000 | B00000001;
  pinMode(13, OUTPUT);
  pinMode(51, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  Serial.println("Hello World!");
  pinMode(currentRead, INPUT);
  pinMode(voltageRead, INPUT);
  analogWrite(highWrite, 200);
  analogWrite(lowWrite, 200);
  analogWrite(powerOutput, 0);
  
    digitalWrite(enableWrite, LOW);
    digitalWrite(enableWriteB, LOW);
}

void loop() {
  int i;
  realTimeVoltage=0;
  realTimeCurrent=0;
  

  
  for(i=0; i <10000; i++) {
  realTimeVoltage = realTimeVoltage + (double)analogRead(voltageRead);
  }
  realTimeVoltage = realTimeVoltage/10000;
  
  Serial.print("Voltage: ");
  Serial.println(realTimeVoltage);
  // load detection
//  while(current>0){
//    
   for(i=0; i <100; i++) {
  realTimeCurrent = realTimeCurrent + (double)analogRead(currentRead);
  }
  realTimeCurrent = realTimeCurrent/100;

  
  Serial.print("Current: ");
  Serial.println(realTimeCurrent);
 // Serial.print(" ");
  //Serial.println(current);

//  if (realTimeCurrent == 0) {
//    current--;
//  }else if(realTimeCurrent >= 1){
//    current=50;
//  }
//  }
//   digitalWrite(enableWrite, HIGH);
//   digitalWrite(enableWriteB, HIGH);
//   Serial.print("load not connected load not connected load not connected load not connected load not connected load not connected load not connected load not connected load not connected load not connected load not connected ");
//
//   delay(100000);
//   digitalWrite(enableWrite, LOW);
//   digitalWrite(enableWriteB, LOW);
//   current=200;

  
  //Serial.print("OldPower: ");
  //Serial.println(oldInstantPower);
  // MPPT & driver

  if (PWM > 200) {
    Serial.println("PWM too high");
    delay(9999999999999999999);
  }
  //newInstantPower = realTimeCurrent * realTimeVoltage;
  //Serial.print("NewPower: ");
  //Serial.println(newInstantPower);
newPower = realTimeVoltage*realTimeCurrent;
   Serial.print("new power: ");
  Serial.println(newPower);  
   
  if (newPower + 5000 < oldPower) {
    
    count++;
    if (count>4){
      
dir *=-1 ;
count=0;
    
//max point

    
    Serial.print("load not connected load not connected load not connected load not connected load not connected load not connected load not connected load not connected load not connected load not connected load not connected ");
//
    Serial.println("MPP found");
    delay(99999999999999999);
    }
  }

  
oldPower = newPower;


  PWM = PWM + 5;
  if (PWM < 0){
    PWM = 0;
    dir=1;
  }
  Serial.print("PWM: ");
  Serial.println(PWM);
  analogWrite(powerOutput, PWM);
   Serial.print("old power: ");
  Serial.println(oldPower);
    //Serial.print("voltage: ");
  //Serial.println(oldrealTimeVoltage);
  //oldInstantPower = newInstantPower;
}
