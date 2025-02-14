// Graphic.c : 图形界面的简单实现
// Windows 2000 风格
#include <dos.h>
#include <gui.h>
struct VBEINFO* vbinfo = NULL;
char mouse_cur_graphic[16][16] = {
    "*...............", "**..............", "*O*.............",
    "*OO*............", "*OOO*...........", "*OOOO*..........",
    "*OOOOO*.........", "*OOOOOO*........", "*OOOOOOO*.......",
    "*OOOO*****......", "*OO*O*..........", "*O*.*O*.........",
    "**..*O*.........", "*....*O*........", ".....*O*........",
    "......*........."};
vram_t *buf_back, *buf_win, *buf_cur, *buf_fm;
vram_t buf_mouse[256];
vram_t* buf_win_src;
struct SHEET *sht_back, *sht_mouse, *sht_win, *sht_cur, *sht_fm;
struct SHEET* sht;
struct SHTCTL* shtctl;
struct SHEET* sht_win_;
struct SHEET* sht_b_cur;
List* list;   // Button
List* list1;  // TextBox
List* list2;  // NoFrameButton
struct SHTCTL* close_tid;
Button* button_cl;  // 目前按下的按钮
listBox_t* list_fm;
extern char drive;
static int small_window[32] = {NULL};
static void fm_handler();
static void reset_fm(listBox_t* fm) {
  uint8_t item[24];
  for (int i = 0; i != 24; i++)
    item[i] = ' ';
  item[23] = NULL;
  for (uint8_t* p = (uint8_t*)NowTask()->directory; *p != NULL; p += 32) {
    memcpy(item, p, 8);
    memcpy(item + 8 + 3, p + 8, 3);
    if ((*(p + 11) & 0x18) == 0) {
      memcpy(item + 8 + 3 + 3 + 3, "<FILE>", 6);
    }
    if (*(p + 11) == 0x10) {
      memcpy(item + 8 + 3 + 3 + 3, "<DIR> ", 6);
    }
    AddItem(list_fm, item, fm_handler);
  }
}
static void fm_handler() {
  extern Button* button_cl;
  listBox_t* listBox = list_fm;
  for (int i = listBox->now_min; i != listBox->now_max; i++) {
    if (listBox->btns[i] == button_cl) {
      uint32_t cmdline_tid = Get_Tid(GetTaskForName("Cmdline"));
      if (strcmp(listBox->item[i] + 17, "<FILE>") == 0) {
        listBox->item[i][14] = NULL;
        if (strcmp(listBox->item[i] + 11, "BIN") == 0) {
          unsigned char name[9];
          name[8] = NULL;
          memcpy(name, listBox->item[i], 8);
          for (int i = 0; i != 8; i++) {
            if (name[i] == ' ') {
              name[i] = NULL;
              break;
            }
          }
          SendIPCMessage(cmdline_tid, name, strlen(name), asynchronous);
          SendIPCMessage(cmdline_tid, "BIN", 3, asynchronous);  // 优先取后发的
        } else if (strcmp(listBox->item[i] + 11, "JPG") == 0) {
          unsigned char name[15];
          memcpy(name, listBox->item[i], 14);
          unsigned char tmp[8 + 12 + 1];
          strcpy(tmp, "jpgview ");
          name[8] = NULL;
          for (int i = 0; i != 8; i++) {
            if (name[i] == ' ') {
              name[i] = NULL;
              break;
            }
          }
          memcpy((void*)(tmp + 8), (void*)name, strlen(name));
          tmp[8 + strlen(name)] = '.';
          memcpy((void*)(tmp + 8 + strlen(name) + 1), (void*)(name + 8 + 3), 3);
          tmp[8 + strlen(name) + 4] = NULL;
          SendIPCMessage(cmdline_tid, tmp, strlen(tmp), asynchronous);
          SendIPCMessage(cmdline_tid, "JPG", 3, asynchronous);
        } else if (strcmp(listBox->item[i] + 11, "BAT") == 0) {
          unsigned char name[9];
          name[8] = NULL;
          memcpy(name, listBox->item[i], 8);
          for (int i = 0; i != 8; i++) {
            if (name[i] == ' ') {
              name[i] = NULL;
              break;
            }
          }
          SendIPCMessage(cmdline_tid, name, strlen(name), asynchronous);
          SendIPCMessage(cmdline_tid, "BAT", 3, asynchronous);  // 优先取后发的
        } else {
        }
        listBox->item[14] = ' ';
      } else if (strcmp(listBox->item[i] + 17, "<DIR> ") == 0) {
        for (int j = 0; j != 11; j++) {
          if (listBox->item[i][j] == ' ') {
            listBox->item[i][j] = NULL;
          }
        }
        SendIPCMessage(cmdline_tid, listBox->item[i], strlen(listBox->item[i]),
                       asynchronous);
        SendIPCMessage(cmdline_tid, "DIR", 3, asynchronous);
      }
    }
  }
}
static void cmdline(void) {
  clean(line, 1024);
  if (fork() != 0) {
    uint32_t shell_tid = Get_Tid(GetTaskForName("Shell"));
    while (1) {
      if (IPCMessageStatus() != 0 && IPCMessageLength(shell_tid) == 3) {
        unsigned char com[4];
        com[3] = NULL;
        GetIPCMessage(com, shell_tid);
        if (strcmp(com, "BIN") == 0) {
          unsigned char name[9];
          clean(name, 9);
          GetIPCMessage(name, shell_tid);
          struct TASK* thread;
          for (int i = 1; GetTask(i) != NULL; i++) {
            thread = GetTask(i);
            if (thread->is_child == 1 && thread->thread.father == NowTask()) {
              SleepTask(thread);
              break;
            }
          }
          thread->is_child = 0;  // 暂时让你不当儿子
          thread->thread.father = NULL;
          cmd_app(name);
          thread->is_child = 1;  // 你还是我孩子哈哈哈
          thread->thread.father = NowTask();
          WakeUp(thread);
        } else if (strcmp(com, "BAT") == 0) {
          unsigned char name[9];
          clean(name, 9);
          GetIPCMessage(name, shell_tid);
          run_bat(name);
        } else if (strcmp(com, "JPG") == 0) {
          unsigned char tmp[21];
          clean(tmp, 21);
          GetIPCMessage(tmp, shell_tid);
          command_run(tmp);
        } else if (strcmp(com, "DIR") == 0) {
          /*static uint32_t all;
          all = list_fm->item_num;
          unsigned char dict[12];
          clean(dict, 12);
          GetIPCMessage(dict, shell_tid);
          changedict(dict);
          for (int j = 0; j != all; j++) {
            RemoveItem(list_fm, 0);
          }
          reset_fm(list_fm);
          printf("\n%c:\\%s>", drive, path);*/
          static uint32_t all;
          all = list_fm->item_num;
          unsigned char dict[12];
          clean(dict, 12);
          GetIPCMessage(dict, shell_tid);
          changedict(dict);
          int bx, by, bw, bh;
          bx = list_fm->x;
          by = list_fm->y;
          bw = list_fm->width;
          bh = list_fm->height;
          struct SHEET* sheet;
          sheet = list_fm->sheet;
          DeleteListBox(list_fm);
          list_fm = MakeListBox(sheet, bx, by, bw, bh);
          reset_fm(list_fm);
        }
      }
    }
  }
  command_run("ver");
  struct TASK* task = NowTask();
  while (1) {
    printchar(task->drive);
    print(":\\");
    print(task->path);
    print(">");
    int i;
    clean(line, 1024);
    input(line, 1024);
    command_run(line);
  }
}
void init_screen(vram_t* vram, int x, int y) {
  // 壁纸
  SDraw_Box(vram, 0, 0, x, y, argb(0, 58, 110, 165), x);
  // 画任务栏
  boxfill(vram, x, COL_C6C6C6, 0, y - 28, x - 1, y - 28);
  boxfill(vram, x, COL_FFFFFF, 0, y - 27, x - 1, y - 27);
  boxfill(vram, x, COL_C6C6C6, 0, y - 26, x - 1, y - 1);
}
void init_mouse_cursor(vram_t* mouse, int bc) {
  int x, y;

  for (y = 0; y < 16; y++) {
    for (x = 0; x < 16; x++) {
      if (mouse_cur_graphic[y][x] == '*') {
        mouse[y * 16 + x] = COL_000000;
      }
      if (mouse_cur_graphic[y][x] == 'O') {
        mouse[y * 16 + x] = COL_FFFFFF;
      }
      if (mouse_cur_graphic[y][x] == '.') {
        mouse[y * 16 + x] = bc;
      }
    }
  }
  return;
}
static void small_window_handler() {
  int cnt = button_cl->x / 32;
  sheet_updown((struct SHEET*)(small_window[cnt]), shtctl->top);
  DeleteButton(button_cl);
  small_window[cnt] = NULL;
}
int gmx, gmy;  // 鼠标的坐标
void graphic(void) {
  if (set_mode(1024, 768, 32) != 0) {
    printf("Can't enable 1024x768x32 VBE mode.\n\n");
    return;
  }
  gotoxy(0, 0);
  io_cli();
  // 不定 -> 定值
  vbinfo = (struct VBEINFO*)page_malloc(sizeof(struct VBEINFO));
  struct VBEINFO* vbinfo0 = (struct VBEINFO*)VBEINFO_ADDRESS;
  vbinfo->vram = vbinfo0->vram;
  vbinfo->xsize = vbinfo0->xsize;
  vbinfo->ysize = vbinfo0->ysize;

  // 桌面
  int new_mx = 0, new_my = 0;
  int new_wx = 0x7fffffff, new_wy = 0, mmx2 = 0;
  int x, y;
  bool button_clicking = false;
  bool sheet_updown_flags = true;
  struct FIFO8 fifo;
  int fifobuf[128];
  int mmx = -1, mmy = -1;
  fifo8_init(&fifo, 128, fifobuf);
  sht = (struct SHEET*)NULL;
  shtctl = shtctl_init((vram_t*)vbinfo->vram, vbinfo->xsize, vbinfo->ysize);
  static struct SHTCTL* shtctl2;
  sht_back = sheet_alloc(shtctl);
  sht_mouse = sheet_alloc(shtctl);
  sht_win = sheet_alloc(shtctl);
  sht_win->Close = NULL;
  buf_back =
      (vram_t*)page_malloc(vbinfo->xsize * vbinfo->ysize * sizeof(color_t));
  buf_win = (vram_t*)page_malloc(CMDLINEXSIZE * CMDLINEYSIZE * sizeof(color_t));
  buf_win_src =
      (vram_t*)page_malloc(CMDLINEXSIZE * CMDLINEYSIZE * sizeof(color_t));
  buf_cur = (vram_t*)page_malloc(135 * 75 * sizeof(color_t));
  vram_t* b_cur = (vram_t*)page_malloc(16 * 8 * sizeof(color_t));
  shtctl2 = shtctl_init((vram_t*)buf_win_src, CMDLINEXSIZE, CMDLINEYSIZE);
  sht_win_ = sheet_alloc(shtctl2);
  sheet_setbuf(sht_win_, buf_win, CMDLINEXSIZE, CMDLINEYSIZE, -1);
  sheet_slide(sht_win_, 0, 0);
  sheet_updown(sht_win_, 0);
  sht_b_cur = sheet_alloc(shtctl2);
  sheet_setbuf(sht_b_cur, b_cur, 8, 16, -1);
  for (int i = 0; i < 16 * 8; i++) {
    b_cur[i] = COL_FFFFFF;
  }
  sheet_slide(sht_b_cur, 5, 24);
  sheet_slide(sht_b_cur, 5, 24);
  sheet_updown(sht_b_cur, -1);
  sheet_setbuf(sht_back, buf_back, vbinfo->xsize, vbinfo->ysize,
               -1); /* 没有透明色 */
  sheet_setbuf(sht_mouse, buf_mouse, 16, 16, COL_TRANSPARENT); /* 透明色号99 */
  sheet_setbuf(sht_win, buf_win_src, CMDLINEXSIZE, CMDLINEYSIZE, -1);
  init_screen(buf_back, vbinfo->xsize, vbinfo->ysize);
  init_mouse_cursor((vram_t*)buf_mouse, COL_TRANSPARENT); /* 背景色号99 */
  make_window(buf_win, CMDLINEXSIZE, CMDLINEYSIZE, "控制台");
  boxfill(buf_win, CMDLINEXSIZE, COL_000000, 5, 24, CMDLINEXSIZE - 5,
          CMDLINEYSIZE - 5);
  sheet_slide(sht_back, 0, 0);
  gmx = (vbinfo->xsize - 16) / 2; /* 按显示在画面中央来计算坐标 */
  gmy = (vbinfo->ysize - 28 - 16) / 2;
  sheet_slide(sht_mouse, gmx, gmy);
  sheet_slide(sht_win, 100, 100);
  sheet_slide(sht_cur, 0, 0);
  sheet_updown(sht_back, 0);
  sheet_updown(sht_win, 1);
  sheet_updown(sht_mouse, 2);
  sheet_updown(sht_cur, 1);
  putfonts_asc(buf_back, vbinfo->xsize, 0, 0, COL_FFFFFF,
               (unsigned char*)"PowerintDOS桌面");
  putfonts_asc(buf_back, vbinfo->xsize, 0, 1 * 16, COL_FFFFFF,
               (unsigned char*)"基于内核：");
  putfonts_asc(buf_back, vbinfo->xsize, 0, 2 * 16, COL_FFFFFF,
               (unsigned char*)"Powerint DOS 386");
  putfonts_asc(buf_back, vbinfo->xsize, 0, 3 * 16, COL_FFFFFF,
               (unsigned char*)"内核版本:0.6c");
  putfonts_asc(buf_back, vbinfo->xsize, 0, 4 * 16, COL_FFFFFF,
               (unsigned char*)"Copyright(C) 2022 min0911_ & zhouzhihao");
  sheet_refresh(sht_win_, 0, 0, CMDLINEXSIZE, CMDLINEYSIZE);
  sheet_refresh(sht_win, 0, 0, CMDLINEXSIZE, CMDLINEYSIZE);
  sheet_refresh(sht_back, 0, 0, vbinfo->xsize, vbinfo->ysize); /* 刷新文字 */
                                                               // for(;;);
  // 控件
  list = NewList();
  list1 = NewList();
  list2 = NewList();
  init_listBox();
  // cmdline
  char* cmdline_esp = (char*)page_malloc(32 * 1024);
  struct TASK* Task_cmdline = AddTask("Cmdline", 2, 2 * 8, (int)cmdline, 1 * 8,
                                      1 * 8, (int)cmdline_esp);
  char* cmdline_kfifo = (struct FIFO8*)page_malloc(sizeof(struct FIFO8));
  char* cmdline_mfifo = (struct FIFO8*)page_malloc(sizeof(struct FIFO8));
  char* cmdline_kbuf = (char*)page_malloc(128);
  char* cmdline_mbuf = (char*)page_malloc(128);
  fifo8_init(cmdline_kfifo, 128, cmdline_kbuf);
  fifo8_init(cmdline_mfifo, 128, cmdline_mbuf);
  TaskSetFIFO(Task_cmdline, cmdline_kfifo, cmdline_mfifo);
  int alloc_addr = (int)page_malloc(512 * 1024);
  char* memman = (char*)page_malloc(4 * 1024);
  Task_cmdline->alloc_addr = alloc_addr;
  Task_cmdline->memman = memman;
  sht_win->task = Task_cmdline;
  sheet_slide(sht_cur, 188, 172 + 28);
  mouse_ready(&mdec);
  struct TIMER* timer;
  timer = timer_alloc();
  timer_init(timer, &fifo, 1);
  timer_settime(timer, 500);
  // file mananger
  buf_fm = (vram_t*)page_malloc(FMXSIZE * FMYSIZE * sizeof(color_t));
  sht_fm = MakeWindow(200, 200, FMXSIZE, FMYSIZE, "文件管理器", shtctl, buf_fm,
                      NULL);
  list_fm = MakeListBox(sht_fm, 6, 28, 320, 320);
  reset_fm(list_fm);

  //gui_login();
  io_sti();
  for (;;) {
    Maskirq(0);
    char s[100];
    sprintf(s, "日期:20%02x年%02x月%02x日 时间:%02x时%02x分%02x秒",
            read_cmos(9), read_cmos(8), read_cmos(7), read_cmos(4),
            read_cmos(2), read_cmos(0));
    boxfill(buf_back, vbinfo->xsize, COL_C6C6C6, 2, vbinfo->ysize - 18,
            2 + strlen(s) * 8, vbinfo->ysize - 2);
    putfonts_asc(buf_back, vbinfo->xsize, 2, vbinfo->ysize - 18, COL_000000, s);
    sheet_refresh(sht_back, 2, vbinfo->ysize - 18, 2 + strlen(s) * 8,
                  vbinfo->ysize - 2);
    /*if (get_login_flag() == 1) {
      sheet_updown(sht_fm, 1);
      sheet_updown(sht_win, 1);
      clean(line, 1024);
    } else if (get_login_flag() == 0) {
      sheet_updown(sht_fm, -1);
      sheet_updown(sht_win, -1);
    }*/
    if (fifo8_status(TaskGetMousefifo(NowTask())) +
            fifo8_status(TaskGetKeyfifo(NowTask())) + fifo8_status(&fifo) ==
        0) {
      if (new_mx >= 0) {
        // io_sti();
        sheet_slide(sht_mouse, new_mx, new_my);
        new_mx = -1;
      } else if (new_wx != 0x7fffffff) {
        // io_sti();
        sheet_slide(sht, new_wx, new_wy);
        new_wx = 0x7fffffff;
      } else {
      }
      WakeUp(NowTask());
      io_sti();
    } else {
      if (fifo8_status(TaskGetMousefifo(NowTask())) != 0) {
        // printk("Mouse.\n");
        int i = fifo8_get(TaskGetMousefifo(NowTask()));
        if (mouse_decode(&mdec, i) != 0) {
          // io_cli();
          SleepTaskFIFO(NowTask());
          gmx += mdec.x;
          gmy += mdec.y;
          if (gmx > vbinfo->xsize - 10) {
            gmx = vbinfo->xsize - 10;
          } else if (gmx < 0) {
            gmx = 0;
          }
          if (gmy > vbinfo->ysize - 16) {
            gmy = vbinfo->ysize - 16;
          } else if (gmy < 0) {
            gmy = 0;
          }
          new_mx = gmx;
          new_my = gmy;
          for (int i = 1; FindForCount(i, list2) != NULL; i++) {
            Button* btn = (Button*)FindForCount(i, list2)->val;
            if (Button_Click(btn, gmx, gmy)) {
              Button_Draw_NoFrame_Clicking(btn);
            } else {
              Button_Draw_NoFrame(btn);
            }
          }
          if (mdec.roll == MOUSE_ROLL_DOWN) {
            ListBox_Roll_Down();
          } else if (mdec.roll == MOUSE_ROLL_UP) {
            ListBox_Roll_Up();
          }
          if ((mdec.btn & 0x01) != 0) {
            sheet_updown_flags = true;
            for (int i = 1; FindForCount(i, list) != NULL; i++) {
              Button* btn = (Button*)FindForCount(i, list)->val;
              if (Button_Click(btn, gmx, gmy)) {
                Button_Draw_Clicking(btn);
                btn->is_clicking = true;
                button_clicking = true;
                sheet_updown_flags = false;
              } else {
                if (btn->is_clicking) {
                  Button_Draw(btn);
                  btn->is_clicking = false;
                }
              }
            }
            for (int i = 1; FindForCount(i, list2) != NULL; i++) {
              Button* btn = (Button*)FindForCount(i, list2)->val;
              if (Button_Click(btn, gmx, gmy)) {
                if (btn->OnClick != NULL && btn->clicking_first) {
                  sheet_updown_flags = false;
                  button_cl = btn;
                  btn->OnClick(btn);
                }
                btn->clicking_first ^= 1;
              } else if (btn->clicking_first) {
                btn->clicking_first = 0;
              }
            }
            for (int i = 1; FindForCount(i, list1) != NULL; i++) {
              TextBox* txt = (TextBox*)FindForCount(i, list1)->val;
              if (TextBox_Click(txt, gmx, gmy)) {
                txt->is_clicking = true;
              } else {
                txt->is_clicking = false;
              }
            }

            /*如果处于通常模式*/
            /*按照从上到下的顺序寻找鼠标所指向的图层*/
            if (mmx < 0 && sheet_updown_flags) {
              struct SHEET* old = sht;
              bool is_the_same = false;
              for (int j = shtctl->top - 1; j > 0; j--) {
                sht = shtctl->sheets[j];
                x = gmx - sht->vx0;
                y = gmy - sht->vy0;
                if (0 <= x && x < sht->bxsize && 0 <= y && y < sht->bysize) {
                  if (sht == old) {
                    is_the_same = true;
                  }
                  uint32_t times = (sht->bxsize - 8) / (255 - 106) + 1;
                  for (int i = 3; i < 20; i++) {
                    color_t color = argb(0, 10, 36, 106);
                    for (int j = 3, count = 0; j < sht->bxsize - 4;
                         j++, count++) {
                      if (sht->buf[j + i * sht->bxsize] != COL_FFFFFF &&
                          !(sht->bxsize - 37 <= j && j < sht->bxsize - 5 &&
                            5 <= i && i < 19)) {
                        sht->buf[j + i * sht->bxsize] = color;
                      }
                      if (count == times && (color & 0xff) != 255) {
                        color += 0x00010101;
                        count = 0;
                      }
                    }
                    sheet_refresh(sht, 3, 3, sht->bxsize - 4, 20);
                  }
                  if (sht->buf[y * sht->bxsize + x] != sht->col_inv) {
                    // for (int L = 1; GetTask(L) != NULL; L++) {
                    //   if (GetTask(L) == NowTask()) {
                    //  SubTask(L);
                    //  break;
                    //    continue;
                    //  }
                    //  SleepTaskFIFO(GetTask(L));
                    //}
                    if (old->task != NowTask()) {
                      SleepTaskFIFO(old->task);
                    }
                    WakeUp(sht->task);
                    sheet_updown(sht, shtctl->top - 1);
                    if (3 <= x && x < sht->bxsize - 3 && 3 <= y && y < 21) {
                      mmx = gmx; /*进入窗口移动模式*/
                      mmy = gmy;
                      mmx2 = sht->vx0;
                      new_wy = sht->vy0;
                    }
                    if (sht->bxsize - 21 <= x && x < sht->bxsize - 5 &&
                        5 <= y && y < 19) {
                      // 点击“×”按钮
                      if (sht->Close != NULL) {
                        close_tid = sht;
                        sht->Close();
                      }
                    } else if (sht->bxsize - 37 <= x && x < sht->bxsize - 21 &&
                               5 <= y && y < 19) {
                      // 点击“_”按钮
                      sheet_updown(sht, -1);  // 隐藏窗口
                      int cnt;
                      for (cnt = 0; cnt != 32; cnt++) {
                        if (small_window[cnt] == NULL) {
                          Button* btn = MakeButton(
                              cnt * 32 - 3, vbinfo->ysize - 28 - 32 - 28, 32,
                              32, sht_back, "", small_window_handler);
                          small_window[cnt] = sht;
                          Button_Draw(btn);
                          break;
                        }
                      }
                    }
                    break;
                  }
                }
              }
              if (!is_the_same) {
                uint32_t times = (old->bxsize - 8) / (0xc6 - 0x84) + 1;
                for (int i = 3; i < 20; i++) {
                  color_t color = COL_848484;
                  for (int j = 3, count = 0; j < old->bxsize - 4;
                       j++, count++) {
                    if (old->buf[j + i * old->bxsize] != COL_FFFFFF &&
                        !(old->bxsize - 37 <= j && j < old->bxsize - 5 &&
                          5 <= i && i < 19)) {
                      old->buf[j + i * old->bxsize] = color;
                    }
                    if (count == times && color != COL_C6C6C6) {
                      color += 0x00010101;
                      count = 0;
                    }
                  }
                }
                sheet_refresh(old, 3, 3, old->bxsize - 4, 20);
              }
            } else if (sheet_updown_flags) {
              /*如果处于窗口移动模式*/
              x = gmx - mmx; /*计算鼠标的移动距离*/
              y = gmy - mmy;
              // sheet_slide(sht, sht->vx0 + x, sht->vy0 + y);
              new_wx = (mmx2 + x + 2) & ~3;
              new_wy = new_wy + y;
              // mmx = gmx; /*更新为移动后的坐标*/
              mmy = gmy;
            }
          } else if ((mdec.btn & 0x02) != 0) {
            // Sputs(buf_back, "Right", 0, 0, 0xf, 320);
            // sheet_refresh(sht_back, 0, 0, 320, 100);
          } else if ((mdec.btn & 0x04) != 0) {
            // Sputs(buf_back, "MIDDLE", 0, 0, 0xf, 320);
            // sheet_refresh(sht_back, 0, 0, 320, 100);
          } else {
            if (button_clicking) {
              for (int i = 1; FindForCount(i, list) != NULL; i++) {
                Button* btn = (Button*)FindForCount(i, list)->val;
                if (Button_Click(btn, gmx, gmy)) {
                  if (btn->is_clicking) {
                    Button_Draw(btn);
                    if (btn->OnClick != NULL) {
                      button_cl = btn;
                      btn->OnClick(btn);
                    }
                    btn->is_clicking = false;
                    break;
                  }
                }
              }
              button_clicking = false;
            }
            mmx = -1;
          }
          // Sputs(buf_back, "Left", 0, 0, 0xf, 320);
          // sheet_refresh(sht_back, 0, 0, 320, 100);
        }
        sheet_slide(sht_mouse, gmx, gmy);
        // io_sti();
      }
    }
    if (fifo8_status(TaskGetKeyfifo(NowTask())) != 0) {
      int i = fifo8_get(TaskGetKeyfifo(NowTask()));
      if (i > 0x80) {
        ClearMaskIrq(0);
        continue;
      }
      i = sc2a(i);
      for (int j = 1; FindForCount(j, list1) != NULL; j++) {
        TextBox* txt = (TextBox*)FindForCount(j, list1)->val;
        if (txt->is_clicking) {
          TextBox_AddChar(txt, i);
          break;
        }
      }
    }
    if (fifo8_status(&fifo) != 0) {
      int i = fifo8_get(&fifo);
      if (i == 1) {
        sheet_updown(sht_b_cur, -1);
        sheet_refresh(sht_win, sht_b_cur->vx0, sht_b_cur->vy0,
                      sht_b_cur->vx0 + 8, sht_b_cur->vy0 + 16);
        timer_init(timer, &fifo, 2);
        timer_settime(timer, 500);
      } else if (i == 2) {
        sheet_updown(sht_b_cur, 1);
        sheet_refresh(sht_win, sht_b_cur->vx0, sht_b_cur->vy0,
                      sht_b_cur->vx0 + 8, sht_b_cur->vy0 + 16);
        timer_init(timer, &fifo, 1);
        timer_settime(timer, 500);
      }
    }
    ClearMaskIrq(0);
  }
}