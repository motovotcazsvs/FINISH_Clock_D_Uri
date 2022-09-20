//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// поменяли пины 11-12 и 3-5
#include <iarduino_IR_RX.h>                      // Подключаем библиотеку для работы с ИК-приёмником                        
#include <RGBmatrixPanel.h>
#include <Adafruit_GFX.h>
#include <iarduino_RTC.h>                        //библиотека чассов реального времени
#include <OneWire.h>

OneWire ds(7);
iarduino_IR_RX IR(5);    // Объявляем объект IR, с указанием вывода к которому подключён ИК-приёмник    // поменяли пины 11-12 и 3-5
#define OE   9
#define LAT 10
#define CLK 12  // поменяли пины 11-12 и 3-5
#define A   A0
#define B   A1
#define C   A2
#define _D   A3

unsigned int r = 0;
unsigned int g = 7;
unsigned int b = 0;
unsigned int r2 = 7;
unsigned int g2 = 7;
unsigned int b2 = 7;
#define greeeen matrix.Color333(r, g, b)
#define white matrix.Color333(r2, g2, b2)
int brightness_max = 7;
boolean program_color = false;
int n1 = -1;
int n2 = -1;
int n3 = -1;
int n4 = -1;
int n5 = -1;
int n6 = -1;
boolean zahod = false;
int vvedeno_cifr = 0;

iarduino_RTC Time(RTC_DS3231); //подключаем чассы к пинам SDA - 20, SCL - 21 (к пинам для I2C)
RGBmatrixPanel matrix(A, B, C, _D, CLK, LAT, OE, false, 64);
unsigned char* den[7] = {"НЕДІЛЯ", "ПОНЕДІЛОК", "ВІВТОРОК", "СЕРЕДА", "ЧЕТВЕР", "П'ЯТНИЦЯ", "СУБОТА"};
unsigned char* mes[12] = {"СІЧЕНЬ", "ЛЮТИЙ", "БЕРЕЗЕНЬ", "КВІТЕНЬ", "ТРАВЕНЬ", "ЧЕРВЕНЬ", "ЛИПЕНЬ", "СЕРПЕНЬ", "ВЕРЕСЕНЬ", "ЖОВТЕНЬ", "ЛИСТОПАД", "ГРУДЕНЬ"};
unsigned char* numb[10] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9" };

