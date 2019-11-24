#include "Header.h"
////PHẦN NÉN 

NODE* CreateNode(unsigned char c)
{
	NODE* p = new NODE;
	p->c = c;
	p->freq = 1;
	p->pLeft = NULL;
	p->pRight = NULL;
	return p;
}
//kiểm tra xem kí tự đó đã xuất hiện trong bảng chưa trả về vị trí xuất hiện, trả về -1 nếu không tìm thấy
int KiemTraXuatHien(vector<NODE*> table, unsigned char c)
{
	int size = table.size();
	for (int i = 0; i < size; i++) {
		if (table[i]->c == c) {
			return i;
		}
	}
	return -1;
}

//đọc file input để thống kê tần số của mỗi kí tự và lập thành vector
vector<NODE*> TaoBangTanSo(fstream& fInput)
{
	vector<NODE*> table;
	NODE* p;
	char c;
	string strData;
	int vt = -1;
	//duyệt chuỗi truyền vào
	while (1)
	{
		fInput.read(&c, sizeof(c));
		if (fInput.eof())
		{
			break;
		}
		vt = KiemTraXuatHien(table, c);
		if (vt == -1)
		{
			p = CreateNode(c);
			table.push_back(p);
		}
		else
		{
			table[vt]->freq++;
		}
	}
	return table;
}

//hoán vị
void Swap(NODE* p, NODE* q)
{
	NODE temp;
	temp = *p;
	*p = *q;
	*q = temp;
}

// sắp xếp tần số của mỗi phần tử trong bảng
void SortTable(vector<NODE*>& table)
{
	int size = table.size();
	int vt;
	for (int i = 0; i < size - 1; i++) {
		vt = i;
		for (int j = i + 1; j < size; j++) {
			if (table[vt]->freq > table[j]->freq) {
				vt = j;
			}
			else if (table[vt]->freq == table[j]->freq) {
				if (table[vt]->c > table[j]->c) {
					vt = j;
				}
			}
		}
		Swap(table[vt], table[i]);
	}
}

//tạo cây Huffman
HFNTree CreateHFNTree(vector<NODE*> table)
{
	HFNTree tree;
	NODE* p;
	SortTable(table);		//sắp xếp bảng tần số

	while (table.size() > 1) {
		p = new NODE;
		p->pLeft = table[0];
		p->pRight = table[1];
		p->c = (table[0]->c < table[1]->c) ? table[0]->c : table[1]->c;
		p->freq = table[0]->freq + table[1]->freq;
		table.erase(table.begin(), table.begin() + 2);
		table.resize(table.size());
		table.push_back(p);
		SortTable(table);
	}
	tree.root = table[0];
	return tree;
}

//chuyển một kí tự thành dãy bit
string charToBit(unsigned char c)
{
	string str = "";
	int iBit = (int)c;
	int bit;
	for (int i = 0; i < 8; i++)
	{
		bit = iBit % 2;
		str = str.insert(0, 1, bit + 48);
		iBit = iBit / 2;
	}
	return str;
}

//chuẩn hóa chuỗi mã hóa
//thêm bit 0 lên đầu để cho đủ bội của 8
string ChuanHoaChuoiBit(string str)
{
	int sl = (8 - str.length() % 8) % 8;
	str.insert(str.length(), sl, '0');
	return str;
}

// mã hóa bảng tần số thành một day bit
string MaHoaBangTanSo(vector<NODE*> table)
{
	int size = table.size();
	string bitc, strBitTable = "";
	int bitso;
	for (int i = 0; i < size - 1; i++)
	{
		bitc = table[i]->c;// kí tự tại node i
		bitso = table[i]->freq;// trọng số tại node i
		strBitTable = strBitTable + bitc + to_string(bitso) + ' ';
	}
	bitc = table[size - 1]->c;
	bitso = table[size - 1]->freq;
	strBitTable = strBitTable + bitc + to_string(bitso);
	size = strBitTable.length();
	strBitTable = to_string(size) + ' ' + strBitTable; // chuỗi: kích thước chuỗi lưu bảng tần số + khoảng trắng + chuỗi mã hóa từ dãy bit
	return strBitTable;
}

