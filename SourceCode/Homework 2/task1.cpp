#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

static const int imageWidth = 1920;
static const int imageHeight = 1080;
const int numRectanglesX = 4;
const int numRectanglesY = 4;

void generatePPM(const std::string& filename) {
    std::ofstream ppmFileStream(filename);
    if (!ppmFileStream) {
        std::cerr << "Error opening file." << std::endl;
        return;
    }

    ppmFileStream << "P3\n" << imageWidth << " " << imageHeight << "\n255\n";
    std::srand(std::time(nullptr));

    const int rectWidth = imageWidth / numRectanglesX;
    const int rectHeight = imageHeight / numRectanglesY;

    for (int row = 0; row < imageHeight; ++row) {
        for (int col = 0; col < imageWidth; ++col) {
            int rectX = col / rectWidth;
            int rectY = row / rectHeight;

            //so the colors are in a nice gradient
            int red = rectX * 64;
            int green = rectY * 64;
            int blue = 128;

            //for the noise:
            red = std::min(255, std::max(0, red + std::rand() % 150)); 
            green = std::min(255, std::max(0, green + std::rand() % 150));
            blue = std::min(255, std::max(0, blue + std::rand() % 150));

            ppmFileStream << red << " " << green << " " << blue << " ";
        }
        ppmFileStream << "\n";
    }

    ppmFileStream.close();
}

int main() {
    generatePPM("noisy_image.ppm");
    return 0;
}
