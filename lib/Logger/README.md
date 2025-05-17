## Logger

Basic leveled Logger written in C++.

## 📩 Installation

1. Download the project

2. Build the project
```shell
make -s
```

3. And use it anywhere! Include the headers located in the `include/` folder
   and link the library to your project.

## Usage

### Initialization

At the beginning of your program, most likely in your `main()` function, you
**MUST** call `Logger::init()`:

```cpp
int main(int argc, char *argv[])
{
    std::string projectName("Love Me Not");
    logger::Level minimumLogLevel = logger::Level::DEBUG;

    try {
        Logger::init(
            projectName,
            argc, argv,
            minimumLogLevel
        );

        // Rest of your code
    }
    catch (const logger::IException&) {
        /**
         * Beware, Logger initialization can throw an error. It is up to you to
         * handle it!
         */
        // ...
    }
}
```

## Log levels

Currently supported log levels are (in ascending order):
- `DEBUG`: For debugging purposes
- `INFO`: Normal logging
- `ERROR`: When errors are detected throughout the execution
- `WARNING`: Small issue, but important enough to log
- `CRITICAL`: When a serious error is detected but the program still can keep
              running
- `FATAL`: **ONLY** when program can't keep running.

You can access them in the enum `logger::Level`.

## ✏️ Logging

To log something, simply call `Logger::log()` function.
```cpp
Logger::log(logger::Level::DEBUG, "Debug log");
```

A bit long, isn't it? To fix this, you can use the built-in macros:
```cpp
LOG_DEBUG("Debug log");
LOG_INFO("Info log");
LOG_ERR("Error log");
LOG_WARN("Warning log");
LOG_CRIT("Critical log");
LOG_FATAL("FATAL log");
```

Here is an example log line:
```plaintext
[2025-05-16 18:46:21.824] <INFO> Server started! (src/Network/Server/Server.cpp:72)
```

File name and line number are automatically managed, no need to worry about
that ;)

## 📰 Log file

Every log file is stored in the `logs/` folder. If the folder doesn't exist, it
will automatically be created.

Log file name format is the project name followed by the current date and time.
\
__Example:__ `Love Me Not-2025-04-02_12-57-44.log`

Latest log file will also be saved in the file `latest.log`.

Every log file has a header which contains:
- Project's name
- Program start timestamp
- Command used by the user
- OS used
- Kernel version
- Minimum log level
- *And a small random music lyric :P*

__Example:__
```plaintext
╔══════════════════━━━━━━━━╍╍╍╍╍
║ LOG FILE - Raytracer
║
║ Datetime: 2025-05-16 18:46:21.445
║ Command: ./raytracer --mode server scenes/test.yml scenes/test5.yml -p 4248 --config config/server.yml
║ OS: Arch Linux
║ Kernel: 6.14.6-arch1-1
║ Minimum log level: INFO
║
║ Love's gonna get you killed
╚══════════════════━━━━━━━━╍╍╍╍╍
```

## Time spent

Around 5 hours.\
My wakatime statistics aren't representative of my real time
spent, as it also counted my time spent on the Jetpack and Raytracer project in
general.

### Made with 💜 by [Lysandre B.](https://www.github.com/shuvlyy)
