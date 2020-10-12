#include "Meko.h"

using namespace Meko;
using namespace Meko::Exec;

int main(int argc, char** argv) {
    try {
        // oneshot (blocks until the task is done)
        raw_str result = Shell("find . | egrep -i '\\.cpp'");
        std::cout << "cpp files:" << std::endl
                  << result;

        // execution in background
        Shell async("sleep 2;echo test123");
        // trigger start of execution
        if (!async.execute()) {
            std::cout << "failed to execute!" << std::endl;
        }
        std::cout << "executing async with sleep:" << std::endl
                  << (raw_str)async;

    } catch (const Meko::Exec::ExitCleanup& e) {
        // for cleanup when we want to quit the executable (and to avoid false-positives after forking in valgrind)
    }
    return RETURN_OK;
}