/**
  Microprocessor MIDTERM Task
  Name: Quad_SW_TMR_Gappi_vLH.ino

  @author Gappi, Jeric Marcel L.
  @version Lily / Haewon

  Quad 7-Segment Display program for Microprocessor Midterm Performance Task.

  This Arduino Project has the following features.
  :: Responsive Four tact push buttons. [A] [B] [C] [D]
    >>  [A] Switch from Stopwatch to Timer. (default = Stopwatch)
    >>  [B] Increment timer countdown by 5. (from 00:00 to 59:59)
    >>  [C] Reset/Clear button from the current segment mode.
    >>  [D] Start/Pause the program.
        ~~ Buttons A-C will not work unless the segment is currently paused and is on the proper segment mode.
  :: LED Indicator for switching modes and Run/Stop.
*/

#include "Button.h"
#include "QuadSegments.h"


#define BUTTON_PIN_A A0
#define BUTTON_PIN_B A1
#define BUTTON_PIN_C A3
#define BUTTON_PIN_D A2

#define LED_TMR_PIN A4
#define LED_SW_PIN A5

#define PORT B1111111
#define LED_PIN 13
#define DOT 12
#define SEG_MINS1 11
#define SEG_MINS2 10
#define SEG_SECS1 9
#define SEG_SECS2 8




// Array for designating which Digital pins are HIGH on PORTD. 0 = HIGH, 1 = LOW.
int INTEGER_PORTD[10] = {B1000000, B1110011, B0100100, B0100010, B0010011, 
                        B0001010, B0001000, B1100011, B0000000, B0000011};

bool start_flag = false;
bool dp_blink = false;

int prev_sw_num[4] = {0, 0, 0, 0}; // Serves as the keeper and recorder for stopwatch values.
int prev_timer_num[4] = {0, 0, 0, 0}; // Serves as the keeper and recorder for timer values.
int tmr_count = 0;
              
int DELAY_SEGMENT = 2;
int SEGMENT_FRAMES = 54; // Constant value for how many times the display will loop. (52 or 54)
int DELAY_BUTTON = 42;

byte seg_mode = 0; // 0 sw // 1 timer

byte led_state = 0;
byte increment = 5;

Button pushButton(BUTTON_PIN_A, BUTTON_PIN_B, BUTTON_PIN_C, BUTTON_PIN_D); // Creates Pushbutton class.
QuadSegments QSeg(SEG_MINS1, SEG_MINS2, SEG_SECS1, SEG_SECS2, DOT, PORT, DELAY_SEGMENT); // Creates Quad 7Seg class.


void setup()
{
  pinMode(LED_PIN, OUTPUT);
  pinMode(LED_SW_PIN, OUTPUT);
  pinMode(LED_TMR_PIN, OUTPUT);
}


void loop()
{
  // If BUTTON D is pressed once, the program will start to enter the current segment mode.
  if( led_state == 1 || start_flag )
  {
    if(seg_mode == 0)
      display_sw_driver();
    else
      display_timer_driver();

   start_flag = true;
  }

  // If BUTTON D is pressed and the LED is LOW, the program will loop the current values of the current segment mode.
  while(led_state == 0)
  {
    digitalWrite(12, LOW);
    if(seg_mode == 0)
    {
      digitalWrite(LED_TMR_PIN, LOW);
      digitalWrite(LED_SW_PIN, HIGH); 
      seg_display_driver(prev_sw_num[0], prev_sw_num[1], prev_sw_num[2], prev_sw_num[3]);
    }
    else
    {
      digitalWrite(LED_TMR_PIN, HIGH);
      digitalWrite(LED_SW_PIN, LOW);
      seg_display_driver(prev_timer_num[0], prev_timer_num[1], prev_timer_num[2], prev_timer_num[3]);
    } 
  } // end while
} // end loop


