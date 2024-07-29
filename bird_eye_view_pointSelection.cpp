#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>

class PointSelector {
private:
    static std::vector<cv::Point> points;
    static cv::Mat img;
    static std::string windowName;

public:
    static void mouseCallback(int event, int x, int y, int flags, void* userdata) {
        if (event == cv::EVENT_LBUTTONDOWN) {
            if (points.size() < 4) {
                points.push_back(cv::Point(x, y));
                cv::circle(img, cv::Point(x, y), 5, cv::Scalar(0, 0, 255), -1);
                cv::imshow(windowName, img);

                if (points.size() == 4) {
                    std::cout << "4 points selected. Press any key to continue." << std::endl;
                } else {
                    std::cout << "Point " << points.size() << " selected. Select " << 4 - points.size() << " more point(s)." << std::endl;
                }
            }
        }
    }

    static std::vector<cv::Point> selectPoints(const cv::Mat& image, const std::string& winName) {
        img = image.clone();
        windowName = winName;
        points.clear();

        cv::namedWindow(windowName);
        cv::setMouseCallback(windowName, mouseCallback);

        std::cout << "Please select 4 points on the image." << std::endl;
        cv::imshow(windowName, img);
        cv::waitKey(0);

        cv::destroyWindow(windowName);
        return points;
    }
};

std::vector<cv::Point> PointSelector::points;
cv::Mat PointSelector::img;
std::string PointSelector::windowName;

class Camera {
private:
    int img_width;
    int img_height;
    cv::Mat overhead_transform;

public:
    Camera(int width, int height) : img_width(width), img_height(height) {}

    void calculateTransform(const std::vector<cv::Point>& lane_shape) {
        // ... (前のコードと同じ)
    }

    cv::Mat warp_to_overhead(const cv::Mat& undistorted_img, const std::string& save_path = "") {
        cv::Mat result;
        cv::warpPerspective(undistorted_img, result, overhead_transform, cv::Size(img_width, img_height));
        
        if (!save_path.empty()) {
            bool saved = cv::imwrite(save_path, result);
            if (saved) {
                std::cout << "Overhead image saved to: " << save_path << std::endl;
            } else {
                std::cerr << "Failed to save overhead image to: " << save_path << std::endl;
            }
        }
        
        return result;
    }
};

int main() {
    std::string imagePath = "/home/tki256/myProject/opencv-class/img/view1.jpg";  // 入力画像のパスを指定してください
    cv::Mat image = cv::imread(imagePath);
    resize(image, image, cv::Size(), 0.2, 0.2);
    if (image.empty()) {
        std::cerr << "Error: Could not read the image." << std::endl;
        return -1;
    }

    std::vector<cv::Point> selectedPoints = PointSelector::selectPoints(image, "Select 4 Points");

    Camera camera(image.cols, image.rows);
    camera.calculateTransform(selectedPoints);

    cv::Mat overhead = camera.warp_to_overhead(image, "overhead_view.jpg");

    cv::imshow("Overhead View", overhead);
    cv::waitKey(0);

    return 0;
}