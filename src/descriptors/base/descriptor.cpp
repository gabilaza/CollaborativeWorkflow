
#include <unistd.h>
#include <fcntl.h>
#include "descriptor.hpp"
#include "utils/logs.hpp"

static const char *tag = "Descriptor";


Descriptor::Descriptor(int fd) : _fd(fd) {
}


Descriptor::~Descriptor() {
    if(_fd >= 0)
        if(::close(_fd) < 0)
            logger::raiseSystemError(tag, "close()");
}


Descriptor::Descriptor(Descriptor &&other) : _fd(other._fd) {
    other._fd = -1;
}


Descriptor &Descriptor::operator=(Descriptor &&other) {
    std::swap(_fd, other._fd);
    return *this;
}


Descriptor::operator int() const {
    return _fd;
}


int Descriptor::fcntl(int cmd) {
    int ret = ::fcntl(_fd, cmd);
    if(ret < 0)
        logger::raiseSystemError(tag, "fcntl()");
    return ret;
}


int Descriptor::fcntl(int cmd, int arg) {
    int ret = ::fcntl(_fd, cmd, arg);
    if(ret < 0)
        logger::raiseSystemError(tag, "fcntl()");
    return ret;
}


int Descriptor::fcntl(int cmd, struct flock *arg) {
    int ret = ::fcntl(_fd, cmd, arg);
    if(ret < 0)
        logger::raiseSystemError(tag, "fcntl()");
    return ret;
}


int Descriptor::fd() const {
    return _fd;
}