// Display driver for Quad 7-Segment STOPWATCH Display that can catch button inputs.
void display_sw_driver()
{
  for(int mins1=prev_sw_num[0]; mins1<=5 && led_state == 1; mins1++)
  {
    for(int mins2=prev_sw_num[1]; mins2<=9 && led_state == 1; mins2++)
    {
      for(int secs1=prev_sw_num[2]; secs1<=5 && led_state == 1; secs1++)
      {
        for(int secs2=prev_sw_num[3]; secs2<=9 && led_state == 1; secs2++)
        {
          store_sw_nums(0, 0, 0, 0);
          seg_display_driver(mins1, mins2, secs1, secs2);   
        } // for secs2 
      } // for secs1
    } // for mins2
  } // for mins1
  if(prev_sw_num[0] == 0 && prev_sw_num[1] == 0 && prev_sw_num[2] == 0 && prev_sw_num[3] == 0)
  {
    start_flag = false;
    led_state = 0;
    blink_animation();
    store_sw_nums(0, 0, 0, 0);
  }
}


// Display driver for Quad 7-Segment TIMER Display that can catch button inputs.
void display_timer_driver()
{
  int i = 0;
  int j = 0;
  
  for(int mins1 = prev_timer_num[0], c = 0; mins1 >= 0 && led_state == 1; mins1--, c++)
  {
    if( c != 0){
      prev_timer_num[1] = 9; 
    }  

    for(int mins2 = prev_timer_num[1]; mins2 >= 0 && led_state == 1; mins2--, i++) // b
    {
      if((mins2 - i) < prev_timer_num[1])
      {
        prev_timer_num[2] = 5; 
      }
      
      for(int secs1 = prev_timer_num[2]; secs1 >= 0 && led_state == 1; secs1--, j++) // c
      {
        if(secs1 - j < prev_timer_num[2])
        {
          prev_timer_num[3] = 9;          
        }
          
        for(int secs2 = prev_timer_num[3]; secs2 >= 0 && led_state == 1; secs2--)
        {
          seg_display_driver(mins1, mins2, secs1, secs2);
          if(mins1 + mins2 + secs1 + secs2 == 0)
          {
            led_state = 0;
            blink_animation();
            store_timer_nums(0, 0, 0, 0);
          }
        }
      }
    }  
  }
}


// Display driver for Quad 7-Segment Display that can catch button inputs.
void seg_display_driver(int mins1, int mins2, int secs1, int secs2)
{
  unsigned long prev_time = 0;
  unsigned long curr_time = millis();

  for(int time = 0; time < SEGMENT_FRAMES; time++)
  {
    QSeg.display(INTEGER_PORTD[mins1], INTEGER_PORTD[mins2], INTEGER_PORTD[secs1], INTEGER_PORTD[secs2]);
    button_logic(mins1, mins2, secs1, secs2);
    QSeg.dpBlink(curr_time, &prev_time, &dp_blink);
  } // for time
}


// Delays the Segment display by 500ms for 3 times.
void blink_animation()
{
  SEGMENT_FRAMES = 8;
  for(int i = 0; i <= 2; i++)
  {
    seg_display_driver(0, 0, 0, 0);
    delay(500);
  }
  SEGMENT_FRAMES = 54;
}


