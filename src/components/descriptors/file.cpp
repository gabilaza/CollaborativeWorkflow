
#include <sys/file.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cerrno>
#include "file.hpp"
#include "utils/logs.hpp"

static const char *tag = "File";


File::File(const char *path, int flags) : IODescriptor(open(path, flags)) {
    if(_fd < 0)
        logger::raiseSystemError(tag, "File()");
}


File::File(const char *path, int flags, mode_t mode) : IODescriptor(open(path, flags, mode)) {
    if(_fd < 0)
        logger::raiseSystemError(tag, "File()");
}


File::File(const std::string &path, int flags) : File(path.c_str(), flags) {
}


File::File(const std::string &path, int flags, mode_t mode) : File(path.c_str(), flags, mode) {
}


File::File(Descriptor &&other) : IODescriptor(std::move(other)) {
    if(_fd < 0)
        logger::raiseSystemError(tag, "File()", EBADF);
}


File::File(const File &other) : IODescriptor(other._fd) {
    if(_fd < 0)
        logger::raiseSystemError(tag, "dup()");
}


const File &File::operator=(const File &other) const {
    int err = ::dup2(other._fd, _fd);
    if(err < 0)
        logger::raiseSystemError(tag, "dup2()");

    return *this;
}


File File::dup() const {
    return File(*this);
}


void File::dup2(const File &other) const {
    *this = other;
}


size_t File::lseek(off_t offset, int whence) const {
    auto n = ::lseek(_fd, offset, whence);
    if(n < 0)
        logger::raiseSystemError(tag, "lseek()");

    return static_cast<size_t>(n);
}


void File::fstat(struct stat &st) const {
    auto err = ::fstat(_fd, &st);
    if (err < 0)
        logger::raiseSystemError(tag, "fstat()");
}

