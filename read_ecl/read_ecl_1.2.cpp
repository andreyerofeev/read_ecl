// Eclipse binary reader
// Author: Andrey Erofeev

#include<stdio.h>
#include<fstream>
#include<iostream>
#include<vector>
#include<cstdio>
#include<string>

const size_t maxkeyWordLength = 8;
const size_t dtypeLength = 4;
const size_t numberOfParametres = 24;
const char *filename = "6dp_sint_oil_res1.txt";
//create the folder for results with name "results", here 24 files with data will appear
const char *resultpath = "c:\\Users\\EAS\\Documents\\Visual Studio 2015\\Projects\\read_ecl\\read_ecl\\results";

using namespace std;

//struct Data
//{
//	vector <double> vecDoubleValues;
//	vector <char> vecCharValues;
//	vector <bool> vecBoolValues;
//	vector< vector <double> > vecDoubleVal2D;
//	vector< vector <char> > vecCharVal2D;
//	vector< vector <bool> > vecBoolVal2D;
//	string valName; //keyword - name of values
//	char typeUse; //we use just one field, which should be specified by one letter: b - bool, c - char, d - double;
//	void printMe();
//};

struct PhysicalCharacteristics
{

	double P;
	double S;
}Data;

struct DATAS {
	vector<PhysicalCharacteristics> Datas;
};

void readData(double num, string dtype, ifstream& in, Data *paramater, string keyword);
string readWord(ifstream& in, size_t wordLength);
int readNum(ifstream& in);
float readNumF(ifstream& in);
double readNumD(ifstream& in);
//float powerOfTwo(int power);

int main()
{	
	int parCounter, colCounter, numI;
	string keyWord, dtype;
	Data allParameteres[numberOfParametres];
	ifstream in;
	in.open(filename, ios::binary | ios::in);
	in.ignore(4, EOF); //skip head word
	parCounter = 0; //counter of parametres
	colCounter = 0; //counter of columns of data of the same parameter
	while (!in.eof())
	{
		keyWord = readWord(in, maxkeyWordLength);
		numI = readNum(in);
		dtype = readWord(in, dtypeLength);
		cout << keyWord << "  " << dtype << "  " << numI << endl;
		in.ignore(4, EOF);
		allParameteres[parCounter].valName = keyWord;
		readData(numI, dtype, in, &allParameteres[parCounter], keyWord);
		in.ignore(4, EOF);
		cout << "                  parcounter = " << parCounter << "  colcouter = " << colCounter << endl;
		parCounter++;
		if (parCounter >= numberOfParametres)
		{
			parCounter = 0;
			colCounter++;
		}
	}
	//for (size_t i = 0; i < numberOfParametres; i++)
	//	allParameteres[i].printMe();
	in.close();
	system("pause");
	return(0);
}

void Data::printMe()
{
	//int isRight;
	string outFileName;
	//isRight = mkdir("\\result");
	outFileName = ".\\result\\" + valName + ".txt";
	ofstream out;
	out.open(outFileName, 'w');
	if (typeUse = 'b')
	{
		for (size_t i = 0; i < vecBoolVal2D[0].size(); i++)
			out << vecBoolVal2D[0][i] << "  " << vecBoolVal2D[1][i] << endl;;
	}
	if (typeUse = 'c')
	{
		for (size_t i = 0; i < vecCharVal2D[0].size(); i++)
		{
			vecCharValues[i] = vecCharVal2D[0][i];
			vecCharValues[i + vecCharVal2D[0].size()] = vecCharVal2D[1][i];
		}
		for (size_t i = 0; i < vecCharValues.size(); i++)
		{
			out << vecCharValues[i] << "  ";
			if ((i + 1) % 16 == 0)
				out << endl;
		}
	}
	if (typeUse = 'd')
	{
		for (size_t i = 0; i < vecDoubleVal2D[0].size(); i++)
			out << vecDoubleVal2D[0][i] << "  " << vecDoubleVal2D[1][i] << endl;
	}
	out.close();
}

