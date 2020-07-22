#include <fstream>
#include <vector>

#include <spdlog/spdlog.h>
#include <yolo_v2_class.hpp>

#include "detect_objects.hpp"

namespace iu {

namespace fs = std::filesystem;

constexpr int NUM_LABELS = 80;

std::vector<std::string> read_names(const std::string& filename)
{
    std::ifstream in; in.open(filename);
    std::vector<std::string> names(NUM_LABELS);
    for(int i = 0; i < NUM_LABELS; i++)
        std::getline(in, names[i]);
    return names;
}

std::set<std::string> detect_objects(const fs::path p)
{
    static Detector obj("yolov4.cfg", "yolov4.weights");
    static std::vector<std::string> labels;
    if (labels.empty()) {
        labels = read_names("coco.names");
    }

    std::set<std::string> detected_labels;

    std::vector<bbox_t> boxes = obj.detect(p.string(), 0.3);
    for(int i = 0; i < boxes.size(); i++)
    {
        auto detected_label = labels[boxes[i].obj_id];
        if (!detected_label.empty()) {
            spdlog::debug("Detected {}", detected_label);
            detected_labels.insert(detected_label);
        }
    }
    return detected_labels;
}

} // namespace iu
