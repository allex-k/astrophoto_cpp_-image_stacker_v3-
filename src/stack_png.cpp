// [this file can be deleted]

#include <string>
#include <iostream>
#include <filesystem>
#include <vector>
#include <chrono>

namespace fs = std::filesystem;

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

void toLowerCase(std::string& str)
{
     std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) {
        return std::tolower(c);
        });
}

int main()
{
    std::cout<<"Stack png: \n";
    std::string path;
    std::cout<<"Enter the path to directory: \n";
    std::getline( std::cin, path);
    // path = "E:\\other\\temp\\png";

    auto startTime = std::chrono::high_resolution_clock::now();

    std::vector<fs::path> pathsToImages;

    for (const auto & entry : fs::directory_iterator(path)) 
    {
        //entry.path().root_path();
        std::string ext = entry.path().extension().generic_string();
        toLowerCase(ext);
        if (ext == ".png")
            pathsToImages.push_back(entry.path());
    }

    int width, height, numChannels;
    uint8_t *imageData = stbi_load(pathsToImages[0].generic_string().c_str(), &width, &height, &numChannels, 0);
    
    float* resultArray = new float[width*height*numChannels]; // Note the () to value-initialize to zero
    uint8_t* resultArrayInt = new uint8_t[width*height*numChannels];

     //first
    for(int i = 0; i < width*height*numChannels; ++i)
    {
        resultArray[i] = static_cast<float>(imageData[i]);
    }
    stbi_image_free(imageData);

    int numImages = pathsToImages.size();

    std::cout<<"1 / "<< numImages << std::endl;

    //sum
    for(int picNum = 1; picNum < numImages; ++picNum)
    {
        imageData = stbi_load(pathsToImages[picNum].generic_string().c_str(), &width, &height, &numChannels, 0);
        
        for(int i = 0; i < width*height*numChannels; ++i)
        {
            resultArray[i] += static_cast<float>(imageData[i]);
        }
        stbi_image_free(imageData);

        std::cout << picNum + 1 << " / " << numImages << std::endl;
    }

    //result
    for(int i = 0; i < width*height*numChannels; ++i)
    {
        resultArray[i] /=  static_cast<float>(numImages);
        resultArray[i] = std::powf((resultArray[i]/255.f), 1.f / 2.2f) * 255.f; //gamma correction
        resultArrayInt[i] = static_cast<int8_t>(resultArray[i]);
    }

    std::string resultPath = path + "\\result.bmp";
    int err = stbi_write_bmp(resultPath.c_str(), width, height, numChannels, resultArrayInt);
    
    delete [] resultArrayInt;
    // ==== timer ====
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
    std::cout << "Execution time: " << duration << " milliseconds" << std::endl;

    return 0;
}