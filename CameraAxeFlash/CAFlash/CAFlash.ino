// CaFlash
// Compiled with Arduino Software 1.8.5 (http://arduino.cc) plus the attiny module
// This site explains how to setup tiny: http://highlowtech.org/?p=1695

// Here are the steps to loading the firmware:
//   Set Tools->Board to: ATtiny24/44/84
//   Set Tools->Processor to: ATtiny44
//   Set Tools->Clock to: Internal 16 MHz
//     (may need to add this to Arduino15\packages\attiny\hardware\avr\1.0.2\boards.txt)
//   Set Tools->Programmer to: USBtinyISP
//   Tools->Burn Bootloader - This sets some fuses so the chip runs at 16 Mhz.
//   Upload the program

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// REVISIONS:
// 1.00 January 16, 2018 - Maurice Ribble
//      Initial version
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ADC maps 0->5V with values from 0 to 255
#define MAX_CAP_VOLTAGE 20        // v = 20V*20K/(1M+20K)*255/5
#define MIN_INDUCTOR_CURRENT 6    // v = .1A*1*255/5
#define MAX_INDUCTOR_CURRENT 102  // v = 2A*1*255/5

// Digital/analog pins
#define PIN_BOOST           2   // PORT_B - Output. Controls current flow to boost inductor
#define PIN_GREEN           1   // PORT_B - Output. Controls green indicator led  
#define PIN_RED             6   // PORT_A - Output. Controls red indicator led
#define PIN_LED             0   // PORT_B - Output. Controls pulse to big led
#define PIN_TIP             3   // PORT_A - Digital input. Reads tip of 3.5mm jack input
#define PIN_RING            2   // PORT_A - Digital input. Reads ring of 3.5mm jack input
#define APIN_CURRENT        7   // PORT_A - Analog input. Reads current of 12V power supply (also current into inductor)
#define APIN_TEMP           1   // PORT_A - Analog input.  Reads thermistor sensing temp of big led
#define APIN_VOLTAGE        0   // PORT_A - Analog input.  Reads voltage of the big capacitor

// Fast digital read macros
#define READ_TIP()   bitRead(PINA, PIN_TIP)
#define READ_RING()  bitRead(PINA, PIN_RING)

// Fast digital write macros
#define SET_BOOST()   bitSet(PORTB, 1<<PIN_BOOST)
#define CLR_BOOST()   bitClear(PORTB, 1<<PIN_BOOST)
#define SET_GREEN()   bitSet(PORTB, 1<<PIN_GREEN)
#define CLR_GREEN()   bitClear(PORTB, 1<<PIN_GREEN)
#define SET_RED()     bitSet(PORTA, 1<<PIN_RED)
#define CLR_RED()     bitClear(PORTA, 1<<PIN_RED)
#define SET_LED()     bitSet(PORTB, 1<<PIN_LED)
#define CLR_LED()     bitClear(PORTB, 1<<PIN_LED)

// Fast Analog read macros
#define READ_ADC(v) ({\
  do{\
    ADMUX = v; /* This is deceptively simple.  See setAdcParams() to see all the bitfields*/\
    bitSet(ADCSRA, ADSC);\
    while( bitRead(ADCSRA, ADSC) == 1) {};\
  }while(0);\
