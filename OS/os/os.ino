/*

MonciaOS
Version 0.2

CC0 2024 P1X
Made by Krzysztof Krystian Jankowski


*/

#include <PS2Keyboard.h>
#include <TVout.h>
#include <TVoutfonts/font8x8.h>

#include "intro.h"
#include "intro2.h"
#include "p1x.h"
#include "wallpaper.h"

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
  /*TV.draw_rect(0,0,TV.hres()-1,TV.vres()-1,WHITE);
  TV.print(32,32,"MonciaOS");
  TV.print(8,72,"v0.01 @128x96");  
  TV.print(8,82,"CC0 2024 P1X");
  */
  TV.bitmap(0,0,img_intro);
  delay(3000);
  TV.bitmap(0,0,img_intro2);
  delay(3000);
  TV.bitmap(0,0,img_bg);
  TV.bitmap(64-8,48-11,img_p1x);
  delay(3000);
}

void reset_os(){
  TV.clear_screen();
  TV.bitmap(0,0,img_bg); 
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
