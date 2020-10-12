# Strings

The `StringUtilities` class is a collection of many useful features, check the Strings.cpp file for simple example for most of them.

# Displaying Time

Besides the `StringUtilities` there is a `TimeUtilities` and `ETASimple` helping with time-related issues.

`TimeUtilities` has a `renderSimple` method helping with displaying time in a human-readable format.

it renders time-values (std::chrono::duration) using hours, minutes, seconds, milliseconds and nanoseconds.
- Bigger zero values will be hidden (eg. 0h 0m 3s is shown as just 3s)
- By default it tries to avoid ms and ns when there is at least one unit of higher order (eg. 20ms is shown as such, yet when printing 1s and 20ms, the 20ms will be hidden)
- you can enable a precise printing including ms and ns when passing `true` to the `precise` parameter

Also included is an ETA-calculator