

/*
------------------------------------
MonciaOS
Version alpha 11 - 2024-03-17

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
- -----------------------------------------
- [done] big memory management refactor
- [done] new look and feel
- [done] updated look app
- [release] alpha 7
- -----------------------------------------
- [done] screensaver
- [done] dim background under windows
- [done] arrows to select icons app
- [done] enter for running apps from icon selection 
- [done] fixed root problem for crashing (one global text buffer without free())
- [done] 4 arrows for selecting icons 
- [done] nicer keyboard sound
- [done] removed last command history
- [release] alpha 8
- -----------------------------------------
- [done] stars screensaver
- [done] screensaver setting in look app
- [done] removed command prompt
- [release] alpha 9 / 188 bytes free
- -----------------------------------------
- [done] procedural corners
- [done] dimmer dim background
- [done] resolution changed to 112x88
- [done] all code resolution independent
- [release] alpha 10 / 302 bytes free
- -----------------------------------------
- [done] optimized bitmaps files
- [done] combined bitmaps into one header file
- [done] combined fonts into one header file
- [done] combined strings into one header file
- [done] renaming function for consistent
- [done] updated about (more line NeoFetch)
- [done] refactor applications logic
- [release] alpha 11 / 259 bytes free
- -----------------------------------------
- [done] reduced to 16 stars - 355b free
- [done] EEPROM storage
- [done] base text editor (4 banks of 250 characters)
-
- cursors for editing
- changing banks
- 
- -----------------------------------------


ToDo:
- remove ICONS, calculate via activated APPS.

- text editor prog
- piano prog
- paint prog
- amiga mouse input
- SD card read/write
*/

#include <avr/pgmspace.h>
#include <stdlib.h>
#include <math.h>
#include <PS2Keyboard.h>
#include <TVout.h>
#include <EEPROM.h>

#include "fonts.h"
#include "bitmaps.h"
#include "strings.h"

PS2Keyboard keyboard;
TVout TV;

#define VERSION 12

#define MAX_RAM 2048
#define KB_DATA 8
#define KB_SYNC 3
#define WIDTH 112
#define HEIGHT 88
#define TUNE_OS 0
#define TUNE_KB 1
#define TUNE_POS 2
#define TUNE_NEG 3
#define TUNE_NEU 4

#define WALLPAPERS 4
#define SCREENSAVERS 2
#define APPS 9
#define ICONS 7
#define ICONS_COLUMNS 3
#define SCREENSAVER_TIMEOUT 60000
#define NUM_STARS 16
#define EDIT_BUFFER 250
#define EDIT_BANKS 4

const byte HALF_WIDTH PROGMEM = (WIDTH/2);
const byte HALF_HEIGHT PROGMEM = (HEIGHT/2);

char txtBuf[24];
char txtBuf8[8];
int editBank = 0;

struct App {
  byte id;
  const char* name;
  byte iconPos;
  bool interactive;
};
static App apps[APPS];

byte appIndex = 0;
byte wallpaper = 0;
byte screensaver = 0;
unsigned long lastActivityTime = 0;
  
struct Star {
  float x, y;
  float speed;
};
static Star stars[NUM_STARS];

struct Vec2 {
  int x,y;
};

static int freeRam();
void drawIcon(const byte pos, const byte id, const bool selected);
void drawDesktop();
void drawSplashScreen();
void play_tune(const byte no);
Vec2 drawWindow(const byte ww, const byte wh, const bool btn = true);
void drawP1XDemo();
void processCommand();
void addApp(byte appId, const char* name, byte iconPos, bool interactive);

void setup()  {
  pinMode(A0, INPUT);
  TV.begin(PAL,WIDTH,HEIGHT);
  TV.select_font(font4x6);
  TV.clear_screen();
  keyboard.begin(KB_DATA, KB_SYNC);
  randomSeed(analogRead(0));
  
  play_tune(TUNE_OS);
  drawSplashScreen();

  for(int i=0; i<NUM_STARS; i++){
    resetStar(&stars[i]);
  }
  
  for (int i = 0; i < APPS; i++) {
    apps[i].id = 255;
  }
  
  addApp(0, TXT_APP0, 0, false); 
  addApp(1, TXT_APP1, 1, false);
  addApp(2, TXT_APP2, 2, true);
  addApp(3, TXT_APP3, 3, false);
  addApp(4, TXT_APP4, 4, true);
  addApp(5, TXT_APP5, 5, false);
  addApp(6, TXT_APP6, 6, false);
  addApp(7, TXT_APP7, 7, false);

  drawDesktop();
}


void addApp(byte appId, const char* name, byte iconPos, bool interactive) {
  apps[appId].id = appId;
  apps[appId].name = name;
  apps[appId].iconPos = iconPos;
  apps[appId].interactive = interactive;
}