//" АБВГҐДЕЄЖЗИІЇЙКЛМНОПРСТУФХЦЧШЩЬЮЯабвгґдеєжзиіїйклмнопрстуфхцчшщьюя0123456789:°'*/+-@~#()<>.,!?;";
const unsigned char set_char[106][7]= {
0x0, 0x0, 0x1C, 0x12, 0x1C, 0x10, 0x10,     //р 0 209-128 
0x0, 0x0, 0xE, 0x10, 0x10, 0x10, 0xE,       //с 1 209-129 
0x0, 0x0, 0x1F, 0x4, 0x4, 0x4, 0x4,         //т 2 209-130
0x0, 0x0, 0x11, 0xA, 0x4, 0x8, 0x10,        //у 3 209-131
0x0, 0x0, 0xE, 0x15, 0xE, 0x4, 0x4,         //ф 4 209-132
0x0, 0x0, 0x11, 0xA, 0x4, 0xA, 0x11,        //х 5 209-133
0x0, 0x0, 0x12, 0x12, 0x12, 0x1F, 0x1,      //ц 6 209-134
0x0, 0x0, 0x12, 0x12, 0x1E, 0x2, 0x2,       //ч 7 209-135
0x0, 0x0, 0x0, 0x15, 0x15, 0x15, 0x1F,      //ш 8 209-136
0x0, 0x0, 0x15, 0x15, 0x15, 0x1F, 0x1,      //щ 9 209-137
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,          //
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,          //
0x0, 0x0, 0x10, 0x10, 0x1E, 0x12, 0x1E,     //ь 12 209-140
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,          //
0x0, 0x0, 0x12, 0x15, 0x1D, 0x15, 0x12,     //ю 14 209-142
0x0, 0x0, 0xE, 0x12, 0xE, 0xA, 0x12,        //я 15 209-143
0x1, 0x1F, 0x10, 0x10, 0x10, 0x10, 0x10,    //Ґ 16 210-144
0x0, 0x2, 0x1E, 0x10, 0x10, 0x10, 0x10,     //ґ 17 210-145
0xE, 0xA, 0xE, 0x0, 0x0, 0x0, 0x0,          //° 18 194-176
0x0, 0x0, 0x1, 0xE, 0x10, 0x0, 0x0,         //~ 19 126
0x0, 0x0, 0xE, 0x10, 0x1C, 0x10, 0xE,       //є 20 209-148
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,          //
0x0, 0x0, 0x10, 0x0, 0x10, 0x10, 0x10,      //і 22 209-150
0x0, 0x0, 0x14, 0x0, 0x8, 0x8, 0x8,         //ї 23 209-151
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,          //  24 32
0x10, 0x10, 0x10, 0x10, 0x10, 0x0, 0x10,    //! 25 33
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,          //
0x0, 0xA, 0x1F, 0xA, 0x1F, 0xA, 0x0,        //# 27 35
0xE, 0x11, 0x10, 0x1E, 0x10, 0x11, 0xE,     //Є 28 208-132
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,          //
0xE, 0x4, 0x4, 0x4, 0x4, 0x4, 0xE,          //І 30 208-134
0x14, 0x0, 0x1C, 0x8, 0x8, 0x8, 0x1C,       //Ї 31 208-135
0xE, 0x0, 0x11, 0x13, 0x15, 0x19, 0x11,     //й 32 208-185
0x0, 0x0, 0x12, 0x14, 0x18, 0x14, 0x12,     //к 33 208-186
0x0, 0x0, 0x6, 0xA, 0x12, 0x12, 0x12,       //л 34 208-187
0x0, 0x0, 0x11, 0x1B, 0x15, 0x11, 0x11,     //м 35 208-188
0x0, 0x0, 0x12, 0x12, 0x1E, 0x12, 0x12,     //н 36 208-189
0x0, 0x0, 0xE, 0x11, 0x11, 0x11, 0xE,       //о 37 208-190
0x0, 0x0, 0x1E, 0x12, 0x12, 0x12, 0x12,     //п 38 208-191
0xC, 0xC, 0x4, 0x0, 0x0, 0x0, 0x0,          //' 39
0x8,0x10, 0x10, 0x10, 0x10, 0x10, 0x8,      //( 40
0x10, 0x8, 0x8, 0x8, 0x8, 0x8, 0x10,        //) 41
0x0, 0x0, 0x14, 0x8, 0x14, 0x0, 0x0,        //* 42
0x0, 0x4, 0x4, 0x1F, 0x4, 0x4, 0x0,         //+ 43
0x0, 0x0, 0x0, 0x0, 0x0, 0x10, 0x10,        //, 44
0x0, 0x0, 0x0, 0x1F, 0x0, 0x0, 0x0,         //- 45
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x4,          //. 46
0x0, 0x1, 0x2, 0x4, 0x8, 0x10, 0x0,         /// 47
0xE, 0x11, 0x13, 0x15, 0x19, 0x11, 0xE,     //0 48
0x4, 0xC, 0x4, 0x4, 0x4, 0x4, 0xE,          //1 49
0xE, 0x11, 0x1, 0x2, 0x4, 0x8, 0x1F,        //2 50
0x1F, 0x2, 0x4, 0x2, 0x1, 0x11, 0xE,        //3 51
0x2, 0x6, 0xA, 0x12, 0x1F, 0x2, 0x2,        //4 52
0x1F, 0x10, 0x1E, 0x1, 0x1, 0x11, 0xE,      //5 53
0x6, 0x8, 0x10, 0x1E, 0x11, 0x11, 0xE,      //6 54
0x1F, 0x1, 0x2, 0x4, 0x8, 0x10, 0x10,       //7 55
0xE, 0x11, 0x11, 0xE, 0x11, 0x11, 0xE,      //8 56
0xE, 0x11, 0x11, 0xF, 0x1, 0x2, 0xC,        //9 57
0x0, 0x0, 0x4, 0x0, 0x4, 0x0, 0x0,          //: 58
0x0, 0x0, 0x0, 0x10, 0x0, 0x10, 0x10,       //; 59
0x0, 0x5, 0xA, 0x14, 0xA, 0x5, 0x0,         //<< 60
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,          //
0x0, 0x14, 0xA, 0x5, 0xA, 0x14, 0x0,        //>> 62
0xE, 0x11, 0x1, 0x2, 0x4, 0x0, 0x4,         //? 63
0xE, 0x1, 0xD, 0x15, 0x15, 0x15, 0xA,       //@ 64
0xE, 0x11, 0x11, 0x11, 0x1F, 0x11, 0x11,    //А 65 208-144
0x1F, 0x10, 0x10, 0x1E, 0x11, 0x11, 0x1E,   //Б 66 208-145
0x1E, 0x11, 0x11, 0x1E, 0x11, 0x11, 0x1E,   //В 67 208-146
0x1F, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,   //Г 68 208-147
0xE, 0xA, 0xA, 0xA, 0x1F, 0x11, 0x11,       //Д 69 208-148
0x1F, 0x10, 0x10, 0x1E, 0x10, 0x10, 0x1F,   //Е 70 208-149
0x15, 0x15, 0xE, 0x4, 0xE, 0x15, 0x15,      //Ж 71 208-150
0xE, 0x11, 0x1, 0x2, 0x1, 0x11, 0xE,        //З 72 208-151
0x11, 0x11, 0x13, 0x15, 0x19, 0x11, 0x11,   //И 73 208-152
0x4, 0x11, 0x11, 0x13, 0x15, 0x19, 0x11,    //Й 74 208-153
0x11, 0x12, 0x14, 0x18, 0x14, 0x12, 0x11,   //К 75 208-154
0x3, 0x5, 0x9, 0x9, 0x9, 0x9, 0x11,         //Л 76 208-155
0x11, 0x1B, 0x15, 0x11, 0x11, 0x11, 0x11,   //М 77 208-156
0x11, 0x11, 0x11, 0x1F, 0x11, 0x11, 0x11,   //Н 78 208-157
0xE, 0x11, 0x11, 0x11, 0x11, 0x11, 0xE,     //О 79 208-158
0x1F, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11,   //П 80 208-159
0x1E, 0x11, 0x11, 0x1E, 0x10, 0x10, 0x10,   //Р 81 208-160
0xE, 0x11, 0x10, 0x10, 0x10, 0x11, 0xE,     //С 82 208-161
0x1F, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4,         //Т 83 208-162
0x11, 0x11, 0x11, 0xA, 0x4, 0x8, 0x10,      //У 84 208-163
0x1F, 0x15, 0x15, 0x1F, 0x4, 0x4, 0x4,      //Ф 85 208-164
0x11, 0x11, 0xA, 0x4, 0xA, 0x11, 0x11,      //Х 86 208-165
0x12, 0x12, 0x12, 0x12, 0x12, 0x1F, 0x1,    //Ц 87 208-166
0x11, 0x11, 0x11, 0xF, 0x1, 0x1, 0x1,       //Ч 88 208-167
0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x1F,   //Ш 89 208-168
0x15, 0x15, 0x15, 0x15, 0x15, 0x1F, 0x1,    //Щ 90 208-169
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,          //
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,          //
0x10, 0x10, 0x10, 0x1F, 0x11, 0x11, 0x1F,   //Ь 93 208-172
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,          //
0x12, 0x15, 0x15, 0x1D, 0x15, 0x15, 0x12,   //Ю 95 208-174
0xF, 0x11, 0x11, 0x11, 0xF, 0x9, 0x11,      //Я 96 208-175
0x0, 0x0, 0xE, 0x1, 0xF, 0x11, 0xF,         //а 97 208-176
0xE, 0x10, 0x10, 0x1C, 0x12, 0x12, 0xC,     //б 98 208-177
0x0, 0x0, 0x1C, 0x12, 0x1C, 0x12, 0x1C,     //в 99 208-178
0x0, 0x0, 0x1E, 0x12, 0x10, 0x10, 0x10,     //г 100 208-179
0x0, 0x0, 0xE, 0xA, 0xA, 0x1F, 0x11,        //д 101 208-180
0x0, 0x0, 0xC, 0x12, 0x1E, 0x10, 0xC,       //е 102 208-181
0x0, 0x0, 0x0, 0x15, 0xE, 0xE, 0x15,        //ж 103 208-182
0x0, 0x0, 0xC, 0x12, 0x4, 0x12, 0xC,        //з 104 208-183
0x0, 0x0, 0x11, 0x13, 0x15, 0x19, 0x11,     //и 105 208-184
};

