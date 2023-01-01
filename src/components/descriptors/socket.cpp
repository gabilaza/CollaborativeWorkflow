
#include <unistd.h>
#include "socket.hpp"
#include "utils/logs.hpp"

static const char *tag = "Socket";


Socket::Socket(int domain, int type, int protocol) : IODescriptor(::socket(domain, type, protocol)) {
    if(_fd < 0)
        logger::raiseSystemError(tag, "socket()");
}


Socket::Socket(int fd) : IODescriptor(fd) {
    if(_fd < 0)
        logger::raiseSystemError(tag, "socket()", EBADF);
}


Socket::Socket(Descriptor &&other) : IODescriptor(std::move(other)) {
    if(_fd < 0)
        logger::raiseSystemError(tag, "socket()", EBADF);
}


Socket::Socket(const Socket &other) : IODescriptor(::dup(other._fd)) {
    if(_fd < 0)
        logger::raiseSystemError(tag, "dup()");
}


const Socket &Socket::operator=(const Socket &other) const {
    int err = ::dup2(other._fd, _fd);
    if(err < 0)
        logger::raiseSystemError(tag, "dup2()");

    return *this;
}


Socket Socket::dup() const {
    return Socket(*this);
}


void Socket::dup2(const Socket &other) const {
    *this = other;
}


void Socket::connect(const sockaddr *saddr, socklen_t len) const {
    auto err = ::connect(_fd, saddr, len);
    if(err < 0)
        logger::raiseSystemError(tag, "connect()");
}


void Socket::connect(const struct sockaddr_in &saddr) const {
    connect((const struct sockaddr *) &saddr, sizeof(saddr));
}


void Socket::bind(const sockaddr *saddr, socklen_t len) const {
    auto err = ::bind(_fd, saddr, len);
    if(err < 0)
        logger::raiseSystemError(tag, "bind()");
}


void Socket::bind(const struct sockaddr_in &saddr) const {
    bind((const struct sockaddr *) &saddr, sizeof(saddr));
}


void Socket::listen(int backlog) const {
    auto err = ::listen(_fd, backlog);
    if(err < 0)
        logger::raiseSystemError(tag, "listen()");
}


Socket* Socket::accept() const {
    return accept((struct sockaddr *) nullptr, nullptr);
}


Socket* Socket::accept(struct sockaddr *saddr, socklen_t *len) const {
    auto fd = ::accept(_fd, saddr, len);
    if(fd < 0)
        logger::raiseSystemError(tag, "accept()");

    return new Socket(fd);
}


void Socket::shutdown(int mode) const {
    auto err = ::shutdown(_fd, mode);
    if(err < 0)
        logger::raiseSystemError(tag, "shutdown()");
}

size_t Socket::recv(void *buffer, size_t size, int flags) const {
    auto n = ::recv(_fd, buffer, size, flags);
    if(n < 0)
        logger::raiseSystemError(tag, "recv()");

    return static_cast<size_t>(n);
}

size_t Socket::recvmsg(struct msghdr &msg, int flags) const {
    auto n = ::recvmsg(_fd, &msg, flags);
    if(n < 0)
        logger::raiseSystemError(tag, "recvmsg()");

    return static_cast<size_t>(n);
}


size_t Socket::recvfrom(char *buffer, size_t size, struct sockaddr *saddr, socklen_t *len, int flags) const {
    auto n = ::recvfrom(_fd, buffer, size, flags, saddr, len);
    if(n < 0)
        logger::raiseSystemError(tag, "recvfrom()");

    return static_cast<size_t>(n);
}


size_t Socket::send(const void *buffer, size_t size, int flags) const {
    auto n = ::send(_fd, buffer, size, flags);
    if(n < 0)
        logger::raiseSystemError(tag, "send()");

    return static_cast<size_t>(n);
}


size_t Socket::sendmsg(const struct msghdr &msg, int flags) const {
    auto n = ::sendmsg(_fd, &msg, flags);
    if(n < 0)
        logger::raiseSystemError(tag, "sendmsg()");

    return static_cast<size_t>(n);
}


size_t Socket::sendto(const char *buffer, size_t size, const struct sockaddr *saddr, socklen_t len, int flags) const {
    auto n = ::sendto(_fd, buffer, size, flags, saddr, len);
    if(n < 0)
        logger::raiseSystemError(tag, "sendto()");

    return static_cast<size_t>(n);
}


size_t Socket::sendto(const char *buffer, size_t size, const struct sockaddr_in &saddr, int flags) const {
    const struct sockaddr *sockaddr = (const struct sockaddr *) &saddr;

    return sendto(buffer, size, sockaddr, sizeof(saddr), flags);
}

