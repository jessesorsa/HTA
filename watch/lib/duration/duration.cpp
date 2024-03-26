#include "duration.h"

    Duration::Duration(): elapsed_time_(0), start_time_(0), timer_on_(false) {}

    char* Duration::getAsCString(){
        
        int hours, minutes, seconds;
        hours = elapsed_time_ / 3600;
        elapsed_time_ = elapsed_time_ % 3600;
        minutes = elapsed_time_ / 60;
        seconds = elapsed_time_ % 60;
        if(hours>99){
            // max hours
            hours = 99;
        }
        snprintf(buffer_, sizeof(buffer_), "%02d:%02d:%02d", hours, minutes, seconds);
        return buffer_;
    }

    void Duration::setCount(int time){
        elapsed_time_ = time;
    }

    void Duration::startCount(){
        timer_on_ = true;
        elapsed_time_ = 0;
        start_time_ = time(NULL);
    }
    void Duration::endCount(){
        timer_on_  = false;
    }

    bool Duration::isTiming(){
        return timer_on_;
    }

    void Duration::update(){
        if(timer_on_){
            elapsed_time_ = time(NULL) - start_time_;
        }
    }



    

