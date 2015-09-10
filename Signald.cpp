#include "Signald.h"

Signald::Signald(const char* _fifo) {
    //check root
    if (getuid() != 0) {
        fprintf(stderr, "Signald should be run as root.\n");
        exit(EXIT_FAILURE);
    }

    //init FIFO
    fifo = _fifo;
    unlink(fifo);

    umask(0);
    if (mkfifo(fifo, 0666) == -1) {
        if (errno == EEXIST) {
            fprintf(stderr, "Another fifo server is running.\n");
        } else {
            perror("cannot create fifoserver");
            exit(EXIT_FAILURE);
        }
    }
    umask(022);

    //open FIFO
    fd = open(fifo, O_RDONLY|O_NONBLOCK, 0);
    if (fd == -1) {
        perror("cannot open fifo");
        exit(EXIT_FAILURE);
    }

    //init kqueue
    kq = kqueue();
    if (kq == -1) {
        perror("create kqueue failed");
        exit(EXIT_FAILURE);
    }

    //bind event
    struct kevent event_change;
    EV_SET(&event_change, fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
    kevent(kq, &event_change, 1, NULL, 0, NULL);
}

Signald::~Signald() {
    unlink(fifo);
}

int Signald::run(void (*callback)(string)) {
    //polling
    int event_count = 1;
    struct kevent events[event_count];
    size_t buffer_size = 1024;
    char buffer[buffer_size];
    while (true) {
        int ret = kevent(kq, NULL, 0, events, event_count, NULL);
        if (ret < 0) {
            perror("kevent error");
            continue;
        } else if(ret == 0) {
            printf("kenvent timeout!\n");
            continue;
        } else {
            //ret > 0 返回事件放在 events 中

            for (int i = 0; i < ret; i++) {
                struct kevent current_event = events[i];

                //kevent 中的 ident 就是文件描述符
                if (current_event.ident == fd) {
                    //有数据

                    stringstream ss;
                    ss.clear();

                    ssize_t read_length = buffer_size;
                    while (read_length == buffer_size) {
                        memset(buffer, 0, buffer_size);
                        read_length = read(fd, buffer, buffer_size);
                        if (read_length == -1) {
                            perror("read error");
                        }
                        ss<<buffer;
                    }

                    if (strcmp(buffer, "signald stop\n") == 0) {
                        cout<<"user stop."<<endl;
                        goto exit;
                    }

                    callback(ss.str());

                } else {
                    //sth. wrong
                    stringstream ss;
                    ss<<"unexpected event ["<<current_event.ident<<"]";
                    perror(ss.str().c_str());
                }
            }
        }
    }
    exit:
        return 0;
}