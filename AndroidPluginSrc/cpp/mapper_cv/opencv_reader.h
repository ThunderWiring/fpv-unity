//
// Created by bassam on 01/09/2021.
//

#ifndef OPENNATIVEC_OPENCV_READER_H
#define OPENNATIVEC_OPENCV_READER_H

#include <opencv2/core.hpp>
#include "../macros.h"
#include <thread>

using cv::Mat;
/**
 * Reads the image frames' bytes and converts them to cv::Mat for further
 * processing by openCV.
 * */
class OpenCameraReader {
private:
    Mat lastFrame4ch;
public:
    OpenCameraReader();
    void readImageFrame(
            uchar* buffer, int len, int pixel_width/* how many channels*/,
            int32_t height, int32_t width);
    uchar* getLastImage();
};

#endif //OPENNATIVEC_OPENCV_READER_H
