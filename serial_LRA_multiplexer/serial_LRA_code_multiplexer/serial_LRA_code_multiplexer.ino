#include "Wire.h"
#include "Adafruit_DRV2605.h"

extern "C" {
#include "utility/twi.h"
}
#define TCAADDR 0x70

Adafruit_DRV2605 drv1;
Adafruit_DRV2605 drv2;
Adafruit_DRV2605 drv3;

int m_s_dur = 50;
double soa = 0.32*m_s_dur + 47.3;

//int m_s_dur2 = 100;
//int m_d_dur1 = 100;
//int m_d_dur2 = 100;

unsigned long m_t_now = 0;

int en = 3;
int in1 = 4;
int in2 = 5;

void tcaselect(uint8_t i) {
  if (i>7) return;
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();
}

void setup() {

  Serial.setTimeout(5);

  while(!Serial);   //Serial은 시리얼 포트가 준비되면 true 반환
  delay(1000);
  Wire.begin();
  Serial.begin(57600);

  tcaselect(0);
  drv1.begin();
  // Set Real-Time Playback mode
  drv1.setMode(DRV2605_MODE_REALTIME);

  tcaselect(1);
  drv2.begin();
  // Set Real-Time Playback mode
  drv2.setMode(DRV2605_MODE_REALTIME);

  tcaselect(2);
  drv3.begin();
  // Set Real-Time Playback mode
  drv3.setMode(DRV2605_MODE_REALTIME);

  pinMode(en, OUTPUT);
  pinMode(in1, OUTPUT);   //4번핀 출력으로 설정
  pinMode(in2, OUTPUT);   //5번핀 출력으로 설정
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  analogWrite(en, 0);
  Serial.println("\nTCAScanner ready!");
  /*for (uint8_t t=0;t<8;t++) {
    tcaselect(t);
    Serial.print("TCA Port #"); Serial.println(t);
    for(uint8_t addr = 0;addr<=127;addr++) {
      if(addr == TCAADDR) continue;
      uint8_t data;
      if(! twi_wr67iteTo(addr, &data, 0, 1, 1)) {
        Serial.print("Found I2C 0x"); Serial.println(addr, HEX);
      }
    }
  }*/
  Serial.println("\ndone");
}


String input;

void loop() {

while(Serial.available())
  {
 
  input = Serial.readStringUntil('\n');
    
  if(input.equals("0"))
  {
      tcaselect(0);
      if(m_s_dur == 0) drv1.setRealtimeValue(0x00);
      else {
        m_t_now = millis();   //프로그램 실행 후 지난 밀리 초 숫자 반환
        drv2.setRealtimeValue(0x7F);
        while(millis() < m_t_now + m_s_dur);
        drv2.setRealtimeValue(0x00);
//    m_t_now = millis();
//    while(millis() < m_t_now + m_d_dur1);

      }
        delay(soa - m_s_dur);
        tcaselect(1);
      if(m_s_dur == 0) drv1.setRealtimeValue(0x00);
      else {
        m_t_now = millis();   //프로그램 실행 후 지난 밀리 초 숫자 반환
        drv3.setRealtimeValue(0x7F);
        while(millis() < m_t_now + m_s_dur);
        drv3.setRealtimeValue(0x00);
//    m_t_now = millis();
//    while(millis() < m_t_now + m_d_dur1);

      }
        delay(soa - m_s_dur);
     
      if(Serial.available())
      {
        String tmp = Serial.readStringUntil('\n');
        if(tmp == "1") input = tmp;
      }     
  }

  Serial.flush();
  }
}

/*void serialEvent() {    //시리얼 통신을 통해 데이터가 수신되면 자동으로 호출됨
  if(Serial.available())
  {
  Serial.println ("serialEvent");
  char buffer[100];
  char buf1[4];//, buf2[2], buf3[4];
  int mt_dir = 0, mt_pwm = 0;
  memset(buf1, 0, 4);   //0에서 4 바이트를 가져와 buf1에 저장
//  memset(buf2, 0, 2);
//  memset(buf3, 0, 4);
  buffer[0] = Serial.read();
  //int len = Serial.readBytesUntil('\n', buffer,1);//7);    //시리얼 버퍼에서 문자를 읽어서 배열 안으로 넣음(종료 문자, 버퍼, 읽을 바이트수)
  memcpy(buf1, buffer, 1);    //buffer를 buf1로 3바이트 복사
//  memcpy(buf2, buffer+3, 1);
//  memcpy(buf3, buffer+4, 3);
  input = buf1[0];
  }
  //m_s_dur = atoi(buf1);
  //Serial.print("signal duration: "); Serial.println(m_s_dur);
///
//  mt_dir = atoi(buf2);
//  mt_pwm = atoi(buf3);
//  if(mt_dir == 1) {
//    digitalWrite(in1, LOW);
//    digitalWrite(in2, HIGH);
//    analogWrite(en, mt_pwm);
//  }
//  else if(mt_dir == 2) {
//    digitalWrite(in1, HIGH);
//    digitalWrite(in2, LOW);
//    analogWrite(en, mt_pwm);
//  }
//  else {
//    digitalWrite(in1, LOW);
//    digitalWrite(in2, LOW);
//    analogWrite(en, 0);
//  }
}*/
