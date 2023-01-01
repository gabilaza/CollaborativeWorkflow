
#ifndef __IODESCRIPTOR_H
#define __IODESCRIPTOR_H

#include "descriptor.hpp"


class IODescriptor : public Descriptor {
    public:
        explicit IODescriptor(int fd);
        explicit IODescriptor(Descriptor &&other);
        virtual ~IODescriptor() = default;

        IODescriptor(const IODescriptor &other) = delete;
        IODescriptor(IODescriptor &&other) = default;
        IODescriptor &operator=(const IODescriptor &other) = delete;
        IODescriptor &operator=(IODescriptor &&other) = default;

        size_t read(char *buffer, size_t size) const;
        size_t write(const char *buffer, size_t size) const;

        size_t pread(char *buffer, size_t size, size_t off) const;
        size_t pwrite(const char *buffer, size_t size, size_t off) const;
};

#endif

