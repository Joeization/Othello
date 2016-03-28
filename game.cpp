#include "game.h"
#define debug false

extern ai *umikaze;

game::game(){
  this->now_player = 1;
  this->create_board();
  show = false;
}

game::game(game* obj){
  for (int i = 0; i < 8; i++){
    for (int j = 0; j < 8; j++){
      this->chess[i][j] = obj->chess[i][j];
    }
  }
  this->now_player = obj->now_player;
}

void game::debugging(){
  if (debug){
    cout << "color" << endl;
    for (int i = 0; i < 8; i++){
      for (int j = 0; j < 8; j++){
        cout << this->chess[i][j];
        cout << " ";
      }
      cout << endl;
    }
    
    
    cout << "eight direction" << endl;
    for (int i = 0; i < 3; i++){
      for (int j = 0; j < 3; j++){
        cout << this->change_buffer[i][j];
      }
      cout << endl;
    }

    cout << "now_player " << now_player << endl;
  }
}
  
void game::create_board(){
  int index  = 0;

  for (int i = 0; i < 8; i++){
    for (int j = 0; j < 8; j++){
      this->chess[i][j] = 0;
    }
  }

  // 初始中間四子
  this->chess[4][4] = this->chess[3][3] = -1;
  this->chess[3][4] = this->chess[4][3] = 1;

}

pair<int, int> game::get_now_score(){
  pair<int, int> score (0, 0); // first:black second:while
  for(int i=0;i<8;i++)
    for(int j=0;j<8;j++){
    if(this->chess[i][j]==1)score.first++;
    if(this->chess[i][j]==-1)score.second++;
  }
  return score;
}

bool game::drop_valid(pair<int, int> p, int color){
  bool judge = false;
  int another = (color == 1) ? -1 : 1;
  int temp_x, temp_y;
  int temp_acc; // 暫時存放某方向不同棋子數
 
  if (this->chess[p.first][p.second] != 0) // 已經有棋子
    return  false;

  for (int i = -1; i < 2; i++){
    for (int j = -1; j < 2; j++){
      this->change_buffer[i+1][j+1] = 0;
      temp_acc = 0;
      temp_x = p.first+i;
      temp_y = p.second+j;
      while (!(i == 0 && j == 0) && // 不是自己
            (temp_x >= 0 && temp_x < 8 && temp_y >= 0 && temp_y < 8) && // 沒有超界
            (this->chess[temp_x][temp_y] == another)) // 有不同色
      {
        temp_acc++;
        temp_x += i;
        temp_y += j;
      }

      if (!(i == 0 && j == 0) && // 不是自己
          (temp_x >= 0 && temp_x < 8 && temp_y >= 0 && temp_y < 8) && // 不是因為超界跳出 表示遇到不同色
          (this->chess[temp_x][temp_y] == color) && // 最後是遇到同色
          temp_acc != 0)
      {
        this->change_buffer[i+1][j+1] = temp_acc;
        judge = true;
      }
    }
  }

  return judge;
}

bool game::drop(pair<int, int> p, int color){
  int temp_x, temp_y;
  vector<pair<int, int> > detect_ending;

  if (this->drop_valid(p, color)){
    this->chess[p.first][p.second] = color;
    
    for (int i = -1; i < 2; i++){
      for (int j = -1; j < 2; j++){
        temp_x = p.first+i;
        temp_y = p.second+j;
        while (this->change_buffer[i+1][j+1]--){
          this->chess[temp_x][temp_y] = color;
          temp_x += i;
          temp_y += j;
        }
      }
    }

    this->change_player();

    if(show){
      //輸出比數
      pair<int, int> score = this->get_now_score();
      cout << endl;
      cout << "Black :" << score.first << endl;
      cout << "White :" << score.second;
        cout << endl;
    }

    int cnt = 0;
    detect_ending = can_drop(this->get_now_color());
    if (detect_ending.size() == 0){
      cnt++;
      this->change_player();
    }
    detect_ending = can_drop(this->get_now_color());
    if (detect_ending.size() == 0)
      cnt++;
    if(cnt == 2)
      this->ending_handler();
    
    return true;
  }
  else{
    cout << "can't drop" << endl;

    return false;
  }
}

vector<pair<int, int> > game::can_drop(int color){
  vector<pair<int, int> > drop_set;

  for (int i = 0; i < 8; i++){
    for (int j = 0; j < 8; j++){
      if (drop_valid(make_pair(i,j), color))
        drop_set.push_back(make_pair(i,j));
    }
  }

  return drop_set;
}

void game::ending_handler(){
  if(show)
    cout << "ending" << endl;
}

int game::winner(){
  int b = 0;
  int w = 0;
  for(int i=0;i<8;i++)
    for(int j=0;j<8;j++){
      if(this->chess[i][j] == 1)
        b++;
      else if(this->chess[i][j] == -1)
        w++;
    }
  if(b > w)return 1;
  else if(w > b)return -1;
  else return 0;
}
