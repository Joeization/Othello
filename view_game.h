#include <gtk/gtk.h>
#include <cstdio>
using namespace std;
#ifndef VIEW_GAME_H
#define VIEW_GAME_H
#include "ai.h"
#include "game.h"

class view_game : public game{  
private:

  /** variable **/

  GtkWidget *map_button[8][8];
  GtkWidget *now_player_label;
  GtkWidget *score_label;
  GtkWidget *landing_table;

  const static int button_size = 80; // 格子大小

  /** function **/

  void landing(); // start page
  void create_top_info(GtkWidget *vbox, GtkWidget *hbox);
  void change_top_info();
  void create_board();
  void change_board();

  void ending_handler();
  void message_alert(const char *title, const char *first_msg, const char *second_msg);
  
  // 為了接gtk的event 無奈寫法
  static void introduction(GtkWidget *button, gpointer data);
  static bool put_piece(GtkWidget *widget, GdkEventButton *event, gpointer data);
  static void change_view(GtkWidget *widget, gpointer data);

public:
  view_game (bool s=true); 
  view_game (view_game* b);
  ~view_game() {}
};
#endif
