/**
 * Thread-safe singleton concept patron taken from
 * https://refactoring.guru/fr/design-patterns/singleton/cpp/example#example-1
 */

#pragma once

#define LOG_DIR             "logs"
#define LOG_LATEST_FILENAME LOG_DIR "/latest.log"

// Logging macros for easier access throughout the application
#define LOG_DEBUG(s)    Logger::log(logger::Level::DEBUG, s)
#define LOG_INFO(s)     Logger::log(logger::Level::INFO, s)
#define LOG_ERR(s)      Logger::log(logger::Level::ERROR, s)
#define LOG_WARN(s)     Logger::log(logger::Level::WARNING, s)
#define LOG_CRIT(s)     Logger::log(logger::Level::CRITICAL, s)
#define LOG_FATAL(s)    Logger::log(logger::Level::FATAL, s)

#include <cstdint>
#include <mutex>
#include <string>
#include <fstream>
#include <source_location>

namespace logger
{

    /**
     * Defines the severity levels for log messages.
     */
    enum class Level
        : uint8_t
    {
        DEBUG,
        INFO,
        ERROR,
        WARNING,
        CRITICAL,
        FATAL
    };

}

/**
 * Defines a Logger class responsible for logging messages with varying
 * severity levels, writing to both console and file, and providing formatting
 * utilities for log details.
 */
class Logger final
{
public:
    Logger(const Logger &) = delete;
    Logger &operator=(const Logger &) = delete;

    /**
     * Initializes the Logger singleton instance.
     * Must be called before using the logger.
     *
     * @param   minimumLevel    Minimum log level to be recorded
     */
    static void init(
        const std::string& projectName,
        int argc,
        const char* argv[],
        logger::Level minimumLevel = logger::Level::INFO
    );

    /**
     * Logs a message at the given severity level.
     *
     * @param   level   Log severity level
     * @param   message The message to log
     * @param   loc     From which function the log has been called from
     */
    static void log(
        logger::Level level,
        const std::string &message,
        const std::source_location &loc = std::source_location::current()
    );

    /**
     * @returns True if Logger has been initialized. Otherwise, false.
     */
    static bool isInit();

    /**
     * Generates a timestamp string in formatted style.
     *
     * @param   forFilename If true, format suitable for filenames is
     *                      returned
     * @returns The formatted timestamp
     */
    static std::string getFormattedCurrentTimestamp(bool forFilename = false);

protected:
    std::ofstream _logFile;
    std::ofstream _latestLogFile;
    logger::Level _minimumLevel;

    /**
     * Constructs the Logger with given filepath and minimum log level.
     */
    Logger(
        const std::string &filepath,
        logger::Level minimumLevel
    );

    /**
     * Cleans up and closes file stream on destruction.
     */
    ~Logger();

    /**
     * @returns Reference to the singleton Logger instance
     */
    static Logger &getInstance();

    /**
     * Converts a log level enum to its string representation.
     *
     * @param   level   Log severity level
     * @returns The corresponding string name
     */
    static std::string levelToString(logger::Level level);

    /**
     * Returns the color-coded version of the log level for terminal
     * output.
     *
     * @param   level   Log severity level.
     * @returns Colored level string.
     */
    static std::string getColoredLevel(logger::Level level);

    /**
     * @param   loc         From which function the log has been called from
     * @param   colorized   True for colorized string, otherwise false
     * @return  Formatted location
     */
    static std::string getFormattedLocation(
        const std::source_location &loc,
        bool colorized = true
    );

    /**
     * Generates the full log file path with the project name and timestamp.
     *
     * @param   projectName Name of the project that is being logged
     * @returns The full path to the log file
     */
    static std::string generateLogFile(const std::string& projectName);

    /**
     * Writes the log message to console and file, depending on level.
     *
     * @param   level   Log severity level
     * @param   message Message content
     * @param   loc     From which function the log has been called from
     */
    void write(
        logger::Level level,
        const std::string &message,
        const std::source_location &loc
    );


    /**
     * Writes the header information to the log file including project name,
     * command line arguments, system information (OS name, kernel version),
     * and minimum log level configuration. Also includes a random quote for
     * entertainment :)
     *
     * @param   projectName Name of the project that is being logged
     * @param   argc        Number of command line arguments
     * @param   argv        Array of command line argument strings
     */
    void writeHeader(
        const std::string& projectName,
        int argc,
        const char* argv[]
    );

private:
    static Logger *INSTANCE;
    static std::recursive_mutex MUTEX;

    static std::string getRandomQuote();
};
