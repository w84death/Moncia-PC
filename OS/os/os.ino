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
- [done] 4 arrows for selecting icons 
- [done] nicer keyboard sound
- [done] removed last command history
- [release] alpha 8
- [done] stars screensaver
- [done] screensaver setting in look app
- [done] removed command prompt
- [release] alpha 9 / 188b free
- [done] procedural corners
- [done] dimmer dim background
- 
- [release] alpha 10 / 302b free

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

#define VERSION 10

#define KB_DATA 8
#define KB_SYNC 3
#define WIDTH 112
#define HEIGHT 88
#define TUNE_OS 0
#define TUNE_KB 1
#define TUNE_POS 2
#define TUNE_NEG 3
#define TUNE_NEU 4
#define NUM_STARS 24
#define SCREENSAVER_TIMEOUT 60000
#define WALLPAPERS 4
#define ICONS 7
#define ICONS_COLUMNS 3
#define SCREENSAVERS 2

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
const char TXT_LOOKTITLE_WP[24] PROGMEM = "Wallpaper:\0";
const char TXT_LOOKTITLE_SS[24] PROGMEM = "ScreenSaver:\0";
const char TXT_LOOK1[24] PROGMEM =      " * White fill      \0";
const char TXT_LOOK2[24] PROGMEM =      " * Lines horizontal\0";
const char TXT_LOOK3[24] PROGMEM =      " * Lines vertical  \0";
const char TXT_LOOK4[24] PROGMEM =      " * Digital art     \0";
const char TXT_LOOK5[24] PROGMEM =      " * Sinusoid Artist \0";
const char TXT_LOOK6[24] PROGMEM =      " * Starfield       \0";
const char TXT_LOOKTIP[24] PROGMEM =    "Use left/right arrows\0";
const char TXT_KRZYSZTOF[24] PROGMEM = "Krzysztof\0";
const char TXT_KRYSTIAN[24] PROGMEM = "Krystian\0";
const char TXT_JANKOWSKI[24] PROGMEM = "Jankowski\0";

const byte hw = (WIDTH/2);
const byte hh = (HEIGHT/2);

char txtBuf[24];
char txtBuf8[8];
byte iconIndex = 0;
byte wallpaper = 0;
byte screensaver = 0;
unsigned long lastActivityTime = 0;
  
struct Star {
  float x, y;
  float speed;
};

static Star stars[NUM_STARS];
static int freeRam();
void bootimage();
void drawIcon(const byte pos, const byte id, const bool selected);
void draw_desktop();
void play_tune(const byte no);
void drawWindow(const byte ww, const byte wh, const bool btn = true);
void p1x();
void processCommand();

