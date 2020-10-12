#include "Meko.h"

using namespace Meko::Support;

int main(int argc, char** argv) {
    try {
        // check the "Color" section for more details about O and Style
        auto& o = Meko::Color::O::getInstance();
        Meko::Color::Style h(Meko::Color::FG::Light_Cyan);

        // filters
        std::cout << o.p("# Filters", h) << std::endl;
        // only allow certain characters
        std::cout << StringUtilities::az09("This string will be filtered! :3") << std::endl;
        std::cout << StringUtilities::az("This string will be filtered! :3") << std::endl;
        std::cout << StringUtilities::azAZ09("This string will be filtered! :3") << std::endl;
        std::cout << StringUtilities::azAZ("This string will be filtered! :3") << std::endl;
        // specify allowed characters yourself
        std::cout << StringUtilities::filter("This string will be filtered! :3", {"h", "i", "s", "w", "f"}) << std::endl;

        // manipulators
        std::cout << o.p("# Manipulators", h) << std::endl;
        // replace something with something else
        std::cout << StringUtilities::replace("a certain string", "certain", "different") << std::endl;
        // replacing multiple subjects with their replacements
        std::cout << StringUtilities::replace("a certain string", {
                                                                      {"certain", "different"},
                                                                      {"string", "text"},
                                                                  })
                  << std::endl;
        // to avoid infinite loops, replacements results containing other replacement subjects are not replaced
        std::cout << StringUtilities::replace("a certain string with ff's", {
                                                                                {"certain", "different"},
                                                                                {"ff", "FF"},
                                                                            })
                  << std::endl;
        // useful methods when working with escapes in a string
        // eg. you want to display raw strings without printing actual "newlines" in the terminal
        Meko::raw_str subject = "a string\\nwith actually one line";
        std::cout << subject << "->" << StringUtilities::execEscapes(subject) << std::endl;
        std::cout << StringUtilities::unexecEscapes("a string \n with a newline") << std::endl;

        std::cout << StringUtilities::uppercase("this will be uppercase") << StringUtilities::lowercase(" AND THIS LowerCASE") << std::endl;

        // inspectors
        std::cout << o.p("# Inspectors", h) << std::endl;
        // check if a string is contained within a set of strings
        if (StringUtilities::contains({"one", "two", "three"}, "two")) {
            std::cout << "The first set contains [two]" << std::endl;
        } else {
            std::cout << "The first set does not contain [two]" << std::endl;
        }
        if (StringUtilities::contains({"a", "b", "c"}, "two")) {
            std::cout << "The second set contains [two]" << std::endl;
        } else {
            std::cout << "The second set does not contain [two]" << std::endl;
        }

        // transformations
        std::cout << o.p("# Transformations", h) << std::endl;
        // split a string
        auto list = StringUtilities::explode("some string with several words", ' ');
        // manipulation of it's parts
        std::reverse(list.begin(), list.end());
        // putting it back together
        std::cout << StringUtilities::implode(" <-> ", list) << std::endl;

        // get pointer values
        std::cout << "memory address of the list: " << StringUtilities::getPointer(&list) << std::endl;

        // pad and trim
        std::cout << o.p("# Pad and Trim", h) << std::endl;
        Meko::raw_str initial = "  some string  ";
        std::cout << "left trimmed copy: [" << StringUtilities::ltrim_copy(initial) << "]" << std::endl;
        std::cout << "right trimmed copy: [" << StringUtilities::rtrim_copy(initial) << "]" << std::endl;
        std::cout << "trimmed copy: [" << StringUtilities::trim_copy(initial) << "]" << std::endl;
        // you can also trim the string in place to save performance by avoiding copies
        StringUtilities::trim(initial);
        std::cout << "permanently trimmed: [" << initial << "]" << std::endl;
        std::cout << "padded string: [" << StringUtilities::pad(initial, 30, PadDirection::PAD_BOTH, '.') << "]" << std::endl;

        // there also is a way to automatically pad strings (let's reuse the list from before)
        AutoPadder ap('_', PadDirection::PAD_LEFT);
        // show the auto-padder what content we are dealing with
        ap.show(list);
        for (const auto& s : list) {
            // pad each entry
            std::cout << "[" << ap(s) << "]" << std::endl;
        }
        std::cout << "computed AutoPadder width: " << ap.getWidth() << std::endl;

    } catch (const Meko::Exec::ExitCleanup& e) {
        // for cleanup when we want to quit the executable (and to avoid false-positives after forking in valgrind)
    }
    return RETURN_OK;
}