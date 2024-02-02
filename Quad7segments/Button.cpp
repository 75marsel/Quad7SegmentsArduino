/**
  Microprocessor MIDTERM Task
  Name: Button.cpp
  Type: Source file

  @author Gappi, Jeric Marcel L.
  @version 1.070423BS

  A module created for catching button states.
*/


#include "Arduino.h"
#include "Button.h"


// Creates a constructor class for Button.
Button::Button(byte btn_pin_A, byte btn_pin_B, byte btn_pin_C, byte btn_pin_D)
{
  pinMode(btn_pin_A, INPUT);
  pinMode(btn_pin_B, INPUT);
  pinMode(btn_pin_C, INPUT);
  pinMode(btn_pin_D, INPUT);

  _btn_pin_A = btn_pin_A;
  _btn_pin_B = btn_pin_B;
  _btn_pin_C = btn_pin_C;
  _btn_pin_D = btn_pin_D;

  _btn_new_state_A = 0;
  _btn_new_state_B = 0;
  _btn_new_state_C = 0;
  _btn_new_state_D = 0;

  _btn_old_state_A = 1;
  _btn_old_state_B = 1;
  _btn_old_state_C = 1;
  _btn_old_state_D = 1;
}


// Catches the current state of the buttons.
void Button::catch_new_state()
{
  _btn_new_state_A = digitalRead(_btn_pin_A);
  _btn_new_state_B = digitalRead(_btn_pin_B);
  _btn_new_state_C = digitalRead(_btn_pin_C);
  _btn_new_state_D = digitalRead(_btn_pin_D);
}


// Catches the debounce state of the buttons.
void Button::catch_debounce_state(bool *a, bool *b, bool *c, bool *d)
{
  *a = _btn_old_state_A == 0 && _btn_new_state_A == 1;
  *b = _btn_old_state_B == 0 && _btn_new_state_B == 1;
  *c = _btn_old_state_C == 0 && _btn_new_state_C == 1;
  *d = _btn_old_state_D == 0 && _btn_new_state_D == 1;

  return;
}


// Stores the current state of the buttons as old state.
void Button::catch_old_state()
{
  _btn_old_state_A = _btn_new_state_A;
  _btn_old_state_B = _btn_new_state_B;
  _btn_old_state_C = _btn_new_state_C;
  _btn_old_state_D = _btn_new_state_D;
}