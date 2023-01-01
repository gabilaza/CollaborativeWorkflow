
#include <unistd.h>
#include "iodescriptor.hpp"
#include "utils/logs.hpp"

static const char *tag = "IODescriptor";


IODescriptor::IODescriptor(int fd) : Descriptor(fd) {
}


IODescriptor::IODescriptor(Descriptor &&other) : Descriptor(std::move(other)) {
}


size_t IODescriptor::read(char *buffer, size_t size) const {
    auto n = ::read(_fd, buffer, size);
    if(n < 0)
        logger::raiseSystemError(tag, "read()");

    return static_cast<size_t>(n);
}

size_t IODescriptor::write(const char *buffer, size_t size) const {
    auto n = ::write(_fd, buffer, size);
    if(n < 0)
        logger::raiseSystemError(tag, "write()");

    return static_cast<size_t>(n);
}

size_t IODescriptor::pread(char *buffer, size_t size, size_t off) const {
    auto n = ::pread(_fd, buffer, size, static_cast<off_t>(off));
    if(n < 0)
        logger::raiseSystemError(tag, "pread()");

    return static_cast<size_t>(n);
}

size_t IODescriptor::pwrite(const char *buffer, size_t size, size_t off) const {
    auto n = ::pwrite(_fd, buffer, size, static_cast<off_t>(off));
    if(n < 0)
        logger::raiseSystemError(tag, "pwrite()");

    return static_cast<size_t>(n);
}

