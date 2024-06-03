#include <iostream>
#include <fstream>
#include <cmath>

const int imageWidth = 1920;
const int imageHeight = 1080;
const int maxColorComponent = 255;

void generatePPM(const std::string& filename) {
    std::ofstream ppmFileStream(filename);
    if (!ppmFileStream) {
        std::cerr << "Error opening file." << std::endl;
        return;
    }

    ppmFileStream << "P3\n" << imageWidth << " " << imageHeight << "\n" << maxColorComponent << "\n";

    int centerX = imageWidth / 2;
    int centerY = imageHeight / 2;
    int halfWidth = 300;
    int halfHeight = 400;

    for (int row = 0; row < imageHeight; ++row) {
        for (int col = 0; col < imageWidth; ++col) {

            int dx = abs(col - centerX);
            int dy = abs(row - centerY);

            if ((dx * halfHeight + dy * halfWidth <= halfHeight * halfWidth)) {

                ppmFileStream << "135 206 235 ";
            } else {
                ppmFileStream << "255 182 193 ";
            }
        }
        ppmFileStream << "\n";
    }

    ppmFileStream.close();
}

int main() {
    generatePPM("diamond_image.ppm");
    return 0;
}
