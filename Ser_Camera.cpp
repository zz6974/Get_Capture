#include "Ser_Camera.h"
#include <algorithm>
#include <iostream>

Ser_Camera::Ser_Camera(int id) : camera_id_(id) {
}

Ser_Camera::~Ser_Camera() {
    Camera_Release();
}

int Ser_Camera::Camera_Init(int max_id) {
    if (camera_id_ >= 0) {
        capture_.open(camera_id_);
        if (capture_.isOpened()) return camera_id_;
    }
    for (int id = 0; id < max_id; id++) {
        capture_.open(id);
        if (capture_.isOpened()) {
            camera_id_ = id;
            std::cout << "Camera ID :" << camera_id_ << std::endl;
            return camera_id_;
        }
    }
    camera_id_ = -1;
    return -1;
}

void Ser_Camera::Camera_Release() {
    if (capture_.isOpened()) {
        capture_.release();
    }
}

bool Ser_Camera::IsOpened() const {
    return capture_.isOpened();
}

bool Ser_Camera::Camera_Capture(std::vector<uchar>& out) {
    out.clear();
    if (!capture_.isOpened()) return false;

    cv::Mat frame;
    capture_ >> frame;
    if (frame.empty()) return false;

    std::vector<int> params = { cv::IMWRITE_JPEG_QUALITY, jpeg_quality_ };
    return cv::imencode(".jpg", frame, out, params);
}

bool Ser_Camera::SetResolution(int width, int height) {
    if (!capture_.isOpened()) return false;
    bool ok = capture_.set(cv::CAP_PROP_FRAME_WIDTH, width);
    ok = capture_.set(cv::CAP_PROP_FRAME_HEIGHT, height) && ok;
    return ok;
}

void Ser_Camera::SetJpegQuality(int quality) {
    jpeg_quality_ = std::clamp(quality, 1, 100);
}


