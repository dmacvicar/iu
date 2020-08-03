#include <fstream>
#include <vector>
#include <sstream>

#include <spdlog/spdlog.h>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/dnn.hpp>

#include "resources.hpp"
#include "detect_objects.hpp"

namespace iu {

namespace fs = std::filesystem;

constexpr int NUM_LABELS = 1000;

std::vector<std::string> read_names(const std::string& filename)
{
    std::ifstream in; in.open(filename);
    std::vector<std::string> names(NUM_LABELS);
    for(int i = 0; i < NUM_LABELS; i++)
        std::getline(in, names[i]);
    return names;
}

void detect_objects(std::set<std::string> &detected_labels, const fs::path p)
{
    static auto model_filepath = find_resource("bvlc_googlenet.caffemodel");
    if (!model_filepath) {
        throw std::runtime_error("Can't find model data, used for image classification");
    }

    static cv::dnn::Net net = cv::dnn::readNet(*model_filepath, "bvlc_googlenet.prototxt");
    if (net.empty()) {
        throw std::runtime_error(fmt::format("Loaded network '{}' has no layers", (*model_filepath).string()));
    }
    static std::vector<std::string> labels;
    if (labels.empty()) {
        labels = read_names("classification_classes_ILSVRC2012.txt");
    }

    cv::Mat img = cv::imread(p.string());
    if (img.data == nullptr) {
        throw std::runtime_error(fmt::format("Can't read {}", p.string()));
    }
    // Create 4-dimensional blob from the image and set it as the input to the DNN
    cv::Mat blob = cv::dnn::blobFromImage(img, 1, cv::Size(224, 224), cv::Scalar(104, 117, 123));
    if (blob.empty()) {
        throw std::runtime_error(fmt::format("Can't create blob from {}", p.string()));
    }

    net.setInput(blob);
    cv::Mat prob = net.forward();
    cv::Mat sorted_idx;
    cv::sortIdx(prob, sorted_idx, cv::SORT_EVERY_ROW + cv::SORT_DESCENDING);
    for (int i = 0; i < 5; ++i) {
        auto probability = prob.at<float>(sorted_idx.at<int>(i));
        auto label_terms = labels[sorted_idx.at<int>(i)];

        if (probability < 0.48) {
            continue;
        }

        // every class has multiple words separated by comma
        std::stringstream str(label_terms);
        std::string term;
        while (std::getline(str, term, ',')) {
            detected_labels.insert(term);
        }
        spdlog::debug("{} - probability: {}", label_terms, probability);
    }
}

} // namespace iu
