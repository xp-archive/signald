#include "Signald.h"

//#include <iostream>
#include <csignal>
//#include <ctime>
//#include <sstream>

using namespace std;

void foo(string str) {
    time_t lt = time(NULL);
    struct tm *cur_time = localtime(&lt);
    char ltstr[128];
    strftime(ltstr, 128, "%F %X", cur_time);

    cout<<"["<<ltstr<<"] "<<str.length()<<" Bytes\n";
    cout<<"input:\n"<<str<<"\n";
    cout<<"output:\n";
    if (strcmp(str.c_str(), "nginx reopen log\n") == 0) {
        ifstream ifs("/usr/local/var/run/nginx.pid");
        if (!ifs) {
            cout<<"nginx is not run.\n";
            return ;
        }
        int pid;
        ifs>>pid;
        int ret = kill(pid, SIGUSR1);
        if (ret == -1) {
            stringstream ss;
            ss<<"nginx ["<<pid<<"]";
            perror(ss.str().c_str());
        } else {
            cout<<"nginx ["<<pid<<"] SIGUSR1 successfully.\n";
        }
    } else if (strcmp(str.c_str(), "php-fpm reopen log\n") == 0) {
        ifstream ifs("/usr/local/var/run/php-fpm.pid");
        if (!ifs) {
            cout<<"php-fpm is not run.\n";
            return ;
        }
        int pid;
        ifs>>pid;
        int ret = kill(pid, SIGUSR1);
        if (ret == -1) {
            stringstream ss;
            ss<<"php-fpm ["<<pid<<"]";
            perror(ss.str().c_str());
        } else {
            cout<<"php-fpm ["<<pid<<"] SIGUSR1 successfully.\n";
        }
    }
}

int main (int argc, const char * argv[])
{
    Signald* sgd;
    cout<<argv[0]<<endl;
    if (argc > 1) {
        sgd = new Signald(argv[1]);
    } else {
        sgd = new Signald();
    }
    sgd->run(&foo);
    sgd = nullptr;
    return 0;
}