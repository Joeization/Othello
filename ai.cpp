#include "ai.h"

ai::ai(){
    //do nothing
}
ai::~ai(){
    //do nothing
}
void ai::run(view *v){
    //testing
    int color=v->get_now_color();
    vector<pair<int, int> > s = v->can_drop(color);
    if(s.size() > 0){
        int pos = rand() % s.size();
        v->drop(s[pos], color);
    }
}