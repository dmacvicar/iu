#include <filesystem>
#include <cmath>

#include <spdlog/spdlog.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include <opencv2/quality.hpp>

#include "detect_quality.hpp"
#include "resources.hpp"

namespace iu {

double detect_quality(const std::filesystem::path p) {
    cv::Mat img = cv::imread(p.string());
    if (img.data == nullptr) {
        throw std::runtime_error(fmt::format("Can't read {}", p.string()));
    }

    static auto model_filepath = find_resource("brisque_model_live.yml");
    if (!model_filepath) {
        throw std::runtime_error("Can't find BRISQUE model data, used for image quality classification");
    }

    static auto range_filepath = find_resource("brisque_range_live.yml");
    if (!range_filepath) {
        throw std::runtime_error("Can't find BRISQUE model range data, used for image quality classification");
    }

    const auto ptr = cv::quality::QualityBRISQUE::create(model_filepath->string(), range_filepath->string());

    return ptr->compute(img)[0];
}

} // namespace iu
