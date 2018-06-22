#include <EEPROM.h>

#define dbg false

const double MAX_OUTV = 3.39f;

const double CONTINOUSG= 2.85f;
const double CONTINOUSR = 3.24f;

double table_bankG(int ch){
  switch(ch){
    case 1: return 2.33f;
    case 2: return 1.95f;
    case 3: return 1.32f;
    case 4: return 0.82f;
  }
}

double table_bankR(int ch){
  switch(ch){
    case 1: return 2.58f;
    case 2: return 2.12f;
    case 3: return 1.40f;
    case 4: return 0.85f;
  }
}

double table(boolean bank, int ch){
  if(bank)
    return table_bankG(ch);
  else
     return table_bankR(ch);
}

unsigned long T0 = 0 ;
unsigned long T1 = 0 ;
unsigned long T2 = 0;
volatile bool newMinus = false;
volatile bool newPlus = false;

bool first = true;

int bankPin = 0;
int channelPin = 1;

int led1 = 11;
int led2 = 10;
int led3 = 9;
int led4 = 6;

int OUT = 5;

char s[120];
boolean bank = true;
int ch = 1;
double v;

int chEEPROM = 1;
boolean bankEEPROM = true;

double continous = CONTINOUSG;

void setup() {

  if(dbg){
    Serial.begin(115200);  
    char s[120];
    sprintf(s,"debug active");
    Serial.println(s);
  }

  ch = EEPROM.read(0);
  bank = EEPROM.read(1);
  chEEPROM = ch;
  bankEEPROM = bank;
  
  //analogWriteFreq(5000);

  //TCCR3B = (TCCR3B & 0xF8) | 0x02;
  TCCR0B = TCCR0B & B11111000 | B00000010;    // set timer 0 divisor to     8 for PWM frequency of  7812.50 Hz
  pinMode(OUT, OUTPUT);

  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
  pinMode(led3,OUTPUT);
  pinMode(led4,OUTPUT);
  
  attachInterrupt(bankPin,bankINT,CHANGE);
  attachInterrupt(channelPin,channelINT,CHANGE);

  T2 = millis();

  digitalWrite(led1, HIGH);
  delay(700);
  //digitalWrite(led1, LOW);
  digitalWrite(led2, HIGH);
  delay(700);
  //digitalWrite(led2, LOW);
  digitalWrite(led3, HIGH);
  delay(700);
  //digitalWrite(led3, LOW);
  digitalWrite(led4, HIGH);
  delay(2000);
  digitalWrite(led4, LOW);
  //digitalWrite(led3, HIGH);
  delay(700);
  digitalWrite(led3, LOW);
  //digitalWrite(led2, HIGH);
  delay(700);
  digitalWrite(led2, LOW);
  //digitalWrite(led1, HIGH);

  v = table(bank,ch) ;

  int out = v*255/MAX_OUTV;
  out = constrain(out, 0, 255);
  analogWrite(OUT, out);
  
  delay(2000);
  digitalWrite(led1, LOW);

  /*v = table(bank,ch) ;

  int out = v*255/MAX_OUTV;
  out = constrain(out, 0, 255);
  analogWrite(OUT, out);

  delay(2000);*/
  
}

void loop() {  
  if(newPlus){
    T1 = millis();
    ch++;
    newPlus = false;

    if(ch>4){
      bank = !bank;
      ch = 1;
      T2 = millis();
    }
    
    v = table(bank,ch) ;
  }
  
  else if(newMinus){
    T1 = millis();
    ch--;
    newMinus = false;

    if(ch<1){
      bank = !bank;
      ch = 4;
      T2 = millis();
    }
      
    v = table(bank,ch) ;
  }else if( T1+500 > millis()){
    
  }else {
    v = bank ? CONTINOUSG : CONTINOUSR;
  }

  if (readVcc()<3100){
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    digitalWrite(led4, LOW);

    if (chEEPROM != ch or bankEEPROM != bank){
      EEPROM.write(0, ch);
      EEPROM.write(1, bank);

      chEEPROM = ch;
      bankEEPROM = bank;
    }
    


  }

  if (bank) {
    switch(ch){
      case 1:
        digitalWrite(led1, HIGH);
        digitalWrite(led2, LOW);
        digitalWrite(led3, LOW);
        digitalWrite(led4, LOW);
  
        break;
      case 2:
        digitalWrite(led2, HIGH);
        digitalWrite(led1, LOW);
        digitalWrite(led3, LOW);
        digitalWrite(led4, LOW);
        break;
      case 3:
        digitalWrite(led3, HIGH);
        digitalWrite(led1, LOW);
        digitalWrite(led2, LOW);
        digitalWrite(led4, LOW);
        break;
      case 4:
        digitalWrite(led4, HIGH);
        digitalWrite(led1, LOW);
        digitalWrite(led2, LOW);
        digitalWrite(led3, LOW);
        break;

    }
  }
  else {
    if ((T2 + 1000) > millis()){
      digitalWrite(led1, LOW);
      digitalWrite(led2, LOW);
      digitalWrite(led3, LOW);
      digitalWrite(led4, LOW);
    }
    else if ((T2 + 2000) > millis()){
      switch(ch){
        case 1:
          digitalWrite(led1, HIGH);
          digitalWrite(led2, LOW);
          digitalWrite(led3, LOW);
          digitalWrite(led4, LOW);
    
          break;
        case 2:
          digitalWrite(led2, HIGH);
          digitalWrite(led1, LOW);
          digitalWrite(led3, LOW);
          digitalWrite(led4, LOW);
          break;
        case 3:
          digitalWrite(led3, HIGH);
          digitalWrite(led1, LOW);
          digitalWrite(led2, LOW);
          digitalWrite(led4, LOW);
          break;
        case 4:
          digitalWrite(led4, HIGH);
          digitalWrite(led1, LOW);
          digitalWrite(led2, LOW);
          digitalWrite(led3, LOW);
          break;

      }
    }
    else {
      T2 = millis();
    }
  }
 
    
  int out = v*255/MAX_OUTV;
  out = constrain(out, 0, 255);  
  //out = 255;
  analogWrite(OUT, out);
  if(dbg){
    sprintf(s,"bank %d ",bank);
    Serial.print(s);
    sprintf(s,"ch %d ",ch);
    Serial.print(s);
    sprintf(s,"v=%d out=%d",(int)(v*100),out);
    Serial.println(s);
  }
}

void bankINT(){
  if ( millis() > T0  + 200)
  {   
    newPlus = true;
    T0 = millis();
    //Serial.println("Plus button");
  } 
}

void channelINT(){
  if ( millis() > T0  + 200)
  {   
    newMinus = true;
    T0 = millis();
    //Serial.println("Minus button");
  } 
}

long readVcc() {
  // Read 1.1V reference against AVcc
  // set the reference to Vcc and the measurement to the internal 1.1V reference
  #if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
    ADMUX = _BV(MUX5) | _BV(MUX0);
  #elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
    ADMUX = _BV(MUX3) | _BV(MUX2);
  #else
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #endif  

  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Start conversion
  while (bit_is_set(ADCSRA,ADSC)); // measuring

  uint8_t low  = ADCL; // must read ADCL first - it then locks ADCH  
  uint8_t high = ADCH; // unlocks both

  long result = (high<<8) | low;

  result = 1125300L / result; // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
  return result; // Vcc in millivolts
}