//tạo bảng mã bit mới cho chữ cái
void TaoMaBit(NODE* root, vector<Bit>& tableBit, string bit)
{
	Bit bitTemp;
	if (root != NULL) {
		if (root->pLeft == NULL && root->pRight == NULL) {
			bitTemp.c = root->c;
			bitTemp.bit = bit;
			tableBit.push_back(bitTemp);
		}
		else
		{
			TaoMaBit(root->pLeft, tableBit, bit + "0");
			TaoMaBit(root->pRight, tableBit, bit + "1");
		}
	}
}

//chuyển chuỗi 8 bit thành kí tự tương ứng
unsigned char BitToChar(string bit)
{
	unsigned char c;
	int gt = 0;
	for (int i = 0; i < 8; i++)
	{
		if (bit[7 - i] == '1')
		{
			gt = gt + pow((float)2, i);
		}
	}
	c = gt;
	return c;
}

//đếm số kí tự trong file cần nén dựa vào bảng tần số
int CountChar(vector<NODE*> table)
{
	int sum = 0;
	for (int i = 0; i < table.size(); i++) {
		sum = sum + table[i]->freq;
	}
	return sum;
}

// Nén file một file riêng lẻ
void NenFile(string fileNameInput, string fileNameNen)
{
	fstream fInput, fNen;
	vector<NODE*> table;
	vector<Bit> tableBit;
	HFNTree tree;
	string strTemp;
	string strBitTable = "";//chuỗi mã hóa bảng tần số thành chuỗi bit
	string strBitInput = "";//chuỗi mã hóa chuỗi input thành chuoi bit_thay thế các kí tự trong chuỗi input bằng bit mới tạo
	string strMain = "";	//chuỗi mã hóa chuỗi trên thành kí tự
	char c;
	int size, sum, count = 0;
	fInput.open(fileNameInput, ios::in | ios::binary);
	if (!fInput)
	{
		cout << "Khong mo duoc file in\n";
		exit(0);
	}
	fNen.open(fileNameNen, ios::out | ios::binary);
	if (!fNen)
	{
		cout << "Khong mo duoc file out\n";
		exit(0);
	}

	table = TaoBangTanSo(fInput);				//tạo bảng tần số
	fInput.close();
	sum = CountChar(table);                    // tống số kí tự trong chuỗi đầu vào

	strBitTable = MaHoaBangTanSo(table);		//mã hóa bảng tần số

												//ghi chuỗi mã hóa bảng tần số vào file
												//gồm size chuỗi + ' ' + bảng tần số
	for (int i = 0; i < strBitTable.length(); i++)
	{
		c = strBitTable[i];
		fNen.write(&c, sizeof(c));
	}

	//fInput.seekg(fInput.beg);
	tree = CreateHFNTree(table);				//tạo cây Huffman
	TaoMaBit(tree.root, tableBit, "");			//tạo bảng mã bit
	size = tableBit.size();
	//mở file Input lại
	fInput.open(fileNameInput, ios::in | ios::binary);
	if (!fInput)
	{
		cout << "Khong mo duoc file\n";
		exit(0);
	}
	//đọc từng kí tự trong file input biến đổi rồi đưa vào file nén
	while (1)
	{
		fInput.read(&c, sizeof(c));
		if (fInput.eof())
		{
			break;
		}
		count++;
		for (int j = 0; j < size; j++)
		{
			if (c == tableBit[j].c)
			{
				strBitInput = strBitInput + tableBit[j].bit;
				break;
			}
		}
		while (strBitInput.length() >= 8)
		{
			strTemp = strBitInput.substr(0, 8);					//lấy 8 kí tự nhị phân chuyển thành kí tự ascii
			strBitInput.erase(0, 8);							//xóa 8 kí tự đó đi
			c = BitToChar(strTemp);
			fNen.write(&c, sizeof(c));
		}
		if (count == sum)
		{
			char addBit = 8 - strBitInput.length() % 8;
			//cout << (int)addBit << endl;
			if (addBit < 8 && addBit > 0)
			{
				c = BitToChar(ChuanHoaChuoiBit(strBitInput));
				//cout << c << endl;
				fNen.write(&c, sizeof(c));
				fNen.write(&addBit, sizeof(c));
				break;
			}
			addBit = 0;
			fNen.write(&addBit, sizeof(c));					//nếu bằng addBit = 8 thì chuỗi rỗng
		}
	}

	cout << strBitTable << endl;
	cout << strBitInput << endl;
	cout << strMain << endl;
	fNen.close();
	fInput.close();
}


