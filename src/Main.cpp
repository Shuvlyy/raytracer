#include "Common/Exit.hpp"
#include "Parser/Parser.hpp"
#include "Logger.hpp"
#include "yml/Yml.hpp"
#include "Renderer.hpp"
#include "Multithreading/Multithreading.hpp"

#include <iostream>
#include <filesystem>

#include "App.hpp"

using namespace raytracer;

int
main
(
    const int argc,
    char *argv[]
)
{
    try {
        const Parser parser(argc, argv);

        if (parser.processFlags() == true) {
            return EXIT_OK;
        }

        const std::string sceneFilepath = parser.getSceneFilepath();

        Logger::init(logger::Level::DEBUG);

        const yml::Yml config(sceneFilepath);
        LOG_DEBUG("Loaded YML configuration! (\"" + sceneFilepath + "\")");

        App app(config);
        app.run();
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
