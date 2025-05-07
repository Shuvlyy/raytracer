#include "Common/Exit.hpp"
#include "Parser/Parser.hpp"
#include "Logger.hpp"
#include "yml/Yml.hpp"
#include "Renderer.hpp"
#include "Multithreading/Multithreading.hpp"

#include <iostream>
#include <filesystem>

using namespace raytracer;
namespace fs = std::filesystem;

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

        yml::Yml config(sceneFilepath);
        LOG_DEBUG("Loaded YML configuration! (\"" + sceneFilepath + "\")");

        Renderer renderer(config);
        multithreading::render(renderer);

        /* Temporary!! ===------------------- */
        const std::unique_ptr<Image>& image = renderer.getRender();

        std::string outputDirectory = config["outputDirectory"].as();

        if (!fs::exists(outputDirectory)) {
            fs::create_directory(outputDirectory);
        }
        image->save(outputDirectory + "/" + Logger::getFormattedCurrentTimestamp() + ".ppm");
        /* ---------------------------------- */
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
