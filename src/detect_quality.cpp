#include <filesystem>
#include <cmath>

#include <spdlog/spdlog.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include "detect_quality.hpp"

namespace iu {

double detect_quality(const std::filesystem::path p) {
    cv::Mat img = cv::imread(p.string());
    if (img.data == nullptr) {
        throw std::runtime_error(fmt::format("Can't read {}", p.string()));
    }

    cv::Mat grey;
    cv::cvtColor(img, grey, cv::COLOR_BGR2GRAY);
    cv::Mat laplacian;
    cv::Scalar mean, stddev;
    cv::Laplacian(grey, laplacian, CV_64F);
    cv::meanStdDev(laplacian, mean, stddev);

    return stddev[0] * stddev[0];
}

} // namespace iu
