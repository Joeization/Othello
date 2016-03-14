#include "view.h"
#include "ai.h"

extern view *main_window;
extern GtkWidget *window;

int main(int argc, char *argv[])
{
  srand(time(0));
  main_window = new view();
  gtk_main();

  return 0;
}
