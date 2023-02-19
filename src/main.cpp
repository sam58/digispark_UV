
//#define PSTR(s) (__extension__({static const char __c[] PROGMEM = (s); &__c[0];}))
#include <TinyI2CMaster.h>
#include <Tiny4kOLED.h>


#define ONE_STEP 3.3 / 1024 // Value in volt for one step ADC
#define TOLERANCE 10 // how many steps out of 1024 ADC will consider the same when redrawing. Made to not flicker
unsigned int height = 32;
unsigned int width = 128;

const uint8_t indicator_bitmap[]  = {
  0b11100000,
  0b11100000,
  0b00000000,0b10000000,
  0b00000000,0b10000000,
  0b00000000,0b10000000,
  0b00000000,0b10000000,
  0b00000000,0b10000000,
  0b00000000,
  0b11000000,
  0b00000000,
  0b10000000,0b00000000,
  0b10000000,0b00000000,
  0b10000000,0b00000000,
  0b10000000,0b00000000,
  0b10000000,0b00000000,
  0b11100000,
  0b11100000,
  0b00000000,0b10000000,
  0b00000000,0b10000000,
  0b00000000,0b10000000,
  0b00000000,0b10000000,
  0b00000000,0b10000000,
  0b00000000,
  0b11000000,
  0b00000000,
  0b10000000,0b00000000,
  0b10000000,0b00000000,
  0b10000000,0b00000000,
  0b10000000,0b00000000,
  0b10000000,0b00000000,
  0b11100000,
  0b11100000,
  0b00000000,0b10000000,
  0b00000000,0b10000000,
  0b00000000,0b10000000,
  0b00000000,0b10000000,
  0b00000000,0b10000000,
  0b00000000,
  0b11000000,
  0b00000000,
  0b10000000,0b00000000,
  0b10000000,0b00000000,
  0b10000000,0b00000000,
  0b10000000,0b00000000,
  0b10000000,0b00000000,
  0b11100000,
  0b11100000,
  0b00000000,0b10000000,
  0b00000000,0b10000000,
  0b00000000,0b10000000,
  0b00000000,0b10000000,
  0b00000000,0b10000000,
  0b00000000,
  0b11000000,
  0b00000000,
  0b10000000,0b00000000,
  0b10000000,0b00000000,
  0b10000000,0b00000000,
  0b10000000,0b00000000,
  0b10000000,0b00000000,
  0b11100000,
  0b11100000,
  0b00000000,0b10000000,
  0b00000000,0b10000000,
  0b00000000,0b10000000,
  0b00000000,0b10000000,
  0b00000000,0b10000000,
  0b00000000,
  0b11000000,
  0b00000000,
  0b10000000,0b00000000,
  0b10000000,0b00000000,
  0b10000000,0b00000000,
  0b10000000,0b00000000,
  0b10000000,0b00000000,
  0b11100000,
  0b11100000,
  0b00000000
};


void showIndicator(uint16_t stepsValue){
  uint8_t lengthByX = (stepsValue >310) ? ((stepsValue-310)/5.6)  : 0; //1 вольт на 0 значении датчика
  
  oled.setCursor(0, 3);
  oled.startData();
  for( uint8_t i = 0;i<128;i++){
    uint8_t dataOut = indicator_bitmap[i];
    if(i <= lengthByX){
     dataOut |= 0x06;
    }
    oled.sendData(dataOut);    
  }
  oled.endData();
};

void showValueInSteps(uint16_t stepsValue){
  oled.setFont(FONT6X8P); 
  oled.setCursor(90,0);
  oled.clearToEOL();//Clear the place for new input

  if(stepsValue<1000){
    oled.setCursor(110,0);
  }else{
    oled.setCursor(105,0);
  }
  oled.print(stepsValue);
}

void showValueInVolt(uint16_t stepsValue){
    oled.setFont(FONT6X8P); 
    oled.setCursor(0, 0);
    oled.fillLength(0x00 , 40);
    oled.setCursor(0, 0);
    oled.print(stepsValue*ONE_STEP);oled.print("v");   
}

void showCenterValue(uint16_t stepsValue){
    oled.setCursor(38, 0);
    oled.setFont(FONT8X16);
    oled.print("      ");
    float x = ONE_STEP * stepsValue;
    float uvData = 7.5 * ((x < 1 ? 1 : x) - 1);
    oled.setCursor(38, 0);
    oled.print(uvData);
}



void setup() {

 oled.begin();
 oled.on();
 oled.clear();
//  oled.setFont(FONT6X8P); 
//  oled.setCursor(0, 0);
//  oled.print("-.--v");//Value in volt
// oled.setFont(FONT8X16);
// oled.setCursor(38, 0);
// oled.print("--.--"); 
 oled.setFont(FONT6X8P);
 oled.setCursor(38, 2);
 oled.print("mW/cm^2");
 oled.setCursor(0, 3);
 oled.startData();
 for( uint8_t i = 0;i<128;i++){
  oled.sendData(indicator_bitmap[i] );
 }
  oled.endData();
}

void loop() {
  unsigned int static prevUvValue=0;
  uint16_t uvValue = analogRead(2);
  if (abs(prevUvValue - uvValue) <= TOLERANCE ){
   return;
  }
  
  prevUvValue = uvValue;
  //Read PB4 AD2
  showValueInSteps(uvValue);
  showValueInVolt(uvValue);
  showCenterValue(uvValue);   
  showIndicator(uvValue);
  delay(1000);
}
