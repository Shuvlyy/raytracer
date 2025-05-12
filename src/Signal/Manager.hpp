#pragma once

#include "IHandler.hpp"

#include <memory>
#include <unordered_map>

namespace raytracer
{
    class App;
}

namespace raytracer::signal
{

    class Manager
    {
    public:
        explicit Manager(App *app);
        Manager(Manager const &) = delete;
        ~Manager() = default;

    private:
        static Manager *instance;
        App *_app;
        std::unordered_map<int, std::unique_ptr<IHandler>> _handlers;

        void registerHandlers();
        void registerHandler(std::unique_ptr<IHandler> handler);
        void setupListener();
        static void dispatchHandler(int signum);
    };

}
