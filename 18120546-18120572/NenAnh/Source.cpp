#include "bitmap.h"
#include <fstream>
#include <iostream>

using namespace std;

//ham tao
Bitmap::Bitmap()
{
	bfh.type = 0x4d42;
	bfh.size = 0;
	bfh.reserved1 = 0;
	bfh.reserved2 = 0;
	bfh.offset_bit = 0;
	bih.size = 40;
	bih.width = 0;
	bih.height = 0;
	bih.planes = 1;
	bih.bit_count = 24;
	bih.compression = 0;
	bih.image_size = 0;
	bih.pixel_per_meter_x = 0;
	bih.pixel_per_meter_y = 0;
	bih.color_used = 256;
	bih.color_important = 0;
	data_image = 0;
	color_table = 0;
	bpp = 0;
	length = 0;
	width = 0;
	height = 0;
	row_width = 0;
	channel_mode = bgr_mode;
}

//ham tao
Bitmap::Bitmap(const string& filename)
{
	bfh.type = 0x4d42;
	bfh.size = 0;
	bfh.reserved1 = 0;
	bfh.reserved2 = 0;
	bfh.offset_bit = 0;
	bih.size = 40;
	bih.width = 0;
	bih.height = 0;
	bih.planes = 1;
	bih.bit_count = 24;
	bih.compression = 0;
	bih.image_size = 0;
	bih.pixel_per_meter_x = 0;
	bih.pixel_per_meter_y = 0;
	bih.color_used = 256;
	bih.color_important = 0;
	data_image = 0;
	color_table = 0;
	bpp = 0;
	length = 0;
	width = 0;
	height = 0;
	row_width = 0;
	channel_mode = bgr_mode;
	Open(filename);
}

//hàm tao
Bitmap::Bitmap(const unsigned int width, const unsigned int height)
{
	bfh.type = 0x4d42;
	bfh.size = 0;
	bfh.reserved1 = 0;
	bfh.reserved2 = 0;
	bfh.offset_bit = 0;
	bih.size = 40;
	bih.width = width;
	bih.height = height;
	bih.planes = 1;
	bih.bit_count = 24;
	bih.compression = 0;
	bih.image_size = 0;
	bih.pixel_per_meter_x = 0;
	bih.pixel_per_meter_y = 0;
	bih.color_used = 256;
	bih.color_important = 0;
	data_image = 0;
	color_table = 0;
	bpp = 0;
	length = 0;
	this->width = width;
	this->height = height;
	row_width = 0;
	channel_mode = bgr_mode;
	CreateImage();
}

//ham huy
Bitmap::~Bitmap()
{
	delete[] data_image;
}

//tạo ảnh
void Bitmap::CreateImage()
{
	length = width * height * (bpp >> 3);
	row_width = width * (bpp >> 3);
	if (data_image != 0) delete[] data_image;
	data_image = new unsigned char[length];
}

//doc phan dau file bitmap(bitmap file header)
void Bitmap::read_bfh(ifstream& f)
{
	f.read((char*)&bfh, bfh.bfh_size());
}

//doc phan 2 file bitmap(bitmap information header)
void Bitmap::read_bih(ifstream& f)
{
	f.read((char*)&bih, bih.bih_size());
}

