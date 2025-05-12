#include "Common/Exit.hpp"
#include "Parser/Parser.hpp"
#include "logger/Logger.hpp"
#include "yml/Yml.hpp"
#include "Renderer.hpp"
#include "Factory/AppFactory.hpp"

#include <iostream>
#include <filesystem>

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

        const std::string sceneFilepath = attributes.sceneFilepath;

        Logger::init(attributes.debugMode ? logger::Level::DEBUG : logger::Level::INFO);

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
