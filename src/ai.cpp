#include <filesystem>

#include <nlohmann/json.hpp>
#include <openai.hpp>
#include <opencv2/opencv.hpp>
#include <spdlog/spdlog.h>

#include "ai.hpp"
#include "base64.hpp"

namespace iu {

namespace fs = std::filesystem;

using json = nlohmann::json;

using namespace std::literals;
    const auto AI_PROMPT = "Describe this image as a comma separated list of single world tags that can be used for search. Output no more than ten tags. You can use text you find in the photo as tags."sv;

// resizes and base 64 image
static std::string prepare_image_base64(const std::string& filepath, int target_size = 768) {
    cv::Mat img = cv::imread(filepath);
    if (img.empty()) {
        throw std::runtime_error("Failed to load image: " + filepath);
    }

    double aspect_ratio = static_cast<double>(img.cols) / img.rows;
    int new_width, new_height;

    if (aspect_ratio > 1.0) {
        // Width is larger
        new_width = target_size;
        new_height = static_cast<int>(target_size / aspect_ratio);
    } else {
        // Height is larger or square
        new_height = target_size;
        new_width = static_cast<int>(target_size * aspect_ratio);
    }

    cv::Mat resized;
    cv::resize(img, resized, cv::Size(new_width, new_height), 0, 0, cv::INTER_AREA);

    std::vector<uchar> buf;
    cv::imencode(".jpg", resized, buf);

    return base64_encode(buf.data(), buf.size());
}

void ai_init(const index_opts &opts) {
    auto& openai = openai::start();
    openai.setBaseUrl(opts.ai_base_url);
    openai.setToken(opts.ai_api_key);

}

void ai_describe_image(const index_opts &opts, std::set<std::string> &detected_labels, const fs::path p)
{
    auto& openai = openai::instance();

    auto models = openai::model().list();
    spdlog::debug("model: {}", models.dump());

    json j;
    j["model"] = opts.ai_model;
    j["messages"] = json::array();

    json message;
    message["role"] = "user";
    message["content"] = json::array();

    json question;
    question["type"] = "text";
    question["text"] = AI_PROMPT;

    json image;
    image["type"] = "image_url";

    // Read and encode image
    std::string base64_image = prepare_image_base64(p.string());
    image["image_url"] ="data:image/jpeg;base64," + base64_image;

    message["content"].push_back(question);
    message["content"].push_back(image);

    j["messages"].push_back(message);

    auto completion = openai::chat().create(j);

    spdlog::debug("response: {}", completion.dump(2));

    auto labels_completion = completion["choices"][0]["message"]["content"];
    if (!labels_completion.is_string()) {
        spdlog::debug("can't get completion from AI response {}", labels_completion.dump(2));
        return;
    }

    std::stringstream str(labels_completion.template get<std::string>());
    std::string label;
    while (std::getline(str, label, ',')) {
        detected_labels.insert(label);
    }
}


} // namespace iu
