
#include "../Meko.h"

namespace Meko::Exec {
CommPipe::CommPipe(Run* host, PipeType type) : type(type),
                                               instance(host),
                                               writer(&*this),
                                               reader(&*this) {
}
void CommPipe::enable() {
    if (enabled) {
        return;
    }
    int pipeStatus = pipe(pipeFDs);
    if (pipeStatus < 0) {
        isOk = false;
    }
    enabled = true;
}
void CommPipe::disable() {
    if (!enabled) {
        return;
    }
    if (isOk) {
        bool openPipe = getOpenPipe();
        if (!partialCloseDone) {
            close(pipeFDs[!openPipe]);
        }
        close(pipeFDs[openPipe]);
    }
    enabled = false;
}
CommPipe::~CommPipe() {
    disable();
}
/**
 * when data is to be published
 * INCOMING host: --- read by user
 * OUTGOING host: push to other process
 * 
 * INCOMING client: push to other process
 * OUTGOING client: --- read by user (maybe signalize new data?)
 */
int CommPipe::sync() {
    if (getOpenPipe()) {
        writeToPipe();
    }
    return std::stringbuf::sync();
}
/**
 * when data is requested
 * INCOMING host: check pipes for new data
 * OUTGOING host: --- End of current data reached
 * 
 * INCOMING client: --- End of current data reached
 * OUTGOING client: check pipes for new data
 */
int CommPipe::underflow() {
    if (!getOpenPipe()) {
        readFromPipe();
    }
    return std::stringbuf::underflow();
}
//*
void CommPipe::updateContent() {
    // only in main process! we don't want to clear data from child-pipes of the main process!
    // Todo: only clear if we are the host of the to-clear childs!
    if (!(*this && enabled && (!Run::IS_FORKED_PROCESS))) {
        return;
    }
    if (!getOpenPipe()) {
        readFromPipe();
    }
}
//*/
std::ostream& CommPipe::getWriter() {
    return writer;
}
std::istream& CommPipe::getReader() {
    return reader;
}
CommPipe::operator bool() const {
    return isOk;
}
CommPipe::operator std::ostream &() {
    return getWriter();
}
CommPipe::operator std::istream &() {
    return getReader();
}
void CommPipe::readFromPipe() {
    if (!(*this && enabled)) {
        return;
    }
    getWriter() << getPipeData();
}
void CommPipe::writeToPipe() {
    if (!(*this && enabled)) {
        return;
    }
    writePipeData(getBufferData());
}
raw_str CommPipe::getBufferData() {
    if (!(*this && enabled)) {
        return "";
    }
    return Support::StringUtilities::dumpStream(*this);
}
raw_str CommPipe::getPipeData() {
    if (!(*this && enabled)) {
        return "";
    }
    std::stringstream ss;
    const int BUFSIZE = 1024;
    char buffer[BUFSIZE];
    int pipe = pipeFDs[getOpenPipe()];
    while (true) {
        ssize_t rs = read(pipe, buffer, BUFSIZE);
        if (rs == 0) {
            // no more data to read
            break;
        }
        if (rs == -1) {
            auto e = errno;
            if (e != EAGAIN) {
                isOk = false;
                std::cout << "PIPE BROKE ON READ" << std::endl;
                // TODO: report error!!!
                break;
            }
            // no more data to read
            break;
        }
        // add data that has been red
        ss << raw_str(buffer, rs);
    }
    raw_str d = ss.str();
    return d;
}
void CommPipe::writePipeData(raw_str_arg data) {
    if (!enabled) {
        return;
    }
    if (data.length() == 0) {
        return;  // nothing to write
    }
    int pipe = pipeFDs[getOpenPipe()];
    size_t length = data.length();
    for (size_t cpos = 0; cpos < length;) {
        const char* raw = data.c_str();
        ssize_t rs = write(pipe, raw + cpos, length - cpos);
        if (rs == -1) {
            isOk = false;
            std::cout << "PIPE BROKE ON WRITE" << std::endl;
            // TODO: report error!!!
            return;
        }
        cpos += rs;
    }
    fsync(pipe);
}
bool CommPipe::getOpenPipe() {
    return (type == PipeType::OUTGOING) == instance->isHost();
}
bool CommPipe::closeUnused() {
    if (!enabled) {
        return false;
    }
    bool openPipe = getOpenPipe();
    close(pipeFDs[!openPipe]);
    partialCloseDone = true;
    return unblockPipe(pipeFDs[openPipe]);
}
void CommPipe::replaceStdPipe(int fd) {
    if (!enabled) {
        return;
    }
    bool openPipe = getOpenPipe();
    // dup the not-closed pipe
    dup2(pipeFDs[openPipe], fd);
}
bool CommPipe::unblockPipe(int fd) {
    int flags = fcntl(fd, F_GETFL);
    flags |= O_NONBLOCK;
    return fcntl(fd, F_SETFL, flags);
}
}  // namespace Meko::Exec