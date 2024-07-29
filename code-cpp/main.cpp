#include <opencv2/opencv.hpp>

int main()
{
    // image.pngをimgに代入
    cv::Mat img = cv::imread("../image.jpg");

    // canny法でエッジ検出
    cv::Mat edge;
    cv::Canny(img, edge, 100, 200);

    // ハフ変換で直線検出
    std::vector<cv::Vec4i> lines;
    cv::HoughLinesP(edge, lines, 1, CV_PI / 180, 50, 50, 10);
    
    // 直線を描画
    for (int i = 0; i < lines.size(); i++)
    {
        cv::Vec4i l = lines[i];
        cv::line(img, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(0, 0, 255), 3, cv::LINE_AA);
    }

    // imgの保存
    cv::imwrite("../output.jpg", img);

    return 0;
}