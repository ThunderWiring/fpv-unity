//
// Created by bassam on 01/09/2021.
//
#include "opencv_reader.h"

OpenCameraReader::OpenCameraReader() {}

void OpenCameraReader::readImageFrame(uchar *buffer, int len, int pixel_width, int32_t height, int32_t width) {
    if (buffer == nullptr || len <= 0) {
        LOGE("OpenCameraReader::readImageFrame", "null image buffer received");
        return;
    }
    if (width * height * pixel_width != len) {
        LOGE(
                "OpenCameraReader::readImageFrame",
                "image buffer length (%d) does not match image dims (rows:%d. cols: %d)", len,
                height, width);
        return;
    }

    lastFrame4ch = cv::Mat(height, width, CV_8UC4, buffer).clone();
    std::thread panorama_thread([&](){
        // TODO: process image
    });
    panorama_thread.join();

}

uchar* OpenCameraReader::getLastImage() {
    return lastFrame4ch.data;
}


