#pragma once

namespace raytracer
{

    enum class Mode
    {
        /// The program will render the image all by itself.
        SELF,
        /// Cluster server. The program will be the master of a network of clients that will render the image.
        SERVER,
        /// Cluster client. The program will render what the server tells it to render and give the output.
        CLIENT
    };

}
