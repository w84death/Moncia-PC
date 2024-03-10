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

#include <avr/pgmspace.h>
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


const int PROGMEM VERSION = 7;
const byte PROGMEM KB_DATA = 8;
const byte PROGMEM KB_SYNC =  3;
const byte PROGMEM WIDTH =  128;
const byte PROGMEM HEIGH =  96;
const byte PROGMEM CMD_MAX = 24;
const byte PROGMEM TUNE_OS = 0;
const byte PROGMEM TUNE_POS = 1;
const byte PROGMEM TUNE_NEG = 2;
const byte PROGMEM TUNE_NEU = 3;
const byte PROGMEM WALLPAPERS = 4;

const char TXT_ICON0[8] PROGMEM = "Free\0";
const char TXT_ICON1[8] PROGMEM = "About\0";
const char TXT_ICON2[8] PROGMEM = "Look\0";
const char TXT_ICON3[8] PROGMEM = "P1X\0";
const char TXT_ICON4[8] PROGMEM = "Edit\0";
const char TXT_ICON5[8] PROGMEM = "Files\0";
const char TXT_BTNCLOSE[8] PROGMEM = "Close\0";
const char TXT_BTNOK[8] PROGMEM = "OK\0";
const char TXT_BTNCHANGE[8] PROGMEM = "Change\0";
const char TXT_UNKNOWN[24] PROGMEM = "Unknown command\0";
const char TXT_MONCIAABOUT[24] PROGMEM = "MonciaOS: alpha\0";
const char TXT_CC[24] PROGMEM = "CC0 2024 P1X\0";
const char TXT_FREERAM[24] PROGMEM = "Free RAM: \0";
const char TXT_BYTES[24] PROGMEM = " bytes\0";
const char TXT_LOOK0[24] PROGMEM = "Wallpaper changed to \0";
const char TXT_LOOK1[24] PROGMEM = "[1] - lines horizontal\0";
const char TXT_LOOK2[24] PROGMEM = "[2] - lines vertical\0";
const char TXT_LOOK3[24] PROGMEM = "[3] - digital art\0";
const char TXT_LOOK4[24] PROGMEM = "[4] - gradient\0";
const char TXT_KRZYSZTOF[24] PROGMEM = "Krzysztof\0";
const char TXT_KRYSTIAN[24] PROGMEM = "Krystian\0";
const char TXT_JANKOWSKI[24] PROGMEM = "Jankowski\0";

static char commandBuffer[CMD_MAX + 1];
static char commandBufferLast[CMD_MAX + 1];
static byte cmdIndex = 0;
static byte cmdIndexLast = 0;
static byte wallpaper = 2;

static int freeRam();
static void bootimage();
static void prompt();
static void clear_cmd();
static void drawIcon(const byte pos, const byte id);
static void draw_desktop();
static void play_tune(const byte no);
static void drawWindow(const byte ww, const byte wh);
static void p1x();
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

static int freeRam() {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

static void bootimage(){
  TV.bitmap(0,0,img_intro2); _delay_ms(500);
}

static void prompt(){
  TV.draw_rect(6+2,80+2,110,12,BLACK,BLACK);
  TV.draw_rect(6,80,110,12,WHITE,BLACK);
  TV.set_cursor(10,84);
  TV.print("> ");
}

static void clear_cmd(){
  cmdIndex = 0;
  memset(commandBuffer, 0, sizeof(commandBuffer));
}

static void drawIcon(const byte pos, const byte id){
  char txtBuff[8];
  const byte ix = 8 + (pos%3)*38;
  const byte iy = 4 + (pos/3)*22;
  TV.draw_rect(ix+2,iy+2,32,16,BLACK,BLACK);
  TV.draw_rect(ix,iy,32,16,BLACK,WHITE);
  TV.set_cursor(ix+2,iy+2);

  switch (id) {
    case 0: strcpy_P(txtBuff,TXT_ICON0); TV.print(txtBuff); break;
    case 1: strcpy_P(txtBuff,TXT_ICON1); TV.print(txtBuff); break;
    case 2: strcpy_P(txtBuff,TXT_ICON2); TV.print(txtBuff); break;
    case 3: strcpy_P(txtBuff,TXT_ICON3); TV.print(txtBuff); break;
    case 4: strcpy_P(txtBuff,TXT_ICON4); TV.print(txtBuff); break;
    case 5: strcpy_P(txtBuff,TXT_ICON5); TV.print(txtBuff); break;
    default: TV.print("Unknow");
  }


}

static void drawWindow(const byte ww, const byte wh){
  char txtBuff[8];
  const byte wx = 64 - ww/2;  
  byte wy = 48 - wh/2;
  if (wh>48) { wy = 4; }
    
  TV.draw_rect(wx+4,wy+4,ww,wh,BLACK,BLACK);
  TV.draw_rect(wx,wy,ww,wh,WHITE,BLACK);

 // one default button
  TV.draw_rect(64-18+4,wy+wh+2+4,36,12,BLACK,BLACK);
  TV.draw_rect(64-18,wy+wh+2,36,12,BLACK,WHITE);
  TV.set_cursor(64-10,wy+wh+4+2);
  strcpy_P(txtBuff, TXT_BTNCLOSE);
  TV.print(txtBuff);

  // decision buttons
  
  
  TV.set_cursor(wx+4,wy+4);

}

static void drawDesktop(){
  
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
    byte lx=2 + rand()%126;
    byte ly=2 + rand()%94;
    for (int i=0;i<14;i++){
      const byte x=2+rand()%126;
      const byte y=2+rand()%94;
      TV.draw_line(lx,ly,x,y,BLACK);
      lx = x;
      ly = y;
    }
  } else if (wallpaper == 3){
    TV.bitmap(0,0,img_bg);
  }
  
  drawIcon(0,0);
  drawIcon(1,1);
  drawIcon(2,2);
  drawIcon(4,3);
  drawIcon(5,4);
  drawIcon(7,5);

  clear_cmd();
  prompt();
}

