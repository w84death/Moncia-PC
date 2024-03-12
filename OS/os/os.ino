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
- [release] alpha 6
- [done] big memory management refactor
- [done] new look and feel
- [done] updated look app
- [release] alpha 7
- [done] screensaver
- [done] dim background under windows
- [done] arrows to select icons app
- [done] enter for running apps from icon selection 
- [done] fixed root problem for crashing (one global text buffer without free())

ToDo:
- SD card read/write
- text editor prog
- piano prog
- paint prog
- amiga mouse input

*/

#include <avr/pgmspace.h>
#include <stdlib.h>
#include <math.h>
#include <PS2Keyboard.h>
#include <TVout.h>
#include <TVoutfonts/font8x8.h>
#include <TVoutfonts/font4x6.h>

#include "title.h"
#include "wallpaper0.h"
#include "p1x.h"
#include "kj.h"


PS2Keyboard keyboard;
TVout TV;


const int VERSION = 9;
const byte KB_DATA = 8;
const byte KB_SYNC =  3;
const byte WIDTH =  128;
const byte HEIGH =  96;
const byte CMD_MAX = 24;
const byte TUNE_OS = 0;
const byte TUNE_POS = 1;
const byte TUNE_NEG = 2;
const byte TUNE_NEU = 3;
const byte WALLPAPERS = 3;
const byte ICONS = 7;

const char TXT_ICON0[8] PROGMEM = "Free\0";
const char TXT_ICON1[8] PROGMEM = "About\0";
const char TXT_ICON2[8] PROGMEM = "Look\0";
const char TXT_ICON3[8] PROGMEM = "P1X\0";
const char TXT_ICON4[8] PROGMEM = "Edit\0";
const char TXT_ICON5[8] PROGMEM = "Files\0";
const char TXT_ICON6[8] PROGMEM = "Moncia\0";
const char TXT_ICON7[8] PROGMEM = "SSaver\0";
const char TXT_BTNCLOSE[8] PROGMEM = "Close\0";
const char TXT_BTNOK[8] PROGMEM = "OK\0";
const char TXT_BTNCHANGE[8] PROGMEM = "Change\0";
const char TXT_UNKNOWN[24] PROGMEM = "Unknown command\0";
const char TXT_MONCIAABOUT[24] PROGMEM = "MonciaOS: alpha \0";
const char TXT_DEBUG[24] PROGMEM = "DEBUG\0";
const char TXT_DONE[24] PROGMEM = "Done\0";
const char TXT_CC[24] PROGMEM = "CC0 2024 P1X\0";
const char TXT_FREERAM[24] PROGMEM = "Free RAM: \0";
const char TXT_BYTES[24] PROGMEM = " bytes\0";
const char TXT_LOOKTITLE[24] PROGMEM =  "----= System look =----\0";
const char TXT_LOOKTITLE2[24] PROGMEM = "Wallpaper: \0";
const char TXT_LOOK1[24] PROGMEM =      " * White fill      \0";
const char TXT_LOOK2[24] PROGMEM =      " * Lines horizontal\0";
const char TXT_LOOK3[24] PROGMEM =      " * Lines vertical  \0";
const char TXT_LOOK4[24] PROGMEM =      " * Digital art     \0";
const char TXT_LOOKTIP[24] PROGMEM =    "Use left/right arrows\0";
const char TXT_KRZYSZTOF[24] PROGMEM = "Krzysztof\0";
const char TXT_KRYSTIAN[24] PROGMEM = "Krystian\0";
const char TXT_JANKOWSKI[24] PROGMEM = "Jankowski\0";

char commandBuffer[CMD_MAX + 1];
char commandBufferLast[CMD_MAX + 1];
char txtBuf[24];
char txtBuf8[8];
byte cmdIndex = 0;
byte cmdIndexLast = 0;
byte iconIndex = 0;
byte wallpaper = 0;
unsigned long lastActivityTime = 0;
const unsigned long screensaverTimeout = 60000;


