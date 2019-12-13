#pragma once
//Static Huffman
#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cmath>
#include <Windows.h>
#include <dirent.h>
#include <bitset>
using namespace std;

//struct NODE
struct NODE
{
	unsigned char c; //kí tự
	int freq;//trọng số
	NODE* pLeft;
	NODE* pRight;
	NODE()
	{
		c = '\0';
		freq = -1;
		pLeft = NULL;
		pRight = NULL;
	}
};

struct HFNTree //cây Huffman
{
	NODE* root;
	HFNTree()
	{
		root = NULL;
	}
};

//struct Bit
struct Bit
{
	unsigned char c; //kí tự để mã hóa 
	string bit;	//dãy bit đã được mã hóa
};

//khởi tạo một node
NODE* CreateNode(unsigned char c);
//kiểm tra xem kí tự đó đã xuất hiện trong bảng chưa trả về vị trí xuất hiện, trả về -1 nếu không tìm thấy
int KiemTraXuatHien(vector<NODE*> table, unsigned char c);
//đọc file input để thống kê tần số của mỗi kí tự và lập thành vector
vector<NODE*> TaoBangTanSo(fstream& fInput);
//hoán vị
void Swap(NODE* p, NODE* q);
// sắp xếp tần số của mỗi phần tử trong bảng
void SortTable(vector<NODE*>& table);
void SortTable(vector<NODE*>& table, int left, int right);
//tạo cây Huffman
HFNTree CreateHFNTree(vector<NODE*> table);
//chuyển một kí tự thành dãy bit
string charToBit(unsigned char c);
//chuẩn hóa chuỗi mã hóa
//thêm bit 0 lên đầu để cho đủ bội của 8
string ChuanHoaChuoiBit(string str);

// mã hóa bảng tần số thành một day bit
string MaHoaBangTanSo(vector<NODE*> table);
//tạo bảng mã bit mới cho chữ cái
void TaoMaBit(NODE* root, vector<Bit>& tableBit, string bit);
//chuyển chuỗi 8 bit thành kí tự tương ứng
unsigned char BitToChar(string bit);
//đếm số kí tự trong file cần nén dựa vào bảng tần số
int CountChar(vector<NODE*> table);
// Nén file một file riêng lẻ
void NenFile(string fileNameInput, string fileNameNen);
//chuẩn hóa chuỗi
string wchar_t2string(const wchar_t* wchar);
wchar_t* string2wchar_t(const string& str);
//hàm duyệt folder
vector<string> listFilesInDirectory(string &directoryName);
// Nén folder
void NenFolder(string folderNameInput, string fileNameNen);
//PHẦN GIẢI NÉN

//đọc từng kí tự chuỗi đã nén,tách ra chuỗi lưu bảng tần số và chuỗi lưu mã bit
string LayChuoiMaHoaBangTanSo(fstream& fNen);
//xây dựng lại bảng tần số
vector<NODE*> ReCreateTable(string strBitTable);
unsigned char GiaiMaBit(NODE* root, string& strMaHoa);
//giải nén file riêng lẻ, lưu riêng ra
void GiaiNen(string fileNameOutput, string fileNameNen);
//giải nén file riêng lẻ , lưu vào trong folder
void GiaiNen(string fileNameOutput, string fileNameNen, string folderNameOut);
//giải nén nguyên folder, kết quả lưu vào folder khác
void GiaiNenFolder(string fileNameNen, string folderNameOut, string folderNameIn);
// giải nén riêng lẻ từng file trong một folder, lưu kết quả vào trong folder khác
void GiaiNenFileRiengLe(string fileNameNen, string folderNameOut, string folderNameIn);
void XemKichThuocFile(string file_Nen, string folderNameInput);