void setup()  {
  pinMode(A0, INPUT);
  TV.begin(PAL,WIDTH,HEIGHT);
  TV.select_font(font4x6);
  TV.clear_screen();
  keyboard.begin(KB_DATA, KB_SYNC);
  randomSeed(analogRead(0));
  
  play_tune(TUNE_OS);
  bootimage();

  for(int i=0; i<NUM_STARS; i++){
    resetStar(&stars[i]);
  }

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

void drawIcon(const byte pos, const byte id, const bool selected = false){
  const byte ix = 3 + (pos%ICONS_COLUMNS)*36;
  const byte iy = 12 + (pos/ICONS_COLUMNS)*22;
  TV.draw_rect(ix+2,iy+2,32,16,BLACK,BLACK);
  if (selected) TV.draw_rect(ix,iy,32,16,BLACK,BLACK);
  else TV.draw_rect(ix,iy,32,16,BLACK,WHITE);
  
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
  const byte wx = hw - ww/2;
  const byte btnShift = 6;
  byte wy = hh - wh/2;
  if (wh>hh) { wy = 4; }
    
  TV.draw_rect(wx+4,wy+4,ww,wh,BLACK,BLACK);
  TV.draw_rect(wx,wy,ww,wh,WHITE,BLACK);

  if (btn){
    TV.draw_rect(hw-18+4,wy+wh+btnShift+4,36,12,BLACK,BLACK);
    TV.draw_rect(hw-18,wy+wh+btnShift,36,12,BLACK,WHITE);
    TV.set_cursor(hw-10,wy+wh+btnShift+4);
    strcpy_P(txtBuf8, TXT_BTNCLOSE);
    TV.print(txtBuf8);
  }
  
  TV.set_cursor(wx+4,wy+4);
}

void drawDesktop(){
  TV.fill(WHITE);

  // corners
  TV.draw_column(0,0,6,BLACK);
  TV.draw_column(1,0,3,BLACK);
  TV.draw_row(0,0,6,BLACK);
  TV.draw_row(1,0,3,BLACK);
    
  TV.draw_column(WIDTH-1,0,6,BLACK);
  TV.draw_column(WIDTH-2,0,3,BLACK);
  TV.draw_row(0,WIDTH-6,WIDTH,BLACK);
  TV.draw_row(1,WIDTH-3,WIDTH,BLACK);
  
  TV.draw_column(0,HEIGHT-6,HEIGHT,BLACK);
  TV.draw_column(1,HEIGHT-3,HEIGHT,BLACK);
  TV.draw_row(HEIGHT-2,0,3,BLACK);
  TV.draw_row(HEIGHT-1,0,6,BLACK);
  
  TV.draw_column(WIDTH-1,HEIGHT-6,HEIGHT,BLACK);
  TV.draw_column(WIDTH-2,HEIGHT-3,HEIGHT,BLACK);
  TV.draw_row(HEIGHT-1,WIDTH-6,WIDTH,BLACK);
  TV.draw_row(HEIGHT-2,WIDTH-3,WIDTH,BLACK);
  
  // wallpaper
  switch(wallpaper){
    case 0: break;
    case 1:
    for (int y=4;y<HEIGHT-4;y=y+6){
      TV.draw_row(y,2,WIDTH-2,BLACK);
    }
    break;
    case 2:
    for (int x=4;x<WIDTH-4;x=x+6){
      TV.draw_column(x,2,HEIGHT-2,BLACK);
    }
    break;
    case 3:
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

  TV.set_cursor(20,2);
  strcpy_P(txtBuf,TXT_MONCIAABOUT);
  TV.print(txtBuf);TV.print(VERSION);    
}

void play_tune(const byte no){

  if (no==TUNE_OS){ // MonciaOS tune
    TV.tone(1000,200);_delay_ms(20);
    TV.tone(2000,200);_delay_ms(20);
    TV.tone(300,500);_delay_ms(20);
    TV.tone(1500,200);_delay_ms(10);
    TV.tone(3000,200);_delay_ms(20);
  }else
  if (no==TUNE_KB){ // Keyboard click
    TV.tone(100,50);_delay_ms(10);
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
  drawWindow(88,60);
  strcpy_P(txtBuf,TXT_LOOKTITLE_WP);
  TV.print(txtBuf);
     
  switch (wallpaper) {
    case 0: strcpy_P(txtBuf,TXT_LOOK1); break;
    case 1: strcpy_P(txtBuf,TXT_LOOK2); break;
    case 2: strcpy_P(txtBuf,TXT_LOOK3); break;
    case 3: strcpy_P(txtBuf,TXT_LOOK4); break;
  }
  TV.set_cursor(14,20);
  TV.print(txtBuf);
  
  TV.set_cursor(14,32);
  strcpy_P(txtBuf,TXT_LOOKTITLE_SS);
  TV.print(txtBuf);

  switch (screensaver) {
    case 0: strcpy_P(txtBuf,TXT_LOOK5); break;
    case 1: strcpy_P(txtBuf,TXT_LOOK6); break;
  }
  TV.set_cursor(14,42);
  TV.print(txtBuf);
  
  TV.set_cursor(14,54);
  strcpy_P(txtBuf,TXT_LOOKTIP);
  TV.print(txtBuf);

  play_tune(TUNE_POS);
  
  while (!keyboard.available()) {}
  while (char c = keyboard.read()) {
      if (c == PS2_LEFTARROW) {       
        if (wallpaper > 0) wallpaper--;       
        play_tune(TUNE_KB);
      } else if (c == PS2_RIGHTARROW) {
        if (wallpaper < WALLPAPERS-1) wallpaper++;
        play_tune(TUNE_KB);
      } else if (c == PS2_UPARROW) {
        if (screensaver > 0) screensaver--;       
        play_tune(TUNE_KB);
      } else if (c == PS2_DOWNARROW) {  
       if (screensaver < SCREENSAVERS-1) screensaver++;       
        play_tune(TUNE_KB);
      } else if (c == PS2_ESC) {  
        wallpaper = wallpaperSave;
        play_tune(TUNE_NEG);
        break;
      } else if (c == PS2_ENTER) {  
        dimScreen;
        drawWindow(40,16);
        strcpy_P(txtBuf,TXT_DONE);
        TV.print(txtBuf);
        play_tune(TUNE_POS);
        break;
      }

      TV.set_cursor(14,20);
      switch (wallpaper) {
        case 0: strcpy_P(txtBuf,TXT_LOOK1); break;
        case 1: strcpy_P(txtBuf,TXT_LOOK2); break;
        case 2: strcpy_P(txtBuf,TXT_LOOK3); break;
        case 3: strcpy_P(txtBuf,TXT_LOOK4); break;
      }
      TV.print(txtBuf);

      TV.set_cursor(14,42);
      switch (screensaver) {
        case 0: strcpy_P(txtBuf,TXT_LOOK5); break;
        case 1: strcpy_P(txtBuf,TXT_LOOK6); break;
      }
      TV.print(txtBuf);
  }
}

void activateScreensaver(){
  TV.fill(BLACK);
  
  byte lx=2 + rand()%(WIDTH-2);
  byte ly=2 + rand()%(HEIGHT-2);
  byte x,y;  

  while (!keyboard.available()) {  

    if (screensaver == 0) { // sinusiod artist      
      x = hw + sin(millis()*0.0077f) * 24 + (sin(millis()*0.017)*40); //40
      y = hh + cos(millis()*0.0066f) * 24 + (sin(millis()*0.013)*24); //24
      if ((millis()*10) % 50000 == 0) TV.fill(BLACK);
      TV.draw_line(lx,ly,x,y,WHITE);
      lx = x;
      ly = y;    
    }else if (screensaver == 1) { // starfield
      for(int i = 0; i < NUM_STARS; i++) {
        drawStar(&stars[i], BLACK);
        updateStar(&stars[i]);
      
        if (stars[i].x > (WIDTH-4) || stars[i].x < 4 || (stars[i].y > HEIGHT-4) || stars[i].y < 4) {
          resetStar(&stars[i]);
        }
        drawStar(&stars[i], WHITE);
      }
    }
  }
}

void splash(){
  TV.bitmap(0,0,img_title);   
}

void dimScreen(){
  static int x,y;
  for (y=0;y<HEIGHT;y=y+2){
    TV.draw_row(y,0,WIDTH,BLACK);
  }
  for (x=0;x<WIDTH;x=x+2){
    TV.draw_column(x,0,HEIGHT,BLACK);
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


void resetStar(Star *star) {
  star->x = random(32, WIDTH-32);
  star->y = random(32, HEIGHT-32);
  star->speed = -random(1,6) / 2.0;
}

void updateStar(Star *star) {
  star->x += ((WIDTH/2) - star->x) * star->speed / 100.0;
  star->y += ((HEIGHT/2) - star->y) * star->speed / 100.0;
}

void drawStar(Star *star, byte c) {  
  TV.set_pixel(star->x, star->y, c);  
}

void processCommand(){
  
  switch (iconIndex) {
    case 0: // FREE
      dimScreen();
      drawWindow(88,14);
      strcpy_P(txtBuf,TXT_FREERAM);
      TV.print(txtBuf);
      TV.print(freeRam());
      strcpy_P(txtBuf,TXT_BYTES);
      TV.print(txtBuf);
      play_tune(TUNE_POS);
      break;
      
     case 1: // ABOUT
      dimScreen();
      drawWindow(88,50);
      TV.set_cursor(20,8);
      strcpy_P(txtBuf,TXT_MONCIAABOUT);
      TV.print(txtBuf);TV.print(VERSION);    
      TV.bitmap(hw-8,20,img_p1x);
      TV.set_cursor(35,47);
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
        drawWindow(88,14);
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
        drawWindow(88,14);
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
    
    if (c == PS2_ENTER) {
      processCommand();
      drawDesktop();
      
    } else if (c == PS2_ESC) {
      play_tune(TUNE_NEU);
      drawDesktop();
      
    } else if (c == PS2_LEFTARROW) {
      if (iconIndex>0){ 
        iconIndex--;
        play_tune(TUNE_KB);
        drawDesktop();
      }
    } else if (c == PS2_RIGHTARROW) {
      if (iconIndex<ICONS) { 
        iconIndex++; 
        play_tune(TUNE_KB);
        drawDesktop(); 
      }
    } else if (c == PS2_UPARROW) {
      if (iconIndex-ICONS_COLUMNS>=0){ 
        iconIndex-=ICONS_COLUMNS;
        play_tune(TUNE_KB);
        drawDesktop();
      }   
    } else if (c == PS2_DOWNARROW) {  
      if (iconIndex+ICONS_COLUMNS<=ICONS){ 
        iconIndex+=ICONS_COLUMNS;
        play_tune(TUNE_KB);
        drawDesktop();
      }   
    }

    lastActivityTime = millis();
  }

  if (millis() - lastActivityTime > SCREENSAVER_TIMEOUT) {
      activateScreensaver();
      drawDesktop();
  }
}
