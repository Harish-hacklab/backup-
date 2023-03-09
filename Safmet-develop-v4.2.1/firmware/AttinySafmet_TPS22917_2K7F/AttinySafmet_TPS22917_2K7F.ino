#include <avr/io.h>
#include <util/delay.h>
#include <avr/sleep.h>                    
#include <avr/power.h>    
#include <avr/interrupt.h>
#include <avr/wdt.h>

#define LS_EN 3
#define ATMEGA_GPIO 4

volatile int count = 0;
bool ResetFlag = true; //Reset Flag 

//ISR function
/*
void ResetFlag_Func()
{
  count = 0;//Reinit count to 0
  ResetFlag = false; //Set the sleep flag
}
*/

void TimerInit()
{
  //gloabal enable interrupts
  sei();

  //Timer Registers
  //Setting TCCR0A register
  TCCR0A = (1 << WGM01) | (1 << WGM00); //Fast PWM mode with top as 0xFF

  //Setting TCCR0B register
  TCCR0B = (0  << WGM02) | (1 << CS02) | (0 << CS01) | (1 << CS00); //Clock and waveform select - clk/1024 prescalar

  //Setting TIMSK0 register
  TIMSK0 = (1 << TOIE0); //Timer overflow interrupt enable

  //Initializing the timer register
  TCNT0 = 0x00; //set to 0 (max value is 255) 

  //ADC Registers - Disabling the ADC
  //ADCSRA Resgister
  ADCSRA &= ~(1<<ADEN); //write ADEN to 0, hence turning off ADC

  //Disabling Analog Comparator 
  //ACSR register
  ACSR &= ~(1<<ACIE); //disabling the interrupt enable bit
  ACSR |= (1<<ACD); //Switch off the power to ADC 

  //Disabling Clock to ADC using Power Reduction Register
  PRR = (1<<PRADC); //shut down ADC
  
  //Init count value 
  count = 0;

  
}

// the setup function runs once when you press reset or power the board
void setup() 
{
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LS_EN, OUTPUT);
  pinMode(ATMEGA_GPIO, INPUT);
  //attachInterrupt(digitalPinToInterrupt(ATMEGA_GPIO), ResetFlag_Func, RISING); //Attach interrupt to ATMEGA_GPIO PIN

  //Pin change interrupt on ATMEGA_GPIO
  GIMSK |= (1<<PCIE); //Pin change interrupt enable
  PCMSK = (1<<PCINT4); //enabling pin change interrupt on PB4 I/O pin - Atmega GPIO and disabling pin change interrupt on all other pins
  
  TimerInit(); //Start the 10 second timer
  wdt_disable(); //disable watch dog timer - test
}

void loop() 
{
  //Pull LS_EN to low 
  digitalWrite(LS_EN, LOW);
  //int count = 0;

  //Sleep Init
  set_sleep_mode(SLEEP_MODE_IDLE); //set sleep mode
  cli(); //clear Interrupts
  sleep_enable(); //Enable Sleep
  sleep_bod_disable(); //Brown Out Detector Disabled 
  sei(); //set enable interrupts
  sleep_cpu(); //Put the ATtiny in Sleep 

  /* --------------------------------------------------------------------------------
  Code continues to execute from here after execution of ISR function after interrupt
  Executing begins from here after receiving pulse from atmega or after ISR of timer
  -------------------------------------------------------------------------------- */

  if(!ResetFlag) //If reset flag is false
  {
    //IDLE
  }
  else //reset flag is true - reset the system
  {
    digitalWrite(LS_EN, HIGH);
    _delay_ms(1000); //1 second delay
    digitalWrite(LS_EN, LOW);
    TimerInit(); //reinit timer
    ResetFlag = false; //set reset flag to false
  }
  
  sleep_disable(); //Sleep Disable 

  /*
  //Resetting the system
  if (!SleepFlag) 
  {
    digitalWrite(LS_EN, HIGH);
    delay(1000);
    digitalWrite(LS_EN, LOW);
    while (!digitalRead(ATMEGA_GPIO)) //if there is no signal from Atmega, then every 10 seconds ATtiny resets the system
    { 
      count++;
      delay(100);
      if (count > 100) 
      {
        count = 0;
        digitalWrite(LS_EN, HIGH);
        delay(1000);
        digitalWrite(LS_EN, LOW);
      }
    }
    delay(1000);
    SleepFlag = true; //Set sleep flag after resetting is completed - go to sleep
  }
  */
}

//Timer overflow ISR
ISR(TIM0_OVF_vect) //Executes every 217.6ms
{
  if(count==46) //Total of 10 seconds
  {
    ResetFlag = true; //Set reset flag to true
    count = 0; //re initialize count
  }
  count++;
}

//Pin change interrupt ISR
ISR(PCINT0_vect)
{
  if(PINB & (1<<PINB4)) //if pinb4 is digital high (to detect a change from 0 to 1) - ATMEGA GPIO
  {
    count = 0;//Reinit count to 0
    ResetFlag = false; //Set the sleep flag
  }
}