//mo anh
bool Bitmap::Open(const string& filename)
{
	ifstream file;
	file.open(filename.c_str(), ios::binary | ios::in);
	if (file.bad())
	{
		cout << "\n - Khong tim thay file: " << filename;
		return false;
	}
	if (!file.is_open())
	{
		cout << "\n - Khong mo duoc file: " << filename;
		return false;
	}
	read_bfh(file);
	read_bih(file);
	if (bfh.type != 0x4d42)
	{
		cout << "\n - Khong phai loai anh bitmap image";
		file.close();
		return false;
	}
	else
	{
		cout << "\n => Bitmap file header:";
		cout << "\n  + Type image: bitmap image";
	}
	/*cout<<"\n  + Size: "<<bfh.size<<"byte";
	cout<<"\n  + Reserved1: "<<bfh.reserved1;
	cout<<"\n  + Reserved2: "<<bfh.reserved2;
	cout<<"\n  + Offset_Bit: "<<bfh.offset_bit;
	cout<<"\n\n => Bitmap information header:";
	cout<<"\n  + Size: "<<bih.size<<"byte";
	cout<<"\n  + Width of image: "<<bih.width;
	cout<<"\n  + Height of image: "<<bih.height;
	cout<<"\n  + Planes: "<<bih.planes;
	cout<<"\n  + Bit per pixel: "<<bih.bit_count<<"bpp";
	cout<<"\n  + Compression: "<<bih.compression;
	cout<<"\n  + Image size: "<<bih.image_size / 1024<<"kb";
	cout<<"\n  + Pixel per meter of width: "<<bih.pixel_per_meter_x;
	cout<<"\n  + Pixel per meter of height: "<<bih.pixel_per_meter_y;
	cout<<"\n  + Color used: "<<bih.color_used;
	cout<<"\n  + Color important: "<<bih.color_important;*/

	unsigned int padding, i;
	height = bih.height;
	width = bih.width;
	bpp = bih.bit_count;
	if (bpp == 8)
	{
		color_table = new unsigned char[1024];
		file.read((char*)color_table, 1024);
	}
	padding = (4 - ((3 * width) % 4)) % 4;
	char padding_data[4] = { 0,0,0,0 };
	CreateImage();
	unsigned char* p = 0;
	for (i = 0; i < height; ++i)
	{
		p = data_image + i * row_width;
		file.read((char*)p, sizeof(char) * row_width);
		file.read(padding_data, padding);
	}
	file.close();
	return true;
}


unsigned char* Bitmap::Convert16Bpp(unsigned int w)
{
	unsigned int i, j, k, temp;
	unsigned char* data;
	data = new unsigned char[w];
	if (bpp == 32)
	{
		unsigned short r, g, b;
		unsigned char* p;
		unsigned short* q;
		temp = width * 2;
		for (i = 0; i < height; i++)
		{
			p = data_image + i * row_width;
			q = (unsigned short*)(data + i * temp);
			k = 0;
			for (j = 0; j < row_width; j += 4)
			{
				b = p[j];
				g = p[j + 1];
				r = p[j + 2];
				r = (unsigned short)(r >> 3 << 10);
				g = (unsigned short)(g >> 3 << 5);
				b = (unsigned short)(b >> 3);
				q[k] = b | g | r;
				k++;
			}
		}
	}
	else if (bpp == 24)
	{
		unsigned short r, g, b;
		unsigned char* p;
		unsigned short* q;
		temp = width * 2;
		for (i = 0; i < height; i++)
		{
			p = data_image + i * row_width;
			q = (unsigned short*)(data + i * temp);
			k = 0;
			for (j = 0; j < row_width; j += 3)
			{
				b = p[j];
				g = p[j + 1];
				r = p[j + 2];
				r = (unsigned short)(r >> 3 << 10);
				g = (unsigned short)(g >> 3 << 5);
				b = (unsigned short)(b >> 3);
				q[k] = b | g | r;
				k++;
			}
		}
	}
	else if (bpp == 8)
	{
		unsigned short r, g, b;
		unsigned char* p;
		unsigned short* q, * u;
		temp = width * 2;
		for (i = 0; i < height; i++)
		{
			p = (unsigned char*)(data_image + i * row_width);
			q = (unsigned short*)(data + i * temp);
			u = q;
			for (j = 0; j < row_width; j++)
			{
				b = (unsigned short)p[j];
				g = b;
				r = b;
				b = (unsigned short)(b >> 6 << 3);
				g = (unsigned short)(g << 10 >> 13 << 7);
				r = (unsigned short)(r << 13 >> 1);
				q[j] = b | g | r;
			}
		}
	}
	return data;
}

