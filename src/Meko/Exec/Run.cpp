
#include "../Meko.h"

namespace Meko::Exec {
const raw_str Run::STD_IN_NAME = "stdin";
const raw_str Run::STD_OUT_NAME = "stdout";
const raw_str Run::STD_ERR_NAME = "stderr";
bool Run::IS_FORKED_PROCESS = false;
Run::Run(std::unordered_map<raw_str, CommPipe::PipeType> additionalPipes) : fallback(this, CommPipe::PipeType::EMPTY) {
    pipes[STD_IN_NAME] = {nullptr, new CommPipe(this, CommPipe::PipeType::HOST_TO_CLIENT)};
    pipes[STD_OUT_NAME] = {new CommPipe(this, CommPipe::PipeType::CLIENT_TO_HOST)};
    pipes[STD_ERR_NAME] = {new CommPipe(this, CommPipe::PipeType::CLIENT_TO_HOST)};
    for (const auto& [pipeName, pipeType] : additionalPipes) {
        switch (pipeType) {
            case CommPipe::PipeType::MAKE_BIDIRECTIONAL:
                addIncomingPipe(pipeName);
                addOutgoingPipe(pipeName);
                break;
            case CommPipe::PipeType::INCOMING:
                addIncomingPipe(pipeName);
                break;
            case CommPipe::PipeType::OUTGOING:
                addOutgoingPipe(pipeName);
                break;
            default:
                // other types will be ignored...
                break;
        }
    }
}
void Run::POSTEXEC(bool normalExit) {
    NOT_USED(normalExit);
}
bool Run::isHost() const { return host; }
Run::operator bool() const {
    return pipesOk && forkOk && implOk;
}
Run::operator raw_str() {
    wait(true);
    return Support::StringUtilities::dumpStream(getReadPipe(STD_OUT_NAME));
}
void Run::addIncomingPipe(raw_str_arg name) {
    const auto& it = pipes.find(name);
    if (it != pipes.end()) {
        if (it->second.incoming == nullptr) {
            pipes[name].incoming = new CommPipe(this, CommPipe::PipeType::INCOMING);
        }
        // do not override existing pipes...
    } else {
        pipes[name] = {new CommPipe(this, CommPipe::PipeType::INCOMING)};
    }
}
void Run::addOutgoingPipe(raw_str_arg name) {
    const auto& it = pipes.find(name);
    if (it != pipes.end()) {
        if (it->second.outgoing == nullptr) {
            pipes[name].outgoing = new CommPipe(this, CommPipe::PipeType::OUTGOING);
        }
        // do not override existing pipes...
    } else {
        pipes[name] = {nullptr, new CommPipe(this, CommPipe::PipeType::OUTGOING)};
    }
}
Run::~Run() {
    wait(false);
    for (const auto& [pipeName, pipeHolder] : pipes) {
        if (pipeHolder.incoming != nullptr) {
            delete pipeHolder.incoming;
        }
        if (pipeHolder.outgoing != nullptr) {
            delete pipeHolder.outgoing;
        }
    }
}
raw_str Run::getPipeName(CommPipe* pipe) const {
    for (const auto& [pipeName, pipeHolder] : pipes) {
        if (pipeHolder.incoming == pipe || pipeHolder.outgoing == pipe) {
            return pipeName;
        }
    }
    return "";
}
std::istream& Run::getReadPipe(raw_str_arg pipeName) {
    execute();
    const auto& it = pipes.find(pipeName);
    if (it == pipes.end()) {
        return fallback;
    }
    if (isHost()) {
        if (it->second.incoming == nullptr) {
            return fallback;
        }
        return *it->second.incoming;
    } else {
        if (it->second.outgoing == nullptr) {
            return fallback;
        }
        return *it->second.outgoing;
    }
}
std::ostream& Run::getWritePipe(raw_str_arg pipeName) {
    execute();
    const auto& it = pipes.find(pipeName);
    if (it == pipes.end()) {
        return fallback;
    }
    if (isHost()) {
        if (it->second.outgoing == nullptr) {
            return fallback;
        }
        return *it->second.outgoing;
    } else {
        if (it->second.incoming == nullptr) {
            return fallback;
        }
        return *it->second.incoming;
    }
}
bool Run::isRunning() {
    if (!(hasStarted && running)) {
        return false;
    }
    updateChildStatus();
    return running;
}
int Run::returnValue() {
    wait();
    return rvalue;
}
bool Run::stoppedBySignal() {
    return signalExit;
}
void Run::wait(bool runIfNotYet) {
    if (runIfNotYet && !hasStarted) {
        // will return if not necessary
        execute();
    }
    while (running) {
        //updatePipes();
        updateChildStatus(true);
    }
}
void Run::updateChildStatus(bool wait) {
    if (!(hasStarted && running)) {
        return;
    }
    int options = (!wait) * WNOHANG;
    pid_t status = 0;
    pid_t r = waitpid(pid, &status, options);
    if (r == 0 && (!wait)) {
        return;
    }
    if (WIFEXITED(status)) {
        rvalue = WEXITSTATUS(status);
        POSTEXEC(true);
        disablePipes();
        running = false;
        return;
    }
    if (WIFSIGNALED(status)) {
        rvalue = WTERMSIG(status);
        POSTEXEC(false);
        disablePipes();
        signalExit = true;
        running = false;
        return;
    }
}
bool Run::enablePipes() {
    for (const auto& [pipeName, pipeHolder] : pipes) {
        if (pipeHolder.incoming != nullptr) {
            pipeHolder.incoming->enable();
            if (!(*pipeHolder.incoming)) {
                return false;
            }
        }
        if (pipeHolder.outgoing != nullptr) {
            pipeHolder.outgoing->enable();
            if (!(*pipeHolder.outgoing)) {
                return false;
            }
        }
    }
    return true;
}
void Run::disablePipes() {
    for (const auto& [pipeName, pipeHolder] : pipes) {
        if (pipeHolder.incoming != nullptr) {
            pipeHolder.incoming->updateContent();
            pipeHolder.incoming->disable();
        }
        if (pipeHolder.outgoing != nullptr) {
            pipeHolder.outgoing->updateContent();
            pipeHolder.outgoing->disable();
        }
    }
}
/*
void Run::updatePipes() {
    for (const auto& [pipeName, pipeHolder] : pipes) {
        if (pipeHolder.incoming != nullptr) {
            pipeHolder.incoming->updateContent();
        }
        if (pipeHolder.outgoing != nullptr) {
            pipeHolder.outgoing->updateContent();
        }
    }
}
//*/
bool Run::hasStopped() {
    if (stopChecked || (!hasStarted)) {
        return false;
    }
    if (!isRunning()) {
        stopChecked = true;
        return true;
    }
    return false;
}
bool Run::execute() {
    if (hasStarted || (!isHost())) {
        //pseudoOk || guard...
        return true;
    }
    if ((!*this)) {
        return false;
    }
    if (!enablePipes()) {
        pipesOk = false;
        disablePipes();
        return false;
    }
    pid = fork();
    hasStarted = true;
    if (pid == -1) {
        forkOk = false;
        return false;
    } else if (pid == 0) {
        host = false;
        IS_FORKED_PROCESS = true;
    }
    running = true;
    for (const auto& [pipeName, pipeHolder] : pipes) {
        if (pipeHolder.incoming != nullptr) {
            pipeHolder.incoming->closeUnused();
        }
        if (pipeHolder.outgoing != nullptr) {
            pipeHolder.outgoing->closeUnused();
        }
    }
    if (!isHost()) {
        pipes[STD_IN_NAME].outgoing->replaceStdPipe(0);
        pipes[STD_OUT_NAME].incoming->replaceStdPipe(1);
        pipes[STD_ERR_NAME].incoming->replaceStdPipe(2);
        EXEC();
        throw ExitCleanup();
    }

    return true;
}

}  // namespace Meko::Exec