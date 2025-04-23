#define EXIT_KO 84

#include "Logger.hpp"
#include "Colors.hpp"
#include "Exception/Exceptions/CouldNotOpenFile.hpp"

#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

Logger *Logger::INSTANCE = nullptr;
std::recursive_mutex Logger::MUTEX;

void
Logger::init
(
    const logger::Level minimumLevel
)
{
    std::lock_guard lock(MUTEX);

    if (INSTANCE == nullptr) {
        INSTANCE = new Logger(
            generateLogFile(),
            minimumLevel
        );

        log(
            logger::Level::INFO,
            "Logger initialized! "
            "Minimum log level: " + levelToString(minimumLevel)
        );
    }
}

void
Logger::log
(
    const logger::Level level,
    const std::string &message,
    const std::source_location &loc
)
{
    getInstance().write(level, message, loc);
}

Logger::Logger
(
    const std::string &filepath,
    const logger::Level minimumLevel
)
    : _minimumLevel(minimumLevel)
{
    if (!fs::exists(LOG_DIR)) {
        fs::create_directory(LOG_DIR);
    }

    this->_logFile.open(filepath, std::ios::app);

    if (!this->_logFile.is_open()) {
        throw logger::exception::CouldNotOpenFile(filepath);
    }

    this->_latestLogFile.open(LOG_LATEST_FILENAME, std::ios::trunc);

    if (!this->_latestLogFile.is_open()) {
        throw logger::exception::CouldNotOpenFile(LOG_LATEST_FILENAME);
    }
}

Logger::~Logger
()
{
    if (this->_logFile.is_open()) {
        this->_logFile.close();
    }

    if (this->_latestLogFile.is_open()) {
        this->_latestLogFile.close();
    }
}

bool
Logger::isInit
()
{
    return INSTANCE != nullptr;
}

Logger &
Logger::getInstance
()
{
    std::lock_guard lock(MUTEX);

    if (INSTANCE == nullptr) {
        std::cerr << "Logger not initialized! Call Logger::init() first." << std::endl;
        std::exit(EXIT_KO);
    }
    return *INSTANCE;
}

std::string
Logger::levelToString
(
    const logger::Level level
)
{
    switch (level)
    {
        case logger::Level::DEBUG:
            return "DEBUG";
        case logger::Level::INFO:
            return "INFO";
        case logger::Level::WARNING:
            return "WARNING";
        case logger::Level::ERROR:
            return "ERROR";
        case logger::Level::CRITICAL:
            return "CRITICAL";
        case logger::Level::FATAL:
            return "FATAL";
        default:
            return "UNKNOWN";
    }
}

std::string
Logger::getColoredLevel
(
    const logger::Level level
)
{
    std::string color;

    switch (level) {
        case logger::Level::DEBUG:
            color = COLOR_BLUE;
            break;
        case logger::Level::WARNING:
            color = COLOR_YELLOW;
            break;
        case logger::Level::ERROR:
            color = COLOR_BRIGHT_RED;
            break;
        case logger::Level::CRITICAL:
            color = COLOR_RED;
            break;
        case logger::Level::FATAL:
            color = "\033[41;97m"; // Red BG + white text
            break;
        default:
            color = ""; // Default / INFO
            break;
    }

    return color + "<" + levelToString(level) + ">" + COLOR_RESET;
}

std::string
Logger::getFormattedCurrentTimestamp
(
    const bool forFilename
)
{
    using namespace std::chrono;

    const auto now = system_clock::now();
    const auto timeT = system_clock::to_time_t(now);
    const auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

    std::ostringstream oss;

    if (forFilename) {
        oss << std::put_time(std::localtime(&timeT), "%Y-%m-%d_%H-%M-%S");
    } else {
        oss << std::put_time(std::localtime(&timeT), "%Y-%m-%d %H:%M:%S")
            << '.' << std::setw(3) << std::setfill('0') << ms.count();
    }

    return oss.str();
}

std::string
Logger::getFormattedLocation
(
    const std::source_location &loc,
    const bool colorized
)
{
    std::stringstream ss;

    if (colorized) {
        ss << COLOR_BRIGHT_BLACK; // Grey
    }

    ss << "(" << loc.file_name() << ":" << loc.line() << ")";

    if (colorized) {
        ss << COLOR_RESET;
    }

    return ss.str();
}

std::string
Logger::generateLogFile
()
{
    return LOG_DIR
        "/RAYTRACER-" +
        getFormattedCurrentTimestamp(true) +
        ".log";
}

void
Logger::write
(
    logger::Level level,
    const std::string &message,
    const std::source_location &loc
)
{
    /* If minimum log level hasn't been met, then don't write. */
    if (static_cast<uint8_t>(level) < static_cast<uint8_t>(_minimumLevel)) {
        return;
    }

    const std::string timestamp = getFormattedCurrentTimestamp();
    const std::string levelStr = levelToString(level);
    const std::string coloredLevelStr = getColoredLevel(level);

    std::ostringstream fullMessage;
    fullMessage << "[" << timestamp << "] <"
                << levelStr << "> " << message << " "
                << getFormattedLocation(loc, false) << "\n";

    std::ostringstream consoleMessage;
    consoleMessage << COLOR_BRIGHT_BLACK "[" << timestamp << "] " << COLOR_RESET
                   << coloredLevelStr << " " << message << " "
                   << getFormattedLocation(loc) << "\n";

    std::lock_guard lock(MUTEX);

    std::ostream &consoleOutput = level >= logger::Level::ERROR ? std::cerr : std::cout;
    consoleOutput << consoleMessage.str();

    if (this->_logFile.is_open()) {
        this->_logFile << fullMessage.str();
        this->_logFile.flush();
    }

    if (this->_latestLogFile.is_open()) {
        this->_latestLogFile << fullMessage.str();
        this->_latestLogFile.flush();
    }
}