//chuyen sang 24bpp
unsigned char* Bitmap::Convert24Bpp(unsigned int w)
{
	unsigned int i, j, k, temp;
	unsigned char* data;
	data = new unsigned char[w];
	if (bpp == 32)
	{
		unsigned char* p, * q;
		temp = width * 3;
		for (i = 0; i < height; i++)
		{
			p = data_image + i * row_width;
			q = data + i * temp;
			k = 0;
			for (j = 0; j < row_width; j += 4)
			{
				q[k] = p[j];
				q[k + 1] = p[j + 1];
				q[k + 2] = p[j + 2];
				k += 3;
			}
		}
	}
	else if (bpp == 16)
	{
		unsigned short b, g, r;
		unsigned char* q;
		unsigned short* p;
		temp = width * 3;
		int u;
		for (i = 0; i < height; i++)
		{
			p = (unsigned short*)(data_image + i * row_width);
			q = (unsigned char*)(data + i * temp);
			k = 0;
			u = 0;
			for (j = 0; j < row_width; j += 2)
			{
				b = p[u];
				g = b;
				r = b;
				q[k + 2] = (unsigned char)(b << 1 >> 11 << 3);
				q[k + 1] = (unsigned char)(g << 6 >> 11 << 3);
				q[k] = (unsigned char)(r << 11 >> 11 << 3);
				k += 3;
				u++;
			}
		}
	}
	else if (bpp == 8)
	{
		unsigned char r, g, b;
		unsigned char* p;
		unsigned char* q;
		temp = width * 3;
		for (i = 0; i < height; i++)
		{
			p = (unsigned char*)(data_image + i * row_width);
			q = (unsigned char*)(data + i * temp);
			k = 0;
			for (j = 0; j < row_width; j++)
			{
				b = p[j];
				g = b;
				r = b;
				b = (unsigned char)(b >> 6 << 6);
				g = (unsigned char)(g << 2 >> 5 << 5);
				r = (unsigned char)(r << 5 >> 5 << 5);
				q[k] = b;
				q[k + 1] = g;
				q[k + 2] = r;
				k += 3;
			}
		}
	}
	return data;
}

//chuyen sang 32bpp
unsigned char* Bitmap::Convert32Bpp(unsigned int w)
{
	unsigned int i, j, k, temp;
	unsigned char* data;
	data = new unsigned char[w];
	if (bpp == 24)
	{
		unsigned char* p, * q;
		temp = width * 4;
		for (i = 0; i < height; i++)
		{
			p = data_image + i * row_width;
			q = data + i * temp;
			k = 0;
			for (j = 0; j < row_width; j += 3)
			{
				q[k] = p[j];
				q[k + 1] = p[j + 1];
				q[k + 2] = p[j + 2];
				q[k + 3] = 255;
				k += 4;
			}
		}
	}
	else if (bpp == 16)
	{
		unsigned short b, g, r;
		unsigned char* q;
		unsigned short* p;
		temp = width * 4;
		int u;
		for (i = 0; i < height; i++)
		{
			p = (unsigned short*)(data_image + i * row_width);
			q = (unsigned char*)(data + i * temp);
			k = 0;
			u = 0;
			for (j = 0; j < row_width; j += 2)
			{
				b = p[u];
				g = b;
				r = b;
				q[k + 2] = (unsigned char)(b << 1 >> 11 << 3);
				q[k + 1] = (unsigned char)(g << 6 >> 11 << 3);
				q[k] = (unsigned char)(r << 11 >> 11 << 3);
				k += 4;
				u++;
			}
		}
	}
	else if (bpp == 8)
	{
		unsigned char r, g, b;
		unsigned char* p;
		unsigned char* q;
		temp = width * 4;
		for (i = 0; i < height; i++)
		{
			p = (unsigned char*)(data_image + i * row_width);
			q = (unsigned char*)(data + i * temp);
			k = 0;
			for (j = 0; j < row_width; j++)
			{
				b = p[j];
				g = b;
				r = b;
				b = (unsigned char)(b >> 6 << 6);
				g = (unsigned char)(g << 2 >> 5 << 5);
				r = (unsigned char)(r << 5 >> 5 << 5);
				q[k] = b;
				q[k + 1] = g;
				q[k + 2] = r;
				q[k + 3] = 255;
				k += 4;
			}
		}
	}
	return data;
}