ADCH;})

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// setAdcParams - Sets the ADC parameters
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setAdcParams()
{
 // Setup ADC parameters.
 // Manually triggering the adc allows conversions to run in the background once initiated.
 // In manual mode ADCH must be read to allow data from the next conversion to load into the result registers.
 // Note: Performing an Arduino analogRead() may change some of these parameters so don't do that.

  // ADCSRA , ADC control & status reg A:
  // ADC clock divisor
  // ADPS2  ADPS1  ADPS0  Div Factor
  // 0      0      0      2
  // 0      0      1      2          // Fastest
  // 0      1      0      4
  // 0      1      1      8
  // 1      0      0      16
  // 1      0      1      32
  // 1      1      0      64
  // 1      1      1      128        // Slowest (default)
  // Default set by wiring is 128 (16 MHz/128 = 128 Khz)
  // Since a conversion takes 13 adc clocks the sample rate at 128Khz/13 = 9.8Khz or about 99 us
  // With a 16mhz system clock the div factor will be set to 16, (adc clock = 16 mhz/16 = 1 mhz).
  // 1mhz is at the highest suggested speed of the adc clock when full resolution is not required.
  // At a 1mhz clock the 13 required cycles will give an adc sample rate of 1 mhz/13 = 76.9 khz (13 uS).
  ADCSRA &= ~(1<<ADPS0);   // 0
  ADCSRA &= ~(1<<ADPS1);   // 0
  ADCSRA |=  (1<<ADPS2);   // 1
  ADCSRA &= ~(1<<ADIE) ;   // 0, ADC Interrupt Enable
  //ADCSRA &= ~(1<<ADIF);  // 0, ADC Interrupt Flag, since IE is not enabled no need to this setup
  ADCSRA &=  ~(1<<ADATE);  // 0, ADC Auto Trigger Enable
  //ADCSRA &= ~(1<<ADSC);  // 0, ADC Start Conversion, no need to setup here
  ADCSRA |=  (1<<ADEN)  ;  // 1, ADC Enable
   
  //ADMUX, ADC Mux Seletion Register
  ADMUX |=  (1<<MUX0);   // 1, adc Mux channel (ADMUX bits MUX[3-0], for ADC3(PB3) single ended need "B0011")
  ADMUX |=  (1<<MUX1);   // 1
  ADMUX &= ~(1<<MUX2);   // 0
  ADMUX &= ~(1<<MUX3);   // 0
  
  ADMUX &= ~(1<<REFS0);  // 0, adc reference (ADMUX bits REF[1-0], for Vcc need "B00")
  ADMUX &= ~(1<<REFS1);  // 0
  
  ADCSRB |=  (1<<ADLAR);  // 1, Set the upper 8 bits are found in ADCH, so reading ADCH satisfies update requirement

//ADCSRB, ADC control & status reg B:  ADCSRB default values are good..
}

void pinModePortA(uint8_t pin, uint8_t mode) {
  if (mode == INPUT) {
    bitClear(DDRA, pin);
  }
  else if (mode == OUTPUT) {
    bitSet(DDRA, pin);
  }
  else if (mode == INPUT_PULLUP) {
    bitClear(DDRA, pin);
    bitSet(PORTA, pin);
  }
}

void pinModePortB(uint8_t pin, uint8_t mode) {
  if (mode == INPUT) {
    bitClear(DDRB, pin);
  }
  else if (mode == OUTPUT) {
    bitSet(DDRB, pin);
  }
  else if (mode == INPUT_PULLUP) {
    bitClear(DDRB, pin);
    bitSet(PORTB, pin);
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// setup - Main program init (this is just how Arduino code works)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup()
{
  setAdcParams();
  
  pinModePortB(PIN_BOOST, OUTPUT);
  pinModePortB(PIN_GREEN, OUTPUT);
  pinModePortA(PIN_RED, OUTPUT);
  pinModePortB(PIN_LED, OUTPUT);
  pinModePortA(PIN_TIP, INPUT);
  pinModePortA(PIN_RING, INPUT);
  pinModePortA(APIN_CURRENT, INPUT);
  pinModePortA(APIN_TEMP, INPUT);
  pinModePortA(APIN_VOLTAGE, INPUT);
  
  CLR_BOOST();
  CLR_GREEN();
  SET_RED();
  CLR_LED();

  delay(100);    // Let things settle
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// loop - Main program loop (this is just how Arduino code works)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop()
{
  while(1) {
    uint8_t voltage = READ_ADC(APIN_VOLTAGE);
  
    if (voltage < MAX_CAP_VOLTAGE) {
      uint8_t current = READ_ADC(APIN_CURRENT);
      if (current < MIN_INDUCTOR_CURRENT) {
        SET_BOOST();
      }
      else if (current > MAX_INDUCTOR_CURRENT) {
        CLR_BOOST();
      }
      CLR_GREEN();
      SET_RED();
    } 
    else {
      CLR_BOOST();
      SET_GREEN();
      CLR_RED();
    }
    
    if (READ_TIP() == LOW) {
      SET_LED();
      __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\tnop\n\t""nop\n\t""nop\n\t""nop\n\t"); // 0.5 us
      __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\tnop\n\t""nop\n\t""nop\n\t""nop\n\t"); // 0.5 us
      CLR_LED();
      delay(1000);
    }
  }
}