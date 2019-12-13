#include "bitmap.h"
#include <iostream>
#include <conio.h>

using namespace std;

#define format_16bpp_rgb  5
#define format_24bpp_rgb  6
#define format_32bpp_rgb  7


int main()
{
	Bitmap image;
	string filename, fileout;
	int key;
	while (true)
	{
		cout << "0.Dung lai" << endl;
		cout << "1.Nen anh " << endl;
		cout << "2.Giai nen anh " << endl;
		cout << "Your Choose : ";
		cin >> key;
		if (key == 0)
			break;
		else if (key == 1)
		{
			cout << "Nhap ten file anh bmp: ";
			cin >> filename;
			image.Open(filename);
			cout << "\n\nNhap ten file sau khi nen: ";
			cin >> fileout;
			image.Save(fileout, format_16bpp_rgb);
		}
		else if (key == 2)
		{
			cout << "Nhap ten file can giai nen: ";
			cin >> fileout;
			image.Open(fileout);
			cout << "\nNhap ten sau khi giai nen: ";
			cin >> fileout;
			image.Save(fileout, format_24bpp_rgb);
			cout << "\n";
		}
	}
	system("pause");
	return 0;
}