/*
------------------------------------
MonciaOS
Version 0.04 - 2024-03-09

CC0 2024 P1X
Made by Krzysztof Krystian Jankowski

------------------------------------

Quick log:
- [done] protoboard
- [done] composite tv out
- [done] ps/2 kbrd input
- [done] soldered hat PC
- [done] soldered pizo speaker
- [done] tunes for the OS
- 

ToDo:
- desktop
- icons
- about/status prog
- demo prog
- text editor prog
- piano prog
- SD card read/write
- paint prog
- amiga mouse input

*/
//#include <string.h>
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


const int VERSION = 4;
const int KB_DATA = 8;
const int KB_SYNC =  3;
const int WIDTH =  128;
const int HEIGH =  96;
const int CMD_MAX = 24;
const int TUNE_OS = 0;
const int TUNE_POS = 1;
const int TUNE_NEG = 2;
const int TUNE_NEU = 3;

char commandBuffer[CMD_MAX + 1];
int cmdIndex = 0;

void bootimage();
void clear_cmd();
void play_tune(byte no=TUNE_POS);
int freeRam();
void processCommand(char *command);

void setup()  {
  TV.begin(PAL,WIDTH,HEIGH);
  TV.select_font(font4x6);
  TV.clear_screen();
  keyboard.begin(KB_DATA, KB_SYNC);
  memset(commandBuffer, 0, sizeof(commandBuffer));
  
  play_tune();
  bootimage();
  delay(1000);
  clear_cmd();
}

int freeRam() {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

void play_tune(byte no=TUNE_POS){

  if (no==TUNE_OS){ // MonciaOS tune
    TV.tone(1000,200);delay(100);
    TV.tone(2000,200);delay(100);
    TV.tone(300,500);delay(100);
    TV.tone(1500,200);delay(50);
    TV.tone(3000,200);delay(100);
   }else
  if (no==TUNE_POS){ // positive
    TV.tone(261,100);delay(100);
    TV.tone(329,100);delay(100);
    TV.tone(392,100);delay(100);
    TV.tone(523,200);delay(100);
  }else
  if (no==TUNE_NEG){ // negative
    TV.tone(392,100);delay(100);
    TV.tone(329,100);delay(100);
    TV.tone(261,200);delay(100);
  }else
  if (no==TUNE_NEU){ // neutral
    TV.tone(329,150);delay(100);
    TV.tone(329,75);delay(100);
    TV.tone(392,75);delay(100);
  }
}

void bootimage(){
  TV.bitmap(0,0,img_intro2);   
}

void clear_cmd(){
  TV.clear_screen();
  TV.select_font(font4x6);
  TV.bitmap(0,0,img_bg); 
  TV.bitmap(128-16,0,img_p1x);
  TV.set_cursor(0,8);
  TV.print(">");
}


void p1x(){
  TV.clear_screen();
  TV.bitmap(128-85,0,img_kj);
  delay(1000);  
  TV.tone(329,150);delay(100);
  TV.bitmap(24,24,img_p1x);
  TV.tone(329,75);delay(100);
  delay(1000);
  TV.select_font(font8x8);
  TV.print(4,62,"Krzysztof");
  TV.print(4,72,"Krystian");
  TV.print(4,82,"Jankowski");
  TV.tone(392,75);delay(100);
  delay(2000);
  TV.select_font(font4x6);
  clear_cmd();
}

void processCommand(char *command){
  TV.println();
  TV.print(" ");
  
  if (strcmp(command, "free") == 0) {
    play_tune(TUNE_POS);
    TV.print("Free RAM: ");
    TV.print(freeRam());
    TV.print(" bytes");
    
  } else if (strcmp(command, "about") == 0) {
    play_tune(TUNE_OS);
    TV.println("MonciaOS");
    TV.print("Version ");
    TV.print(VERSION);    
    
  } else if (strcmp(command, "p1x") ==0) {
    play_tune(TUNE_POS);
    p1x();
    
  } else {
    play_tune(TUNE_NEG);
    TV.print("Unknown command");
    
  }

  TV.println();
  TV.println();
  TV.print(">");
  
}

void loop() {
  if (keyboard.available()) {
    char c = keyboard.read();
    
    if (c == PS2_ENTER || cmdIndex == CMD_MAX) {
      
      commandBuffer[cmdIndex] = '\0';
      processCommand(commandBuffer);
      cmdIndex = 0;
      memset(commandBuffer, 0, sizeof(commandBuffer));
      
    } else if (c == PS2_ESC) {
      
      play_tune(TUNE_NEU);
      clear_cmd();
    
    } else if (c == PS2_LEFTARROW) {
      
    } else if (c == PS2_RIGHTARROW) {
      
    } else if (c == PS2_UPARROW) {
      
    } else if (c == PS2_DOWNARROW) {
      
    } else if (c == PS2_BACKSPACE) {
      
    }else{
      TV.tone(523,50);
      commandBuffer[cmdIndex++] = c;
      TV.print(c);
    }
  }
}
