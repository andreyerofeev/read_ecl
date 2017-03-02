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
//const char *filename = "6dp_sint_oil_res1.txt";
const char *filename = "SPE1_ECLIPSE_HLAT_TREND_DIAG_BIG.UNRST";
//create the folder for results with name "results", here 24 files with data will appear
//const char *resultpath = "c:\\Users\\EAS\\Documents\\Visual Studio 2015\\Projects\\read_ecl\\read_ecl\\results";

using namespace std;

struct PVTproperties
{
	vector<int> seqnum, intehead, igrp, iwel, icon;
	vector<bool> logihead;
	vector<char> zwel;
	vector<double> doubhead;
	vector<float> bgas, boil, bwat, deng, deno, denw, hoil;
	vector<float> pressure, sgas, soil, ssolid, swat, temp, voil;
};

struct DATAS
{
	vector<PVTproperties> Datas;
	void printMe(int timeStep, int num);
};

void readData(int num, string dtype, ifstream& in, DATAS *parametres, string keyword, int currTimeStep);
string readWord(ifstream& in, size_t wordLength);
int readNum(ifstream& in);
float readNumF(ifstream& in);
double readNumD(ifstream& in);
void fillStructI(PVTproperties *oneProperty, string keyword, int rembufnum, int wsize, ifstream& in);
void fillStructF(PVTproperties *oneProperty, string keyword, int rembufnum, int wsize, ifstream& in);
void fillStructD(PVTproperties *oneProperty, string keyword, int rembufnum, int wsize, ifstream& in);
void fillStructB(PVTproperties *oneProperty, string keyword, int rembufnum, int wsize, ifstream& in);
void fillStructC(PVTproperties *oneProperty, string keyword, int rembufnum, int wsize, ifstream& in);
void fillPropertyI(vector<int> *property, int rembufnum, int wsize, ifstream& in);
void fillPropertyF(vector<float> *property, int rembufnum, int wsize, ifstream& in);

int main()
{
	int currTimeStep, numI;
	string keyWord, dtype;
	DATAS allParametres;
	ifstream in;
	in.open(filename, ios::binary | ios::in);
	in.ignore(4, EOF); //skip head word
	currTimeStep = -1;
	while (!in.eof())
	{
		keyWord = readWord(in, maxkeyWordLength);
		if (keyWord == "SEQNUM") // the initial word
		{
			currTimeStep++;
			allParametres.Datas.resize(currTimeStep + 1);
		}
		numI = readNum(in);
		dtype = readWord(in, dtypeLength);
		cout << keyWord << "  " << dtype << "  " << numI << endl;
		in.ignore(4, EOF);
		readData(numI, dtype, in, &allParametres, keyWord, currTimeStep);
		in.ignore(4, EOF);
	}
	cout << currTimeStep << "   " << numI << endl;
	//allParametres.printMe(currTimeStep, 296);
	in.close();
	system("pause");
	return(0);
}

