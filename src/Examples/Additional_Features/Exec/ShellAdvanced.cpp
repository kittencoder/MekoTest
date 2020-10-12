#include "Meko.h"

using namespace Meko;
using namespace Meko::Exec;

int main(int argc, char** argv) {
    try {
        // the whole interface used to communicate with this implementation of `Run` can be used with all implementations

        Shell advancedIO("scriptvar=`cat`;ls -lah --color $scriptvar;exit 42");
        // pass a directory to std-in of the executed process to be listed
        // REMEMBER TO FLUSH!!!
        advancedIO.getWritePipe(Run::STD_IN_NAME) << "/var" << std::flush;

        // wait for the process to finish (the following two steps are automatically done when casting to string)
        advancedIO.wait();
        // dump the whole istream of stdout
        std::cout << "result of advancedIO:" << std::endl
                  << Support::StringUtilities::dumpStream(advancedIO.getReadPipe(Run::STD_OUT_NAME)) << std::endl;
        // also you have access to the return value of that process once it's done executing
        std::cout << "process exited with exit code: " << advancedIO.returnValue() << std::endl;

        // you can also use different shells
        std::cout << "different shell in use:" << (raw_str)Shell("echo $0", "bash") << std::endl;

    } catch (const Meko::Exec::ExitCleanup& e) {
        // for cleanup when we want to quit the executable (and to avoid false-positives after forking in valgrind)
    }
    return RETURN_OK;
}