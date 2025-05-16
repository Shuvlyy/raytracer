#include "Common/Exit.hpp"
#include "Parser/Parser.hpp"
#include "logger/Logger.hpp"
#include "yml/Yml.hpp"
#include "Renderer/Renderer.hpp"
#include "Factory/AppFactory.hpp"

#include "Network/Client/Packet/IHandler.hpp"

#include <iostream>
#include <filesystem>

#define PROJECT_NAME    "Raytracer"

using namespace raytracer;

int
main
(
    const int argc,
    char *argv[]
)
{
    try {
        Parser parser;

        if (parser.parse(argc, argv) == true) {
            return EXIT_OK;
        }

        const Attributes& attributes = parser.getAttributes();

        Logger::init(
            PROJECT_NAME,
            argc, const_cast<const char **>(argv),
            attributes.debugMode ? logger::Level::DEBUG : logger::Level::INFO
        );

        const std::unique_ptr<App> app = app::fromAttributes(attributes);
        app->run();
    }
    catch (const std::exception& exception) {
        std::string message("Error while running Raytracer: ");
        message += exception.what();

        if (Logger::isInit()) {
            LOG_FATAL(message);
        }
        else {
            std::cerr << message << std::endl;
        }
        return EXIT_KO;
    }
    return EXIT_OK;
}
