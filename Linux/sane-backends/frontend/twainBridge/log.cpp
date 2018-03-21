

#include "log.h"


static fstream myfile;

int writeToLog(std::string buffer){
    if(!myfile.is_open()){
        time_t seconds;
        seconds = time(NULL);
        myfile.open("logs/" +std::to_string(seconds)  + ".txt",fstream::in |  fstream::out | fstream::app);
    }    
    myfile << buffer;
}
