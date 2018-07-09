#include <LiquidCrystal.h>
#include <TimeLib.h>


//  VARIABLES
int pulsePin = 0;                 // Pulse Sensor purple wire connected to analog pin 0
int blinkPin = 13;                // pin to blink led at each beat
int fadePin = 5;                  // pin to do fancy classy fading blink at each beat
int fadeRate = 0;                 // used to fade LED on with PWM on fadePin


// these variables are volatile because they are used during the interrupt service routine!
volatile int BPM;                   // used to hold the pulse rate
volatile int Signal;                // holds the incoming raw data
volatile int IBI = 600;             // holds the time between beats, the Inter-Beat Interval
volatile boolean Pulse = false;     // true when pulse wave is high, false when it's low
volatile boolean QS = false;        // becomes true when Arduoino finds a beat.


float radius = 25; // 바퀴당 이동 거리를 확인 하기 위해 자전거 바퀴의 반지름을 입력해 줍니다.(Cm 단위)
float circle = (2 * radius * 3.14) / 100;  // 자전거 바퀴의 둘레를 계산(단위를 m로 바꿔주기 위해 100을 나눕니다.)

float bySpeed = 0; // 자전거의 속도
float ckTime = 0;  // 리드스위치가 
float uckTime = 0; // Unckecked
float cycleTime = 0;  // 리드스위치가 인식이 안됬을 시간 부터 인식됬을 때까지의 시간
float distance = 0; // 자전거의 누적 이동 거리
float lcdDis = 0; // 자전거의 이동 거리를 LCD출력에 맞게 바꿔즌 값.(단위 수정 or 소숫점 제거)

int count = 0;  // 리드스위치의 노이즈를 제거하기 위해 카운트를 넣어줍니다.
boolean temp = 0;  // 리드 스위치가 닫혔는지 확인하는 변수


void setup(){
  pinMode(blinkPin,OUTPUT);         // pin that will blink to your heartbeat!
  pinMode(fadePin,OUTPUT);          // pin that will fade to your heartbeat!
  Serial.begin(9600);             // we agree to talk fast!
  interruptSetup();                 // sets up to read Pulse Sensor signal every 2mS 
   // UN-COMMENT THE NEXT LINE IF YOU ARE POWERING The Pulse Sensor AT LOW VOLTAGE, 
   // AND APPLY THAT VOLTAGE TO THE A-REF PIN
   //analogReference(EXTERNAL);   
}

unsigned long prev_time = millis();

void loop(){
  //sendDataToProcessing('S', Signal);     // send Processing the raw Pulse Sensor data
  if (QS == true){                       // Quantified Self flag is true when arduino finds a heartbeat
        fadeRate = 255;                  // Set 'fadeRate' Variable to 255 to fade LED with pulse
        sendDataToProcessing('B',BPM);   // send heart rate with a 'B' prefix
       // sendDataToProcessing('Q',IBI);   // send time between beats with a 'Q' prefix
        QS = false;                      // reset the Quantified Self flag for next time    
     }
  
  //ledFadeToBeat();
  unsigned long current_time = millis();
  
    boolean check = digitalRead(A1); // 리드스위치의 상태를 확인합니다.
  
  if(check == 1 && temp == 0){  // 리드 스위치가 열릴 때(닫힘 -> 열림)
    ckTime = millis();  // 시간을 확인해서 저장합니다.
    temp = 1;  // temp값을 1로 바꿔줍니다.(리드스위치가 열려있는 상태값 저장)
  }
  
  else if(check == 0 && temp == 1 && count > 5){  // 리드 스위치가 닫히고(열림 -> 닫힘), 노이즈 방지 카운트가 5이상일때
    uckTime = millis();  // 시간을 확인해서 저장합니다.
      
    cycleTime = (uckTime - ckTime) / 1000; // 열릴 때 시각과 닫힐 때 시각의 차를 이용하여 바퀴가 한바퀴 돌때 걸린 시간을 계산합니다.
    bySpeed = (circle / cycleTime) * 3.6; // 바퀴가 한바퀴 돌때의 거리와 시간을 가지고 속도를 구해줍니다.(단위는 Km/h입니다.)
    temp = 0; 
    count = 0; 
    distance += circle;  // 한바퀴 돌았으면 이동거리를 누적 이동거리에 더해줍니다.
  }
  
  if(check == 1){  // 리드 스위치가 열려있으면 카운트를 1씩 증가 시켜 줍니다.
    count++;
    if(count > 150){ // 카운트가 150이 넘어가면(자전거가 멈췄을 때) 속도를 0으로 바꿔줍니다.
      bySpeed = 0;
    }
  }
  
  fadeRate -= 15;                         //  set LED fade value
  fadeRate = constrain(fadeRate,0,255);   //  keep LED fade value from going into negative numbers!
  analogWrite(fadePin,fadeRate);          //  fade LED
 
// prev_time = current_time;
    if (current_time - prev_time > 3000) {
       Serial.print(distance/1000.0);
       Serial.print(" ");
       Serial.println(bySpeed);    
    }
  


//  delay(20);                             //  take a break

}


void ledFadeToBeat(){
    fadeRate -= 15;                         //  set LED fade value
    fadeRate = constrain(fadeRate,0,255);   //  keep LED fade value from going into negative numbers!
    analogWrite(fadePin,fadeRate);          //  fade LED
  }


void sendDataToProcessing(char symbol, int data ){
 //   Serial.print(symbol);                // symbol prefix tells Processing what type of data is coming
  //  if(!strcmp(symbol, 'B'))    
       Serial.println(data);                // the data to send culminating in a carriage return
       
  }