//chuẩn hóa chuỗi
string wchar_t2string(const wchar_t* wchar)
{
	string str = "";
	int index = 0;
	while (wchar[index] != 0) {
		str += (char)wchar[index];
		++index;
	}
	return str;
}

wchar_t* string2wchar_t(const string& str)
{
	wchar_t wchar[260];
	int index = 0;
	while (index < str.size()) {
		wchar[index] = (wchar_t)str[index];
		++index;
	}
	wchar[index] = 0;
	return wchar;
}

//hàm duyệt folder
vector<string> listFilesInDirectory(string directoryName)
{
	WIN32_FIND_DATA FindFileData;
	wchar_t* FileName = string2wchar_t(directoryName);// ten folder

	HANDLE hFind = FindFirstFile(FileName, &FindFileData);

	vector<string> listFileNames;
	listFileNames.push_back(wchar_t2string(FindFileData.cFileName));

	while (FindNextFile(hFind, &FindFileData))
		listFileNames.push_back(wchar_t2string(FindFileData.cFileName));

	return listFileNames;
}

//
void NenFolder(string folderNameInput, string fileNameNen)
{
	// duyet cai tap tin co trong folder
	vector<string> listFile;
	string str = folderNameInput;
	str = str + "\\*txt";

	listFile = listFilesInDirectory(str);


	for (int i = 0; i < listFile.size(); i++) {
		listFile[i] = folderNameInput + "//" + listFile[i];
	}

	// so luong tap tin co trong folder
	int lengthFolder = listFile.size();

	fstream fInput, fNen;
	vector<HFNTree> _tree;
	vector<string> _strBitTable;
	vector<string> _strBitInput;
	vector<string> _strMain;

	_strBitInput.resize(lengthFolder);
	_strBitTable.resize(lengthFolder);
	_strMain.resize(lengthFolder);
	_tree.resize(lengthFolder);

	for (int i = 0; i < lengthFolder; i++) {
		_strBitInput[i] = "";
		_strBitTable[i] = "";
		_strMain[i] = "";
	}

	// mo file nen de tien hanh ghi thong tin cac file 
	fNen.open(fileNameNen, ios::out | ios::binary);
	if (!fNen) {
		cout << "khong mo dc file nen!" << endl;
		exit(0);
	}

	// tien hanh nen tung tap tin
	for (int i = 0; i < lengthFolder; i++) {

		vector<NODE*>table;
		vector<Bit>tableBit;
		string strTemp;
		char c;
		int size, sum, count = 0;

		// mo tap tin ra de xay dung bang tan so
		fInput.open(listFile[i], ios::in | ios::binary);
		if (!fInput) {
			cout << "khong tim thay tap tin " << endl;
			exit(0);
		}

		table = TaoBangTanSo(fInput);
		fInput.close();// sau khi da xay dung bang tan so thi dong tap tin lai
		sum = CountChar(table);


		_strBitTable[i] = MaHoaBangTanSo(table);


		// ghi chuoi tan so cua tap tin thu i vao file
		// gom size cua chuoi + ' ' + bang tan so
		for (int k = 0; k < _strBitTable[i].length(); k++) {

			c = _strBitTable[i][k];
			fNen.write(&c, sizeof(c));
		}

		//tao cay huffman tu bang tan so
		_tree[i] = CreateHFNTree(table);
		// tao ma bit 
		TaoMaBit(_tree[i].root, tableBit, "");
		size = tableBit.size();

		// mo lai tap tin thu i de duyet
		fInput.open(listFile[i], ios::in | ios::in);

		while (1) {

			fInput.read(&c, sizeof(c));
			if (fInput.eof())
				break;
			count++;

			for (int j = 0; j < size; j++) {
				if (c == tableBit[j].c) {
					_strBitInput[i] = _strBitInput[i] + tableBit[j].bit;
					break;
				}
			}

			while (_strBitInput[i].length() >= 8) {
				strTemp = _strBitInput[i].substr(0, 8);
				_strBitInput[i].erase(0, 8);
				c = BitToChar(strTemp);
				fNen.write(&c, sizeof(c));
			}

			if (count == sum) {
				char addBit = 8 - _strBitInput[i].length() % 8;
				if (addBit < 8 && addBit>0) {
					c = BitToChar(ChuanHoaChuoiBit(_strBitInput[i]));
					fNen.write(&c, sizeof(c));
					fNen.write(&addBit, sizeof(c));
					break;
				}
				addBit = 0;
			}//end if
		}//end while

		fInput.close();
		// sau moi lan nen moi tap tin thi ghi chuoi "  "(gom 2 khoang trang) vao trong file nen
		string add = " mai ";
		char tm = ' ';
		for (int i = 0; i < add.length(); i++) {
			tm = add[i];
			fNen.write(&tm, sizeof(tm));
		}

	}//end for

	cout << "du lieu duoc luu trong file nen co ten 18120572.huff " << endl;

	fNen.close();

}