enum REGIM1 {FON, ALARM, SETTING}; 
REGIM1 regim = FON;
enum  REGIM2 {OFF = 0, H, MI, CH, ME, Y, D};
REGIM2 ok = OFF;

boolean setting = false;
int h, mi, ch, me, y , d;
int num_ok = -1;
int knopka = -1;
boolean perevod = false;
int ledState = LOW; 
unsigned long previousMillis = 0;       
int mig_dvtoch = LOW;
unsigned long start_time = 0;
unsigned long start_time_clear = 0;
boolean show_perevod = false;
unsigned long start_time2 = 0;
boolean perevedeno = false;

enum REGIM3 {NOM = 1, HOU, MIN, POV}; 
REGIM3 ok3 = NOM;
int clock_hours1 = 99;
int clock_minutes1 = 99;
int clock_hours2 = 99;
int clock_minutes2 = 99;
int clock_weekday[7] = {-1, -1, -1, -1, -1, -1, -1};
boolean nd = false;
boolean pn = false;
boolean vt = false;
boolean sr = false;
boolean cht = false;
boolean pt = false;
boolean sb = false;
boolean bell = false;
int allarmm_1_2 = 1;
boolean alarm_1 = false;
boolean zvonok_alarm_1 = false;
unsigned long nachalo_alarm_time1 = 0;
unsigned long nachalo_alarm_time2 = 0;
int svetit1 = HIGH;
int svetit2 = HIGH;
boolean alarm = false;
unsigned long zvonok_start = 0;
#define zvuk 4

void setup() 
{
  IR.begin();// Инициируем работу с ИК-приёмником
  matrix.begin();
  Time.begin();
  Time.gettime(); 
  pinMode(zvuk, OUTPUT);
}

int clear_day1 = Time.day;
int clear_month1 = Time.month;

