
#ifndef __SOCKET_H
#define __SOCKET_H

#include <sys/socket.h>
#include <netinet/in.h>
#include "base/iodescriptor.hpp"


class Socket : public IODescriptor {
    public:
        Socket(int domain, int type, int protocol = 0);
        explicit Socket(Descriptor &&other);
        Socket(Socket &&other) = default;

        virtual ~Socket() = default;

        Socket &operator=(Socket &&other) = default;

        Socket dup() const;
        void dup2(const Socket &other) const;

        void connect(const struct sockaddr *saddr, socklen_t len) const;
        void connect(const struct sockaddr_in &saddr) const;

        void bind(const struct sockaddr *saddr, socklen_t len) const;
        void bind(const struct sockaddr_in &saddr) const;

        void listen(int backlog = 5) const;

        Socket* accept() const;
        Socket* accept(struct sockaddr *saddr, socklen_t *len) const;

        void shutdown(int mode) const;

        size_t recv(void *buffer, size_t size, int flags = 0) const;
        size_t recvmsg(struct msghdr &msg, int flags = 0) const;
        size_t recvfrom(char *buffer,
                        size_t size,
                        struct sockaddr *saddr,
                        socklen_t *len,
                        int flags = 0) const;

        size_t send(const void *buffer, size_t size, int flags = 0) const;
        size_t sendmsg(const struct msghdr &msg, int flags = 0) const;
        size_t sendto(const char *buffer,
                      size_t size,
                      const struct sockaddr *saddr,
                      socklen_t len,
                      int flags = 0) const;
        size_t sendto(const char *buffer,
                      size_t size,
                      const struct sockaddr_in &saddr,
                      int flags = 0) const;

    private:
        explicit Socket(int fd);
        Socket(const Socket &other);

        const Socket &operator=(const Socket &other) const;
};

#endif

