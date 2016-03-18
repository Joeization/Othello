#include <iostream>
#include <vector>
#include <utility>
using namespace std;
#ifndef GAME_H
#define GAME_H
#include "ai.h"

class game{
protected:
  /** variable **/
  
  int chess[8][8]; // 棋子資訊

  bool show;
  //test

  // drop_valid判斷可否落子同時算出結果給drop用
  // 紀錄8個方向個要改變幾顆棋子
  int change_buffer[3][3];

  int now_player; // 當前顏色

  /** function **/

  void debugging(); // show class status
  void change_player() { this->now_player = (this->now_player == 1) ? -1 : 1; }
  bool drop_valid(pair<int, int> p, int color);
  void message_alert(const char *title, const char *first_msg, const char *second_msg);
  void ending_handler();
 
  static void testing(); // test all the class function

  friend class train;

public:
  game();
  game(game* b); // 複製一個測試用
  ~game() {}

  void create_board();
  
  int get_now_color() { return this->now_player; }
  int get_pos(pair<int, int> p) { return this->chess[p.first][p.second]; }
  
  bool drop(pair<int, int> p, int color); // 會拿drop_valid的暫存值算
  vector<pair<int, int> > can_drop(int color); // 拿可下的位置組合
  
  int winner();//訓練的時候讓AI知道誰贏
};

#endif
