/*
------------------------------------
MonciaOS
Version 0.06 - 2024-03-09

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
- [done] input for apps
- [done] backspace for fixing typos
- [done] up arrow for last command
- [done] desktop wallpaper
- [done] window with button
- [done] 'about' prog
- [done] 'free' prog
- [done] demo prog
- [done] basic icons

ToDo:
- icons glyphs
- text editor prog
- piano prog
- SD card read/write
- paint prog
- amiga mouse input

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


const int VERSION = 7;
const byte KB_DATA = 8;
const byte KB_SYNC =  3;
const byte WIDTH =  128;
const byte HEIGH =  96;
const byte CMD_MAX = 24;
const byte TUNE_OS = 0;
const byte TUNE_POS = 1;
const byte TUNE_NEG = 2;
const byte TUNE_NEU = 3;
const byte WALLPAPERS = 4;

char commandBuffer[CMD_MAX + 1];
char commandBufferLast[CMD_MAX + 1];
byte cmdIndex = 0;
byte cmdIndexLast = 0;
byte wallpaper = 2;
void bootimage();

int freeRam();
void bootimage();
void prompt();
void clear_cmd();
void drawIcon(const byte id, const char *title);
void draw_desktop();
void play_tune(const byte no);
void drawWindow(const byte ww, const byte wh);
void p1x();
void processCommand(char *command);

void setup()  {
  pinMode(A0, INPUT);
  TV.begin(PAL,WIDTH,HEIGH);
  TV.select_font(font4x6);
  TV.clear_screen();
  keyboard.begin(KB_DATA, KB_SYNC);
  memset(commandBuffer, 0, sizeof(commandBuffer));
  
  play_tune(TUNE_OS);
  bootimage();
  

  drawDesktop();
}

int freeRam() {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

void bootimage(){
  TV.bitmap(0,0,img_intro2); delay(1000);
}

void prompt(){
  TV.draw_rect(6+2,80+2,110,12,BLACK,BLACK);
  TV.draw_rect(6,80,110,12,WHITE,BLACK);
  TV.set_cursor(10,84);
  TV.print("> ");
}

void clear_cmd(){
  TV.select_font(font4x6);
  cmdIndex = 0;
  memset(commandBuffer, 0, sizeof(commandBuffer));
  prompt();
}

void drawIcon(const byte id, const char *title){
  const byte ix = 8 + (id%3)*38;
  const byte iy = 4 + (id/3)*22;
  TV.draw_rect(ix+2,iy+2,32,16,BLACK,BLACK);
  TV.draw_rect(ix,iy,32,16,BLACK,WHITE);
  TV.set_cursor(ix+2,iy+2);
  TV.print(title);
}

void drawWindow(const byte ww, const byte wh){
  const byte wx = 64 - ww/2;  
  byte wy = 48 - wh/2;
  if (wh>48) { wy = 4; }
    
  TV.draw_rect(wx+4,wy+4,ww,wh,BLACK,BLACK);
  TV.draw_rect(wx,wy,ww,wh,WHITE,BLACK);

  TV.draw_rect(64-18+4,wy+wh+4+4,36,12,BLACK,BLACK);
  TV.draw_rect(64-18,wy+wh+4,36,12,WHITE,BLACK);
  TV.set_cursor(64-10,wy+wh+6+2);
  TV.print("Close");
  
  TV.set_cursor(wx+4,wy+4);

}

void drawDesktop(){
  
  if (wallpaper == 0) {
    TV.fill(WHITE);
    for (int y=2;y<95;y=y+6){
      TV.draw_row(y,2,126,BLACK);
    }
  } else if (wallpaper == 1){
    TV.fill(WHITE);
    for (int x=2;x<127;x=x+6){
      TV.draw_column(x,2,94,BLACK);
    }
  } else if (wallpaper == 2){
    TV.fill(WHITE);
    int lx=2 + rand()%126;
    int ly=2 + rand()%94;
    for (int i=0;i<14;i++){
      const int x=2+rand()%126;
      const int y=2+rand()%94;
      TV.draw_line(lx,ly,x,y,BLACK);
      lx = x;
      ly = y;
    }
  } else if (wallpaper == 3){
    TV.bitmap(0,0,img_bg);
  }
  
  clear_cmd();
  
  drawIcon(0,"Free");
  drawIcon(1,"About");
  drawIcon(2,"Look");
  drawIcon(4,"P1X");
  //drawIcon(4,"Writer");
  //drawIcon(5,"Piano");
  
  prompt();
}

void play_tune(const byte no){

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
  drawDesktop();
}

void processCommand(char *command){
  if (strcmp(command, "free") == 0) {
    
    drawWindow(100,14);
    TV.print("Free RAM: ");
    TV.print(freeRam());
    TV.print(" bytes");
    play_tune(TUNE_POS);
    
  } else if (strcmp(command, "about") == 0) {
    
    drawWindow(100,50);
    TV.set_cursor(35,8);
    TV.print("MonciaOS: alpha ");TV.print(VERSION);    
    TV.bitmap(64-8,20,img_p1x);
    TV.set_cursor(40,47);
    TV.print("CC0 2024 P1X");
    play_tune(TUNE_OS);
    
  } else if (strcmp(command, "look") == 0) {
    
    wallpaper++;
    if (wallpaper == WALLPAPERS) { wallpaper=0; }
    
    drawWindow(100,60);
    TV.print("Wallpaper changed to ");
    TV.print(wallpaper+1);
    TV.set_cursor(20,24);
    TV.print("[1] - lines horizontal");
    TV.set_cursor(20,34);
    TV.print("[2] - lines vertical");
    TV.set_cursor(20,44);
    TV.print("[3] - digital art");
    TV.set_cursor(20,54);
    TV.print("[4] - gradient");
    play_tune(TUNE_POS);
   
  } else if (strcmp(command, "p1x") == 0) {
    p1x();
   
  } else {
    drawWindow(100,14);
    TV.print("Unknown command");
    play_tune(TUNE_NEG);
          
  }

  while (!keyboard.available()) {}
  keyboard.read();
}

void loop() {
  
  if (keyboard.available()) {    
    char c = keyboard.read();
    
    if (c == PS2_ENTER || cmdIndex == CMD_MAX) {

      commandBuffer[cmdIndex] = '\0';
      processCommand(commandBuffer);
      strcpy(commandBufferLast,commandBuffer);
      cmdIndexLast = cmdIndex;
      drawDesktop();
        
    } else if (c == PS2_ESC) {

      play_tune(TUNE_NEU);
      drawDesktop();
      
    } else if (c == PS2_BACKSPACE) {
      if (cmdIndex > 0) {
        play_tune(TUNE_NEU);
        cmdIndex--;
        commandBuffer[cmdIndex] = '\0';
        prompt();
        TV.print(commandBuffer);
      }else{
        play_tune(TUNE_NEG);
      }
    } else if (c == PS2_LEFTARROW) {
      
    } else if (c == PS2_RIGHTARROW) {
      
    } else if (c == PS2_UPARROW) {
      strcpy(commandBuffer,commandBufferLast);
      prompt();
      TV.print(commandBuffer);
      cmdIndex = cmdIndexLast;
          
    } else if (c == PS2_DOWNARROW) {  
        
    }else{
      TV.tone(523,50);
      commandBuffer[cmdIndex++] = c;
      TV.print(c);
    }
 
    delay(50);
  }
}
