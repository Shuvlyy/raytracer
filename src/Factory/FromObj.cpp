#include <fstream>
#include <sstream>
#include <vector>

#include "Factory.hpp"
#include "Shape/Shapes/Triangle.hpp"
#include "Math/Point.hpp"
#include "logger/Logger.hpp"

namespace raytracer::factory {

    void
    FromObj
    (
        const std::string &filepath,
        std::shared_ptr<shape::Material> material,
        std::vector<std::unique_ptr<Shape>> &shapes
    )
    {
        std::ifstream objFile(filepath.substr(1, filepath.length() - 2));
        if (!objFile.is_open()) {
            LOG_WARN("Could not open file [" + filepath + "]");
            return;
        }
        std::string line;
        std::vector<math::Point<3>> vertices;
        std::vector<shape::Triangle> tri;

        while (std::getline(objFile, line)) {
            std::istringstream iss(line);
            std::string prefix;
            iss >> prefix;

            if (prefix == "v") {
                double x, y, z;
                iss >> x >> y >> z;
                vertices.emplace_back(x, y, z);
            }
            if (prefix == "f") {
                int i1, i2, i3;
                iss >> i1 >> i2 >> i3;

                if (static_cast<size_t>(i1 - 1) > vertices.size() ||
                    static_cast<size_t>(i2 - 1) > vertices.size() ||
                    static_cast<size_t>(i3 - 1) > vertices.size()) {
                    LOG_WARN("Obj file wrongly formatted, ignoring whole file");
                    return;
                }
                const math::Point<3>& p1 = vertices[i1 - 1];
                const math::Point<3>& p2 = vertices[i2 - 1];
                const math::Point<3>& p3 = vertices[i3 - 1];

                math::Vec<3> axis = math::cross(p2 - p1, p3 - p1).normalized();

                tri.emplace_back(p1, p2, p3, axis, material);
            }
            for (auto &triangle : tri) {
                shapes.emplace_back(std::make_unique<shape::Triangle>(triangle));
            }
        }
    }

}
