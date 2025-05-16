#define EXIT_KO 84

#include "logger/Logger.hpp"
#include "Colors.hpp"
#include "Exception/Exceptions/CouldNotOpenFile.hpp"

#include <iostream>
#include <filesystem>
#include <random>
#include <vector>
#include <fstream>
#include <sys/utsname.h>

namespace fs = std::filesystem;

Logger *Logger::INSTANCE = nullptr;
std::recursive_mutex Logger::MUTEX;

void
Logger::init
(
    const std::string& projectName,
    const int argc,
    const char* argv[],
    const logger::Level minimumLevel
)
{
    std::lock_guard lock(MUTEX);

    if (INSTANCE == nullptr) {
        INSTANCE = new Logger(
            generateLogFile(projectName),
            minimumLevel
        );

        getInstance().writeHeader(projectName, argc, argv);
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

Logger::~Logger()
{
    if (this->_logFile.is_open()) {
        this->_logFile.close();
    }

    if (this->_latestLogFile.is_open()) {
        this->_latestLogFile.close();
    }
}

bool
Logger::isInit()
{
    return INSTANCE != nullptr;
}

Logger &
Logger::getInstance()
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
        ss << COLOR_BRIGHT_BLACK; // Gray
    }

    ss << "(" << loc.file_name() << ":" << loc.line() << ")";

    if (colorized) {
        ss << COLOR_RESET;
    }

    return ss.str();
}

std::string
Logger::generateLogFile
(
    const std::string& projectName
)
{
    return LOG_DIR
        "/" + projectName + "-" +
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

static
std::string
getOsName()
{
    std::ifstream osRelease("/etc/os-release");
    std::string line;

    while (std::getline(osRelease, line)) {
        if (line.substr(0, 5) == "NAME=") {
            std::string res = line.substr(6);
            res.pop_back();
            return res;
        }
    }
    return "Unknown";
}

static
std::string
getKernelName()
{
    utsname uname_data{};

    if (uname(&uname_data) == 0) {
        return uname_data.release;
    }
    return "Unknown";
}

void
Logger::writeHeader
(
    const std::string& projectName,
    const int argc,
    const char* argv[]
)
{
    std::stringstream command;

    for (int k = 0; k < argc; k++) {
        command << argv[k] << (k == argc - 1 ? "" : " ");
    }

    std::stringstream ss;

    ss  << "╔══════════════════━━━━━━━━╍╍╍╍╍"                               << std::endl
        << "║ LOG FILE - " << projectName                                   << std::endl
        << "║"                                                              << std::endl
        << "║ Datetime: " << getFormattedCurrentTimestamp()                 << std::endl
        << "║ Command: " << command.str()                                   << std::endl
        << "║ OS: " << getOsName()                                          << std::endl
        << "║ Kernel: " << getKernelName()                                  << std::endl
        << "║ Minimum log level: " << levelToString(this->_minimumLevel)  << std::endl
        << "║"                                                              << std::endl
        << "║ " << getRandomQuote()                                         << std::endl
        << "╚══════════════════━━━━━━━━╍╍╍╍╍"                               << std::endl
        << std::endl
    ;

    if (this->_logFile.is_open()) {
        this->_logFile << ss.str();
        this->_logFile.flush();
    }

    if (this->_latestLogFile.is_open()) {
        this->_latestLogFile << ss.str();
        this->_latestLogFile.flush();
    }
}

std::string
Logger::getRandomQuote()
{
    const static std::vector<std::string> quotes =
    {
        "Because love can burn like a cigarette",
        "Everyone adores you, at least I do",
        "You know where to find me, and I know where to look",
        "But is there something more than that?",
        "Shade is my place but you live by the sun",
        "I've been hurt so many times, life and love are all things pass",
        "What could you lose? Well for one, her heart belongs to another",
        "Must be another one she loves",
        "'Cause my love is mine, all mine",
        "There are waves in your eyes",
        "Oh my love, can't you see yourself by my side?",
        "I was doing fine without ya, 'til I saw your face, now I can't erase",
        "I don't drive R8s, I don't like those",
        "Wish I could turn you back into a stranger",
        "They be lovin' someone and I'm another story",
        "Will I be known and loved?",
        "Do you like these little sonnets? 'Cause I wrote them just for you",
        "Tell me, how am I to feel?",
        "Erase me from here and set me free",
        "Wherever you are, I'll be there for you",
        "I might be full of flaws, but you will crave my heart",
        "You love me no longer, I know",
        "I know you want to cry all night, all night",
        "You're my everything",
        "I've seen a wonderful sight",
        "We should be dancing, romancing",
        "And she cried over nothing, so there was nothing I could do to stop",
        "How could I not rely on you, peach eyes?",
        "How could my day be bad when I'm with you?",
        "Yes, my heart, it's full of your smile",
        "pink, love.",
        "Can you hear the music?",
        "But I'll pray for you all the time",
        "When I was young, it seemed that life was so wonderful, a miracle",
        "I wonder if you look both ways, when you cross my mind",
        "Can I get a kiss?",
        "Love's gonna get you killed",
        "I searched to find a love within",
        "There's a world within me I cannot explain",
        "I don't care what anyone says, everyone is disconnected these days",
        "Tell me if it's real like that, then please don't wait on me",
        "The friend I'm dreaming of is far away",
        "late nights",
        "I had a late arrival so, we never saw the start of each other's lives",
        "Is this the real life? Is this just fantasy?",
        "I feel like loving you tonight",
        "I wish I knew you wanted me",
        "Saying you'd wait for me to stay",
        "You love me? Not especially. I'm alone",
        "Memories swim and haunt you",
        "Les rêves des amoureux sont comme le bon vin, ils donnent de la joie ou bien du chagrin",
        "Take my breath away",
        "Et je sais vraiment pas quoi faire de toi",
        "I get messages from the stars when you make sweet love to me",
        "This rainy day is temporary",
        "Hey, it's not like you've every tried to stay",
        "So this is love, I know it is",
        "I don't know what I'm doing here",
        "So sick of my phone, I wish that it would die",
        "Spend some time alone, understand that soon you'll run with better men",
        "What if all this counts for nothing, everything I thought I'd be?",
        "Old enough to understand",
        "Show them how funky and strong is your fight",
        "I'm sitting here alone but I wish that you were here",
        "Just let your fire set me free",
        "Honey, don't you love me and you know it makes me sad?",
        "Key",
        "Count what you have now, don't count what you don't have",
        "Do you feel me? Take a look inside my brain",
        "The people always different, but it always feels the same",
        "I Secretly Love U",
        "I get those goosebumps every time",
        "Stuff We Did",
        "When I think of you sometime, and I wanna spend some time with you"
    };

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, static_cast<int>(quotes.size()) - 1);

    return quotes[dis(gen)];
}
