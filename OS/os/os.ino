/*

MonciaOS
Version 0.03

CC0 2024 P1X
Made by Krzysztof Krystian Jankowski

*/

#include <stdlib.h>
#include <PS2Keyboard.h>
#include <TVout.h>
#include <TVoutfonts/font8x8.h>
#include <TVoutfonts/font4x6.h>

#include "intro2.h"
#include "p1x.h"
#include "wallpaper.h"
#include "kj.h"



PS2Keyboard keyboard;
TVout TV;



const int KBDATA = 8;
const int KBSYNC =  3;
const int WIDTH =  128;
const int HEIGH =  96;



void setup()  {
  TV.begin(PAL,WIDTH,HEIGH);
  TV.select_font(font4x6);
  TV.clear_screen();
  keyboard.begin(KBDATA, KBSYNC);
  bootimage();
  delay(1000);
  reset_os();
}

int freeRam () {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

void bootimage(){
   TV.bitmap(0,0,img_intro2);
}

void statusbar(){
  
  TV.print(0,0,"Memory: "); TV.print(2048-freeRam()); TV.print("/2048 bytes");
}

void reset_os(){
  TV.clear_screen();
  TV.select_font(font4x6);
  TV.bitmap(0,0,img_bg); 
  statusbar();
  TV.bitmap(128-16,0,img_p1x);
  TV.set_cursor(0,8);
  TV.print(">");
}


void p1x(){
  TV.clear_screen();
  TV.bitmap(128-85,0,img_kj);
  delay(1000);  
  TV.bitmap(24,24,img_p1x);
  delay(1000);
  TV.select_font(font8x8);
  TV.print(4,62,"Krzysztof");
  TV.print(4,72,"Krystian");
  TV.print(4,82,"Jankowski");
  TV.select_font(font4x6);
  delay(2000);
  reset_os();
}

void loop() {
  if (keyboard.available()) {
    char c = keyboard.read();
    if (c == PS2_ENTER) {
      TV.println();
      TV.println("Unknown command");
      TV.print(">");      
    } else if (c == PS2_ESC) {
      reset_os();
    } else if (c == PS2_LEFTARROW) {
      TV.print("[Left]");
    } else if (c == PS2_RIGHTARROW) {
      
    } else if (c == PS2_UPARROW) {
      TV.select_font(font8x8);
      TV.println();
      TV.print(">");
    } else if (c == PS2_DOWNARROW) {
      TV.select_font(font4x6);
      TV.println();
      TV.print(">");
    } else if (c == PS2_BACKSPACE) {
      p1x();
    }else{
      TV.print(c);
    }
  }
}
