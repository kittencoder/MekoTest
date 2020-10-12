# Basics

These examples provide a basic understanding of the testing framework developed for Meko.

Here comes a list of details about everything you will find there

## Simple1 - Basic Usage

1. you only have to include the `Meko.h` file! (so simple, few include, such simplicity)\
Meko uses some headers (found in `externals.h`) that are available if you have g++ installed.

That's all you have to include if you want to use `Meko` and it's parts.

Further configuration of `Meko` is achieved using the `MekoConfig.h` file.
Copy it one directory up and change values (without changing the original `MekoConfig.h` file)

2. Every test is a class that implements `Meko::TestSuite::Test`.

First you have to set the `Test` class up:
- testname: The name of this test will be shown when it fails
- topic: tests with the same topic are grouped together.
    Use this when using multiple tests to test a single feature.
- shouldSucceed: set this to false if your test is designed to fail.
    eg. you want to trigger a divide-by-zero or something

Then you implement a method being used for testing.
Returning true means success, returning false failure. Easy, right?

IMPORTANT: you should be aware, that this method might be called multiple times to get a better average runtime value if requested.
More on that later.

3. now let's look at the main method of this application:

Catch all `Meko::Exec::ExitCleanup` exceptions and exit your application. This ensures, that all objects are properly destructed, memory is freed and no leak is possible. (and also valgrind won't cry ;3)

Now to the testing:

First you need a `Meko::TestSuite::TestSuite` instance. This collects tests, runs them and spits out a `Meko::TestSuite::Report`.

Add tests via the `addTest` method. The suite will take of deleting those tests and freeing their memory.

then call the `run` method to execute tests. This results in a `Report` that can be stringified via it's `render` method and printed to the `stdout` of your application.

### Trying out

try to modify the return-value of your test and what it passes to the `Test` constructor. Also try adding your own test-class and adding some more to the `TestSuite`.

It's real power shines when you add loads of tests.

## Simple2 - Reading the report

You might have noticed some fancy output with lot's of data. but don't be afraid! it will all make sense.

A test-report is divided into multiple sections, that can be turned on and off. More on that and some advanced sections later.

First we are going to take a look at some basic data you can gather:

### The Graph

That's the fancy box with + and - signs in it.
Each row describes a tested topic and has the following data:

1. The name of the tested topic.

This is what tests are grouped by.

2. a + and - row showing graphically how many positive and negative tests are present and how many were successful and how many have failed.

The more red you see here, the worse are the results.

3. the number of successful positive tests / total count of positive tests

Positive tests are tests where you expect the result to be `true`. Those are the usual tests you might know.

3. the number of successful negative tests / total count of negative tests

Negative tests are tests designed to fail.
When one of those return a positive result it's considered a failure.
Eg. when you expect your code to throw an exception, yet it does not.

4. average time of a successful positive test
5. average time of a successful negative test

### The Error List

Every test that does not meet it's requirements will be listed here.
First the topic will be announced, followed by a list of the following data:
1. the testname
2. the reason that test failed
3. a desceription provided by that test

### Statistics

Don't be confused when the "total time" is more that the time you spent testing since tests might have been run in parallel.

### Information while running tests and more

You might have noticed the `Done...Running...` text popping up before the results were shown.
This contains details on running tests:
- Done: how many tests have been completed
- Running: how many tests are currently running/maximum parallel tests
- Ok/Fail: in green you can see the number of successful tests, in red you can already see if some tests have failed
- followed by that is the time passed since testing has started
- and last you can see an estimate of how much more (reallife) time the tests will take until completing all.

you can hide this output by passing `false` as the first argument to the `TestSuite`.

Sections not yet described will follow in the other Example-Explainations.

## FailException

You can also use exceptions to mark the failure of a test.
When throwing a `std::exception` the "what()" will be used to fill in the error-details.

If you want more control over the reason and success-state, you can use a `Meko::TestSuite::TestFailed` exception.
