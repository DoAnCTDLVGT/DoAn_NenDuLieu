﻿#include"Header.h"
void main()
{
	while (1)
	{
		int choo;
		cout << "--------------------------------------------------" << endl;
		cout << "Press 0 : Stop !" << endl;
		cout << "Press 1 : Xu li file " << endl;
		cout << "Press 2 : Xu li folder " << endl;
		cout << "Your Choose : ";
		cin >> choo;
		cout << endl;
		if (choo == 0)
			break;
		else if (choo == 1)
		{
			while (1)
			{
				int choose;
				cout << "----------------------------------" << endl;
				cout << "0. Quay lai menu truoc" << endl;
				cout << "1.Nen file " << endl;
				cout << "2.Giai nen file" << endl;
				cout << "Your Choose :";
				cin >> choose;
				cout << endl << "----------------------------------" << endl;
				if (choose == 0)
					break;
				else if (choose == 1)
				{
					string file_in;
					string file_out;
					rewind(stdin);
					cout << "Nhap ten file in : ";
					getline(cin, file_in);
					rewind(stdin);
					cout << "Nhap ten file out : ";
					getline(cin, file_out);
					NenFile(file_in, file_out);
					cout << "----------------------------------" << endl;
				}
				else if(choose==2)
				{
					string file_nen;
					string file_out;
					rewind(stdin);
					cout << "Nhap ten file nen : ";
					getline(cin, file_nen);
					rewind(stdin);
					cout << "Nhap ten file out : ";
					getline(cin, file_out);
					GiaiNen(file_out, file_nen);
					cout << "----------------------------------" << endl;
				}
			}
		}
		else if (choo == 2)
		{
			int choose;
			while (1) {

				cout << "---------------------------------------------------------------" << endl;
				cout << endl;
				cout << "Press 0: Quay lai menu truoc !" << endl;
				cout << "Press 1: Nen toan bo file trong folder !" << endl;
				//cout << "Press 2: Xem noi dung file nen !" << endl;
				cout << "Press 2: Giai nen toan bo folder !" << endl;
				//cout << "Press 4: Chon va giai nen file rieng le tong folder do !" << endl;
				cout << endl;
				cout << "-----------------------------------------------------------------" << endl;
				cout << "Your choose : ";
				cin >> choose;
				cout << endl;


				if (choose == 0)
					break;
				else if (choose == 1) 
				{
					string folderin = "";
					string folderout = "";
					rewind(stdin);
					cout << "nhap ten folder in : ";
					getline(cin, folderin);
					cout << endl;
					rewind(stdin);
					cout << "nhap ten folder out: ";
					getline(cin, folderout);
					cout << endl;
					NenFolder(folderin, folderout);
					cout << "----------------------------------" << endl;
				}

				/*else if (choose == 2) 
				{
					string folderin = "";
					string folderout = "";
					rewind(stdin);
					cout << "nhap ten folder in : ";
					getline(cin, folderin);
					cout << endl;
					rewind(stdin);
					cout << "nhap ten folder out: ";
					getline(cin, folderout);
					cout << endl;
					XemKichThuocFile(folderout, folderin);
				}
				*/

				else if (choose == 2) 
				{
					string filenen = "";
					string folderout = "";
					rewind(stdin);
					cout << "nhap ten folder nen : ";
					getline(cin, filenen);
					cout << endl;
					rewind(stdin);
					cout << "nhap ten folder out: ";
					getline(cin, folderout);
					cout << endl;

					GiaiNenFolder(filenen, folderout);
				}
			}
		}

	}
	system("pause");

}