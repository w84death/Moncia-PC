/*

MonciaOS
Version 0.2

CC0 2024 P1X
Made by Krzysztof Krystian Jankowski


*/

#include <PS2Keyboard.h>
#include <TVout.h>
#include <TVoutfonts/font8x8.h>
PS2Keyboard keyboard;
TVout TV;

const int KBDATA = 8;
const int KBSYNC =  3;
const int WIDTH =  128;
const int HEIGH =  96;
void setup()  {
  TV.begin(PAL,WIDTH,HEIGH);
  TV.select_font(font8x8);
  TV.clear_screen();
  keyboard.begin(KBDATA, KBSYNC);
  
  intro();
  reset_os();
}

void intro() {
  TV.draw_rect(0,0,TV.hres()-1,TV.vres()-1,WHITE);
  TV.print(32,32,"MonciaOS");
  TV.print(8,72,"v0.01 @128x96");  
  TV.print(8,82,"CC0 2024 P1X");
  delay(3000);
}

void reset_os(){
  TV.clear_screen();
  TV.set_cursor(0,0);
  TV.print(">");
}

void loop() {
  if (keyboard.available()) {
    char c = keyboard.read();
    if (c == PS2_ENTER) {
      TV.println();
    } else if (c == PS2_ESC) {
      reset_os();
    } else if (c == PS2_LEFTARROW) {
      TV.print("[Left]");
    } else if (c == PS2_RIGHTARROW) {
      TV.print("[Right]");
    } else if (c == PS2_UPARROW) {
      TV.print("[Up]");
    } else if (c == PS2_DOWNARROW) {
      TV.print("[Down]");
    }else{
      TV.print(c);
    }
  }
}
