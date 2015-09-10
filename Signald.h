#ifndef SIGNALDAEMON_SIGNALD_H
#define SIGNALDAEMON_SIGNALD_H

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fstream>
#include <memory>
#include <cerrno>
#include <unistd.h>

//for getuid
#include <sys/types.h>

//for kqueue
#include <sys/event.h>

//for fifo
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;

class Signald {

protected:
    const char* fifo;
    int fd;
    int kq;

public:
    int event_count;

    Signald(const char* = "/tmp/signald");
    ~Signald();

    int run(void (*)(string));
};


#endif //SIGNALDAEMON_SIGNALD_H
