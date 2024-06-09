#include <iostream>
#include <fstream>
#include <cmath>
#include <string>

static const int maxColorComponent = 255;

struct Vector3 {
    double x, y, z;

    Vector3(double x_ = 0, double y_ = 0, double z_ = 0) : x(x_), y(y_), z(z_) {}

    Vector3 normalize() const 
    {
        double length = std::sqrt(x * x + y * y + z * z);
        return Vector3(x / length, y / length, z / length);
    }
};

//i made width and height as input parameters so i can try the different resolutions
void generatePPM(const std::string& fileName, int width, int height) 
{
    std::ofstream ppmFileStream(fileName, std::ios::out | std::ios::binary);
    ppmFileStream << "P3\n";
    ppmFileStream << width << " " << height << "\n";
    ppmFileStream << 255 << "\n";

    for (int row = 0; row < height; ++row) 
    {
        for (int col = 0; col < width; ++col) 
        {
            double x = (col + 0.5) / width;
            double y = (row + 0.5) / height;

            x = 2.0 * x - 1.0;
            y = 1.0 - 2.0 * y;

            x *= static_cast<double>(width) / height;

            Vector3 rayDir(x, y, -1.0);
            //side note: while i was fidgeting around with ray directions[rayDir(x,y,0)] i found a cool (almost 3D looking) effect which 
            //i also included in the uploaded pictures 

            rayDir = rayDir.normalize();

            //handling negative:
            int r = (rayDir.x + 1.0) * 0.5 * maxColorComponent;
            int g = (rayDir.y + 1.0) * 0.5 * maxColorComponent; 
            int b = (rayDir.z + 1.0) * 0.5 * maxColorComponent;

            
            ppmFileStream << r << " " << g << " " << b << "\t";
        }
        ppmFileStream << "\n";
    }

    ppmFileStream.close();
}

int main() 
{
    generatePPM("raycolors_small.ppm", 800, 600);  // which is 4:3 aspect ratio
    generatePPM("raycolors_big.ppm", 1280, 720); // which is  16:9 aspect ratio
    return 0;
}