static int freeRam() {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

void drawIcon(App *app, const bool selected = false){
  const byte ix = 3 + (app->iconPos%ICONS_COLUMNS)*36;
  const byte iy = 20 + (app->iconPos/ICONS_COLUMNS)*22;
  TV.draw_rect(ix+2,iy+2,32,16,BLACK,BLACK);
  if (selected) TV.draw_rect(ix,iy,32,16,BLACK,BLACK);
  else TV.draw_rect(ix,iy,32,16,BLACK,WHITE);
  
  TV.set_cursor(ix+2,iy+2);
  strcpy_P(txtBuf8,app->name);
  TV.print(txtBuf8);
}

Vec2 drawWindow(const byte ww, const byte wh, const bool btn = true){
  Vec2 v2;
  const byte wx = HALF_WIDTH - ww/2;
  const byte btnShift = 0;
  byte wy = HALF_HEIGHT - wh/2;
  if (wh>HALF_HEIGHT) { wy = 4; }
    
  TV.draw_rect(wx+4,wy+4,ww,wh,BLACK,BLACK);
  TV.draw_rect(wx,wy,ww,wh,WHITE,BLACK);

  if (btn){
    TV.draw_rect(HALF_WIDTH-18+4,wy+wh+btnShift+4,36,12,BLACK,BLACK);
    TV.draw_rect(HALF_WIDTH-18,wy+wh+btnShift,36,12,BLACK,WHITE);
    TV.set_cursor(HALF_WIDTH-10,wy+wh+btnShift+4);
    strcpy_P(txtBuf8, TXT_BTNCLOSE);
    TV.print(txtBuf8);
  }
  v2.x=wx+4;
  v2.y=wy+4;
  TV.set_cursor(v2.x, v2.y);
  
  return v2;
}

void drawBackground(byte bgColor = WHITE, byte cornerColor = BLACK, bool gradient = false){
  
  TV.fill(bgColor);

  if(gradient){
    for (byte i=0; i<WIDTH; i+=8){
      TV.bitmap(i,0,img_gradient); // 8
    }
  }

  TV.draw_column(0,0,6,cornerColor);
  TV.draw_column(1,0,3,cornerColor);
  TV.draw_row(0,0,6,cornerColor);
  TV.draw_row(1,0,3,cornerColor);
    
  TV.draw_column(WIDTH-1,0,6,cornerColor);
  TV.draw_column(WIDTH-2,0,3,cornerColor);
  TV.draw_row(0,WIDTH-6,WIDTH,cornerColor);
  TV.draw_row(1,WIDTH-3,WIDTH,cornerColor);
  
  TV.draw_column(0,HEIGHT-6,HEIGHT,cornerColor);
  TV.draw_column(1,HEIGHT-3,HEIGHT,cornerColor);
  TV.draw_row(HEIGHT-2,0,3,cornerColor);
  TV.draw_row(HEIGHT-1,0,6,cornerColor);
  
  TV.draw_column(WIDTH-1,HEIGHT-6,HEIGHT,cornerColor);
  TV.draw_column(WIDTH-2,HEIGHT-3,HEIGHT,cornerColor);
  TV.draw_row(HEIGHT-1,WIDTH-6,WIDTH,cornerColor);
  TV.draw_row(HEIGHT-2,WIDTH-3,WIDTH,cornerColor);
}


void drawSplashScreen(){
  drawBackground(WHITE,BLACK,true);
  TV.draw_rect(HALF_WIDTH-26,HALF_HEIGHT-15,60,40, BLACK, BLACK);
  TV.draw_rect(HALF_WIDTH-30,HALF_HEIGHT-19,60,40, BLACK, WHITE);
  TV.bitmap(HALF_WIDTH-28,HALF_HEIGHT-17,img_chip); // 56x37 -> 28x17
  TV.bitmap(HALF_WIDTH-40,HEIGHT-14,img_logo); // 80x11 -> 40x5
  _delay_ms(500);
}

void dimScreen(){
  static byte x,y;
  for (y=0;y<HEIGHT;y=y+2){
    TV.draw_row(y,0,WIDTH,BLACK);
  }
  for (x=0;x<WIDTH;x=x+2){
    TV.draw_column(x,0,HEIGHT,BLACK);
  }
}

void drawDesktop(){
  drawBackground();
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

  for (int i = 0; i < APPS; i++) {
    if(apps[i].id<255) drawIcon(&apps[i],appIndex == i);
  }

  TV.set_cursor(20,2);
  strcpy_P(txtBuf,TXT_OS);
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
  const Vec2 pos = drawWindow(88,60);
  strcpy_P(txtBuf,TXT_LOOKTITLE_WP);
  TV.print(txtBuf);
     
  switch (wallpaper) {
    case 0: strcpy_P(txtBuf,TXT_LOOK1); break;
    case 1: strcpy_P(txtBuf,TXT_LOOK2); break;
    case 2: strcpy_P(txtBuf,TXT_LOOK3); break;
    case 3: strcpy_P(txtBuf,TXT_LOOK4); break;
  }
  TV.set_cursor(pos.x,pos.y+8);
  TV.print(txtBuf);
  
  TV.set_cursor(pos.x,pos.y+20);
  strcpy_P(txtBuf,TXT_LOOKTITLE_SS);
  TV.print(txtBuf);

  switch (screensaver) {
    case 0: strcpy_P(txtBuf,TXT_LOOK5); break;
    case 1: strcpy_P(txtBuf,TXT_LOOK6); break;
  }
  TV.set_cursor(pos.x,pos.y+28);
  TV.print(txtBuf);
  
  TV.set_cursor(pos.x,pos.y+44);
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
        play_tune(TUNE_POS);
        break;
      }

      
      switch (wallpaper) {
        case 0: strcpy_P(txtBuf,TXT_LOOK1); break;
        case 1: strcpy_P(txtBuf,TXT_LOOK2); break;
        case 2: strcpy_P(txtBuf,TXT_LOOK3); break;
        case 3: strcpy_P(txtBuf,TXT_LOOK4); break;
      }
      TV.set_cursor(pos.x,pos.y+8);
      TV.print(txtBuf);

      TV.set_cursor(14,42);
      switch (screensaver) {
        case 0: strcpy_P(txtBuf,TXT_LOOK5); break;
        case 1: strcpy_P(txtBuf,TXT_LOOK6); break;
      }
      TV.set_cursor(pos.x,pos.y+28);
      TV.print(txtBuf);
  }
}

