#include <opencv2/opencv.hpp>
#include <vector>

int th = 127;
void changeThreshold(int position, void* userdata);

int main()
{
    // image.pngをimgに代入
    cv::Mat img = cv::imread("../image.jpg");
    cv::Mat img_resize;
    cv::resize(img, img_resize, cv::Size(), 0.3, 0.3);

    // imgを白黒にしてimg_binに代入
    cv::Mat img_bin;
    cv::cvtColor(img_resize, img_bin, cv::COLOR_BGR2GRAY);

    // canny法
    cv::Mat img_canny;
    cv::Canny(img_resize, img_canny, 300.0, 500.0);

    std::vector<cv::Vec2f> lines;
    cv::HoughLines(img_canny, lines, 0.5, CV_PI / 360, 80);

    std::vector<cv::Vec2f>::iterator it = lines.begin();
    for (; it != lines.end(); ++it) {
        float rho = (*it)[0], theta = (*it)[1];
        cv::Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a * rho, y0 = b * rho;
        pt1.x = cv::saturate_cast<int>(x0 + 1000 * (-b));
        pt1.y = cv::saturate_cast<int>(y0 + 1000 * (a));
        pt2.x = cv::saturate_cast<int>(x0 - 1000 * (-b));
        pt2.y = cv::saturate_cast<int>(y0 - 1000 * (a));
        // 直線描画
        cv::line(img_resize, pt1, pt2, cv::Scalar(0, 0, 255), 2, cv::LINE_AA);
    }
    std::cout << "Detected lines: " << lines.size() << std::endl;

    cv::namedWindow("canny", cv::WINDOW_AUTOSIZE);

    cv::imshow("canny", img_resize);
    cv::waitKey(0);

    return 0;
}

