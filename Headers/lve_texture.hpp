#pragma once

#include <cstdio>
#include <vulkan.h>
#include "Structs.h"
#include <stdexcept>
#include <string>
#include <fstream>
#include "lve_device.hpp"

namespace lve {
	struct bmfh
	{
		short bfType;
		int bfSize;
		short bfReserved1;
		short bfReserved2;
		int bfOffBits;
	};

	struct bmih
	{
		int biSize;
		uint32_t biWidth;
		uint32_t biHeight;
		short biPlanes;
		short biBitCount;
		int biCompression;
		int biSizeImage;
		int biXPelsPerMeter;
		int biYPelsPerMeter;
		int biClrUsed;
		int biClrImportant;
	};

	class LveTexture {
	public:
		LveTexture(LveDevice& device, std::vector<std::string> fPath);
		~LveTexture();

		std::vector<TextureFile> texFile{};
	private:
		LveDevice& lveDevice;
		std::vector<std::string> filePath;
		void initTextureSampler(size_t size);
		void initTextureBuffer();
		void fillfromBmpFile(std::vector<std::string> filePath);
		void createTransferCommandPool();
		std::vector<bmfh> FileHeader;
		std::vector<bmih> InfoHeader;
		VkCommandBuffer	TextureCommandBuffer;
		VkCommandPool TransferPool;
		unsigned char* texture;
        std::vector<VkImage> texDelImage;
        std::ifstream fp;

		int ReadInt(std::ifstream* fp)
		{
			char b3, b2, b1, b0;
            fp->get(b0);
            fp->get(b1);
            fp->get(b2);
            fp->get(b3);
			return (b3 << 24) | (b2 << 16) | (b1 << 8) | b0;
		}

		short ReadShort(std::ifstream* fp)
		{
			char b1, b0;
            fp->get(b0);
            fp->get(b1);
			return (b1 << 8) | b0;
		}
		
	};
}
