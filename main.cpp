#include <opencv2/opencv.hpp>

int th = 127;
void changeThreshold(int position, void* userdata);

int main()
{
    // image.pngをimgに代入
    cv::Mat img = cv::imread("../image.jpg");


    // imgを白黒にしてimg_binに代入
    cv::Mat img_bin;
    cv::cvtColor(img, img_bin, cv::COLOR_BGR2GRAY);

    // トラックバーの設置
    cv::namedWindow("canny", cv::WINDOW_AUTOSIZE);
    cv::createTrackbar("th", "canny", &th, 800, changeThreshold);

    cv::Mat img_canny;
    while (1){
        // canny法でエッジ検出
        cv::Canny(img, img_canny, th, 800.0);
        // imgの表示
        cv::imshow("canny", img_canny);

        if (cv::waitKey(0) == 27) break;        
    }


    return 0;
}

void changeThreshold(int position, void* userdata){
    th = position;
}