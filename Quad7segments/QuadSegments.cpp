/**
  Microprocessor MIDTERM Task
  Name: QuadSegments.cpp
  Type: Source file

  @author Gappi, Jeric Marcel L.
  @version 1.070423QS
*/


#include "Arduino.h"
#include "QuadSegments.h"


QuadSegments::QuadSegments(byte seg_1, byte seg_2, byte seg_3, byte seg_4, byte dp, int portd, int seg_delay)
{
  pinMode(seg_1, OUTPUT); // minutes 1 (tens)
  pinMode(seg_2, OUTPUT); // minutes 2 (ones)
  pinMode(seg_3, OUTPUT); // seconds 1 (tens)
  pinMode(seg_4, OUTPUT); // seconds 2 (ones)
  pinMode(dp, OUTPUT); // s2

  // Port manipulation on PORT D
  DDRD = portd;
  
  _seg_1 = seg_1;
  _seg_2 = seg_2;
  _seg_3 = seg_3;
  _seg_4 = seg_4; 

  _dp = dp;

  _seg_delay = seg_delay;
}


// Driver for displaying Quad 7-Segment display using multiplexing.
void QuadSegments::display(int mins1, int mins2, int secs1, int secs2)
{
    digitalWrite(_seg_4, HIGH);
    PORTD = secs2;
    delay(_seg_delay);
    digitalWrite(_seg_4, LOW);
    delay(_seg_delay);
    
    digitalWrite(_seg_3, HIGH);
    PORTD = secs1;
    delay(_seg_delay);
    digitalWrite(_seg_3, LOW);
    delay(_seg_delay);

    digitalWrite(_seg_2, HIGH);
    PORTD = mins2;
    delay(_seg_delay);
    digitalWrite(_seg_2, LOW);
    delay(_seg_delay);

    digitalWrite(_seg_1, HIGH);
    PORTD = mins1;
    delay(_seg_delay);
    digitalWrite(_seg_1, LOW);
    delay(_seg_delay); 
}


// Concatenate two integers. 1 and 5 returns 15.
int QuadSegments::concatInt(int x, int y)
{
  int pow = 10;
  while(y >= pow)
  {
    pow *= 10;
  }
  return x * pow + y;        
}


// Parse two digit numbers and return Tens and Ones of the timer count.
void QuadSegments::parseNum(int *x, int *y, int tmr_count)
{
  *x = (tmr_count / 10) % 10; // first digit
  *y = (tmr_count / 1) % 10; // second digit

  return;
}


// Allows the DP to blink and act as the colon in a digital clock.
void QuadSegments::dpBlink(unsigned long curr_time, unsigned long *prev_time, bool *dp_blink)
{
  if(curr_time - *prev_time >= 1000)
  {
    if(*dp_blink == false)
    {
      *dp_blink = true;
      digitalWrite(_dp, HIGH);
    }
    else
    {
      *dp_blink = false;
      digitalWrite(_dp, LOW);
    }
  }

  *prev_time = curr_time;
}