//PHẦN GIẢI NÉN

//đọc từng kí tự chuỗi đã nén,tách ra chuỗi lưu bảng tần số và chuỗi lưu mã bit
string LayChuoiMaHoaBangTanSo(fstream& fNen)
{
	string strTemp;
	string strTable;
	char c;
	int size = 0, temp;
	//đọc kí tự đầu tiên cho đến gặp khoang trắng là độ dài chuỗi
	fNen.read(&c, sizeof(c));
	while (c != ' ') {
		temp = c - 48;
		size = size * 10 + temp;
		fNen.read(&c, sizeof(c));
	}

	for (int i = 1; i <= size; i++) {
		fNen.read(&c, sizeof(c));
		strTable += c;
	}
	return strTable;
}

//xây dựng lại bảng tần số
vector<NODE*> ReCreateTable(string strBitTable)
{
	vector<NODE*> table;
	NODE* p;
	unsigned char c;
	int freq, temp, i = 0;
	//cứ kí tự đầu tiên là kí tự, kí tự tiếp theo là tần số của kí tự trc đó
	while (i < strBitTable.size()) {
		c = strBitTable[i];
		p = CreateNode(c);
		freq = 0;
		i++;
		while (strBitTable[i] != 32 && strBitTable[i] != NULL) {
			temp = strBitTable[i] - 48;
			freq = freq * 10 + temp;
			i++;
		}
		p->freq = freq;
		table.push_back(p);
		i++;
	}
	SortTable(table);
	return table;
}

unsigned char GiaiMaBit(NODE* root, string& strMaHoa)
{
	//nếu là node lá thì trả về kí tự đó luôn
	if (root->pLeft == NULL && root->pRight == NULL) {
		return root->c;
	}
	if (strMaHoa.length() == 0) {
		return NULL;
	}
	if (strMaHoa[0] == '0') {
		strMaHoa.erase(0, 1);
		GiaiMaBit(root->pLeft, strMaHoa);
	}
	else {
		strMaHoa.erase(0, 1);
		GiaiMaBit(root->pRight, strMaHoa);
	}
}

