# Color

Printing colors in the terminal requires escape-sequences like `\e[95m` for light magenta. `Meko::Color` has a collection of classes dealing with that.

## Printing colors in the terminal

At the Core of working with Color's in meko is the (C)`O`(olor) class. It's a singleton, that means you only get an instance reference via `O::getInstance()`.
This instance is global and changing properties like disabling the color output has global effect.
You can pass foreground and background colors as well as `Style` objects to it's `p`(riniting) method.

## Style-Objects

Style objects contain information about the desired foreground and background colors. This way you can define colors in one place and use that style in different places.
You can also create inverse style objects by flipping the foreground and background colors.