# Display Features

## Enabling and disabling sections

Sometimes you might only be interested in certain result-data like timing or a list of errors, sometimes you want to show more data.
To do that you can an instance of `Report::ReportDisplayConfig` to the render method. On that instance you can change methods described below. Those methods can be chained together.
Additionally you can also manipulate the variables held by that object if you want more dynamic control over it.

### disableColor
This disables the output of color when reporting
### disableGraph
Disables the per-topic graph.
### skipMissingReport
Missing topics won't be reported at all. This also disables reporting missing simple topics as well as missing sub-topics.
### showSimpleTopics
Activating this shows missing simple topics that have not been covered and would be hidden by default.
### hideSubTopics
Disables the report of "possibly" indirectly tested sub-topics.
### showDuplicates
Outputs a list of all known duplicate topics.
### hideFailDetails
Skip rendering the list of failed tests and their details.
### setGraphWidth
Set's the maximum +- bar-width in the graph
### approximateRuns
When displaying the average runtime per test in the graph, multiply that time by the given amount to approximate a time that would be necessary when running that test x times. (eg. you want to know how long it would take to run that functionality a million times in sequence.)
### hideStatistics
Hides the final statistics table.
### hideSimpleInGraph
Hide tested simple topics in the first graph table (to avoid clutter when there are many simple topics being tested and you are interested in complex topics only.)


## Combining multiple results

This features comes in very handy when testing plugins or extensions.
Each of said plugin might test it's own features and provide a report to a central hub containing the results of it's tests.

These tests can be combined into a single `Report` to avoid having to print and read multiple reports.

You can even combine multiple `CombinedResults` recursively.

## Providing "translations"

You can change the statically displayed text-parts by providing a translation object. All static text's are being red from that object.

You can use it to adjust the texts to your needs (eg. change "Topic" to "Situation" or "Website", etc.) or provide translations.

Unset translations will remain in their default form.
