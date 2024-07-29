#include <opencv2/opencv.hpp>
#include <vector>

class Camera {
private:
    int img_width;
    int img_height;
    cv::Mat overhead_transform;

public:
    Camera(int width, int height) : img_width(width), img_height(height) {
        std::vector<cv::Point2f> lane_shape = {
            cv::Point2f(584, 458), cv::Point2f(701, 458),
            cv::Point2f(295, 665), cv::Point2f(1022, 665)
        };

        cv::Point2f top_left = lane_shape[0];
        cv::Point2f top_right = lane_shape[1];
        cv::Point2f bottom_left = lane_shape[2];
        cv::Point2f bottom_right = lane_shape[3];

        std::vector<cv::Point2f> source = {top_left, top_right, bottom_right, bottom_left};
        std::vector<cv::Point2f> destination = {
            cv::Point2f(bottom_left.x, 0),
            cv::Point2f(bottom_right.x, 0),
            cv::Point2f(bottom_right.x, img_height - 1),
            cv::Point2f(bottom_left.x, img_height - 1)
        };

        overhead_transform = cv::getPerspectiveTransform(source, destination);
    }

    cv::Mat warp_to_overhead(const cv::Mat& undistorted_img) {
        cv::Mat result;
        cv::warpPerspective(undistorted_img, result, overhead_transform, cv::Size(img_width, img_height));
        return result;
    }
};


Camera camera(4032, 3024)
cv::Mat input_image = cv::imread("input.jpg");
cv::Mat overhead_image = camera.warp_to_overhead(input_image, "overhead_view.jpg");
