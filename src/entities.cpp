#include <fstream>
#include <vector>
#include <sstream>

#include <spdlog/spdlog.h>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/dnn.hpp>
#include <csv2/reader.hpp>

#include "resources.hpp"
#include "detect_objects.hpp"

namespace iu {

namespace fs = std::filesystem;

// imagenet default
constexpr int NUM_LABELS = 1000;

template <class Container>
void read_names(Container &names, const fs::path& filename)
{
    std::ifstream in(filename.string());
    std::string name;
    auto it = std::inserter(names, names.end());
    while (std::getline(in, name)) {
        it = name;
    }
}

void detect_entities(std::set<std::string> &detected_labels, const fs::path p)
{
    // contains words for every concept
    static auto words_filepath = find_resource("words_full_hierarchy.txt");
    if (!words_filepath) {
        throw std::runtime_error("Can't find synset words list, used for image classification");
    }

    static auto model_filepath = find_resource("bvlc_googlenet.caffemodel");
    if (!model_filepath) {
        throw std::runtime_error("Can't find model data, used for image classification");
    }

    static cv::dnn::Net net = cv::dnn::readNet(*model_filepath, "bvlc_googlenet.prototxt");
    if (net.empty()) {
        throw std::runtime_error(fmt::format("Loaded network '{}' has no layers", (*model_filepath).string()));
    }

    static std::vector<std::string> labels;
    labels.reserve(NUM_LABELS);
    if (labels.empty()) {
        read_names(labels, *words_filepath);
        spdlog::debug("Loaded {} image words", labels.size());
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

        spdlog::debug("{} - probability: {}", label_terms, probability);
        if (probability < 0.40) {
            continue;
        }
        // every class has multiple words separated by comma
        std::stringstream str(label_terms);
        std::string term;
        while (std::getline(str, term, ',')) {
            detected_labels.insert(term);
        }
    }
}

} // namespace iu