void loop() {
  Time.gettime();
  h = Time.Hours;
  mi = Time.minutes;
  ch = Time.day;
  me = Time.month;
  y = Time.year;
  d = Time.weekday;  
  knopka = -1;

  automatic_time_translation();
  clear_d_m();
  
  if (IR.check()) {
    switch(IR.data){
    case 0xFF02FD:  //ok
      knopka = 10; 
      break;
    case 0xFF629D: //up
      if(program_color == false && regim == FON && show_perevod == false) ink_dek_brightness(7);
      break;
    case 0xFFA857:  //douwn
      if(program_color == false && regim == FON && show_perevod == false) ink_dek_brightness(1);
      break;
    case 0xFFC23D:  //right
      if(regim == FON && show_perevod == false){
        matrix.fillScreen(matrix.Color333(0, 0, 0));
        vvedeno_cifr = 0;
        if(program_color){
          if(n1 != -1) r = n1;
          if(n2 != -1) g = n2; 
          if(n3 != -1) b = n3;
          if(n4 != -1) r2 = n4;
          if(n5 != -1) g2 = n5;
          if(n6 != -1) b2 = n6;
        }
        program_color = !program_color; 
      }
      break;
    case 0xFF22DD:  //left
      if(regim == FON && show_perevod == false){
        matrix.fillScreen(matrix.Color333(0, 0, 0));
        program_color = false;
        zahod = false;
        n1 = -1;
        n2 = -1;
        n3 = -1;
        n4 = -1;
        n5 = -1;
        n6 = -1;
        r = 0;
        g = 7;
        b = 0;
        r2 = 7;
        g2 = 7;
        b2 = 7;
        brightness_max = g;
      }
      break;
    case 0xFF6897:  //1
      knopka = 1;
      break;
    case 0xFF9867:  //2
      knopka = 2;
      break;
    case 0xFFB04F:  //3
      knopka = 3;
      break;
    case 0xFF30CF:  //4
      knopka = 4;
      break;
    case 0xFF18E7:   //5
      knopka = 5;
      break;
    case 0xFF7A85:   //6
      knopka = 6;
      break;
    case 0xFF10EF:  //7
      knopka = 7;
      break;
    case 0xFF38C7:  //8
      knopka = 8;
      break;
    case 0xFF5AA5:  //9
      knopka = 9;
      break;
    case 0xFF4AB5:  //0
      knopka = 0;
      if(regim == FON && program_color == false) {
        perevod = !perevod;
        perevedeno = false;
        show_perevod = true;
        matrix.fillScreen(matrix.Color333(0, 0, 0));
        start_time2 = millis();
      }
      break;
    case 0xFF42BD:  //*
      knopka = 15;
      matrix.fillScreen(matrix.Color333(0, 0, 0));
      if(!program_color) enter_clock();
      break;
    case 0xFF52AD: //#
      knopka = 16;
      matrix.fillScreen(matrix.Color333(0, 0, 0));
      if(!program_color) enter_settings();
      break;
    }
  }
  
  if(program_color && regim == FON && show_perevod == false){
    if(n1 != -1) r = n1;
    if(n2 != -1) g = n2; 
    if(n3 != -1) b = n3;
    if(n4 != -1) r2 = n4;
    if(n5 != -1) g2 = n5;
    if(n6 != -1) b2 = n6;
    to_program_color();
  }
  else auto_brightness();
   
  if(regim == FON && program_color == false) {
    if(!show_perevod){
      matrix.drawLine(0, 0, 0, matrix.height() - 1, white);
      matrix.drawLine(matrix.width() - 1, 0, matrix.width() - 1, matrix.height() - 1, white);
      matrix.drawLine(1, 0, matrix.width() - 2, 0, white);
      matrix.drawLine(1, 8, matrix.width() - 2, 8, white);
      matrix.drawLine(1, 16, matrix.width() - 2, 16, white);
      matrix.drawLine(1, 24, matrix.width() - 2, 24, white);
      matrix.drawLine(31, 1, 31, 7, white);
      enter_text(1, 1, Time.gettime("H"));
      blink_time2();
      enter_text(19, 1, Time.gettime("i"));
      ds18b20();
      enter_text(1, 9, Time.gettime("d.m.Y"));
      enter_text(1, 17, den[Time.weekday]);
      enter_text(1, 25, mes[Time.month - 1]); 
    }     
  }
  else if(regim == ALARM) {
    r = 5;
    g = 0;
    b = 0;
    show_alarm_clock();
    set_alarm_clock();  
    enter_text(47, 0, ":");
    
  }
  else if(regim == SETTING) {
      r = 5;
      g = 0;
      b = 0;
      r2 = 5;
      g2 = 5;
      b2 = 5;
      matrix.drawLine(0, 0, 0, matrix.height() - 1, white);
      matrix.drawLine(matrix.width() - 1, 0, matrix.width() - 1, matrix.height() - 1, white);
      matrix.drawLine(1, 0, matrix.width() - 2, 0, white);
      matrix.drawLine(1, 8, matrix.width() - 2, 8, white);
      matrix.drawLine(1, 16, matrix.width() - 2, 16, white);
      matrix.drawLine(1, 24, matrix.width() - 2, 24, white);
      matrix.drawLine(31, 1, 31, 7, white);
    if(num_ok != -1) num_ok_print();
    else blink_time();
    if(ok == H || ok == MI) enter_text(13, 1, ":");
    else blink_time2();
    set_clock();  
  }
  else if (regim == FON && program_color == true){
    enter_text(18, 0, "текст:");
    enter_text(24, 8, numb[r]);
    enter_text(30, 8, numb[g]);
    enter_text(36, 8, numb[b]);
    enter_text(18, 16, "рамка:");
    enter_text(24, 24, numb[r2]);
    enter_text(30, 24, numb[g2]);
    enter_text(36, 24, numb[b2]);
  }

  alarm_clock();
  if(bell) zvonok();
}

void enter_text(int x, int y, unsigned char* str)
{
  int element;
  
  for(int i = 0; str[i]; i++) {
    element = -1;
    
    if((int)str[i] == 208 && ((int)str[i+1] == 134 || (int)str[i+1] == 135 || (int)str[i+1] == 132)) element = (int)str[i+1] - 104;//
    else if((int)str[i] == 208 && (int)str[i+1] >= 144 && (int)str[i+1] <= 184) element = (int)str[i+1] - 79;//
    else if((int)str[i] == 208 && (int)str[i+1] >= 185 && (int)str[i+1] <= 191) element = (int)str[i+1] - 153;//
    else if((int)str[i] == 209 && (int)str[i+1] >= 128 && (int)str[i+1] <= 151) element = (int)str[i+1] - 128;//
    else if((int)str[i] == 194 && (int)str[i+1] == 176) element = 18;//
    else if((int)str[i] == 210 && ((int)str[i+1] == 144 || (int)str[i+1] == 145)) element = (int)str[i+1] - 128;//
    else if((int)str[i] >= 39 && (int)str[i] <= 64) element = (int)str[i];//
    else if((int)str[i] == 32 || (int)str[i] == 33 || (int)str[i] == 35) element = (int)str[i] - 8;//
    else if((int)str[i] == 126) element = 19;//
    
    if(element != -1) { 
      for(int ba = 0; ba < 7; ba++) {
        for(int bi = 0; bi < 5; bi++) {
          if(bitRead(set_char[element][ba],4 - bi)) {
            matrix.drawPixel(x + bi, y + ba, greeeen);
          }
          else {
            matrix.drawPixel(x + bi, y + ba, 0);
          }
        }
      }
      
      for(int bi = 0; bi < 5; bi++) { 
        for(int ba = 0; ba < 7; ba++) {
          if(bitRead(set_char[element][ba], 4 - bi)) ba = 7;
        }
      }
      x += 6;  
    } 
  }
}

