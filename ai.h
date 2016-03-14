#include <cstdlib>
#include <ctime>
#ifndef AI_H
#define AI_H
#include "view.h"

class view;

class ai{
private:
    int test;
public:
    ai();
    ~ai();
    void run(view *v);
};

#endif
