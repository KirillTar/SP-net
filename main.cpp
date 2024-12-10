#include <iostream>
#include <random>
#include "opencv.hpp"
#include "Substitution.hpp"
#include "Plots.hpp"
#include "FInd_seed.hpp"

constexpr const size_t substitution_block_size = 8;
constexpr const size_t permutation_block_size = 64;
constexpr const size_t rounds = 4;

std::vector<ul> s_seeds(rounds);
std::vector<ul> p_seeds(rounds);

std::vector<uchar> transformed;

/*
block size = 4:
diff: 3.125 seed: 366297384
diff: 3.125 seed: 272253696
diff: 3.0625 seed: 188622181
diff: 3.0625 seed: 538262900

block size = 8
diff: 4.14062 seed: 719386823
diff: 4.14062 seed: 293962500
diff: 4.09375 seed: 1474089
diff: 4.02734 seed: 70380482
*/

void fill_seeds() {
    srand(time(nullptr));
    for (int i = 0; i < rounds; i++) {
        p_seeds[i] = rand() * rand();
    }

    s_seeds[0] = 366297384;
    s_seeds[1] = 272253696;
    s_seeds[2] = 188622181;
    s_seeds[3] = 538262900;
}

cv::Mat loop(std::vector<uchar>& data, cv::Mat& image) {

    SP_net net(data, substitution_block_size, permutation_block_size, std::move(s_seeds), std::move(p_seeds));

    transformed.reserve(data.size());
    cv::Mat encrypted;

    plt::histogramm(data);
    plt::histogramm_step_10(data);
    plt::autocorrelation(data, data.begin(), data.begin() + 125'000);
    plt::lattice(data);

    cv::waitKey(0);

    for (int i = 0; i < rounds; i++) {
        std::cout << "round No " << i << std::endl;

        transformed = net.transform(i);
        encrypted = cv::Mat(image.rows, image.cols, image.type(), transformed.data());

        cv::imshow("Encrypted image", encrypted);

        plt::histogramm(transformed);
        plt::histogramm_step_10(transformed);
        plt::autocorrelation(transformed, transformed.begin(), transformed.begin() + 125'000);
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

    cv::Mat image = cv::imread("photos/cat.jpg", cv::IMREAD_GRAYSCALE);
    if (image.empty()) {
        std::cout << "Could not open or find the image!" << std::endl;
        return -1;
    }
    //find_seed();
    cv::imshow("Original image", image);

    std::vector<uchar> vec(image.begin<uchar>(), image.end<uchar>());

    fill_seeds();
    plt::init();

    cv::Mat encrypted = loop(vec, image);
    cv::imshow("Encrypted image", encrypted);
    cv::waitKey(0);
    plt::close();
}