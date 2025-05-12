// ReSharper disable CppUnusedIncludeDirective
#pragma once

#include <iostream>
#include <csignal>

namespace raytracer
{
    class App;
}

namespace raytracer::signal
{

    class IHandler
    {
    public:
        IHandler() = default;
        virtual ~IHandler() = default;

        virtual void handle(App *app) const = 0;

        [[nodiscard]] virtual int getSignalNumber() const = 0;
        [[nodiscard]] virtual std::string getSignalName() const = 0;
    };

}
