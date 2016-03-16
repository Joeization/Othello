#include <gtk/gtk.h>
#include <iostream>
#include <vector>
#include <utility>
using namespace std;
#ifndef VIEW_H
#define VIEW_H
#include "ai.h"

struct chess{
  int color;
  GtkWidget *button;
};
typedef struct chess chess;

class view{
  chess map[8][8]; // 棋子資訊

  // drop_valid判斷可否落子同時算出結果給drop用
  // 紀錄8個方向個要改變幾顆棋子
  int change_buffer[3][3];

  const static int button_size = 80; // 格子大小
  int now_player; // 當前顏色

public:
  view();
  view(view* b); // 複製一個測試用
  ~view() {}

  int get_now_color() { return this->now_player; }
  int get_pos(pair<int, int> p) { return this->map[p.first][p.second].color; }
  
  bool drop(pair<int, int> p, int color); // 會拿drop_valid的暫存值算
  vector<pair<int, int> > can_drop(int color); // 拿可下的位置組合
  
  void worker();

private:
  void testing();
  
  void create_top_info();
  void change_top_info();
  void create_board();
  void change_board();
  
  void change_player() { this->now_player = (this->now_player == 1) ? -1 : 1; }
  
  bool drop_valid(pair<int, int> p, int color);

  void ending_handler();
  void message_alert(const char *title, const char *first_msg, const char *second_msg);
  
  // 為了接gtk的event 無奈寫法
  static bool put_piece(GtkWidget *widget, GdkEventButton *event, gpointer data);

};

struct box{
  gint index;
  view *v;
  box(int _index, view *_v):index(_index),v(_v){};
};

#endif