// Button logic code utilizing Button Library.
void button_logic(int mins1, int mins2, int secs1, int secs2)
{
  pushButton.catch_new_state();

  bool a, b, c, d;

  pushButton.catch_debounce_state(&a, &b, &c, &d);

  if(a || b || c || d)
  {
    if(seg_mode == 0 && pushButton.getNewState_A() == 1 && led_state == 0) // switch mode to timer [BUTTON A]
    {
      seg_mode = 1;
      digitalWrite(LED_TMR_PIN, HIGH);
      digitalWrite(LED_SW_PIN, LOW);
    }
    else if(seg_mode == 1 && pushButton.getNewState_A() == 1 && led_state == 0) // switch mode to stopwatch [BUTTON A]
    {
      seg_mode = 0;
      digitalWrite(LED_TMR_PIN, LOW);
      digitalWrite(LED_SW_PIN, HIGH);
    }
    
    if(seg_mode == 1 && pushButton.getNewState_B() == 1 && led_state == 0) // [BUTTON B]
    {
      // this concatenates the current value in the 7-segments (Minutes)
      // [1][5] to 15.
      int curr_num = QSeg.concatInt(prev_timer_num[0], prev_timer_num[1]);

      /* as the timer counter incremented to 60, it is automatically changed to 59.
      any further additions to the timer count, it will loop around 64. this if statement
      fix that infinite loop addition by setting the curr_num to 64 if the current total
      increment is greater than 64. 59 + 5 = 64.*/
      if(tmr_count >= 64)
      {
        curr_num = 64;
      }

      tmr_count = curr_num + increment; // iterates the current value for minutes whenever the button B is pressed.
      
      if(tmr_count >= 69)
      {
        tmr_count = 0;
        store_timer_nums(0, 0, 0, 0);
      }
      else if(tmr_count >= 64)
      {
        prev_timer_num[2] = 5;
        prev_timer_num[3] = 9;
      }
      else if(tmr_count >= 60 && tmr_count < 64)
      {
        prev_timer_num[0] = 5;
        prev_timer_num[1] = 9;
      }
        
      if(tmr_count >= 10 )
      {
        int x, y;

        QSeg.parseNum(&x, &y, tmr_count);

        if(tmr_count < 60)
        {
          prev_timer_num[0] = x;
          prev_timer_num[1] = y;          
        }
        
      } // end else if

      if(tmr_count < 10)
      {
        prev_timer_num[1] = tmr_count;
      }
    } // end b

    if(led_state == 0 && pushButton.getNewState_C() == 1) // reset [BUTTON C]
    {
      if(seg_mode == 0)
      {
        store_sw_nums(0, 0, 0, 0);
      }
      else{
        store_timer_nums(0, 0, 0, 0);
      }
      
      tmr_count = 0;
    }
    
    if(led_state == 0 && pushButton.getNewState_D() == 1) // start [BUTTON D]
    {
      if(seg_mode == 1)
      {
        if(prev_timer_num[0] + prev_timer_num[1] + prev_timer_num[2] + prev_timer_num[3] != 0)
        {
          start_button();
        }
      }
      else
      {
        start_button();
      }
      delay(50);
    }

    else if(led_state == 1 && pushButton.getNewState_D() == 1) // stop [BUTTON D]
    {
      stop_button(mins1, mins2, secs1, secs2);
      delay(50);
    }
  } // end check btn

  pushButton.catch_old_state();
}


// Switches the current state of the LED STATE and ALSO switches the start flag.
void start_button()
{
  digitalWrite(LED_PIN,HIGH);
  start_flag = true;

  led_state = 1;
}


// Switches the current state of the LED STATE and STORES the current value based on current mode.
void stop_button(int mins1, int mins2, int secs1, int secs2)
{
  digitalWrite(LED_PIN, LOW);
  led_state=0;

  start_flag = false;

  if(seg_mode == 0)
  {
    store_sw_nums(mins1, mins2, secs1, secs2);
  }
  else
  {
    store_timer_nums(mins1, mins2, secs1, secs2);
  }
}


// Stores the current number of stopwatch in an array.
void store_sw_nums(int mins1, int mins2, int secs1, int secs2)
{
  prev_sw_num[0] = mins1;
  prev_sw_num[1] = mins2;
  prev_sw_num[2] = secs1;
  prev_sw_num[3] = secs2;
} // end store_sw_nums


// Stores the current number of timer in an array.
void store_timer_nums(int mins1, int mins2, int secs1, int secs2)
{
  prev_timer_num[0] = mins1;
  prev_timer_num[1] = mins2;
  prev_timer_num[2] = secs1;
  prev_timer_num[3] = secs2;
} // end store_sw_nums