static void play_tune(const byte no){

  if (no==TUNE_OS){ // MonciaOS tune
    TV.tone(1000,200);_delay_ms(20);
    TV.tone(2000,200);_delay_ms(20);
    TV.tone(300,500);_delay_ms(20);
    TV.tone(1500,200);_delay_ms(10);
    TV.tone(3000,200);_delay_ms(20);
   }else
  if (no==TUNE_POS){ // positive
    TV.tone(261,100);_delay_ms(10);
    TV.tone(329,100);_delay_ms(10);
    TV.tone(392,100);_delay_ms(10);
    TV.tone(523,200);_delay_ms(10);
  }else
  if (no==TUNE_NEG){ // negative
    TV.tone(392,100);_delay_ms(10);
    TV.tone(329,100);_delay_ms(10);
    TV.tone(261,200);_delay_ms(10);
  }else
  if (no==TUNE_NEU){ // neutral
    TV.tone(329,150);_delay_ms(10);
    TV.tone(329,75);_delay_ms(10);
    TV.tone(392,75);_delay_ms(10);
  }
}



static void p1x(){
  char txtBuff[24];
  
  TV.clear_screen();
  TV.bitmap(128-85,0,img_kj);
  _delay_ms(40);  
  TV.tone(329,150);_delay_ms(30);
  TV.bitmap(24,24,img_p1x);
  TV.tone(329,75);_delay_ms(30);
  _delay_ms(100);
  TV.select_font(font8x8);

  strcpy_P(txtBuff, TXT_KRZYSZTOF);  TV.print(4,62,txtBuff);
  strcpy_P(txtBuff, TXT_KRYSTIAN);  TV.print(4,72,txtBuff);
  strcpy_P(txtBuff, TXT_JANKOWSKI);  TV.print(4,82,txtBuff);
  TV.tone(392,75);_delay_ms(10);
  _delay_ms(500);
  TV.select_font(font4x6);
  drawDesktop();
}

void processCommand(char *command){
  char txtBuff[24];
  
  if (strcmp(command, "free") == 0) {
    
    drawWindow(100,14);
    strcpy_P(txtBuff,TXT_FREERAM);
    TV.print(txtBuff);
    TV.print(freeRam());
    strcpy_P(txtBuff,TXT_BYTES);
    TV.print(txtBuff);
    play_tune(TUNE_POS);
    
  } else if (strcmp(command, "about") == 0) {
    
    drawWindow(100,50);
    TV.set_cursor(35,8);
    strcpy_P(txtBuff,TXT_MONCIAABOUT);
    TV.print(txtBuff);TV.print(VERSION);    
    TV.bitmap(64-8,20,img_p1x);
    TV.set_cursor(40,47);
    strcpy_P(txtBuff,TXT_CC);
    TV.print(txtBuff);
    play_tune(TUNE_OS);
    
  } else if (strcmp(command, "look") == 0) {
    
    wallpaper++;
    if (wallpaper == WALLPAPERS) { wallpaper=0; }
    
    drawWindow(100,60);
    strcpy_P(txtBuff,TXT_LOOK0);
    TV.print(txtBuff);
    TV.print(wallpaper+1);
    TV.set_cursor(20,24);
    strcpy_P(txtBuff,TXT_LOOK1);
    TV.print(txtBuff);
    TV.set_cursor(20,34);
    strcpy_P(txtBuff,TXT_LOOK2);
    TV.print(txtBuff);
    TV.set_cursor(20,44);
    strcpy_P(txtBuff,TXT_LOOK3);
    TV.print(txtBuff);
    TV.set_cursor(20,54);
    strcpy_P(txtBuff,TXT_LOOK4);
    TV.print(txtBuff);
    play_tune(TUNE_POS);
   
  } else if (strcmp(command, "p1x") == 0) {
    p1x();
   
  } else {
    drawWindow(100,14);
    strcpy_P(txtBuff,TXT_UNKNOWN);
    TV.print(txtBuff);
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
 
    _delay_ms(5);
  }
}
