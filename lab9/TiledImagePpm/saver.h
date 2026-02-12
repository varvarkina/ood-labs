#pragma once
#include "image.h"
#include <fstream>

void SaveImageToPPM(Image& image, const std::string& fileName)
{
	std::ofstream file(fileName + ".ppm");

	file << "P3\n";
	file << image.GetSize().width << " " << image.GetSize().height << "\n";
	file << "255\n";

	for (int y = 0; y < image.GetSize().height; ++y)
	{
		for (int x = 0; x < image.GetSize().width; ++x)
		{
			uint32_t color = image.GetPixel({ x, y });
			int r = (color >> 16) & 0xFF;
			int g = (color >> 8) & 0xFF;
			int b = color & 0xFF;

			file << r << " " << g << " " << b << std::endl;
		}
	}

	file.close();
}