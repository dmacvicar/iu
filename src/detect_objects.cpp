#include <fstream>
#include <vector>
#include <sstream>

#include <spdlog/spdlog.h>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/dnn.hpp>

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

int detect_objects(std::set<std::string> &detected_labels, const fs::path p)
{
    cv::dnn::Net net = cv::dnn::readNet("bvlc_googlenet.caffemodel", "bvlc_googlenet.prototxt");
    if (net.empty()) {
        spdlog::error("There are no layers in the network");
        return -1;
    }
    static std::vector<std::string> labels;
    if (labels.empty()) {
        labels = read_names("classification_classes_ILSVRC2012.txt");
    }

    cv::Mat img = cv::imread(p.string());
    if (img.data == nullptr) {
        spdlog::error("Can't read {}", p.string());
        return -1;
    }
    // Create 4-dimensional blob from the image and set it as the input to the DNN
    cv::Mat blob = cv::dnn::blobFromImage(img, 1, cv::Size(224, 224), cv::Scalar(104, 117, 123));
    if (blob.empty()) {
        spdlog::error("Can't create blob from {}", p.string());
        return -1;
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
    return 0;
}

} // namespace iu
