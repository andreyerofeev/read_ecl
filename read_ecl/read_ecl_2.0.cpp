//// Eclipse binary reader
//// Author: Andrey Erofeev
//
//#include<stdio.h>
//#include<fstream>
//#include<iostream>
//#include<vector>
//#include<cstdio>
//#include<string>
//
//const size_t maxkeyWordLength = 8;
//const size_t dtypeLength = 4;
//const size_t numberOfParametres = 24;
////const char *filename = "6dp_sint_oil_res1.txt";
//const char *filename = "SPE1_ECLIPSE_HLAT_TREND_DIAG_BIG.UNRST";
////create the folder for results with name "results", here 24 files with data will appear
//const char *resultpath = "c:\\Users\\EAS\\Documents\\Visual Studio 2015\\Projects\\read_ecl\\read_ecl\\results";
//
//using namespace std;
//
//struct PVTproperties
//{
//	vector<int> seqnum, intehead, igrp, iwel, icon;
//	vector<bool> logihead;
//	vector<char> zwel;
//	vector<double> doubhead;
//	vector<float> bgas, boil, bwat, deng, deno, denw, hoil;
//	vector<float> pressure, sgas, soil, ssolid, swat, temp, voil;
//};
//
//struct DATAS 
//{
//	vector<PVTproperties> Datas;
//	void printMe(int timeStep, int num);
//};
//
//void readData(int num, string dtype, ifstream& in, DATAS *parametres, string keyword, int currTimeStep);
//string readWord(ifstream& in, size_t wordLength);
//int readNum(ifstream& in);
//float readNumF(ifstream& in);
//double readNumD(ifstream& in);
//void fillStructI(PVTproperties *oneProperty, string keyword, int datum);
//void fillStructF(PVTproperties *oneProperty, string keyword, float datum);
//void allocateMemoryForData(PVTproperties *oneProperty, string keyword, int size);
//
//int main()
//{
//	int currTimeStep, numI;
//	string keyWord, dtype;
//	DATAS allParametres;
//	ifstream in;
//	in.open(filename, ios::binary | ios::in);
//	in.ignore(4, EOF); //skip head word
//	currTimeStep = -1; 
//	//	allParametres.Datas.resize(1);
//	while (!in.eof())
//	{
//		keyWord = readWord(in, maxkeyWordLength);
//		if (keyWord == "SEQNUM") // the initial word
//		{
//			currTimeStep++;
//			allParametres.Datas.resize(currTimeStep + 1);
//		}
//		numI = readNum(in);
//		dtype = readWord(in, dtypeLength);
//		cout << keyWord << "  " << dtype << "  " << numI << endl;
//		in.ignore(4, EOF);
//		readData(numI, dtype, in, &allParametres, keyWord, currTimeStep);
//		in.ignore(4, EOF);
//	}
//	cout << currTimeStep << "   " << numI << endl;
//	allParametres.printMe(currTimeStep, 36000);
//	in.close();
//	system("pause");
//	return(0);
//}
//
//void readData(int num, string dtype, ifstream& in, DATAS *parametres, string keyword, int currTimeStep)
//{
//	int buflenI, datumI, bufnum, rembufnum, wsize, i;
//	char flag, datumC;
//	bool datumB;
//	double datumD;
//	float datumF;
//	int *dataI = new int[num];
//	double *dataD = new double[num];
//	float *dataF = new float[num];
//	bool *dataB = new bool[num];
//	char *dataC = new char[num];
//	//allocateMemoryForData(&parametres->Datas[currTimeStep], keyword, num);
//	i = 0;
//	while (num > 0)
//	{
//		buflenI = readNum(in);
//		wsize = 4;
//		if (dtype == "INTE")
//			flag = 'i';
//		if (dtype == "REAL")
//			flag = 'r';
//		if (dtype == "LOGI")
//			flag = 'b';
//		if (dtype == "DOUB")
//		{
//			wsize = 8;
//			flag = 'd';
//		}
//		if (dtype == "CHAR")
//		{
//			wsize = 1;
//			flag = 'c';
//			num = num * 8;
//		}
//		bufnum = buflenI / wsize;
//		rembufnum = buflenI;
//		switch (flag)
//		{
//		case 'i':
//			while (rembufnum > 0)
//			{
//				datumI = readNum(in);				
//				fillStructI(&parametres->Datas[currTimeStep], keyword, datumI);
//				//dataI[i] = readNum(in);
//				//i++;
//				rembufnum -= wsize;
//			}
//		case 'r':
//			while (rembufnum > 0)
//			{
//				datumF = readNumF(in);
//				fillStructF(&parametres->Datas[currTimeStep], keyword, datumF);
//				//dataF[i] = readNumF(in);
//				//i++;
//				rembufnum -= wsize;
//			}
//		case 'd':
//			while (rembufnum > 0)
//			{
//				datumD = readNumD(in);
//				parametres->Datas[currTimeStep].doubhead.push_back(datumD);
//				//dataD[i] = readNumD(in);
//				//i++;
//				rembufnum -= wsize;
//			}
//		case 'c':
//			while (rembufnum > 0)
//			{
//				//in.read((char *)&dataC[i], 1);
//				//i++;
//				in.read((char *)&datumC, 1);
//				//cout << datumC << endl;
//				parametres->Datas[currTimeStep].zwel.push_back(datumC);
//				rembufnum -= wsize;
//			}
//		case 'b':
//			while (rembufnum > 0)
//			{
//				datumI = readNum(in);
//				datumB = (bool)datumI;
//				parametres->Datas[currTimeStep].logihead.push_back(datumB);
//				//dataB[i] = (bool)readNum(in);
//				//i++;
//				rembufnum -= 4;
//			}
//		}
//		in.ignore(4, EOF);
//		num -= bufnum;
//	}
//}
//
//string readWord(ifstream& in, size_t wordLength)
//{
//	uint8_t word;
//	string wordS;
//	for (size_t i = 0; i < wordLength; i++)
//	{
//		in.read((char *)&word, 1);
//		if ((char)word != ' ')
//		{
//			if ((char)word != '+')
//				wordS += (char)word;
//			else
//				wordS += '_';
//		}
//		//It's need to add replace of + on _
//	}
//	return wordS;
//}
//
//int readNum(ifstream &in)
//{
//	int32_t num;
//	in.read((char *)&num, 4);
//	unsigned char b1, b2, b3, b4;
//	b1 = num & 255;
//	b2 = (num >> 8) & 255;
//	b3 = (num >> 16) & 255;
//	b4 = (num >> 24) & 255;
//	return ((int)b1 << 24) + ((int)b2 << 16) + ((int)b3 << 8) + b4;
//}
//
//float readNumF(ifstream &in)
//{
//	float numF;
//	union
//	{
//		float f;
//		unsigned char b[4];
//	} dat1, dat2;
//	in.read((char *)&numF, 4);
//	dat1.f = numF;
//	dat2.b[0] = dat1.b[3];
//	dat2.b[1] = dat1.b[2];
//	dat2.b[2] = dat1.b[1];
//	dat2.b[3] = dat1.b[0];
//	return dat2.f;
//}
//
//double readNumD(ifstream &in)
//{
//	double numD;
//	union
//	{
//		double d;
//		unsigned char b[8];
//	}dat1, dat2;
//	in.read((char *)&numD, 8);
//	dat1.d = numD;
//	dat2.b[0] = dat1.b[7];
//	dat2.b[1] = dat1.b[6];
//	dat2.b[2] = dat1.b[5];
//	dat2.b[3] = dat1.b[4];
//	dat2.b[4] = dat1.b[3];
//	dat2.b[5] = dat1.b[2];
//	dat2.b[6] = dat1.b[1];
//	dat2.b[7] = dat1.b[0];
//	return dat2.d;
//}
//
//void fillStructI(PVTproperties * oneProperty, string keyword, int datum)
//{
//	if (keyword == "SEQNUM")
//		oneProperty->seqnum.push_back(datum);
//	if (keyword == "INTEHEAD")
//		oneProperty->intehead.push_back(datum);
//	if (keyword == "ICON")
//		oneProperty->icon.push_back(datum);
//	if (keyword == "IGRP")
//		oneProperty->igrp.push_back(datum);
//	if (keyword == "IWEL")
//		oneProperty->iwel.push_back(datum);
//}
//
//void fillStructF(PVTproperties *oneProperty, string keyword, float datum)
//{
//	if (keyword == "BGAS")
//		oneProperty->bgas.push_back(datum);
//	if (keyword == "BOIL")
//		oneProperty->boil.push_back(datum);
//	if (keyword == "BWAT")
//		oneProperty->bwat.push_back(datum);
//	if (keyword == "DENG")
//		oneProperty->deng.push_back(datum);
//	if (keyword == "DENO")
//		oneProperty->deno.push_back(datum);
//	if (keyword == "DENW")
//		oneProperty->denw.push_back(datum);
//	if (keyword == "HOIL")
//		oneProperty->hoil.push_back(datum);
//	if (keyword == "PRESSURE")
//		oneProperty->pressure.push_back(datum);
//	if (keyword == "SGAS")
//		oneProperty->sgas.push_back(datum);
//	if (keyword == "SOIL")
//		oneProperty->soil.push_back(datum);
//	if (keyword == "SSOLID")
//		oneProperty->ssolid.push_back(datum);
//	if (keyword == "SWAT")
//		oneProperty->swat.push_back(datum);
//	if (keyword == "TEMP")
//		oneProperty->temp.push_back(datum);
//	if (keyword == "VOIL")
//		oneProperty->voil.push_back(datum);
//}
//
//void allocateMemoryForData(PVTproperties * oneProperty, string keyword, int size)
//{
//	if (keyword == "SEQNUM")
//		oneProperty->seqnum.resize(size);
//	if (keyword == "INTEHEAD")
//		oneProperty->intehead.resize(size);
//	if (keyword == "ICON")
//		oneProperty->icon.resize(size);
//	if (keyword == "IGRP")
//		oneProperty->igrp.resize(size);
//	if (keyword == "IWEL")
//		oneProperty->iwel.resize(size);
//	if (keyword == "BGAS")
//		oneProperty->bgas.resize(size);
//	if (keyword == "BOIL")
//		oneProperty->boil.resize(size);
//	if (keyword == "BWAT")
//		oneProperty->bwat.resize(size);
//	if (keyword == "DENG")
//		oneProperty->deng.resize(size);
//	if (keyword == "DENO")
//		oneProperty->deno.resize(size);
//	if (keyword == "DENW")
//		oneProperty->denw.resize(size);
//	if (keyword == "HOIL")
//		oneProperty->hoil.resize(size);
//	if (keyword == "PRESSURE")
//		oneProperty->pressure.resize(size);
//	if (keyword == "SGAS")
//		oneProperty->sgas.resize(size);
//	if (keyword == "SOIL")
//		oneProperty->soil.resize(size);
//	if (keyword == "SSOLID")
//		oneProperty->ssolid.resize(size);
//	if (keyword == "SWAT")
//		oneProperty->swat.resize(size);
//	if (keyword == "TEMP")
//		oneProperty->temp.resize(size);
//	if (keyword == "VOIL")
//		oneProperty->voil.resize(size);
//	if (keyword == "ZWEL")
//		oneProperty->zwel.resize(size);
//	if (keyword == "DOUBHEAD")
//		oneProperty->doubhead.resize(size);
//	if (keyword == "LOGIHEAD")
//		oneProperty->logihead.resize(size);
//}
//
//void DATAS::printMe(int timeStep, int num)
//{
//	ofstream out;
//	out.open("boil.txt", ios::out);
//	for (size_t i = 0; i < num; i++)
//	{
//		for (size_t j = 0; j <= timeStep; j++)
//		{
//			out << Datas[j].intehead[i] << "  ";
//			//cout << Datas[j].boil[i] << "  ";
//		}
//		out << endl;
//		//cout << endl;
//	}
//	out.close();
//}