//luu anh
bool Bitmap::Save(const string& filename, unsigned int formatimage)
{
	ofstream file;
	file.open(filename.c_str(), ios::binary | ios::out);
	unsigned char* data;
	bitmap_file_header bfh;
	bitmap_information_header bih;

	if (formatimage == 5 && bpp != 16)
	{
		BitmapHeader16Bpp(bfh, bih);
		file.write((char*)&bfh, bfh.bfh_size());
		file.write((char*)&bih, bih.bih_size());
		unsigned int w = width * height * 2;
		data = Convert16Bpp(w);
		file.write((char*)data, w);
		delete[] data;
	}
	else if (formatimage == 6 && bpp != 24)
	{
		BitmapHeader24Bpp(bfh, bih);
		file.write((char*)&bfh, bfh.bfh_size());
		file.write((char*)&bih, bih.bih_size());
		unsigned int w = width * height * 3;
		data = Convert24Bpp(w);
		file.write((char*)data, w);
		delete[] data;
	}
	else if (formatimage == 7 && bpp != 32)
	{
		BitmapHeader32Bpp(bfh, bih);
		file.write((char*)&bfh, bfh.bfh_size());
		file.write((char*)&bih, bih.bih_size());
		unsigned int w = width * height * 4;
		data = Convert32Bpp(w);
		file.write((char*)data, w);
		delete[] data;
	}
	file.close();
	return true;
}

//sua bitmap header
void Bitmap::BitmapHeader16Bpp(bitmap_file_header& bfh, bitmap_information_header& bih)
{
	bih.width = width;
	bih.height = height;
	bih.bit_count = 16;
	bih.color_important = 0;
	bih.color_used = 65536;
	bih.compression = 0;
	bih.planes = 1;
	bih.size = 40;
	bih.pixel_per_meter_x = 0;
	bih.pixel_per_meter_y = 0;
	bih.image_size = 54 + width * height * 2;

	bfh.type = 0x4d42;
	bfh.size = 54 + bih.image_size;
	bfh.reserved1 = 0;
	bfh.reserved2 = 54;
	bfh.offset_bit = 0;
}

//sua bitmap header
void Bitmap::BitmapHeader24Bpp(bitmap_file_header& bfh, bitmap_information_header& bih)
{
	bih.width = width;
	bih.height = height;
	bih.bit_count = 24;
	bih.color_important = 0;
	bih.color_used = 256;
	bih.compression = 0;
	bih.planes = 1;
	bih.size = 40;
	bih.pixel_per_meter_x = 0;
	bih.pixel_per_meter_y = 0;
	bih.image_size = width * height * 3;

	bfh.type = 0x4d42;
	bfh.size = 54 + bih.image_size;
	bfh.reserved1 = 0;
	bfh.reserved2 = 54;
	bfh.offset_bit = 0;
}

//sua bitmap header
void Bitmap::BitmapHeader32Bpp(bitmap_file_header& bfh, bitmap_information_header& bih)
{
	bih.width = width;
	bih.height = height;
	bih.bit_count = 32;
	bih.color_important = 0;
	bih.color_used = 256;
	bih.compression = 0;
	bih.planes = 1;
	bih.size = 40;
	bih.pixel_per_meter_x = 0;
	bih.pixel_per_meter_y = 0;
	bih.image_size = width * height * 4;

	bfh.type = 0x4d42;
	bfh.size = 54 + bih.image_size;
	bfh.reserved1 = 0;
	bfh.reserved2 = 54;
	bfh.offset_bit = 0;
}