//giải nén file riêng lẻ, lưu riêng ra
void GiaiNen(string fileNameOutput, string fileNameNen)
{
	fstream fOutput, fNen;
	vector<NODE*> table;
	HFNTree tree;
	string strTemp = "", strBitTable = "", strBitInput = "";
	char cstr, c;
	int sum, count = 0, cNum;

	fOutput.open(fileNameOutput, ios::out | ios::binary);
	if (!fOutput) {
		cout << "Khong mo duoc file\n";
		exit(0);
	}

	fNen.open(fileNameNen, ios::in | ios::binary);
	if (!fNen) {
		cout << "Khong mo duoc file\n";
		exit(0);
	}
	strBitTable = LayChuoiMaHoaBangTanSo(fNen);
	//tạo lại bảng
	table = ReCreateTable(strBitTable);
	sum = CountChar(table);
	tree = CreateHFNTree(table);
	//đọc các dòng tiếp theo trong file nén để mã hóa
	while (1) {
		fNen.read(&c, sizeof(c));
		strBitInput = strBitInput + charToBit(c);					//chuyển kí tự đọc dk thành dãy bit
		if (fNen.eof()) {
			break;
		}
		while (strBitInput.length() > 0) {
			if (count == sum) {
				fNen.read(&c, sizeof(c));										//đọc kí tự cuối cùng luôn
				strBitInput = strBitInput + charToBit(c);
				strTemp = strBitInput.substr(strBitInput.length() - 8, 8);		//lấy 8 bít cuối là sô lượng số 0 thêm vào
				strBitInput.erase(strBitInput.length() - 8, 8);					//xóa 8 bit cuối đi
				cNum = BitToChar(strTemp);
				strBitInput.erase(strBitInput.length() - cNum, cNum);			//xóa số số 0 đã thêm vào
				cstr = GiaiMaBit(tree.root, strBitInput);
				fOutput << cstr;
				break;
			}
			strTemp = strBitInput;
			c = GiaiMaBit(tree.root, strBitInput);
			if (c == NULL) {
				strBitInput = strTemp;
				break;
			}
			fOutput.write(&c, sizeof(c));
			count++;
		}
	}
	fNen.close();
	fOutput.close();
}


//giải nén file riêng lẻ , lưu vào trong folder
void GiaiNen(string fileNameOutput, string fileNameNen, string folderNameOut)
{
	fstream fOutput, fNen;
	vector<NODE*> table;
	HFNTree tree;
	string strTemp = "", strBitTable = "", strBitInput = "";
	char cstr, c;
	int sum, count = 0, cNum;

	string name = folderNameOut + "//" + fileNameOutput;

	fOutput.open(name, ios::out | ios::binary);
	if (!fOutput)
	{
		cout << "Khong mo duoc file\n";
		exit(0);
	}

	fNen.open(fileNameNen, ios::in | ios::binary);
	if (!fNen)
	{
		cout << "Khong mo duoc file\n";
		exit(0);
	}
	strBitTable = LayChuoiMaHoaBangTanSo(fNen);
	//tạo lại bảng
	table = ReCreateTable(strBitTable);
	sum = CountChar(table);							//đếm số kí tự trong file input ban đầu
													//tạo lại cây HFN
	tree = CreateHFNTree(table);
	//đọc các dòng tiếp theo trong file nén để mã hóa
	while (1)
	{
		fNen.read(&c, sizeof(c));
		strBitInput = strBitInput + charToBit(c);					//chuyển kí tự đọc dk thành dãy bit
		if (fNen.eof())
		{
			break;
		}
		while (strBitInput.length() > 0)
		{
			if (count == sum)
			{
				fNen.read(&c, sizeof(c));										//đọc kí tự cuối cùng luôn
				strBitInput = strBitInput + charToBit(c);
				strTemp = strBitInput.substr(strBitInput.length() - 8, 8);		//lấy 8 bít cuối là sô lượng số 0 thêm vào
				strBitInput.erase(strBitInput.length() - 8, 8);					//xóa 8 bit cuối đi
				cNum = BitToChar(strTemp);
				strBitInput.erase(strBitInput.length() - cNum, cNum);			//xóa số số 0 đã thêm vào
				cstr = GiaiMaBit(tree.root, strBitInput);
				fOutput << cstr;
				break;
			}
			strTemp = strBitInput;									//lưu tạm thời lại chuỗi xử lí
			c = GiaiMaBit(tree.root, strBitInput);					//chuyển bit thành kí tự
			if (c == NULL)										//không đủ số luong trong mã bít nên không chuyển được thành kí tự
			{
				strBitInput = strTemp;
				break;
			}
			fOutput.write(&c, sizeof(c));
			count++;
		}
	}
	fNen.close();
	fOutput.close();
}

