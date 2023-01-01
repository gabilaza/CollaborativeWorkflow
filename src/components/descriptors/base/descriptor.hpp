
#ifndef __DESCRIPTOR_H
#define __DESCRIPTOR_H

class Descriptor {
    protected:
        int _fd;
    public:
        explicit Descriptor(int fd);
        virtual ~Descriptor();

        Descriptor(const Descriptor &other) = delete;
        Descriptor(Descriptor &&other);
        Descriptor &operator=(const Descriptor &other) = delete;
        Descriptor &operator=(Descriptor &&other);

        operator int() const;

        int fcntl(int cmd);
        int fcntl(int cmd, int arg);
        int fcntl(int cmd, struct flock *fl);

        int fd() const;
};

#endif
