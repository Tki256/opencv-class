#include <opencv2/opencv.hpp>

int main()
{
    // image.pngをimgに代入
    cv::Mat img = cv::imread("../image.jpg");

    // imgを白黒にしてimg_binに代入
    cv::Mat img_bin;
    cv::cvtColor(img, img_bin, cv::COLOR_BGR2GRAY);

    // imgの表示
    cv::imshow("img_bin", img_bin);

    // キーが押されるまで待機
    cv::waitKey(0);

    return 0;
}