//giải nén nguyên folder, kết quả lưu vào folder khác
void GiaiNenFolder(string fileNameNen, string folderNameOut, string folderNameIn) {

	fstream fi;
	fi.open(fileNameNen, ios::in | ios::binary);
	if (!fi) {
		cout << "khong ton tai file nen !" << endl;
		return;
	}

	// mo file nen ra lay du lieu cua  moi file
	// luu du lieu nen cua moi file vao trong moi chuoi
	vector<string>_str;
	string tmp;

	char c1, c2, c3, c;
	while (1) {
		if (fi.eof())
			break;
		tmp = "";

		while (1) {

			fi.read(&c, sizeof(c));
			if (fi.eof())
				break;
			if (c == ' ') {
				if (fi.eof())
					break;
				fi.read(&c1, sizeof(c1));
				fi.read(&c2, sizeof(c2));
				fi.read(&c3, sizeof(c3));
				if (c1 == 'm' && c2 == 'a' && c3 == 'i')
				{
					fi.read(&c, sizeof(c));
					break;
				}
				else {
					tmp = tmp + c + c1 + c2 + c3;
				}
			}
			else
				tmp = tmp + c;
		}
		if (tmp != "") {
			_str.push_back(tmp);
		}
	}


	fi.close();

	// luu du lieu nen cua moi tap tin ra cac file rieng 
	string str = "dataToDecode";
	vector<string>_dataToDecode;

	for (int i = 0; i < _str.size(); i++) {
		str = str + to_string(i) + ".txt";
		_dataToDecode.push_back(str);
		fi.open(str, ios::out | ios::binary);
		for (int j = 0; j < _str[i].size(); j++) {
			c = _str[i][j];
			fi.write(&c, sizeof(c));
		}

		str = "dataToDecode";
		fi.close();
	}

	// tien hanh giai nen tat ca tap tin: 
	vector<string> listFile;
	string str1 = folderNameIn;
	str1 = str1 + "\\*txt";
	//cout << "folder: "<<str1 << endl;
	//str1 = "folder_in\\*txt";
	listFile = listFilesInDirectory(str1);
	cout << listFile[0] << endl;
	for (int i = 0; i < _str.size(); i++)
		GiaiNen(listFile[i], _dataToDecode[i], folderNameOut);

	cout << "toan bo file da duoc luu trong folder !" << endl;
}

// giải nén riêng lẻ từng file trong một folder, lưu kết quả vào trong folder khác
void GiaiNenFileRiengLe(string fileNameNen, string folderNameOut, string folderNameIn) {

	// mo file_out chua du lieu nen cua toan bo folder
	fstream fi;
	fi.open(fileNameNen, ios::in | ios::binary);
	if (!fi) {
		cout << "khong ton tai file nen !" << endl;
		return;
	}

	//tiến hành lấy dữ liệu của mỗi tập tin từ file_out

	vector<string>_str;
	string tmp;

	char c1, c2, c3, c;
	while (1) {
		if (fi.eof())
			break;
		tmp = "";

		while (1) {

			fi.read(&c, sizeof(c));
			if (fi.eof())
				break;
			if (c == ' ') {
				if (fi.eof())
					break;
				fi.read(&c1, sizeof(c1));
				fi.read(&c2, sizeof(c2));
				fi.read(&c3, sizeof(c3));
				if (c1 == 'm' && c2 == 'a' && c3 == 'i')
				{
					//fi.seekg(1, ios::cur);
					fi.read(&c, sizeof(c));
					//fi.seekg(1, ios::cur);
					break;
				}
				else {
					tmp = tmp + c + c1 + c2 + c3;
				}
			}
			else
				tmp = tmp + c;
		}
		if (tmp != "") {
			_str.push_back(tmp);
		}
	}


	//cout << ": " << _str.size() << endl;
	//for (int i = 0; i < _str.size(); i++)
	//	cout << "chuoi : " << i + 1 << "  : " << _str[i] << endl;

	fi.close();


	//Lưu dữ liệu của mỗi tập tin ra file nén riêng
	string str = "dataToDecode";
	vector<string>_dataToDecode;

	for (int i = 0; i < _str.size(); i++) {
		str = str + to_string(i) + ".txt";
		_dataToDecode.push_back(str);
		fi.open(str, ios::out | ios::binary);
		for (int j = 0; j < _str[i].size(); j++) {
			c = _str[i][j];
			fi.write(&c, sizeof(c));
		}

		str = "dataToDecode";
		fi.close();
	}


	//mở folder in để lấy tên ban đầu của mỗi tập tin 
	vector<string> listFile;
	string str1 = folderNameIn;
	str1 = str1 + "\\*txt";
	listFile = listFilesInDirectory(str1);

	// nhập yêu cầu : các tập tin muốn giải nén
	int len, yc;
	cout << "cac tap tin co trong thu muc la : " << endl;
	for (int i = 0; i < listFile.size(); i++) {
		cout << i + 1 << " . " << listFile[i] << endl;
	}


	cout << "nhap so luong tap tin muon giai nen (so luong phai nho hon " << _str.size() + 1 << " ) : " << endl;
	do {
		cin >> len;
		if (len > _str.size())
			cout << "du lieu khong hop le , yeu cau nhap lai !" << endl;
	} while (len > _str.size());

	// giải nén từng tập tin và lưu vào folder_out

	cout << "nhap thu tu cac file muon giai nen ( tu 1 -> " << listFile.size() << " )" << endl;
	for (int i = 0; i < len; i++) {
		cin >> yc;
		if (yc > 0 && yc <= listFile.size())
			GiaiNen(listFile[yc - 1], _dataToDecode[yc - 1], folderNameOut);
		else cout << "khong hop le !" << endl;
	}

	cout << "file duoc luu trong folder !" << endl;
}


