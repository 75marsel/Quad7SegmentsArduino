/**
  Microprocessor MIDTERM Task
  Name: QuadSegments.h
  Type: Header file

  @author Gappi, Jeric Marcel L.
  @version 1.070423QH
*/


#ifndef QuadSegments_h
#define QuadSegments_h

#include "Arduino.h"


class QuadSegments
{
  public:
    QuadSegments(byte seg_1, byte seg_2, byte seg_3, byte seg_4, byte dp, int portd, int seg_delay);
    void display(int mins1, int mins2, int secs1, int secs2);
    int concatInt(int x, int y);
    void parseNum(int *x, int *y, int tmr_count);
    void dpBlink(unsigned long curr_time, unsigned long *prev_time, bool *dp_blink);

  private:
    byte _seg_1;
    byte _seg_2;
    byte _seg_3;
    byte _seg_4;
    byte _dp;

    int _portd;
    int _seg_delay;
};

#endif