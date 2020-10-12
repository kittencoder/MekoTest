#ifndef MEKO_EXEC_COMMPIPE_H
#define MEKO_EXEC_COMMPIPE_H

#include "../Meko.h"

namespace Meko::Exec {
class Run;
/**
 * a communication-pipe to communicate between different processes
 * 
 */
class CommPipe : public std::stringbuf {
   public:
    enum class PipeType {
        // this type is used as a fallback for when no pipe is found... (incoming data will be deleted, never outputs data)
        EMPTY,

        // this is used for Meko::Exec::Run construction and will be translated to 2 pipes with in and out
        MAKE_BIDIRECTIONAL,

        // pipes can have one of those two types

        // transfer from host->client
        OUTGOING,
        // transfer from client->host
        INCOMING,

        // aliases
        HOST_TO_CLIENT = OUTGOING,
        CLIENT_TO_HOST = INCOMING,
    } const type;
    /**
     * only accepts the STD_*, OUTGOING or INCOMING as valid types
     * STD_* will be converted to OUTGOING/INCOMING
     */
    CommPipe(Run* host, PipeType type);
    ~CommPipe();
    /**
     * when data is to be published
     * INCOMING host: --- read by user
     * OUTGOING host: push to other process
     * 
     * INCOMING client: push to other process
     * OUTGOING client: --- read by user (maybe signalize new data?)
     */
    int sync() override;
    /**
     * when data is requested
     * INCOMING host: check pipes for new data
     * OUTGOING host: --- End of current data reached
     * 
     * INCOMING client: --- End of current data reached
     * OUTGOING client: check pipes for new data
     */
    int underflow() override;
    /**
     * get a handle to write to this buffer (exposed by run)
     */
    std::ostream& getWriter();
    /**
     * get a handle to read from this buffer (exposed by run)
     */
    std::istream& getReader();

    /**
     * bridge operator to getWriter
     */
    operator std::ostream &();
    /**
     * bridge operator to getReader
     */
    operator std::istream &();

    /**
     * for being called right before closing (will read all data from the pipe/push all data to the pipe)
     * 
     * won't be run if in child-process
     */
    void updateContent();

   public:  // Meko::Exec::Run interface for setting up
    /**
     * enable pipes THIS CAN ONLY BE USED ONCE!
     */
    void enable();
    /**
     * disable pipes CAN't BE REACTIVATED LATER
     */
    void disable();
    /**
     * call after the fork
     */
    bool closeUnused();
    /**
     * called in the client to redirect stdin, stdout and stderr to the host-pipes instead of the host-stdin, stdout, stderr
     */
    void replaceStdPipe(int fd);

    operator bool() const;

   protected:  // internal init
    bool unblockPipe(int fd);

   protected:  // internal communication with pipes
    /**
     * INCOMING HOST : 0
     * OUTGOING HOST : 1
     * INCOMING CLIENT : 1
     * OUTGOING CLIENT : 0
     */
    bool getOpenPipe();
    void readFromPipe();
    void writeToPipe();
    /**
     * get data stored in the pipeFD
     */
    raw_str getPipeData();
    /**
     * get data stored in this internal buffer for writing to the pipe
     */
    raw_str getBufferData();
    /**
     * write data to the pipeFD
     */
    void writePipeData(raw_str_arg data);

   protected:  // storage
    bool isOk = true;
    bool partialCloseDone = false;
    int pipeFDs[2];
    bool enabled = false;
    Run* instance;
    std::ostream writer;
    std::istream reader;
};
}  // namespace Meko::Exec
#endif