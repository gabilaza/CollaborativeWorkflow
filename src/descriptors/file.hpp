
#ifndef __FILE_H
#define __FILE_H

#include <string>
#include "base/iodescriptor.hpp"


class File : public IODescriptor {
    public:
        File(const char *path, int flags = 0);
        File(const char *path, int flags, mode_t mode);
        File(const std::string &path, int flags = 0);
        File(const std::string &path, int flags, mode_t mode);
        virtual ~File() = default;

        explicit File(Descriptor &&other);
        File(File &&other) = default;
        File &operator=(File &&other) = default;

        File dup() const;
        void dup2(const File &other) const;

        size_t lseek(off_t offset, int whence) const;

        void fstat(struct stat &st) const;

    private:
        File(const File &other);
        const File &operator=(const File &other) const;
};

#endif

