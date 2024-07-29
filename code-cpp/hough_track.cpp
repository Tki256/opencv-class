#include <opencv2/opencv.hpp>
#include <iostream>

// グローバル変数
cv::Mat img, edge, result;
int cannyThreshold1 = 100;
int cannyThreshold2 = 200;
int houghThreshold = 50;
int minLineLength = 50;
int maxLineGap = 10;

// トラックバーのコールバック関数
void onTrackbar(int, void*)
{
    // 元の画像のコピーを作成
    result = img.clone();

    // Canny エッジ検出
    cv::Canny(img, edge, cannyThreshold1, cannyThreshold2);

    // ハフ変換で直線検出
    std::vector<cv::Vec4i> lines;
    cv::HoughLinesP(edge, lines, 1, CV_PI / 180, houghThreshold, minLineLength, maxLineGap);

    // 検出された直線を描画
    for (size_t i = 0; i < lines.size(); i++)
    {
        cv::Vec4i l = lines[i];
        cv::line(result, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(0, 0, 255), 3, cv::LINE_AA);
    }

    // 結果を表示
    cv::imshow("Result", result);
}

int main()
{
    // 画像を読み込む
    img = cv::imread("../image.jpg");
    cv::resize(img, img, cv::Size(), 0.1, 0.1);
    if (img.empty())
    {
        std::cout << "Error: Could not read the image." << std::endl;
        return -1;
    }

    // ウィンドウを作成
    cv::namedWindow("Result", cv::WINDOW_NORMAL);

    // トラックバーを作成
    cv::createTrackbar("Canny Threshold1", "Result", &cannyThreshold1, 255, onTrackbar);
    cv::createTrackbar("Canny Threshold2", "Result", &cannyThreshold2, 255, onTrackbar);
    cv::createTrackbar("Hough Threshold", "Result", &houghThreshold, 200, onTrackbar);
    cv::createTrackbar("Min Line Length", "Result", &minLineLength, 200, onTrackbar);
    cv::createTrackbar("Max Line Gap", "Result", &maxLineGap, 100, onTrackbar);

    // 初期状態を表示
    onTrackbar(0, 0);

    // キー入力待ち
    cv::waitKey(0);

    // 結果を保存
    cv::imwrite("../output.jpg", result);

    return 0;
}