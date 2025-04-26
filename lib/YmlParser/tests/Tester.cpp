#include "Yml.hpp"

#include <iostream>

int main(int argc, char *argv[])
{
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <testfile>" << std::endl;
        return 84;
    }

    yml::Yml yml(argv[1]);

    yml.dump();

    std::cout << "Name: " << yml["name"].as<std::string>() << std::endl;
    std::cout << "Value that doesn't exist:" << std::endl;
    std::cout << yml["unknown"].as<std::string>() << std::endl;

    return 0;
}
