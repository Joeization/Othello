#include "view.h"
#include "ai.h"
#include "train.h"
//#define OUTPUT

extern view *main_window;
extern GtkWidget *window;
extern ai *umikaze;

const int gene = 1;
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
  main_window = new view(true);
  gtk_main();

  return 0;
}