void readData(int num, string dtype, ifstream& in, DATAS *parametres, string keyword, int currTimeStep)
{
	int buflenI, datumI, bufnum, rembufnum, wsize;
	while (num > 0)
	{
		buflenI = readNum(in);
		rembufnum = buflenI;
		wsize = 4;
		bufnum = buflenI / wsize;
		if (dtype == "INTE")
			fillStructI(&parametres->Datas[currTimeStep], keyword, buflenI, wsize, in);
		if (dtype == "REAL")
			fillStructF(&parametres->Datas[currTimeStep], keyword, buflenI, wsize, in);
		if (dtype == "LOGI")
		{
			fillStructB(&parametres->Datas[currTimeStep], keyword, buflenI, wsize, in);
		}
		if (dtype == "DOUB")
		{
			wsize = 8;
			bufnum = buflenI / wsize;
			fillStructD(&parametres->Datas[currTimeStep], keyword, buflenI, wsize, in);
		}
		if (dtype == "CHAR")
		{
			wsize = 1;
			bufnum = buflenI / wsize;
			num = num * 8;
			fillStructC(&parametres->Datas[currTimeStep], keyword, buflenI, wsize, in);
		}
		in.ignore(4, EOF);
		num -= bufnum;
	}
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
			if ((char)word != '+')
				wordS += (char)word;
			else
				wordS += '_';
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

void fillStructI(PVTproperties * oneProperty, string keyword, int rembufnum, int wsize, ifstream& in)
{
	vector<int> voidprorerty;
	if (keyword == "SEQNUM")
	{
		fillPropertyI(&oneProperty->seqnum, rembufnum, wsize, in);
		return void();
	}
	if (keyword == "INTEHEAD")
	{
		fillPropertyI(&oneProperty->intehead, rembufnum, wsize, in);
		return void();
	}
	if (keyword == "ICON")
	{
		fillPropertyI(&oneProperty->icon, rembufnum, wsize, in);
		return void();
	}
	if (keyword == "IGRP")
	{
		fillPropertyI(&oneProperty->igrp, rembufnum, wsize, in);
		return void();
	}
	if (keyword == "IWEL")
	{
		fillPropertyI(&oneProperty->iwel, rembufnum, wsize, in);
		return void();
	}
	fillPropertyI(&voidprorerty, rembufnum, wsize, in);
}

void fillPropertyI(vector<int> *property, int rembufnum, int wsize, ifstream& in)
{
	int datum;
	while (rembufnum > 0)
	{
		datum = readNum(in);
		property->push_back(datum);
		rembufnum -= wsize;
	}
}

void fillStructF(PVTproperties *oneProperty, string keyword, int rembufnum, int wsize, ifstream& in)
{
	vector<float> voidproperty;
	if (keyword == "BGAS")
	{
		fillPropertyF(&oneProperty->bgas, rembufnum, wsize, in);
		return void();
	}
	if (keyword == "BOIL")
	{
		fillPropertyF(&oneProperty->boil, rembufnum, wsize, in);
		return void();
	}
	if (keyword == "BWAT")
	{
		fillPropertyF(&oneProperty->bwat, rembufnum, wsize, in);
		return void();
	}
	if (keyword == "DENG")
	{
		fillPropertyF(&oneProperty->deng, rembufnum, wsize, in);
		return void();
	}
	if (keyword == "DENO")
	{
		fillPropertyF(&oneProperty->deno, rembufnum, wsize, in);
		return void();
	}
	if (keyword == "DENW")
	{
		fillPropertyF(&oneProperty->denw, rembufnum, wsize, in);
		return void();
	}
	if (keyword == "HOIL")
	{
		fillPropertyF(&oneProperty->hoil, rembufnum, wsize, in);
		return void();
	}
	if (keyword == "PRESSURE")
	{
		fillPropertyF(&oneProperty->pressure, rembufnum, wsize, in);
		return void();
	}
	if (keyword == "SGAS")
	{
		fillPropertyF(&oneProperty->sgas, rembufnum, wsize, in);
		return void();
	}
	if (keyword == "SOIL")
	{
		fillPropertyF(&oneProperty->soil, rembufnum, wsize, in);
		return void();
	}
	if (keyword == "SSOLID")
	{
		fillPropertyF(&oneProperty->ssolid, rembufnum, wsize, in);
		return void();
	}
	if (keyword == "SWAT")
	{
		fillPropertyF(&oneProperty->swat, rembufnum, wsize, in);
		return void();
	}
	if (keyword == "TEMP")
	{
		fillPropertyF(&oneProperty->temp, rembufnum, wsize, in);
		return void();
	}
	if (keyword == "VOIL")
	{
		fillPropertyF(&oneProperty->voil, rembufnum, wsize, in);
		return void();
	}
	fillPropertyF(&voidproperty, rembufnum, wsize, in);
}

void fillStructD(PVTproperties * oneProperty, string keyword, int rembufnum, int wsize, ifstream & in)
{
	double datum;
	while (rembufnum > 0)
	{
		datum = readNumD(in);
		oneProperty->doubhead.push_back(datum);
		rembufnum -= wsize;
	}
}

void fillStructB(PVTproperties * oneProperty, string keyword, int rembufnum, int wsize, ifstream & in)
{
	bool datum;
	int datumI;
	while (rembufnum > 0)
	{
		datumI = readNum(in);
		datum = (bool)datumI;
		oneProperty->logihead.push_back(datum);
		rembufnum -= wsize;
	}
}

void fillStructC(PVTproperties * oneProperty, string keyword, int rembufnum, int wsize, ifstream & in)
{
	char datum;
	while (rembufnum > 0)
	{
		in.read((char *)&datum, 1);
		oneProperty->zwel.push_back(datum);
		rembufnum -= wsize;
	}
}

void fillPropertyF(vector<float> *property, int rembufnum, int wsize, ifstream& in)
{
	float datum;
	while (rembufnum > 0)
	{
		datum = readNumF(in);
		property->push_back(datum);
		rembufnum -= wsize;
	}
}

void DATAS::printMe(int timeStep, int num)
{
	ofstream out;
	out.open("boil.txt", ios::out);
	for (size_t i = 0; i < num; i++)
	{
		for (size_t j = 0; j < timeStep; j++)
		{
			out << Datas[j].intehead[i] << "  ";
			//cout << Datas[j].boil[i] << "  ";
		}
		out << endl;
		//cout << endl;
	}
	out.close();
}