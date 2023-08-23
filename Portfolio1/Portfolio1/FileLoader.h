#pragma once
#include <vector>
#include <string>
#include <Windows.h>

#include "stb_image.h"
#include "stb_image_write.h"



class FileLoader
{
	static void LoadImage(
		IN const std::string&		sfileName,
		OUT std::vector<uint8_t>&	vData,
		OUT int&					iWidth,
		OUT int&					iHeight,
        OUT int&                    iChannel
		)
	{
        unsigned char* img = stbi_load(sfileName.c_str(), &iWidth, &iHeight, &iChannel, 0);

        // 4채널로 만들어서 복사
        vData.clear();
        vData.resize(iWidth * iHeight * iChannel);

        if (channels == 1) {
            for (size_t i = 0; i < width * height; i++) {
                uint8_t g = img[i * channels + 0];
                for (size_t c = 0; c < 4; c++) {
                    image[4 * i + c] = g;
                }
            }
        }
        else if (channels == 2) {
            for (size_t i = 0; i < width * height; i++) {
                for (size_t c = 0; c < 2; c++) {
                    image[4 * i + c] = img[i * channels + c];
                }
                image[4 * i + 2] = 255;
                image[4 * i + 3] = 255;
            }
        }
        else if (channels == 3) {
            for (size_t i = 0; i < width * height; i++) {
                for (size_t c = 0; c < 3; c++) {
                    image[4 * i + c] = img[i * channels + c];
                }
                image[4 * i + 3] = 255;
            }
        }
        else if (channels == 4) {
            for (size_t i = 0; i < width * height; i++) {
                for (size_t c = 0; c < 4; c++) {
                    image[4 * i + c] = img[i * channels + c];
                }
            }
        }
        else {
            std::cout << "Cannot read " << channels << " channels" << endl;
        }

	};
};

