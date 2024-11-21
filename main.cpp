#include <iostream>
#include <random>
#include "opencv.hpp"
#include "Substitution.hpp"
#include "Plots.hpp"

constexpr const size_t substitution_block_size = 4;
constexpr const size_t permutation_block_size = 64;
constexpr const size_t rounds = 8;

std::vector<ul> s_seeds(rounds);
std::vector<ul> p_seeds(rounds);

std::vector<uchar> transformed;

void feel_seeds() {
    srand(time(nullptr));
    for (int i = 0; i < rounds; i++) {
        s_seeds[i] = rand() * rand();
        p_seeds[i] = rand() * rand();
    }
}

cv::Mat loop(std::vector<uchar>& data, cv::Mat& image) {

    SP_net net(data, substitution_block_size, permutation_block_size, std::move(s_seeds), std::move(p_seeds));

    transformed.reserve(data.size());
    cv::Mat encrypted;

    plt::histogramm(data);
    plt::autocorrelation(data, data.begin(), data.begin() + 1000);
    plt::lattice(data);

    cv::waitKey(0);

    for (int i = 0; i < rounds; i++) {
        std::cout << "round No " << i << std::endl;
        transformed = net.transform(i);
        encrypted = cv::Mat(image.rows, image.cols, image.type(), transformed.data());
        cv::imshow("Encrypted image", encrypted);
        plt::histogramm(transformed);
        plt::autocorrelation(transformed, transformed.begin(), transformed.begin() + 1000);
        plt::lattice(transformed);
        cv::waitKey(0);
        try {
            cv::destroyWindow("Encrypted image");
        }
        catch (...) {
            return encrypted;
        }
    }

    return encrypted;
}

int main(char* argv, int argc) {

    cv::Mat image = cv::imread("photos/planet.jpg", cv::IMREAD_GRAYSCALE);
    if (image.empty()) {
        std::cout << "Could not open or find the image!" << std::endl;
        return -1;
    }
    cv::imshow("Original image", image);

    std::vector<uchar> vec(image.begin<uchar>(), image.end<uchar>());

    feel_seeds();
    plt::init();

    cv::Mat encrypted = loop(vec, image);
    cv::imshow("Encrypted image", encrypted);

    cv::waitKey(0);
    plt::close();
}