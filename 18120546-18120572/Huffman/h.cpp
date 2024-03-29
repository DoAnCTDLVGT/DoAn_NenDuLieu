#include "CDonThuc.h"

// Cai dat ham tao , huy
CDonThuc::CDonThuc()
{
	this->a = 1;
	this->b = 1;
}
CDonThuc::CDonThuc(float a, float b)
{
	this->a = a;
	this->b = b;
}
CDonThuc::CDonThuc(const CDonThuc& DT)
{
	this->a = DT.a;
	this->b = DT.b;
}
CDonThuc::~CDonThuc()
{
}
// Cai dat ham nhap ,xuat
void CDonThuc::Nhap()
{
	cout << "Nhap a : ";
	cin >> this->a;
	cout << endl << "Nhap b :";
	cin >> this->b;
}
void CDonThuc::Xuat(float x)
{
	cout << this->a << "*" << x << "^" << this->b;
}
// Cai dat phuong thuc nhap , xuat
istream& operator>>(istream& input, CDonThuc *Dt)
{
	cout << "Nhap a :";
	input >> Dt->a;
	cout << "Nhap b :";
	input >> Dt->b;
	return input;
}
ostream& operator<<(ostream& output, CDonThuc *Dt)
{
	output << Dt->a << "*x^" << Dt->b;
	return output;
}
// Cai dat ham tinh gia tri
double CDonThuc::value(float x)
{
	return a * pow(x, b);
}
// Cai dat phuong thuc *,/,*= , /=
CDonThuc CDonThuc::operator*(const CDonThuc& Dt)
{
	CDonThuc temp;
	temp.a = this->a * Dt.a;
	temp.b = this->b + Dt.b;
	return temp;
}

CDonThuc CDonThuc::operator/(const CDonThuc& Dt)
{
	CDonThuc temp;
	temp.a = this->a / Dt.a;
	temp.b = this->b - Dt.b;
	return temp;
}
CDonThuc CDonThuc::operator*=(const CDonThuc& Dt)
{
	*this = (*this) * Dt;
	return (*this);
}
CDonThuc CDonThuc::operator/=(const CDonThuc& Dt)
{
	*this = (*this) / Dt;
	return (*this);
}
// Toan Tu Gan = 
CDonThuc CDonThuc::operator=(const CDonThuc& Dt)
{
	this->a = Dt.a;
	this->b = Dt.b;
	return *this;
}
// Toan tu so sanh
bool CDonThuc::operator==(const CDonThuc &Dt)
{
	if (this->a == Dt.a && this->b == Dt.b)
		return true;
	else return false;
}
bool CDonThuc::operator!=(const CDonThuc &Dt)
{
	if (*this == Dt) return false;
	else return true;
}
 