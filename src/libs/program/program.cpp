#include "program.hpp"
#include "log/stdio_log_backend.hpp"
#include "log/file_log_backend.hpp"
#include "system/env.hpp"
#include "utility/strcat.hpp"

#include <limits>

gorc::program::program()
{
    // Initialize logging

    // Logging to standard output shouldn't be too verbose
    flag_set<log_level> stdout_diagnostic_level {
        log_level::error,
        log_level::warning,
        log_level::info
    };

    if(has_environment_variable("GORC_DEBUG")) {
        stdout_diagnostic_level += log_level::debug;
    }

    emplace_log_backend<stdio_log_backend>(stdout_diagnostic_level,
                                           (has_environment_variable("GORC_NO_WORD_WRAP") ?
                                            std::numeric_limits<int>::max() : 80));

    // Enable file logging if the correct environment variables are set
    maybe<std::string> maybe_log_file = get_environment_variable("GORC_LOG_FILE");
    maybe_if(maybe_log_file, [](std::string const &log_file) {
            emplace_log_backend<file_log_backend>({ log_level::error,
                                                    log_level::warning,
                                                    log_level::info,
                                                    log_level::debug,
                                                    log_level::trace },
                                                  log_file);
            // Clear the log file env var to prevent child processes from writing to
            // the same log file.
            set_environment_variable("GORC_LOG_FILE", "");
        });
}

int gorc::program::start(range<char**> const &args)
{
    try {
        create_options(opts);
        opts.load_from_arg_list(args);

        return run();
    }
    catch(logged_runtime_error const &) {
        // Error message was logged at a lower level.
        // Terminate without re-logging.
        return EXIT_FAILURE;
    }
    catch(std::exception const &e) {
        LOG_ERROR(strcat("Exception thrown: ", e.what()));
        return EXIT_FAILURE;
    }
    catch(...) {
        LOG_ERROR("Unknown exception thrown");
        return EXIT_FAILURE;
    }
}
