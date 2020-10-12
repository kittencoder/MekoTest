# Exec

Sometimes you want to execute tasks asynchronously, in a confined process or some other applications or even shell-scripts.
This section describes how to use the following classes to achieve just this.

The `Meko::Exec::Run` class encapsulates std-in, std-out and std-err so sub-processes won't clutter the terminal and their output and input can be controlled and used in the application.

Be aware that nested `Run` executions are NOT YET supported!

## Shell

The `Meko::Exec::Shell' class allows you to execute shell-scripts like `ls | processor > filename.file`. If you only want to execute a simple progress stick with `Meko::Exec::RunProcess` described below, since it doesn't load a whole shell-environment.

### Shell-Advanced

Here you are shown how to work with input and output streams(/pipes) and how to retrieve the processes return value.

## RunProcess

With `Meko::Exec::RunProcess` (used internally by the Shell-class) you can execute external applications.

## Implementing Run

Sometimes you wish to move parts of your application into a separate confined process that is running independently from the main process.
Here you can even set up separate communication-channels.

A full example of this would be the `Meko::TestSuite::Test` class.

## Communication

Communication with an executed task can be performed via Uni- and Bi-directional pipes.
Unidirectional pipes are like std-in, std-out and std-err transfering data from host to client or the other way around.
Bidirectional pipes can be used to transmit data in both directions using a single identifier.

By default only std-in, std-out and std-err are piped that way, additional pipes can be passed to the `Run`-constructor (see `Test` mentioned above).

Examples of this communication can be found in the `ShellAdvanced.cpp` and `Implement.cpp` files.