void XemKichThuocFile(string file_Nen, string folderNameInput) {
	fstream fi1;
	//	string str1, str2, str3;
	//	vector<string>list_file_befor;
	//	vector<string>list_file_after;
	fi1.open(file_Nen, ios::in | ios::binary);
	if (!fi1) {
		cout << "Khong tim thay file nen" << endl;
		return;
	}
	vector<string>_str;
	string tmp;

	// đoc dữ liệu từ file_out

	char c1, c2, c3, c;
	while (1) {
		if (fi1.eof())
			break;
		tmp = "";

		while (1) {
			fi1.read(&c, sizeof(c));
			if (fi1.eof())
				break;
			if (c == ' ') {
				if (fi1.eof())
					break;
				fi1.read(&c1, sizeof(c1));
				fi1.read(&c2, sizeof(c2));
				fi1.read(&c3, sizeof(c3));
				if (c1 == 'm' && c2 == 'a' && c3 == 'i')
				{
					fi1.read(&c, sizeof(c));
					break;
				}
				else {
					tmp = tmp + c + c1 + c2 + c3;
				}
			}
			else
				tmp = tmp + c;
		}
		if (tmp != "") {
			_str.push_back(tmp);
		}
	}

	fi1.close();

	// luu du lieu nen cua moi tap tin ra cac file rieng 
	string str = "dataToDecode";
	vector<string>_dataToDecode;

	for (int i = 0; i < _str.size(); i++) {
		str = str + to_string(i) + ".txt";
		_dataToDecode.push_back(str);
		fi1.open(str, ios::out | ios::binary);
		for (int j = 0; j < _str[i].size(); j++) {
			c = _str[i][j];
			fi1.write(&c, sizeof(c));
		}

		str = "dataToDecode";
		fi1.close();
	}

	vector<int>_sizeNen;
	int size;
	fstream fi2;
	for (int i = 0; i < _str.size(); i++) {
		fi2.open(_dataToDecode[i], ios::in | ios::out);
		fi2.seekg(0, ios::end);
		size = fi2.tellg();
		_sizeNen.push_back(size);
		fi2.close();
	}

	vector<string> listFile;
	vector<int>_size;
	string str1 = folderNameInput;
	str1 = str1 + "\\*txt";
	listFile = listFilesInDirectory(str1);
	for (int i = 0; i < _str.size(); i++) {

		fi2.open(folderNameInput + "//" + listFile[i], ios::in | ios::out);
		if (!fi2)
			cout << "tap tin " << listFile[i] << "khong ton tai !" << endl;
		fi2.seekg(0, ios::end);
		size = fi2.tellg();
		_size.push_back(size);
		fi2.close();
	}

	cout << "Kich thuoc file truoc va sau khi nen : " << endl;
	cout << "STT" << "    " << "ten File " << "    " << "size truoc nen" << "  " << "size sau nen" << endl;
	for (int i = 0; i < _size.size(); i++) {

		cout << i + 1 << "       " << listFile[i] << "          " << _size[i] << "             " << _sizeNen[i] << endl;
	}

}


