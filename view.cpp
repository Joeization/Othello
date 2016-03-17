#include "view.h"
#define debug false

view *main_window;
GtkWidget *window;
extern ai *umikaze;

view::view(bool s){
  gtk_init(NULL, NULL);
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "betaOthello!");
  this->create_top_info();
  this->create_board();
  show = s;
  if(show)gtk_widget_show(window);

  this->now_player = 1;
}

view::view(view* obj){
  for (int i = 0; i < 8; i++){
    for (int j = 0; j < 8; j++){
      this->map[i][j].color = obj->map[i][j].color;
      this->map[i][j].button = NULL;
    }
  }
  this->now_player = obj->now_player;
}

void view::testing(){
  if (debug){
    cout << "color" << endl;
    for (int i = 0; i < 8; i++){
      for (int j = 0; j < 8; j++){
        cout << this->map[i][j].color;
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

void view::create_top_info(){
  #ifdef OUTPUT
    cout << "ccc";
  #endif
}

void view::change_top_info(){
  #ifdef OUTPUT
    cout << "ccc";
  #endif
}

void view::create_board(){
  GtkWidget *vbox;
  GtkWidget *hbox;
  GtkWidget *button;
  GtkWidget *label;

  int index  = 0;

  vbox = gtk_vbox_new(FALSE, 0);

  hbox = gtk_hbox_new(FALSE, 0);
  label = gtk_label_new("Player:");
  gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 4);
  gtk_widget_show_all(hbox);
  gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);

  for (int i = 0; i < 8; i++){
    hbox = gtk_hbox_new(FALSE, 0);
    for (int j = 0; j < 8; j++){
      button = gtk_toggle_button_new();
      gtk_widget_set_usize(button, button_size, button_size);
      gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 0);
      gtk_widget_show(button);
      
      g_signal_connect(G_OBJECT(button), "button-press-event",
          G_CALLBACK(put_piece), (gpointer) index);
          //AI need the board
      index++;
      this->map[i][j].button = button;
      this->map[i][j].color = 0;
    }
    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);
    gtk_widget_show(hbox);
  }

  gtk_container_add(GTK_CONTAINER(window), vbox);
  gtk_widget_show(vbox);

  // 初始中間四子
  this->map[4][4].color = this->map[3][3].color = 1;
  this->map[3][4].color = this->map[4][3].color = -1;

  this->change_board();
}

void view::change_board(){
  
  GdkColor color;

  for (int i = 0; i < 8; i++){
    for (int j = 0; j < 8; j++){
      if (this->map[i][j].color == 1){
        gtk_button_set_label(GTK_BUTTON(this->map[i][j].button), "X");
        gdk_color_parse ("black", &color);
      }
      else if (this->map[i][j].color == -1){
        gtk_button_set_label(GTK_BUTTON(this->map[i][j].button), "O");
        gdk_color_parse ("white", &color);
      }
      else{
        gtk_button_set_label(GTK_BUTTON(this->map[i][j].button), "");
        gdk_color_parse ("brown", &color);
      }

      // color for all button status
      gtk_widget_modify_bg (GTK_WIDGET(this->map[i][j].button), GTK_STATE_ACTIVE, &color);
      gtk_widget_modify_bg (GTK_WIDGET(this->map[i][j].button), GTK_STATE_NORMAL, &color);
      gtk_widget_modify_bg (GTK_WIDGET(this->map[i][j].button), GTK_STATE_PRELIGHT, &color);
    }
  }
}

bool view::drop_valid(pair<int, int> p, int color){
  bool judge = false;
  int another = (color == 1) ? -1 : 1;
  int temp_x, temp_y;
  int temp_acc; // 暫時存放某方向不同棋子數
 
  if (this->map[p.first][p.second].color != 0) // 已經有棋子
    return  false;

  for (int i = -1; i < 2; i++){
    for (int j = -1; j < 2; j++){
      this->change_buffer[i+1][j+1] = 0;
      temp_acc = 0;
      temp_x = p.first+i;
      temp_y = p.second+j;
      while (!(i == 0 && j == 0) && // 不是自己
            (temp_x >= 0 && temp_x < 8 && temp_y >= 0 && temp_y < 8) && // 沒有超界
            (this->map[temp_x][temp_y].color == another)) // 有不同色
      {
        temp_acc++;
        temp_x += i;
        temp_y += j;
      }

      if (!(i == 0 && j == 0) && // 不是自己
          (temp_x >= 0 && temp_x < 8 && temp_y >= 0 && temp_y < 8) && // 不是因為超界跳出 表示遇到不同色
          (this->map[temp_x][temp_y].color == color) && // 最後是遇到同色
          temp_acc != 0)
      {
        this->change_buffer[i+1][j+1] = temp_acc;
        judge = true;
      }
    }
  }

  return judge;
}

bool view::drop(pair<int, int> p, int color){
  int temp_x, temp_y;
  vector<pair<int, int> > detect_ending;
  GtkWidget *dialog;

  if (this->drop_valid(p, color)){
    this->map[p.first][p.second].color = color;
    
    for (int i = -1; i < 2; i++){
      for (int j = -1; j < 2; j++){
        temp_x = p.first+i;
        temp_y = p.second+j;
        while (this->change_buffer[i+1][j+1]--){
          this->map[temp_x][temp_y].color = color;
          temp_x += i;
          temp_y += j;
        }
      }
    }

    this->change_player();
    this->change_board();

    if(show){
    //輸出比數
      int b = 0;
      int w = 0;
      for(int i=0;i<8;i++)
        for(int j=0;j<8;j++){
        if(map[i][j].color==1)b++;
        if(map[i][j].color==-1)w++;
      }
      cout << endl;
      cout << "Black :" << b << endl;
      cout << "White :" << w;
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
    this->message_alert("注意！", "該點不可落子", "請下在可落子的點");

    return false;
  }
}

vector<pair<int, int> > view::can_drop(int color){
  vector<pair<int, int> > drop_set;

  for (int i = 0; i < 8; i++){
    for (int j = 0; j < 8; j++){
      if (drop_valid(make_pair(i,j), color))
        drop_set.push_back(make_pair(i,j));
    }
  }

  return drop_set;
}

void view::ending_handler(){
  if(show)
    this->message_alert("恭喜！", "玩家", "希望您玩的愉快！");
  /*
  else
    gtk_widget_hide(window);
  */
}

void view::message_alert(const char *title, const char *first_msg, const char *second_msg){
  GtkWidget *dialog  = gtk_message_dialog_new(GTK_WINDOW(window), 
                                  GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, 
                                  GTK_BUTTONS_OK, first_msg);
  gtk_window_set_title(GTK_WINDOW(dialog), title);
  gtk_message_dialog_format_secondary_text(
    GTK_MESSAGE_DIALOG(dialog), second_msg);
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}

int player_color = 1;
bool view::put_piece(GtkWidget *widget, GdkEventButton *event, gpointer data){
  gint index = (*(gint*)(&data));
  int nc = main_window->get_now_color();
  if(nc==player_color&&
    main_window->drop(make_pair(index/8,index%8),main_window->get_now_color())){
    if(main_window->get_now_color()!=nc){
      umikaze->set_value(main_window);
      umikaze->run(main_window);
    }
  }
  else if(nc!=player_color){
    umikaze->set_value(main_window);
    umikaze->run(main_window);
  }
}

int view::winner(){
  int b = 0;
  int w = 0;
  for(int i=0;i<8;i++)
    for(int j=0;j<8;j++){
      if(map[i][j].color == 1)
        b++;
      else if(map[i][j].color == -1)
        w++;
    }
  if(b > w)return 1;
  return -1;
}