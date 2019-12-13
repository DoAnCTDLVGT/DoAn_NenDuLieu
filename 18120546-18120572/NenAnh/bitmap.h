#pragma once
#ifndef bitmap_h
#define bitmap_h

#include <string>
#include "color.h"
using namespace std;

//bitmap file header
struct bitmap_file_header
{
	unsigned short type;
	unsigned int size;
	unsigned short reserved1;
	unsigned short reserved2;
	unsigned int offset_bit;

	unsigned int bfh_size()
	{
		unsigned int k = 0;
		k = sizeof(type) + sizeof(size) + sizeof(reserved1) + sizeof(reserved2) + sizeof(offset_bit);
		return k;
	}
};

//bitmap information header
struct bitmap_information_header
{
	unsigned int   size;
	unsigned int   width;
	unsigned int   height;
	unsigned short planes;
	unsigned short bit_count;
	unsigned int   compression;
	unsigned int   image_size;
	unsigned int   pixel_per_meter_x;
	unsigned int   pixel_per_meter_y;
	unsigned int   color_used;
	unsigned int   color_important;

	unsigned int bih_size()
	{
		unsigned int k = 0;
		k = sizeof(size) +
			sizeof(width) +
			sizeof(height) +
			sizeof(planes) +
			sizeof(bit_count) +
			sizeof(compression) +
			sizeof(image_size) +
			sizeof(pixel_per_meter_x) +
			sizeof(pixel_per_meter_y) +
			sizeof(color_used) +
			sizeof(color_important);
		return k;
	}
};

enum ChannelMode
{
	rgb_mode = 0,
	bgr_mode = 1
};

enum ColorPlane
{
	blue_plane = 0,
	green_plane = 1,
	red_plane = 2
};

class Bitmap
{
private:
	bitmap_file_header bfh;
	bitmap_information_header bih;
	unsigned char* data_image, * color_table;
	unsigned int   bpp;
	unsigned int   length;
	unsigned int   width;
	unsigned int   height;
	unsigned int   row_width;
	ChannelMode channel_mode;
	void read_bfh(ifstream& f);
	void read_bih(ifstream& f);
	void BitmapHeader16Bpp(bitmap_file_header& bfh, bitmap_information_header& bih);
	void BitmapHeader24Bpp(bitmap_file_header& bfh, bitmap_information_header& bih);
	void BitmapHeader32Bpp(bitmap_file_header& bfh, bitmap_information_header& bih);
	unsigned char* Convert16Bpp(unsigned int w);
	unsigned char* Convert24Bpp(unsigned int w);
	unsigned char* Convert32Bpp(unsigned int w);

public:
	Bitmap();
	Bitmap(const string& filename);
	Bitmap(unsigned int width, unsigned int height);
	Bitmap(const Bitmap& image);
	~Bitmap();
	void CreateImage();
	bool Open(const string& filename);
	void operator = (const Bitmap image);
	inline bool operator ! ();
	inline bool Clear(const unsigned char v = 0x00);
	inline unsigned char SetRedChannel(const unsigned int x, const unsigned y, const unsigned char value);
	inline unsigned char SetGreenChannel(const unsigned int x, const unsigned y, const unsigned char value);
	inline unsigned char SetBlueChannel(const unsigned int x, const unsigned y, const unsigned char value);
	Color GetPixel(const unsigned int x, const unsigned int y);
	inline void SetPixel(const unsigned int x, const unsigned int y, Color c);
	inline unsigned int Width();
	inline unsigned int Height();
	inline unsigned int BPP();
	inline unsigned PixelsCount();
	inline unsigned char* row(unsigned int row_index);
	bool Save(const string& filename, unsigned int formatimage);

};

#endif