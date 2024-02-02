/**
  Microprocessor MIDTERM Task
  Name: Button.h
  Type: Header file

  @author Gappi, Jeric Marcel L.
  @version 1.070423BH
*/


#ifndef Button_h
#define Button_h

#include "Arduino.h"

class Button
{
  public:
    Button(byte btn_pin_A, byte btn_pin_B, byte btn_pin_C, byte btn_pin_D);
    void catch_new_state();
    void catch_old_state();
    void catch_debounce_state(bool* a, bool* b, bool* c, bool* d);

    byte getNewState_A(){ return _btn_new_state_A; }
    byte getNewState_B(){ return _btn_new_state_B; }
    byte getNewState_C(){ return _btn_new_state_C; }
    byte getNewState_D(){ return _btn_new_state_D; }

    byte getOldState_A(){ return _btn_old_state_A; }
    byte getOldState_B(){ return _btn_old_state_B; }
    byte getOldState_C(){ return _btn_old_state_C; }
    byte getOldState_D(){ return _btn_old_state_D; }
  private:
    byte _btn_pin_A;
    byte _btn_pin_B;
    byte _btn_pin_C;
    byte _btn_pin_D;

    byte _btn_new_state_A;
    byte _btn_new_state_B;
    byte _btn_new_state_C;
    byte _btn_new_state_D;

    byte _btn_old_state_A;
    byte _btn_old_state_B;
    byte _btn_old_state_C;
    byte _btn_old_state_D;
};

#endif