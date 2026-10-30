//
// 调用摄像头拍下图片并进行图传
//

#ifndef CV_TEST_SER_CAMERA_H
#define CV_TEST_SER_CAMERA_H

#include <opencv2/opencv.hpp>
#include <vector>

class Ser_Camera {
private:
    cv::VideoCapture capture_;          // 保持打开的摄像头句柄
    int camera_id_ = -1;
    int jpeg_quality_ = 85;             // JPEG 编码质量(1~100)

public:
    explicit Ser_Camera(int id = -1);   // id >= 0 时优先打开指定摄像头,否则自动扫描
    ~Ser_Camera();

    // 摄像头句柄不可拷贝
    Ser_Camera(const Ser_Camera&) = delete;
    Ser_Camera& operator=(const Ser_Camera&) = delete;

    int  Camera_Init(int max_id = 10);  // 打开摄像头,成功返回 id,失败返回 -1
    void Camera_Release();              // 释放摄像头
    bool IsOpened() const;              // 摄像头是否处于打开状态

    bool Camera_Capture(std::vector<uchar>& out);  // 抓取一帧并编码为 JPEG,失败返回 false
    bool SetResolution(int width, int height);     // 设置采集分辨率
    void SetJpegQuality(int quality);              // 设置 JPEG 编码质量

};

#endif //CV_TEST_SER_CAMERA_H
