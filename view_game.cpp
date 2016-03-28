#include "view_game.h"
#define debug false

view_game *main_window;
GtkWidget *window;
extern ai *umikaze;

view_game::view_game(bool s){
  gtk_init(NULL, NULL);
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "betaOthello!");
  this->landing();
  
  show = s;
  if(show)gtk_widget_show(window);

  this->now_player = 1;
}

void view_game::landing(){
  GtkWidget *quit_button, *intro_button ,*start_button;

  this->landing_table = gtk_table_new(5, 3, TRUE);
  gtk_widget_set_size_request(landing_table,600,500);

  start_button = gtk_button_new_with_label("開始遊戲");
  gtk_box_pack_start(GTK_BOX(landing_table), start_button, FALSE, FALSE, 0); 
  gtk_table_attach_defaults(GTK_TABLE(landing_table), start_button, 1, 2, 1, 2); 
  g_signal_connect(GTK_OBJECT(start_button),"clicked",G_CALLBACK(change_view),NULL);

  intro_button = gtk_button_new_with_label("遊戲說明");
  gtk_box_pack_start(GTK_BOX(landing_table), intro_button, FALSE, FALSE, 0); 
  gtk_table_attach_defaults(GTK_TABLE(landing_table), intro_button, 1, 2, 2, 3); 
  g_signal_connect(GTK_OBJECT(intro_button), "clicked", G_CALLBACK(introduction), window);

  quit_button = gtk_button_new_with_label ("離開遊戲");
  g_signal_connect_swapped (G_OBJECT (quit_button), "clicked",
          G_CALLBACK (gtk_widget_destroy),
          G_OBJECT (window));
  gtk_table_attach_defaults(GTK_TABLE(landing_table), quit_button, 1, 2, 3, 4); 

  gtk_widget_show_all(landing_table);
  gtk_container_add(GTK_CONTAINER(window), landing_table);
}

void view_game::create_top_info(GtkWidget *vbox, GtkWidget *hbox){
  GtkWidget *top_table = gtk_table_new(1, 5, TRUE);
  gtk_box_pack_start(GTK_BOX(hbox), top_table, FALSE, FALSE, 4);

  this->now_player_label = gtk_label_new("Player:X");
  gtk_misc_set_alignment(GTK_MISC(this->now_player_label),0.0,0.5); // left side
  gtk_table_attach_defaults(GTK_TABLE(top_table), this->now_player_label, 0, 1, 0, 1);

  this->score_label = gtk_label_new("      black: 2 white: 2");
  gtk_table_attach_defaults(GTK_TABLE(top_table), this->score_label,4, 5, 0, 1);
  
  gtk_widget_show_all(hbox);
  gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);
}

void view_game::change_top_info(){
  char temp_string[1000];
  pair<int, int> score = this->get_now_score();

  if (this->get_now_color() == 1)
    gtk_label_set_text(GTK_LABEL(this->now_player_label),"Player:X");
  else  
    gtk_label_set_text(GTK_LABEL(this->now_player_label),"Player:O");

  sprintf(temp_string, "      black:%2d white:%2d", score.first, score.second);
  gtk_label_set_text(GTK_LABEL(this->score_label), temp_string);
}

void view_game::create_board(){
  GtkWidget *vbox;
  GtkWidget *hbox;
  GtkWidget *button;

  int index  = 0;

  vbox = gtk_vbox_new(FALSE, 0);
  hbox = gtk_hbox_new(FALSE, 0);
  create_top_info(vbox ,hbox);

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
      this->map_button[i][j] = button;
      this->chess[i][j] = 0;
    }
    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);
    gtk_widget_show(hbox);
  }

  gtk_container_add(GTK_CONTAINER(window), vbox);
  gtk_widget_show(vbox);

  // 初始中間四子
  this->chess[4][4] = this->chess[3][3] = -1;
  this->chess[3][4] = this->chess[4][3] = 1;

  this->change_board();
}

void view_game::change_board(){
  
  GdkColor color;

  for (int i = 0; i < 8; i++){
    for (int j = 0; j < 8; j++){
      if (this->chess[i][j] == 1){
        gtk_button_set_label(GTK_BUTTON(this->map_button[i][j]), "X");
        gdk_color_parse ("black", &color);
      }
      else if (this->chess[i][j] == -1){
        gtk_button_set_label(GTK_BUTTON(this->map_button[i][j]), "O");
        gdk_color_parse ("white", &color);
      }
      else{
        gtk_button_set_label(GTK_BUTTON(this->map_button[i][j]), "");
        gdk_color_parse ("brown", &color);
      }

      // color for all button status
      gtk_widget_modify_bg (GTK_WIDGET(this->map_button[i][j]), GTK_STATE_ACTIVE, &color);
      gtk_widget_modify_bg (GTK_WIDGET(this->map_button[i][j]), GTK_STATE_NORMAL, &color);
      gtk_widget_modify_bg (GTK_WIDGET(this->map_button[i][j]), GTK_STATE_PRELIGHT, &color);
    }
  }
}

void view_game::ending_handler(){
  if(show)
    this->message_alert("恭喜！", "玩家", "希望您玩的愉快！");
  /*
  else
    gtk_widget_hide(window);
  */
}

void view_game::introduction(GtkWidget *button, gpointer data) {
    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(data), 
                             GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, 
                             GTK_BUTTONS_OK, "黑白棋");
    gtk_window_set_title(GTK_WINDOW(dialog), "遊戲說明");
    gtk_message_dialog_format_secondary_text(
         GTK_MESSAGE_DIALOG(dialog), "玩家與電腦AI對奕，得分高者勝。");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

void view_game::message_alert(const char *title, const char *first_msg, const char *second_msg){
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
bool view_game::put_piece(GtkWidget *widget, GdkEventButton *event, gpointer data){
  gint index = (*(gint*)(&data));
  int nc = main_window->get_now_color();
  
  if(nc==player_color&&
    main_window->drop(make_pair(index/8,index%8),main_window->get_now_color())){
    
    if(main_window->get_now_color()!=nc){
      umikaze->set_value(main_window);
      umikaze->run(main_window, true);
    }
  }
  else if(nc!=player_color){
    umikaze->set_value(main_window);
    umikaze->run(main_window, true);
  }

  main_window->change_board();
  main_window->change_top_info();
}

void view_game::change_view(GtkWidget *widget, gpointer data){
  gtk_widget_destroy(GTK_WIDGET(main_window->landing_table));
  main_window->create_board();
}
