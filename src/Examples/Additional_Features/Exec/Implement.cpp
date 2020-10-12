#include "Meko.h"

using namespace Meko;
using namespace Meko::Exec;

class ExampleTask : public Run {
   public:
    ExampleTask() : Run({
                        {"com 1", CommPipe::PipeType::MAKE_BIDIRECTIONAL},
                        {"com 2", CommPipe::PipeType::HOST_TO_CLIENT},
                        {"com 3", CommPipe::PipeType::CLIENT_TO_HOST},
                    }) {
    }

   protected:
    void EXEC() override {
        raw_str com2;
        size_t runNumber = 0;
        while ((com2 += Support::StringUtilities::dumpStream(getReadPipe("com 2"))) != "exit") {
            getWritePipe("com 3") << ++runNumber << ", " << std::flush;

            // get content from "com 1"
            raw_str input = Support::StringUtilities::dumpStream(getReadPipe("com 1"));
            // and echo it back into "com 1" towards the host
            getWritePipe("com 1") << input << std::flush;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        std::cout << "exited via: " << com2 << std::endl;
    }
};

int main(int argc, char** argv) {
    try {
        ExampleTask et;
        et.execute();

        // push content to "com 1" to make the task return it through "com 1"
        et.getWritePipe("com 1") << "some content that will be returned" << std::flush;
        // let the task work a little
        std::this_thread::sleep_for(std::chrono::seconds(1));
        // check returned content
        std::cout << "returned content: " << Support::StringUtilities::dumpStream(et.getReadPipe("com 1")) << std::endl;

        // show the run-iterations published by the task via "com 3"
        std::cout << "COM 3: " << Support::StringUtilities::dumpStream(et.getReadPipe("com 3")) << std::endl;
        // send the exit command
        et.getWritePipe("com 2") << "exit" << std::flush;
        et.wait();
        std::cout << "Stdout of task: " << (raw_str)et << std::endl;

    } catch (const Meko::Exec::ExitCleanup& e) {
        // for cleanup when we want to quit the executable (and to avoid false-positives after forking in valgrind)
    }
    return RETURN_OK;
}