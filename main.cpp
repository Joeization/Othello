#include "ai.h"
#include "train.h"
#include "game.h"
#include "view_game.h"
#define OUTPUT

extern ai *umikaze;
extern view_game *main_window;
extern GtkWidget *window;
const int gene = 20;
//train times

int main(int argc, char *argv[]){
  cin.tie();cout.tie();
  srand(time(0));
  train *trainer = new train();
  for(int i=0;i<gene;i++){
    #ifdef OUTPUT
      cout << i << ": " << endl;
    #endif
    trainer->next_gen();
  }
  umikaze = trainer->best();
  delete trainer;
  main_window = new view_game(true);
  gtk_main();

  return 0;
}
