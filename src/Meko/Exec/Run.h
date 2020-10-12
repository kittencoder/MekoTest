#ifndef MEKO_EXEC_RUN_H
#define MEKO_EXEC_RUN_H

#include "../Meko.h"

namespace Meko::Exec {
/**
 * Run something async using fork
 * 
 * this class is useless on it's own
 * it just does a setup for communication and forks the process
 * 
 * see RunProcess for running a process or
 * Shell for running a shell script
 */
class Run {
   public:
    /**
     * will always construct pipes for std-in, std-out and std-err
     * accessed via "stdin", "stdout" and "stderr"
     * pass additional pipes you want to have (eg. when not invoking an external process)
     */
    Run(std::unordered_map<raw_str, CommPipe::PipeType> additionalPipes = {});
    virtual ~Run();
    // ok-status of the run
    operator bool() const;
    // to be used as a oneshot (passes stdout)
    operator raw_str();
    /**
     * if you got any tasks in destructors that should NOT run in forks, check using this
     * DO FREE ALL MEMORY EVEN IN FORKED PROCESSES
     * this is just for avoiding sending something out in destructors... (which you shouldn't do)
     * ... good code won't need this, but I put it here as a safety measure...
     */
    static bool IS_FORKED_PROCESS;

   public:  // execution and execution-control
    // run the given task
    bool execute();
    // wait for the execution to finish (and start it if it hasn't been started yet)
    void wait(bool runIfNotYet = true);
    // checks the current run-status
    bool isRunning();
    // get the returned value after execution is done (will run the instance if not yet done)
    int returnValue();
    bool stoppedBySignal();

   public:  // public interface
    const static raw_str STD_IN_NAME;
    const static raw_str STD_OUT_NAME;
    const static raw_str STD_ERR_NAME;
    /**
     * get a pipe that can be read from (like stdout or other established channels)
     * 
     * when in the client the pipe-directions are being viewed as "flipped" (stdout is a "write" pipe)
     */
    std::istream& getReadPipe(raw_str_arg pipeName);
    /**
     * get a pipe that can be written to (like stdin or other established channels)
     * 
     * when in the client the pipe-directions are being viewed as "flipped" (stdin is a "read" pipe)
     */
    std::ostream& getWritePipe(raw_str_arg pipeName);

   protected:  // internal pipe management
    class InternalPipeHolder {
       public:  // add the incoming/outgoing complexity so duplex-capable-pipes are possible...
        CommPipe* incoming = nullptr;
        CommPipe* outgoing = nullptr;
    };
    CommPipe fallback;
    std::unordered_map<raw_str, InternalPipeHolder> pipes;
    void addIncomingPipe(raw_str_arg name);
    void addOutgoingPipe(raw_str_arg name);
    /**
     * translate a pipe-instance to it's name
     * when unknown returns an empty string
     */
    raw_str getPipeName(CommPipe* pipe) const;
    bool enablePipes();
    void disablePipes();
    // void updatePipes();

   protected:  // child interface
    /**
     * IMPLEMENT THIS
     * it will be called in the fork process
     * 
     * after this method is done, the process will commit suicide
     */
    virtual void EXEC() = MUST_BE_IMPLEMENTED;
    /**
     * can be implemented, called when the execution is seen as done for the first time
     * 
     * when "normalExit" is false, it has been killed by a signal
     */
    virtual void POSTEXEC(bool normalExit);
    // set this to false in your implementation if something went wrong... (eg. when exec failed)
    // it's used in the boolean operator
    bool implOk = true;

   protected:  // process related data storage
    bool host = true;
    bool pipesOk = true;
    bool forkOk = true;
    bool running = false;
    bool hasStarted = false;
    bool signalExit = false;
    int rvalue = 0;
    pid_t pid;

   protected:
    void updateChildStatus(bool wait = false);

   protected:
    bool stopChecked = false;

   public:
    /**
     * when the child has stopped execution this will return true ONCE
     * useful for pools
     */
    bool hasStopped();

   public:  // interface for pipes (and processing)
    bool isHost() const;
};
}  // namespace Meko::Exec
#endif