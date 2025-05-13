#pragma once

#include "App/App.hpp"
#include "Network/Client/Channel/Channel.hpp"
#include "Network/Client/Client.hpp"
#include "Parser/Parser.hpp"

namespace raytracer::app
{

    class Client final
        : public App
    {
    public:
        explicit Client(const Attributes& attributes);

        void run() override;
        void stop() override;

    private:
        network::Client _client;
        bool _running{false};
        // ...
    };

}
