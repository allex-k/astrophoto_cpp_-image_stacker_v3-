// stack_16_bit_png
// stacks 16 bit png images (hdr images from dng raw)
//  produses arithmetic mean of photos in directory in *.HDR file

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
    const int desiredChannels = 3;

    auto startTime = std::chrono::high_resolution_clock::now();

    std::string path;
    std::cout<<"Enter the path to directory: \n";
    std::getline(std::cin, path);
    //path = "E:\\other\\temp";

    std::vector<fs::path> pathsToImages;

    for (const auto & entry : fs::directory_iterator(path)) 
    {
        //entry.path().root_path();
        std::string ext = entry.path().extension().generic_string();
        // toLowerCase(ext);
        if (ext == ".png")
            pathsToImages.push_back(entry.path());
    }

    int width, height, numChannels;
    float *imageData = stbi_loadf(pathsToImages[0].generic_string().c_str(), &width, &height, &numChannels, desiredChannels);
    // uint8_t *imageData = stbi_load(pathsToImages[0].generic_string().c_str(), &width, &height, &numChannels, 0);
    
    float* resultArray = new float[width*height*numChannels](); // Note the () to value-initialize to zero
    // uint8_t* resultArrayInt = new uint8_t[width*height*numChannels];

    int numImages = pathsToImages.size();

    //sum
    for(int picNum = 0; picNum < numImages; ++picNum)
    {
        imageData = stbi_loadf(pathsToImages[picNum].generic_string().c_str(), &width, &height, &numChannels, desiredChannels);
        
        for(int i = 0; i < width*height*numChannels; ++i)
        {
            resultArray[i] += imageData[i];
        }
        
        stbi_image_free(imageData);

        std::cout << picNum + 1 << " / " << numImages << std::endl;
    }

    //result
    for(int i = 0; i < width*height*numChannels; ++i)
    {
        resultArray[i] /= static_cast<float>(numImages);
        resultArray[i] = std::powf(resultArray[i], 1/2.2);
        // if(v < 0) v = 0;
        // if(v > 1) v = 1;

       // resultArrayInt[i] = static_cast<uint16_t>(v * 0xFFFF);
        // resultArrayInt[i] = static_cast<uint16_t>(v * 255.f);
         
        // resultArrayInt[i] = static_cast<uint16_t>(resultArray[i]* 65000.f);
    }

    std::string resultPath = path + "\\RESULT.HDR";
    // numChannels = 4;
    // int err = stbi_write_png(resultPath.c_str(), width, height, numChannels, resultArrayInt, width * numChannels * sizeof(uint8_t));
    int err = stbi_write_hdr(resultPath.c_str(), width, height, numChannels, resultArray);
    
    // ==== timer ====
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
    std::cout << "Execution time: " << duration << " milliseconds" << std::endl;

    return 0;
}