void set_clock()
{
  perevedeno = false;
  int x;
  
  if(ok == D) x = enterD();
  else if(ok != OFF) x = enter();
  
  if((ok == H && x != -1) || (okk() == true && num_ok != -1 && ok == H)) {
    if(okk() == true && num_ok != -1) x = num_ok;
    if(x >= 0 && x <= 23) {
      Time.settime(0, mi, x, ch, me, y, d);
      ok = ok + 1;   
    }
    num_ok = -1;  
  }
  else if((ok == MI && x != -1) || (okk() == true && num_ok != -1 && ok == MI)) {
    if(okk() == true && num_ok != -1) x = num_ok;
    if(x >= 0 && x <= 59) {
      Time.settime(0, x, h, ch, me, y, d);
      ok = ok + 1;
    }  
    num_ok = -1;
  }
  else if((ok == CH && x != -1) || (okk() == true && num_ok != -1 && ok == CH)) {
    if(okk() == true && num_ok != -1) x = num_ok;
    if(x >= 1 && x <= 31) {
      Time.settime(0, mi, h, x, me, y, d);
      ok = ok + 1;   
    } 
    num_ok = -1;
  }
  else if((ok == ME && x != -1) || (okk() == true && num_ok != -1 && ok == ME)) {
    if(okk() == true && num_ok != -1) x = num_ok;
    if(x >= 1 && x <= 12) {
      Time.settime(0, mi, h, ch, x, y, d);
      ok = ok + 1;   
    } 
    num_ok = -1;
  }
  else if((ok == Y && x != -1 || okk() == true && num_ok != -1 && ok == Y)) {
    if(okk() == true && num_ok != -1) x = num_ok;
    if(x >= 0 && x <= 99) {
      Time.settime(0, mi, h, ch, me, x, d);
      ok = ok + 1;  
    }
    num_ok = -1;
  }   
  else if((ok == D && x != -1) || (okk() == true && num_ok != -1 && ok == D)) {
    if(okk() == true && num_ok != -1) x = num_ok;
    if(x >= 0 && x <= 6) {
      Time.settime(0, mi, h, ch, me, y, x);
      ok = OFF;
      regim = FON;
      num_ok = -1;
      setting = !setting; 
    }
  }
  else if(okk()){
    ok = ok + 1;
  }
   
  if(ok >= 7){
    ok = OFF;
    regim = FON;
    setting = !setting;
    num_ok = -1;    
  }
}

boolean okk()
{
  boolean okey = false; 
  if (knopka == 10) okey = true;
  
  return okey;
}

int enter()
{
  static int num1 = -1;
  static int num2 = -1;
  if(num2 != -1) {
    num1 = -1;
    num2 = -1;
  }
  int num = numeral();
  
  if(num != -1) {
    if(num_ok != -1) num2 = num1 * 10 + num;
    else {
      num1 = num;
      num_ok = num1;
    }
  }
  return num2;
}

int numeral()
{
  int n = -1;
  if(knopka >= 0 && knopka <= 9) n = knopka;
  return n;
}

int enterD()
{
  int num = -1;
  num = numeral();
  return num;
}

void enter_settings()
{
  setting = !setting;
  if(setting) {
    ok = H;
    num_ok = -1;
    regim = SETTING;
  }
  else { 
    Time.settime(0, mi, h, ch, me, y, d);
    ok = OFF;
    regim = FON;
    num_ok = -1;
  }
}

void blink_time()
{ 
  int x, y;
  unsigned char* v;
  
  switch(ok) {
  case 1:   
    enter_text(19, 1, Time.gettime("i"));
    enter_text(1, 9, Time.gettime("d")); 
    enter_text(37, 9, Time.gettime("Y"));
    enter_text(19, 9, Time.gettime("m"));
    enter_text(1, 17, den[Time.weekday]);
    x = 1;
    y = 1;   
    v = Time.gettime("H");
    break;
  case 2:   
    enter_text(1, 1, Time.gettime("H"));
    enter_text(1, 9, Time.gettime("d"));
    enter_text(19, 9, Time.gettime("m"));
    enter_text(37, 9, Time.gettime("Y"));
    enter_text(1, 17, den[Time.weekday]);
    x = 19;
    y = 1;  
    v = Time.gettime("i");
    break;
  case 3:
    enter_text(19, 1, Time.gettime("i"));
    enter_text(1, 1, Time.gettime("H"));  
    enter_text(37, 9, Time.gettime("Y"));
    enter_text(19, 9, Time.gettime("m"));
    enter_text(1, 17, den[Time.weekday]);
    x = 1;
    y = 9;
    v = Time.gettime("d");
    break;
  case 4:
    enter_text(19, 1, Time.gettime("i"));
    enter_text(1, 9, Time.gettime("d"));
    enter_text(1, 1, Time.gettime("H"));
    enter_text(37, 9, Time.gettime("Y"));
    enter_text(1, 17, den[Time.weekday]);
    x = 19;
    y = 9;
    v = Time.gettime("m");
    break;
  case 5:  
    enter_text(19, 1, Time.gettime("i"));
    enter_text(1, 9, Time.gettime("d"));
    enter_text(1, 1, Time.gettime("H"));
    enter_text(19, 9, Time.gettime("m"));
    enter_text(1, 17, den[Time.weekday]);
    x = 49;
    y = 9;
    v = Time.gettime("y");
    break;
  case 6:
    enter_text(19, 1, Time.gettime("i"));
    enter_text(1, 9, Time.gettime("d"));
    enter_text(1, 1, Time.gettime("H"));
    enter_text(37, 9, Time.gettime("Y"));
    enter_text(19, 9, Time.gettime("m"));
    x = 1;
    y = 17;
    v = den[Time.weekday];
    break;  
  }
  
  unsigned long currentMillis = millis(); 
  if (currentMillis - previousMillis >= 500) {
    previousMillis = currentMillis;
    if (ledState == LOW) {
      ledState = HIGH;
      enter_text(x, y, v);
    } else {
      ledState = LOW;
      if(ok == 6) enter_text(x, y, "         ");
      else enter_text(x, y, "  ");
    }
  } 
}