void readData(double num, string dtype, ifstream& in, Data *paramater, string keyword)
{
	int buflenI, datumI, bufnum, rembufnum, wsize;
	char flag, datumC;
	bool datumB;
	double datumD;
	float datumF;
	while (num > 0)
	{
		buflenI = readNum(in);
		if (dtype == "INTE")
		{
			wsize = 4;
			flag = 'i';
			paramater->typeUse = 'd';
		}
		if (dtype == "REAL")
		{
			wsize = 4;
			flag = 'r';
			paramater->typeUse = 'd';
		}
		if (dtype == "DOUB")
		{
			wsize = 8;
			flag = 'd';
			paramater->typeUse = 'd';
		}
		if (dtype == "LOGI")
		{
			wsize = 4;
			flag = 'b';
			paramater->typeUse = 'b';
		}
		if (dtype == "CHAR")
		{
			wsize = 1;
			flag = 'c';
			paramater->typeUse = 'c';
			num = num * 8;
		}
		bufnum = buflenI / wsize;		
		rembufnum = buflenI;
		//cout << paramater->vecDoubleVal.size() << endl;
		switch (flag)
		{
		case 'i':
			while (rembufnum > 0)
			{
				datumI = readNum(in);
				paramater->vecDoubleValues.push_back(datumI);
				rembufnum -= 4;
			}
		case 'r':
			while (rembufnum > 0)
			{
				datumF = readNumF(in);
				paramater->vecDoubleValues.push_back(datumF);
				rembufnum -= 4;
			}
		case 'd':
			while (rembufnum > 0)
			{
				datumD = readNumD(in);
				paramater->vecDoubleValues.push_back(datumD);
				rembufnum -= 8;
			}
		case 'c':
			while (rembufnum > 0)
			{
				in.read(&datumC, 1);
				paramater->vecBoolValues.push_back(datumC);
				rembufnum -= 1;
			}			
		case 'b':
			while (rembufnum > 0)
			{
				datumI = readNum(in);
				datumB = (bool)datumI;
				paramater->vecBoolValues.push_back(datumB);
				//paramater->vecBoolVal.at(colCounter).push_back((bool)datum);
				rembufnum -= 4;
			}
		}
		in.ignore(4, EOF);
		num -= bufnum;
	}
	paramater->vecBoolVal2D.push_back(paramater->vecBoolValues);
	paramater->vecCharVal2D.push_back(paramater->vecCharValues);
	paramater->vecDoubleVal2D.push_back(paramater->vecDoubleValues);
}

string readWord(ifstream& in, size_t wordLength)
{
	uint8_t word;
	string wordS;
	for (size_t i = 0; i < wordLength; i++)
	{
		in.read((char *)&word, 1);
		if ((char)word != ' ')
		{
			wordS += (char)word;
		}
		//It's need to add replace of + on _
	}
	return wordS;
}

int readNum(ifstream &in)
{
	int32_t num;
	in.read((char *)&num, 4);
	unsigned char b1, b2, b3, b4;
	b1 = num & 255;
	b2 = (num >> 8) & 255;
	b3 = (num >> 16) & 255;
	b4 = (num >> 24) & 255;
	return ((int)b1 << 24) + ((int)b2 << 16) + ((int)b3 << 8) + b4;
}

float readNumF(ifstream &in)
{
	float numF;
	union
	{
		float f;
		unsigned char b[4];
	} dat1, dat2;
	in.read((char *)&numF, 4);
	dat1.f = numF;
	dat2.b[0] = dat1.b[3];
	dat2.b[1] = dat1.b[2];
	dat2.b[2] = dat1.b[1];
	dat2.b[3] = dat1.b[0];
	return dat2.f;
}

double readNumD(ifstream &in)
{
	double numD;
	union
	{
		double d;
		unsigned char b[8];
	}dat1, dat2;
	in.read((char *)&numD, 8);
	dat1.d = numD;
	dat2.b[0] = dat1.b[7];
	dat2.b[1] = dat1.b[6];
	dat2.b[2] = dat1.b[5];
	dat2.b[3] = dat1.b[4];
	dat2.b[4] = dat1.b[3];
	dat2.b[5] = dat1.b[2];
	dat2.b[6] = dat1.b[1];
	dat2.b[7] = dat1.b[0];
	return dat2.d;
}