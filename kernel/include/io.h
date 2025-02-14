#ifndef _IO_H
#define _IO_H
#include <define.h>
// screen.c
#define argb(a, r, g, b) ((a) << 24 | (r) << 16 | (g) << 8 | (b))
void clear();
int getlength(const char* str);
void putstr(const char* str, int length);
void init_s_ne_t();
int Get_S_NE_T();
void printchar(char ch);
void outputchar_b800(int x, int y, char ch);
void putchar(char ch);
void Move_Cursor(short x, short y);
void printOld(const char* str);
void screen_ne_cons();
void screen_ne();
int get_x();
int get_y();
int Get_Raw_x();
int Get_Raw_y();
void print(const char* str);
void GotoXy_No_Safe(int x1, int y1);
void gotoxy(int x1, int y1);
// tty.c
void clear_tty(struct tty* tty);
void screen_ne_tty(struct tty* tty);
void putstr_tty(const char* str, int length, struct tty* tty);
void print_tty(const char* str, struct tty* tty);
void putchar_tty(char ch, struct tty* tty);
void GotoXy_No_Safe_tty(int x1, int y1, struct tty* tty);
void gotoxy_tty(int x1, int y1, struct tty* tty);
// log.c
void kprint(char* str);
void printk(char* str, ...);
// input.c
void input(char* ptr, int len);
void input_No_El(char* ptr, int len);
// Input_Stack.c
void Input_Stack_Init(void);
void Input_Stack_Put(char* str);
int Get_Now();
int Get_times();
char* Pop_Stack();
void Set_Now(int now);
int get_free();
// HighTextMode.c
void High_clear(void);
void High_Screen_ne(void);
void HighPrint(char* string);
void HighPutStr(char* string, int length);
void HighGotoXY(int x, int y);
void Gar_Test_Task();
void SwitchToHighTextMode();
// Draw.c
void Draw_Circle_Hollow_32(unsigned char* vram,
                           int xsize,
                           int x,
                           int y,
                           int r,
                           int Cr,
                           int Cg,
                           int Cb,
                           int br,
                           int bg,
                           int bb);
void black(unsigned char* vram, int xsize, int ysize);
void PUTCHINESE(int x, int y, char color, unsigned short CH);
void PUTCHINESE0(vram_t* vram,
                 int x,
                 int y,
                 color_t color,
                 unsigned short CH,
                 int xsize);
void PutChineseChar0(vram_t* vram,
                     int xsize,
                     int x,
                     int y,
                     color_t color,
                     unsigned short cChar);
void PrintChineseChar(int x, int y, char color, unsigned short Cchar);
void PutChineseStr0(vram_t* vram,
                    int xsize,
                    int x,
                    int y,
                    color_t color,
                    unsigned char* str);
void PrintChineseStr(int x, int y, char color, unsigned char* str);
void Draw_Px(int x, int y, char color);
void Draw_Px_32(unsigned char* buf,
                int x,
                int y,
                char r,
                char g,
                char b,
                int xsize);
void Draw_Box(int x, int y, int w, int h, char color);
void Draw_Box32(unsigned char* vram,
                int xsize,
                int x0,
                int y0,
                int x1,
                int y1,
                int Cr,
                int Cg,
                int Cb);
void Draw_Char(int x, int y, char c, char color);
void Draw_Char_32(unsigned char* vram1,
                  int xsiz,
                  char c,
                  int x,
                  int y,
                  int r,
                  int g,
                  int b);
void Draw_Str(int x, int y, char* str, char color);
void SDraw_Px(vram_t* vram, int x, int y, color_t color, int xsize);
void SDraw_Box(vram_t* vram,
               int x,
               int y,
               int x1,
               int y1,
               color_t color,
               int xsize);
void SDraw_Char(vram_t* vram1,
                int x,
                int y,
                char c,
                color_t color,
                int xsize);
void Sputs(vram_t* vram, char* str, int x, int y, color_t col, int xsize);
void putfonts_asc_sht(struct SHEET* sht,
                       int x,
                       int y,
                       color_t c,
                       color_t b,
                       char* s,
                       int l);
void putfonts_asc(vram_t* vram,
                   int xsize,
                   int x,
                   int y,
                   color_t c,
                   unsigned char* s);
void putfont(vram_t* vram, int xsize, int x, int y, color_t c, char* font);
void Draw_Line_32(uint8_t* Buffer,
                  int x1,
                  int y1,
                  int x2,
                  int y2,
                  int r,
                  int g,
                  int b,
                  int xsize);
void Draw_Line(vram_t* Buffer,
                 int x1,
                 int y1,
                 int x2,
                 int y2,
                 color_t c,
                 int xsize);
void putfonts_asc_cons(vram_t* vram,
                        int xsize,
                        int x,
                        int y,
                        color_t c,
                        unsigned char* s);
void putfont_cons(vram_t* vram, int xsize, int x, int y, color_t c, char* font);
// sheet.c
struct SHEET* sheet_alloc(struct SHTCTL* ctl);
void sheet_free(struct SHEET* sht);
void ctl_free(struct SHTCTL* ctl);
struct SHTCTL* shtctl_init(vram_t* vram, int xsize, int ysize);
void sheet_setbuf(struct SHEET* sht,
                  vram_t* buf,
                  int xsize,
                  int ysize,
                  int col_inv);
void sheet_slide(struct SHEET* sht, int vx0, int vy0);
void sheet_updown(struct SHEET* sht, int height);
void sheet_refresh(struct SHEET* sht, int bx0, int by0, int bx1, int by1);
int HighPrintf(const char* format, ...);
#endif