static int freeRam();
void bootimage();
void prompt();
void clear_cmd();
void drawIcon(const byte pos, const byte id, const bool selected);
void draw_desktop();
void play_tune(const byte no);
void drawWindow(const byte ww, const byte wh, const bool btn = true);
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

static int freeRam() {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

void bootimage(){
  TV.bitmap(0,0,img_title); 
  _delay_ms(500);
}

void prompt(){
  TV.draw_rect(6+2,74+2,110,12,BLACK,BLACK);
  TV.draw_rect(6,74,110,12,WHITE,BLACK);
  TV.set_cursor(10,78);
  TV.print("> ");
}

void clear_cmd(){
  cmdIndex = 0;
  memset(commandBuffer, 0, sizeof(commandBuffer));
}

void drawIcon(const byte pos, const byte id, const bool selected = false){
  const byte ix = 8 + (pos%3)*38;
  const byte iy = 4 + (pos/3)*22;
  TV.draw_rect(ix+2,iy+2,32,16,BLACK,BLACK);
  if (selected) TV.draw_rect(ix,iy,32,16,BLACK,WHITE);
  else TV.draw_rect(ix,iy,32,16,BLACK,BLACK);
  
  TV.set_cursor(ix+2,iy+2);

  switch (id) {
    case 0: strcpy_P(txtBuf8,TXT_ICON0); TV.print(txtBuf8); break;
    case 1: strcpy_P(txtBuf8,TXT_ICON1); TV.print(txtBuf8); break;
    case 2: strcpy_P(txtBuf8,TXT_ICON2); TV.print(txtBuf8); break;
    case 3: strcpy_P(txtBuf8,TXT_ICON3); TV.print(txtBuf8); break;
    case 4: strcpy_P(txtBuf8,TXT_ICON4); TV.print(txtBuf8); break;
    case 5: strcpy_P(txtBuf8,TXT_ICON5); TV.print(txtBuf8); break;
    case 6: strcpy_P(txtBuf8,TXT_ICON6); TV.print(txtBuf8); break;
    case 7: strcpy_P(txtBuf8,TXT_ICON7); TV.print(txtBuf8); break;
  }


}

void drawWindow(const byte ww, const byte wh, const bool btn = true){
  
  const byte wx = 64 - ww/2;  
  byte wy = 48 - wh/2;
  if (wh>48) { wy = 4; }
    
  TV.draw_rect(wx+4,wy+4,ww,wh,BLACK,BLACK);
  TV.draw_rect(wx,wy,ww,wh,WHITE,BLACK);

  if (btn){

 // one default button
  TV.draw_rect(64-18+4,wy+wh+2+4,36,12,BLACK,BLACK);
  TV.draw_rect(64-18,wy+wh+2,36,12,BLACK,WHITE);
  TV.set_cursor(64-10,wy+wh+4+2);
  strcpy_P(txtBuf8, TXT_BTNCLOSE);
  TV.print(txtBuf8);

  // decision buttons
  }
  TV.set_cursor(wx+4,wy+4);
  
}

void drawDesktop(){
  
  switch(wallpaper){
    case 0: TV.bitmap(0,0,img_wallpaper0); break;
    case 1:
    TV.bitmap(0,0,img_wallpaper0);
    for (int y=2;y<95;y=y+6){
      TV.draw_row(y,2,126,BLACK);
    }
    break;
    case 2:
    TV.bitmap(0,0,img_wallpaper0);
    for (int x=2;x<127;x=x+6){
      TV.draw_column(x,2,94,BLACK);
    }
    break;
    case 3:
    TV.bitmap(0,0,img_wallpaper0);
    byte lx=2 + rand()%126;
    byte ly=2 + rand()%94;
    for (int i=0;i<14;i++){
      const byte x=2+rand()%126;
      const byte y=2+rand()%94;
      TV.draw_line(lx,ly,x,y,BLACK);
      lx = x;
      ly = y;
    }
    break;
  }
  
  drawIcon(0,0,iconIndex == 0);
  drawIcon(1,1,iconIndex == 1);
  drawIcon(2,2,iconIndex == 2);
  drawIcon(3,3,iconIndex == 3);
  drawIcon(4,4,iconIndex == 4);
  drawIcon(5,5,iconIndex == 5);
  drawIcon(6,6,iconIndex == 6);
  drawIcon(7,7,iconIndex == 7);

  clear_cmd();
  prompt();
}

void play_tune(const byte no){

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

void appLook(){  
  byte wallpaperSave = wallpaper;
  drawWindow(100,40);
  strcpy_P(txtBuf,TXT_LOOKTITLE);
  TV.print(txtBuf);
  TV.set_cursor(20,42);
  strcpy_P(txtBuf,TXT_LOOKTITLE2);
  TV.print(txtBuf);
     
  switch (wallpaper) {
    case 0: strcpy_P(txtBuf,TXT_LOOK1); break;
    case 1: strcpy_P(txtBuf,TXT_LOOK2); break;
    case 2: strcpy_P(txtBuf,TXT_LOOK3); break;
    case 3: strcpy_P(txtBuf,TXT_LOOK4); break;
  }
  TV.set_cursor(20,50);
  TV.print(txtBuf);
  TV.set_cursor(20,60);
  strcpy_P(txtBuf,TXT_LOOKTIP);
  TV.print(txtBuf);

  play_tune(TUNE_POS);
  
  while (!keyboard.available()) {}
  while (char c = keyboard.read()) {
      if (c == PS2_LEFTARROW) {       
        if (wallpaper > 0) wallpaper--;       
        TV.tone(523,50);
      } else if (c == PS2_RIGHTARROW) {
        if (wallpaper < WALLPAPERS) wallpaper++;
        TV.tone(523,50);
      } else if (c == PS2_UPARROW) {
            
      } else if (c == PS2_DOWNARROW) {  
       
      } else if (c == PS2_ESC) {  
        wallpaper = wallpaperSave;
        break;
      } else if (c == PS2_ENTER) {  
        dimScreen;
        drawWindow(40,16);
        strcpy_P(txtBuf,TXT_DONE);
        TV.print(txtBuf);
        break;
      }

      TV.set_cursor(20,50);
      switch (wallpaper) {
        case 0: strcpy_P(txtBuf,TXT_LOOK1); break;
        case 1: strcpy_P(txtBuf,TXT_LOOK2); break;
        case 2: strcpy_P(txtBuf,TXT_LOOK3); break;
        case 3: strcpy_P(txtBuf,TXT_LOOK4); break;
      }
      TV.print(txtBuf);
      
      _delay_ms(5);
  }
}

void activateScreensaver(){

  // sinusoid artist
  byte lx=2 + rand()%126;
  byte ly=2 + rand()%94;
  byte x,y;  

  TV.fill(BLACK);
  
  while (!keyboard.available()) {  
    x = 64 + sin(millis()*0.0077f) * 24 + (sin(millis()*0.017)*40); //40
    y = 48 + cos(millis()*0.0066f) * 24 + (sin(millis()*0.013)*24); //24
    if ((millis()*10) % 50000 == 0) TV.fill(BLACK);
    TV.draw_line(lx,ly,x,y,WHITE);
    lx = x;
    ly = y;
  }

  // starfield
  
}

void splash(){
  TV.bitmap(0,0,img_title);   
}

void dimScreen(){
  static int x,y;
  for (y=0;y<96;y=y+4){
    TV.draw_row(y,0,128,BLACK);
  }
  for (x=0;x<128;x=x+8){
    TV.draw_column(x,0,96,BLACK);
  }
}

void p1x(){
  TV.clear_screen();
  TV.bitmap(128-85,0,img_kj);
  _delay_ms(40);  
  TV.tone(329,150);_delay_ms(30);
  TV.bitmap(24,24,img_p1x);
  TV.tone(329,75);_delay_ms(30);
  _delay_ms(100);
  
  TV.select_font(font8x8);
  strcpy_P(txtBuf, TXT_KRZYSZTOF);  TV.print(4,62,txtBuf);
  strcpy_P(txtBuf, TXT_KRYSTIAN);  TV.print(4,72,txtBuf);
  strcpy_P(txtBuf, TXT_JANKOWSKI);  TV.print(4,82,txtBuf);
  TV.select_font(font4x6);
  
  TV.tone(392,75);_delay_ms(10);
}

void processCommand(char *command){
  byte appIndex = 200;
  
  if (strcmp(command, "\0") == 0 ) {
    appIndex = iconIndex;   
  }else if (strcmp(command, "free") == 0 ) {
    appIndex = 0;
  } else if (strcmp(command, "about") == 0) {  
    appIndex = 1;
  } else if (strcmp(command, "look") == 0) {
    appIndex = 2;
  } else if (strcmp(command, "p1x") == 0) {  
    appIndex = 3;
  } else if (strcmp(command, "edit") == 0) {  
    appIndex = 4;
  } else if (strcmp(command, "files") == 0) {  
    appIndex = 5;
  } else if (strcmp(command, "ss") == 0) {  
    appIndex = 100;
  } else if (strcmp(command, "moncia") == 0) {  
    appIndex = 101;
  }
  
  switch (appIndex) {
    case 0: // FREE
      dimScreen();
      drawWindow(100,14);
      strcpy_P(txtBuf,TXT_FREERAM);
      TV.print(txtBuf);
      TV.print(freeRam());
      strcpy_P(txtBuf,TXT_BYTES);
      TV.print(txtBuf);
      play_tune(TUNE_POS);
      break;
      
     case 1: // ABOUT
      dimScreen();
      drawWindow(100,50);
      TV.set_cursor(35,8);
      strcpy_P(txtBuf,TXT_MONCIAABOUT);
      TV.print(txtBuf);TV.print(VERSION);    
      TV.bitmap(64-8,20,img_p1x);
      TV.set_cursor(40,47);
      strcpy_P(txtBuf,TXT_CC);
      TV.print(txtBuf);
      play_tune(TUNE_OS);   
      break;  
      
     case 2: // LOOK
      dimScreen();
      appLook(); 
      break;  
      
     case 3: // P1X
      p1x();
      break;  
      
     case 4:
     case 5:
        dimScreen();
        drawWindow(100,14);
        strcpy_P(txtBuf,TXT_DEBUG);
        TV.print(txtBuf);
        play_tune(TUNE_NEU);
      break;
         
     case 7:
     case 100: // SCREEN SAVER
      play_tune(TUNE_POS);
      activateScreensaver();
      break;  
      
     case 6:
     case 101: // MONCIA
      play_tune(TUNE_POS);
      splash(); 
      break;  
      
     case 200: // UNKNOWN
        dimScreen();
        drawWindow(100,14);
        strcpy_P(txtBuf,TXT_UNKNOWN);
        TV.print(txtBuf);
        play_tune(TUNE_NEG);
      break;
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
      
      if (cmdIndex > 0){        
        strcpy(commandBufferLast,commandBuffer);
        cmdIndexLast = cmdIndex;       
      }
      
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
      if (iconIndex>0){ 
        iconIndex--;
        TV.tone(523,50);
        drawDesktop();
      }
    } else if (c == PS2_RIGHTARROW) {
      if (iconIndex<ICONS) { 
        iconIndex++; 
        TV.tone(523,50);
        drawDesktop(); 
      }
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

    lastActivityTime = millis();
    _delay_ms(5);
  }

  if (millis() - lastActivityTime > screensaverTimeout) {
      activateScreensaver();
      drawDesktop();
  }
}
