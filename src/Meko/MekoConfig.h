/**
 * the default shell and pre-command-args for Meko::Exec::Shell
 */
#define DEFAULT_RUN_SHELL "sh"
#define DEFAULT_RUN_SHELL_ARGS "-c"

/// /// ///
/// /// /// TYPES USED IN PROJECT (for quick exchange)
/// /// ///
/**
 * use raw_str when storing a string
 * use raw_str_arg when strings are being passed as arguments and not modified (const and references the string)
 */
namespace Meko {
// data/text storage
typedef std::string raw_str;
typedef const raw_str& raw_str_arg;
#define to_raw_str std::to_string

// time resolution adjustment
// time high resolution
typedef std::chrono::nanoseconds time_hr;
#define DC_HR std::chrono::duration_cast<time_hr>
// time medium resolution
typedef std::chrono::milliseconds time_mr;
#define DC_MR std::chrono::duration_cast<time_mr>
// time normal resolution
typedef std::chrono::seconds time_nr;
#define DC_NR std::chrono::duration_cast<time_nr>
typedef std::chrono::system_clock clock;
}  // namespace Meko