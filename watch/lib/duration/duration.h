#include <stdio.h>
#include <ctime>


class Duration {
    
public:
    Duration();
    char* getAsCString();
    void setCount(int time);
    void startCount();
    void endCount();
    bool isTiming();
    void update();

private:

    int elapsed_time_;
    int start_time_;
    bool timer_on_;
    char buffer_[9];

};