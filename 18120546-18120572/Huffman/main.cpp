#include"Header.h"
int main()
{

	string folderin = "";

	string folderout = "";

	cout << "nhap ten folder in " << endl;
	getline(cin, folderin);

	cout << "nhap ten folder out: " << endl;
	getline(cin, folderout);


	int choo;
	while (1) {

		cout << "---------------------------------------------------------------" << endl;
		cout << endl;
		cout << "press 0 to stop !" << endl;
		cout << "press 1: Nen toan bo file trong folder !" << endl;
		cout << "press 2:  xem noi dung file nen !" << endl;
		cout << "press 3: Giai nen toan bo folder !" << endl;
		cout << "press 4 : chon va giai nen file rieng le tỏng folder do !" << endl;
		cout << endl;
		cout << "-----------------------------------------------------------------" << endl;
		cout << endl;

		cin >> choo;


		if (choo == 0)
			break;
		if (choo == 1) {

			NenFolder(folderin, "18120572.huff");

		}

		if (choo == 2) {
			XemKichThuocFile("18120572.huff", folderin);
		}

		if (choo == 3) {

			GiaiNenFolder("18120572.huff", folderout, folderin);
		}

		if (choo == 4) {
			GiaiNenFileRiengLe("18120572.huff", folderout, folderin);
		}
	}

	return 0;

}