void num_ok_print()
{
  switch(ok) {
  case 1:
    enter_text(7, 1, " ");   
    enter_text(1, 1, numb[num_ok]);
    break;
  case 2:
    enter_text(25, 1, " ");   
    enter_text(19, 1, numb[num_ok]);
    break;
  case 3:
    enter_text(7, 9, " ");
    enter_text(1, 9, numb[num_ok]);
    break;
  case 4:
    enter_text(25, 9, "  ");
    enter_text(19, 9, numb[num_ok]);
    break;
  case 5:
    enter_text(55, 9, "  ");
    enter_text(49, 9, numb[num_ok]);
    break;
  }
}

void blink_time2()
{
  unsigned long finish_time = millis(); 
  if (finish_time - start_time >= 300) {
    start_time = finish_time;
    if (mig_dvtoch == LOW) {
      mig_dvtoch = HIGH;
      enter_text(13, 1, ":");
    } else {
      mig_dvtoch = LOW;   
      enter_text(13, 1, " ");
    }
  } 
}

void clear_d_m()
{
  unsigned long stop_time_clear = millis(); 
  if(stop_time_clear - start_time_clear >= 500){
    start_time_clear = stop_time_clear;                               
    if(clear_day1 != d) {
      clear_day1 = d;
      enter_text(1, 17, "         ");
    }
    if(clear_month1 != me){
      clear_month1 = me;
      enter_text(1, 25, "        ");
    }
  }
}

void automatic_time_translation()
{
  if(perevedeno == true && me == 10 && ch >= 25 && d == 0 && h == 4 && mi == 0 && Time.seconds == 0) perevedeno = false;
  
  if(perevod == true && me == 3 && ch >= 25 && d == 0 && h == 3 && mi == 0 && Time.seconds == 0){
    Time.settime(0, 0, 4, ch, 3, y, 0);
  }
  if(perevedeno == false && perevod == true && me == 10 && ch >= 25 && d == 0 && h == 3 && mi == 0 && Time.seconds == 0){
    Time.settime(0, 0, 2, ch, 10, y, 0);
    perevedeno = true;
  }
  
  if(show_perevod) print_perevod();
}

void print_perevod()
{
  r = 5;
  g = 0;
  b = 0;
  enter_text(19, 1, "АВТО");
  enter_text(13, 9, "ПЕРЕВОД");
  enter_text(7, 17, "ГОДИННИКА");
  r = 5;
  g = 0;
  b = 0;
  if(perevod) enter_text(7, 25, "ВКЛЮЧЕНО");
  if(!perevod) enter_text(7, 25, "ВИКЛЮЧЕНО");
  unsigned long finish_time2 = millis(); 
  if(finish_time2 - start_time2 >= 5000) {
    show_perevod = false;
    matrix.fillScreen(matrix.Color333(0, 0, 0));
  }
}

void set_alarm_clock()
{
  int x;
  
  if(ok3 == POV || ok3 == NOM) x = enterD();
  else x = enter();

  if(ok3 == NOM && x != -1){
    if(x >= 1 && x <= 2){
      matrix.fillScreen(matrix.Color333(0, 0, 0));
      allarmm_1_2 = x;
      ok3 = HOU; 
    }
  }  
  else if((ok3 == HOU && x != -1) || (okk() == true && num_ok != -1 && ok3 == HOU)) {
    if(okk() == true && num_ok != -1) x = num_ok;
    
    if(x >= 0 && x <= 23){
      if(allarmm_1_2 == 1){
        clock_hours1 = x;
      }
      else if(allarmm_1_2 == 2){
        clock_hours2 = x;
      }
      ok3 = MIN;   
    }
    num_ok = -1;  
  }
  else if((ok3 == MIN && x != -1) || (okk() == true && num_ok != -1 && ok3 == MIN)) {
    if(okk() == true && num_ok != -1) x = num_ok;
    
    if(x >= 0 && x <= 59){
      if(allarmm_1_2 == 1){
        clock_minutes1 = x;
      }
      else if(allarmm_1_2 == 2){
        clock_minutes2 = x;
      }
      ok3 = POV;
    }  
    num_ok = -1;
  }
  else if(ok3 == POV && x != -1 || (okk() == true && ok3 == POV)){
    if(x >= 0 && x <= 6 && allarmm_1_2 == 2){
      switch(x){
      case 0:
        nd = !nd;
        if(nd) clock_weekday[0] = 0;
        else{
          clock_weekday[0] = -1;
          enter_text(0, 16, "  ");
        }
        break;
      case 1:
        pn = !pn;
        if(pn) clock_weekday[1] = 1;
        else{
          clock_weekday[1] = -1;
          enter_text(14, 16, "  ");
        }
        break;
      case 2:
        vt = !vt;
        if(vt) clock_weekday[2] = 2;
        else{
          clock_weekday[2] = -1;
          enter_text(28, 16, "  ");
        }
        break;
      case 3:
        sr = !sr;
        if(sr) clock_weekday[3] = 3;
        else{
          clock_weekday[3] = -1;
          enter_text(42, 16, "  ");
        }
        break;
      case 4:
        cht = !cht;
        if(cht) clock_weekday[4] = 4;
        else{
          clock_weekday[4] = -1;
          enter_text(0, 24, "  ");
        }
        break;
      case 5:
        pt = !pt;
        if(pt) clock_weekday[5] = 5;
        else{
          clock_weekday[5] = -1;
          enter_text(14, 24, "  ");
        }
        break;
      case 6:
        sb = !sb;
        if(sb) clock_weekday[6] = 6;
        else{
          clock_weekday[6] = -1;
          enter_text(28, 24, "  ");
        }
        break;
      }
    }
    else if(okk() == true){
      ok3 = NOM;
      regim = FON;
      alarm = !alarm;
      matrix.fillScreen(matrix.Color333(0, 0, 0)); 
    }
  }
  else if(okk()){
    ok3 = ok3 + 1;
  }
   
  if(ok3 > 4){
    matrix.fillScreen(matrix.Color333(0, 0, 0));
    ok3 = NOM;
    regim = FON;
    alarm = !alarm;
    num_ok = -1;    
  }
}

