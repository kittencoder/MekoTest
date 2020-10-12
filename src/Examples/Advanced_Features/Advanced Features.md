# Advanced Features

## preparation and cleanup

You can use the methods `prepare` and `cleanup` to initialize test-data and cleanup afterwards. 
When returning `false` in `prepare` the test is considered to have failed already. (useful if you can't locate test-relevant data.)
When `prepare` fails, `cleanup` won't be called.

## Timing

Knowing the efficency of code can be essential when trying to optimize as good as possible, so this testing framework provides methods to control the measurement of time while testing. Sometimes you only want to measure a part of what has to be done to successfully complete a test.
If you want to measure a section of code you can use `startTimer` and `stopTimer` to let the framework measure time for you, if you got a time-value you can pass it directly via `addTime`.

Only successfully executed tests accumulate to the displayed average topic-time in the first table.

When not providing any time-data the test will measure how long the execution of the `ExecuteTest` method took automatically.

## Multiple Runs

In this example you are shown how multiple runs behave.

You can tell the TestSuite how often each test should be executed by passing it to it's `run` method.

This is used to determine a better average execution time if needed.

Be aware, that between runs all data will be kept, so be sure to reset everything test-relevant, that has changed (or don't save it at all).
This can be used to exclude test-resource-loading (like loading data from a file) from the test itself and then execute the test afterwards.

## Known Topics

Sometimes when you are writing dynamic components and test-files you don't know what topics are required to be tested at compile-time and want to be informed when there are untested ones. To do this you can provide a full list of topics to the `Result` via `setAllTopics`.

Eg. when testing website features you can use the website's URL as a topic and implement tests specific for that website. the list of websites is red from a file and the TestSuite will show you which websites are untested.

## Sub Topics

In more complicated test-scenarios you might be reading topic-data from a file and in case you can determine, that some functionality can cover other functionality you can provide that information too.

Eg. when testing a website you got `availability` as a topic and `login-test` as a topic and when testing the login feature you also cover `availability` without writing a test specifically for that.

## Simple Topics

Sometimes when gathering a list of topics dynamically you can determine which of those topics are so simple, that they might not need testing. (eg. when the test would only consist of "Value A == Value B").

Simple topics will only be shown when requested specifically at the point the report is rendered. (How to do that can be found in the Display-Features).

Simple topics can be set via `setSimpleTopics`.

## Duplicates

Sometimes tests for Topic A are exactly the same as Topic B. Such cases can be handled by passing direct references to `setDuplicates`.
