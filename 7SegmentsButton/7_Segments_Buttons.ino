/**
  BE601A Microprocessor
  Name: Gappi_Jeric_7_Segments_Button.ino

  @author Jeric Marcel L. Gappi
  @version 1.03142023b March 14, 2023

  Two (2) 7 Segment Common Anode display connected in parallel utilizing multiplexing to display two different outputs.
  It has Four (4) buttons to control the flow of the segments in real time without interrupt pins:
    :: Start Button - pin A4 --> Start counting. if in stop state, the recent number will be continued as the start num.
    :: Stop Button - pin A3 --> Stop counting.
    :: Generate Pseudo-Random numbers (0-99) - pin A5 --> Generates a pseudo random number while in start state or stop state.
    :: Reset Button - pin A2 --> Restarts the counting to 0 even in start state or stop state.

  7 segment pins are connected to Digital pins 0 to 7.
  Common Anode's middle pin are connected to --> Digital Pin 8 (Ones), Digital Pin 9 (Tens)
*/


#define delay_time 2
#define delay_time_iter 110 //125
#define anode_ones 8
#define anode_tens 9
#define reset_pin A2
#define stop_pin A3
#define start_pin A4
#define random_pin A5


byte prev_num[2] = {0, 0}; // record the current number
byte bin[10] = {B1000000, B1111001, B0100100, B0110000, B0011001, B0010010, B0000010, B1111000, B0000000, B0011000};
bool start_flag = false; // if start is pressed and for history flag
bool break_flag = false; // to break the nested for
bool stop_flag = false; // for stop


void setup() {
  DDRD = B1111111;
  pinMode(anode_ones, OUTPUT);
  pinMode(anode_tens, OUTPUT);
  pinMode(start_pin, INPUT);
  pinMode(stop_pin, INPUT);
  pinMode(random_pin, INPUT);
  pinMode(reset_pin, INPUT);
}


void loop() {
  if(digitalRead(start_pin) || start_flag){ // if start btn was pressed or to detect if already pressed.
    start_flag = true;
    break_flag = false;
 
    for(int tenZ=prev_num[0]; tenZ<=9 && !break_flag; tenZ++){ // the for loop breaks if the break flag changes state.
      for(int ones=prev_num[1]; ones<=9 && !break_flag; ones++){
          display_driver(tenZ, ones);
      } // end inner for
    } // end outer for 
  } // end if

  while(!digitalRead(start_pin) && stop_flag){
    display_driver(prev_num[0], prev_num[1]);
  } // end while
  
} // end main loop


void display_driver(int tenZ, int ones){
    for(int time = 0; time < delay_time_iter; time++){
        digitalWrite(8, HIGH);
        PORTD = bin[ones];
        delay(delay_time);  
        digitalWrite(8, LOW);

        if(digitalRead(stop_pin) && !stop_flag){ // catch stop btn state
          stop_button(digitalRead(stop_pin), tenZ, ones);
          break;
        }
        else if(digitalRead(start_pin)){ // catch start btn state
            start_button();
            break;
        }
        
        /*if(digitalRead(random_pin)){ // catch random btn state
          random_button();
          break;
         }
        else if(digitalRead(reset_pin)){ // catch reset btn state
          reset_button();
          break;
        }*/

        delay(delay_time);
        digitalWrite(9, HIGH);
        PORTD = bin[tenZ];
        delay(delay_time);
        digitalWrite(9, LOW);
        delay(delay_time);
        }
} // end driver


void reset_button(){ // reset the counter to 0.
  break_flag = true;
  previous_nums(0, 0);
}


void random_button(){ // generates a pseudo random number --> hint: it acts the same as python's random but with the params of random(max) only.
  byte one = random(9);
  byte ten = random(9);
  previous_nums(ten, one);
}


void stop_button(int status, byte tens, byte ones){ // switches the state of flags for stop condition.
  if(status==HIGH){
    previous_nums(tens, ones);
    start_flag = false;
    stop_flag = true;
    break_flag=true;
  }
  else{
    previous_nums(0, 0);
    stop_flag = false;
  }
} // end button


void start_button(){ // switches the state of flags for start condition.
  stop_flag = false;
  start_flag = true;
}


void previous_nums(byte tens, byte ones){ // stores the current number in an array.
  prev_num[0] = tens;
  prev_num[1] = ones;
} // end previous_nums