void alarm_clock()
{
  if(clock_hours1 == Time.Hours && clock_minutes1 == Time.minutes && Time.seconds == 1){
    bell = true;
    zvonok_start = millis();
    alarm_1 = true;
    zvonok_alarm_1 = true;
  }
  
  for(int d = 0; bell == false && d <= 6; d++){
    if(clock_hours2 == Time.Hours && clock_minutes2 == Time.minutes && Time.seconds == 1 && clock_weekday[d] == Time.weekday){
      bell = true;
      zvonok_start = millis();
    }
  }
  
  if(bell == true && okk() == true){
    bell = false;
    digitalWrite(zvuk, LOW);
    if(alarm_1 && zvonok_alarm_1){
      clock_hours1 = 99;
      clock_minutes1 = 99;
      zvonok_alarm_1 = false;
    }
  }
}

void show_alarm_clock()
{
  enter_text(0, 0, "буд.");
  
  if(allarmm_1_2 == 1){
    
    if(ok3 == NOM) blink_alarm_time1(25, 0, "1");
    else enter_text(25, 0, "1");
    
    if(num_ok != -1 && ok3 == HOU){
      enter_text(35, 0, numb[num_ok]);
      enter_text(41, 0, " ");
    }
    else {
      if(ok3 == HOU) blink_alarm_time2(35, 0, numb[clock_hours1 / 10], numb[clock_hours1 % 10]);
      else{
        enter_text(35, 0, numb[clock_hours1 / 10]);
        enter_text(41, 0, numb[clock_hours1 % 10]);
      }
    }
    
    if(num_ok != -1 && ok3 == MIN){
      enter_text(53, 0, numb[num_ok]);
      enter_text(59, 0, " ");
    }
    else{
      if(ok3 == MIN) blink_alarm_time2(53, 0, numb[clock_minutes1 / 10], numb[clock_minutes1 % 10]);
      else{
        enter_text(53, 0, numb[clock_minutes1 / 10]);
        enter_text(59, 0, numb[clock_minutes1 % 10]);
      } 
    }
  }
  
  else if(allarmm_1_2 == 2){
    
    if(ok3 == NOM) blink_alarm_time1(25, 0, "2");
    else enter_text(25, 0, "2");
    
    if(num_ok != -1 && ok3 == HOU){
      enter_text(35, 0, numb[num_ok]);
      enter_text(41, 0, " ");
    }
    else {
      if(ok3 == HOU) blink_alarm_time2(35, 0, numb[clock_hours2 / 10], numb[clock_hours2 % 10]);
      else{
        enter_text(35, 0, numb[clock_hours2 / 10]);
        enter_text(41, 0, numb[clock_hours2 % 10]);
      }
    }
    
    if(num_ok != -1 && ok3 == MIN){
      enter_text(53, 0, numb[num_ok]);
      enter_text(59, 0, " ");
    }
    else{
      if(ok3 == MIN) blink_alarm_time2(53, 0, numb[clock_minutes2 / 10], numb[clock_minutes2 % 10]);
      else{
        enter_text(53, 0, numb[clock_minutes2 / 10]);
        enter_text(59, 0, numb[clock_minutes2 % 10]);
      } 
    }

    if(ok3 == POV) blink_alarm_time1(12, 8, "ПОВТОР:");
    else enter_text(12, 8, "ПОВТОР:");
    
    
    if(nd) enter_text(0, 16, "НД");
    if(pn) enter_text(14, 16, "ПН");
    if(vt) enter_text(28, 16, "ВТ");
    if(sr) enter_text(42, 16, "СР");
    if(cht) enter_text(0, 24, "ЧТ");
    if(pt) enter_text(14, 24, "ПТ");
    if(sb) enter_text(28, 24, "СБ");
  }
}

void enter_clock()
{
  alarm = !alarm;
  if(alarm) {
    ok3 = NOM;
    num_ok = -1;
    regim = ALARM;
  }
  else { 
    ok3 = NOM;
    regim = FON;
    num_ok = -1;
  }
}

