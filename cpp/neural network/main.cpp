#include <iostream>
#include <string>
#include <opencv4/opencv2/core.hpp>
#include <opencv4/opencv2/imgcodecs.hpp>
#include <opencv4/opencv2/highgui.hpp>

int main(int argc, char *argv[])
{
    cv::Mat img { cv::};
    cv::imread("obelLectio.jpg", cv::IMREAD_COLOR);
    
    

    return 0;
}
