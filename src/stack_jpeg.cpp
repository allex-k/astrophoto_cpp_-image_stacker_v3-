// stack_jpeg.cpp
// stacks 8 bit jbeg images
//  produses arithmetic mean of photos in directory

#include <string>
#include <iostream>
#include <filesystem>
#include <vector>
#include <chrono>
#include <algorithm>

namespace fs = std::filesystem;

#define STB_IMAGE_IMPLEMENTATION
#include "./include/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "./include/stb_image_write.h"

void toLowerCase(std::string& str)
{
     std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) {
        return std::tolower(c);
        });
}
// std::string getFileExtension(const std::string& path)
// {
//     size_t lastDotPos = path.find_last_of(".");
//     std::string ext;
//     if (lastDotPos != std::string::npos)
//     {
//         // Extract the substring after the last dot
//         ext = path.substr(lastDotPos + 1);
//     }

//     std::transform(ext.begin(), ext.end(), ext.begin(), [](unsigned char c) {
//         return std::tolower(c);
//     });

//     return ext;
// }
int main()
{
    const std::string defaultPath = "./photos/jpeg-small";
    std::string path;
    std::cout << "Enter the path to directory (press Enter for default path): \n";
    std::getline(std::cin, path);

    if (path.empty()) {
        path = defaultPath;
    }

    auto startTime = std::chrono::high_resolution_clock::now();

    std::vector<fs::path> pathsToImages;

    for (const auto & entry : fs::directory_iterator(path))
    {
        //entry.path().root_path();
        std::string ext = entry.path().extension().generic_string();
        toLowerCase(ext);
        if (ext == ".jpg" || ext == ".jpeg")
            pathsToImages.push_back(entry.path());
    }

    int width, height, numChannels;
    unsigned char *imageData = stbi_load(pathsToImages[0].generic_string().c_str(), &width, &height, &numChannels, 0);

    float* resultArray = new float[width*height*numChannels](); // Note the () to value-initialize to zero
    uint8_t* resultArrayInt = new uint8_t[width*height*numChannels];

    int numImages = pathsToImages.size();
    // std::cout << "numImages " << numImages << '\n';

    //sum
    for(int picNum = 0; picNum < numImages; ++picNum)
    {
        imageData = stbi_load(pathsToImages[picNum].generic_string().c_str(), &width, &height, &numChannels, 0);

        for(int i = 0; i < width*height*numChannels; ++i)
        {
            resultArray[i] += static_cast<float>(imageData[i]);
        }

        stbi_image_free(imageData);

        std::cout << '\r' << picNum + 1 << '/' << numImages;
    }

    //result
    for(int i = 0; i < width*height*numChannels; ++i)
    {
        resultArray[i] /=  static_cast<float>(numImages);
        resultArrayInt[i] = static_cast<int>(resultArray[i]);
    }

    std::string resultPath = path + "/RESULT.bmp";
    int err = stbi_write_bmp(resultPath.c_str(), width, height, numChannels, resultArrayInt);

    // ==== timer ====
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
    std::cout << "\nExecution time: " << duration << " milliseconds" << std::endl;

    return 0;
}