void activateScreensaver(){
  TV.fill(BLACK);
  
  byte lx=2 + rand()%(WIDTH-2);
  byte ly=2 + rand()%(HEIGHT-2);
  byte x,y;  
  unsigned int t = 0;

  while (!keyboard.available()) {  
    t++;
    if (screensaver == 0) { // sinusiod artist      
      x = HALF_WIDTH + sin(t*0.0077f) * 24 + (sin(t*0.017)*20); //40
      y = HALF_HEIGHT + cos(t*0.0066f) * 24 + (sin(t*0.013)*12); //24
      if (t % 1500 == 0) TV.fill(BLACK);
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

void drawP1XDemo(){
  TV.clear_screen();
  TV.bitmap(WIDTH-85,0,img_kj);
  _delay_ms(40);  
  TV.tone(329,150);_delay_ms(30);
  TV.bitmap(12,24,img_p1x);
  TV.tone(329,75);_delay_ms(30);
  _delay_ms(100);
  
  TV.select_font(font8x8);
  strcpy_P(txtBuf, TXT_KRZYSZTOF);  TV.print(4,HEIGHT-30,txtBuf);
  strcpy_P(txtBuf, TXT_KRYSTIAN);  TV.print(4,HEIGHT-20,txtBuf);
  strcpy_P(txtBuf, TXT_JANKOWSKI);  TV.print(4,HEIGHT-10,txtBuf);
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

void aboutApp(){
  const Vec2 pos = drawWindow(100,70);
  
  TV.set_cursor(pos.x,pos.y);
  strcpy_P(txtBuf,TXT_BOARD);
  TV.print(txtBuf);
  
  TV.set_cursor(pos.x,pos.y+10);
  strcpy_P(txtBuf,TXT_OS);
  TV.print(txtBuf);TV.print(VERSION);
  
  TV.set_cursor(pos.x,pos.y+20);
  strcpy_P(txtBuf,TXT_CPU);
  TV.print(txtBuf);

  TV.set_cursor(pos.x,pos.y+30);
  strcpy_P(txtBuf,TXT_RAM);
  TV.print(txtBuf);
  
  TV.set_cursor(pos.x+20,pos.y+30);
  TV.print(MAX_RAM-freeRam());

  TV.set_cursor(pos.x,pos.y+40);
  strcpy_P(txtBuf,TXT_TVOUT);
  TV.print(txtBuf);
  TV.set_cursor(pos.x+28,pos.y+40);
  TV.print(WIDTH);
  TV.set_cursor(pos.x+52,pos.y+40);
  TV.print(HEIGHT);
  
  TV.set_cursor(pos.x,pos.y+58);
  strcpy_P(txtBuf,TXT_CC);
  TV.print(txtBuf);
  
  play_tune(TUNE_OS);   
}

void clearEEPROM(){
  const int startBuf = EDIT_BUFFER*editBank;
  const int endBuf = EDIT_BUFFER + EDIT_BUFFER*editBank;
  for (int i =  startBuf; i < endBuf; i++) {
    EEPROM.write(i, 0);
  } 
  EEPROM.write(endBuf, '\0');
}

void editApp(){  
  char editBuf[EDIT_BUFFER] PROGMEM;
  int editIndex = 0;
  int lastIndex = 0;
  
  drawBackground(WHITE,BLACK);
  TV.set_cursor(HALF_WIDTH-8,1);
  strcpy_P(txtBuf,TXT_APP4);
  TV.print(txtBuf);

  TV.set_cursor(HALF_WIDTH+12,1);
  strcpy_P(txtBuf,TXT_BANK);
  TV.print(txtBuf);
  TV.print(editBank+1);
  
  char readChar = " ";
  TV.set_cursor(0,8);
  
  while (readChar != '\0' and editIndex<EDIT_BUFFER){
    readChar = EEPROM.read(editIndex + EDIT_BUFFER*editBank);
    editBuf[editIndex++] = readChar;
    TV.print(readChar);
  }
  
  editIndex--;
  lastIndex = editIndex;
   
  while (!keyboard.available()) {}
  while (char c = keyboard.read()) {     
    if (c == PS2_ENTER) {      
      for (int i = lastIndex; i < editIndex; i++) {
        EEPROM.write(i + EDIT_BUFFER*editBank, editBuf[i]);
      }
      EEPROM.write(editIndex + EDIT_BUFFER*editBank, '\0');    
      break;
    } else if (c == PS2_ESC) {
      break;      
    } else if (c == PS2_LEFTARROW) {            
      break;
    } else if (c == PS2_RIGHTARROW) {      
      break;
    } else if (c == PS2_UPARROW) {  
      break;
    } else if (c == PS2_DOWNARROW) {      
      break;
    } else if (c == PS2_F1) {
      editBank = 0;
      break;
    } else if (c == PS2_F2) {
      editBank = 1;
      break;
    } else if (c == PS2_F3) {
      editBank = 2;
      break;
    } else if (c == PS2_F4) {
      editBank = 3;
      break;
    } else if (c == PS2_PAGEDOWN) {
      if (editBank > 0) {
        editBank--;
      }
      break;
    } else if (c == PS2_PAGEUP) {
      if (editBank < EDIT_BANKS-1) {
        editBank++;
      }
      break;
    } else if (c == PS2_DELETE) {
      clearEEPROM();
      break;
    }else{
      if (editIndex<EDIT_BUFFER) {
        editBuf[editIndex++] = c;      
        TV.print(c);
      }
    }    
    while (!keyboard.available()) {}
  }  

  free(editBuf);
}

void processCommand(){
  
  switch (appIndex) {
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
      aboutApp();
      break;  
      
     case 2: // LOOK
      dimScreen();
      appLook(); 
      break;  
      
     case 3: // P1X
      drawP1XDemo();
      break;  
      
     case 4:
      editApp();
      break;
      
     case 5:        
        dimScreen();
        drawWindow(88,14);                
        strcpy_P(txtBuf,TXT_DONE);
        TV.print(txtBuf);
        play_tune(TUNE_NEU);
        clearEEPROM();
        play_tune(TUNE_POS);
      break;
         
     case 7:
     case 100: // SCREEN SAVER
      play_tune(TUNE_POS);
      activateScreensaver();
      break;  
      
     case 6:
     case 101: // MONCIA
      play_tune(TUNE_POS);
      drawSplashScreen(); 
      break;  
      
     case 200: // UNKNOWN
        dimScreen();
        drawWindow(88,14);
        strcpy_P(txtBuf,TXT_UNKNOWN);
        TV.print(txtBuf);
        play_tune(TUNE_NEG);
      break;
  }

  if(!apps[appIndex].interactive){
    while (!keyboard.available()) {}
    keyboard.read();
  }
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
      if (appIndex>0){ 
        appIndex--;
        play_tune(TUNE_KB);
        drawDesktop();
      }
    } else if (c == PS2_RIGHTARROW) {
      if (appIndex<ICONS) { 
        appIndex++; 
        play_tune(TUNE_KB);
        drawDesktop(); 
      }
    } else if (c == PS2_UPARROW) {
      if (appIndex-ICONS_COLUMNS>=0){ 
        appIndex-=ICONS_COLUMNS;
        play_tune(TUNE_KB);
        drawDesktop();
      }   
    } else if (c == PS2_DOWNARROW) {  
      if (appIndex+ICONS_COLUMNS<=ICONS){ 
        appIndex+=ICONS_COLUMNS;
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