void blink_alarm_time1(int g, int v, unsigned char* s)
{
  unsigned long proshlo_alarm_time = millis(); 
  if (proshlo_alarm_time - nachalo_alarm_time1 >= 500) {
    nachalo_alarm_time1 = proshlo_alarm_time;
    if (svetit1 == LOW) {
      svetit1 = HIGH;
      enter_text(g, v, s);
    } else {
      svetit1 = LOW;
      if(ok3 == 4) enter_text(g, v, "       ");
      else if (ok3 == 1)enter_text(g, v, " ");
    }
  } 
}

void blink_alarm_time2(int g, int v, unsigned char* s1, unsigned char* s2)
{
  unsigned long proshlo_alarm_time = millis(); 
  if (proshlo_alarm_time - nachalo_alarm_time2 >= 500) {
    nachalo_alarm_time2 = proshlo_alarm_time;
    if (svetit2 == LOW) {
      svetit2 = HIGH;
      enter_text(g, v, s1);
      enter_text(g + 6, v, s2);
    } else {
      svetit2 = LOW;
      enter_text(g, v, "  ");
    }
  } 
}

void zvonok()
{
  unsigned long zvonok_finish = millis();
  if (zvonok_finish - zvonok_start >= 170000) {
    bell = false;
    digitalWrite(zvuk, LOW);
    if(alarm_1 && zvonok_alarm_1){
      clock_hours1 = 99;
      clock_minutes1 = 99;
      zvonok_alarm_1 = false;
    }
  }
  else{
    digitalWrite(zvuk, HIGH);
    digitalWrite(zvuk, LOW);
  }
}

void auto_brightness()
{
  if(program_color == false && regim == FON && show_perevod == false){
    if(h >= 0 && h < 6){
      if(n1 != -1) r = n1;
      else r = 0;
      if(n2 != -1) g = n2; 
      else g = 1;
      if(n3 != -1) b = n3;
      else b = 0;
      if(n4 != -1) r2 = n4;
      else r2 = 1;
      if(n5 != -1) g2 = n5;
      else g2 = 1;
      if(n6 != -1) b2 = n6;
      else b2 = 1;
    }
    else{
      if(n1 != -1) r = n1;
      else r = 0;
      if(n2 != -1) g = n2; 
      else g = brightness_max;
      if(n3 != -1) b = n3;
      else b = 0;
      if(n4 != -1) r2 = n4;
      else r2 = brightness_max;
      if(n5 != -1) g2 = n5;
      else g2 = brightness_max;
      if(n6 != -1) b2 = n6;
      else b2 = brightness_max;
    }
  }
}

void to_program_color()
{
  if(regim == FON && show_perevod == false){
    int x = numeral();
     if(vvedeno_cifr == 0 && x != -1 && x >= 0 && x <= 7){
       n1 = x;
       r = n1;
       vvedeno_cifr = 1;
     }
     else if(vvedeno_cifr == 1 && x != -1 && x >= 0 && x <= 7){
      n2 = x;
      g = n2;
      vvedeno_cifr = 2;
     }
     else if(vvedeno_cifr == 2 && x != -1 && x >= 0 && x <= 7){ 
      n3 = x;
      b = n3;
      vvedeno_cifr = 3;
     }
     else if(vvedeno_cifr == 3 && x != -1 && x >= 0 && x <= 7){ 
      n4 = x;
      r2 = n4;
      vvedeno_cifr = 4;
     }
     else if(vvedeno_cifr == 4 && x != -1 && x >= 0 && x <= 7){
      n5 = x;
      g2 = n5;
      vvedeno_cifr = 5;
     }
     else if(vvedeno_cifr == 5 && x != -1 && x >= 0 && x <= 7){
      n6 = x;
      b2 = n6;
      vvedeno_cifr = 6;
     }
     if(vvedeno_cifr == 6) vvedeno_cifr = 0;
  }
}

void ink_dek_brightness(int ink_dek)
{
  if (ink_dek == 7){
    if(r < 7) r++;
    if(g < 7) g++;
    if(b < 7) b++;
    if(r2 < 7) r2++;
    if(g2 < 7) g2++;
    if(b2 < 7) b2++;
    brightness_max = g;
  }
  else if (ink_dek == 1){
    if(r > 1) r--;
    if(g > 1) g--;
    if(b > 1) b--;
    if(r2 > 1) r2--;
    if(g2 > 1) g2--;
    if(b2 > 1) b2--;
    brightness_max = g;
  }
}

void ds18b20(){
  byte data[2];
  ds.reset(); 
  ds.write(0xCC);
  ds.write(0x44);
  //delay(750);
  
  ds.reset();
  ds.write(0xCC);
  ds.write(0xBE);
  data[0] = ds.read(); 
  data[1] = ds.read();
  int Temp = (data[1]<< 8)+data[0];
  Temp = Temp>>4;
  Temp = Temp - 1;
  if(Temp < 0){
    enter_text(33, 1, "-");
  }
  else if(Temp > 0){
    enter_text(33, 1, "+");
  }
  else if(Temp == 0){
    enter_text(33, 1, " ");
  }
  
  if(Temp / 10 > 0) enter_text(39, 1, numb[Temp / 10]);
  else if(Temp / 10 == 0 && Temp > 0) enter_text(39, 1, "+");
  else if(Temp / 10 == 0 && Temp < 0) enter_text(39, 1, "-");
  else if(Temp / 10 == 0 && Temp == 0) enter_text(39, 1, " ");
  enter_text(45, 1, numb[Temp % 10]);
  enter_text(51, 1, "°С");
}

