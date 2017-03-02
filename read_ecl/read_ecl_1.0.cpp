#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <cstdlib>
#include <stdio.h>
#include <vector>

using namespace std;

//struct Data
//{
//	struct Datum
//	{
//		//size_t size;
//		//double *doubleVal = new double[size];
//		//char *charVal = new char[size];
//		//bool *boolVal = new bool[size];
//		//vector <string> strParam;
//		vector < vector<double> > vecDoubleVal;
//		vector < vector<char> > vecCharVal;
//		vector < vector<bool> > vecBoolVal;
//	} one[24] ;
//	//vector < vector<double> > vecDoubleVal;
//	//vector < vector<char> > vecCharVal;
//	//vector < vector<bool> > vecBoolVal;
//	string datumName[24];
//};

struct Data
{
	vector < vector<double> > vecDoubleVal;
	vector < vector<char> > vecCharVal;
	vector < vector<bool> > vecBoolVal;
	string datumName;

	void printMe()
	{
		size_t length;
		FILE* output;
		const char* filename;
		char flag;
		filename = datumName.c_str();
		output = fopen(filename, "w");
		if (vecDoubleVal.at(0).size() > 1)
		{
			length = vecDoubleVal.at(0).size();
			flag = 'd';
		}
		if (vecCharVal.at(0).size() > 1)
		{
			length = vecCharVal.at(0).size();
			flag = 'c';
		}
		if (vecBoolVal.at(0).size() > 1)
		{
			length = vecBoolVal.at(0).size();
			flag = 'b';
		}
		for (size_t j = 0; j < length; j++)
		{
			switch (flag)
			{
			case 'd':
				fprintf(output, "%f %f\n", vecDoubleVal.at(0)[j], vecDoubleVal.at(1)[j]);
			case 'c':
				fprintf(output, "%c %c\n", vecCharVal.at(0)[j], vecCharVal.at(1)[j]);
			case 'b':
				fprintf(output, "%d %d\n", vecBoolVal.at(0)[j], vecBoolVal.at(1)[j]);
			}
		}
		//fclose(output);
	}
};

string delete_end_gaps(string s);
string replace(string s, string old_sym, string new_sym);
char * uint8ToChar(uint8_t *connectString, size_t connectLength);
void readData(double num, size_t size, char flag, Data* allParametres[], string keyword, FILE* input, size_t counter);

int main()
{
	char * path;
	path = "6dp_sint_oil_res1.txt";
	char * c_keyword = new char[8],* dtype = new char[4], * c_num;
	uint8_t * u_keyword = new uint8_t[8], * u_dtype = new uint8_t[4];
	//uint32_t * u_num, * useless = new uint32_t[1];
	int u_num, useless;
	size_t wsize, counter = 0;
	string keyword;
	double num;
	FILE* input = fopen(path, "rb");
	Data* allParametres = new Data[24];
	fread(&useless, 1, 1, input);
	double useless1;
	useless1 = (double)useless;
	cout << useless << endl;
	system("pause");
	while (!feof(input))
	{
		fread(&u_keyword, 1, 8, input);
		cout << u_keyword << endl;
		c_keyword = uint8ToChar(u_keyword, 8);
		cout << c_keyword << endl;
		strcpy(c_keyword, keyword.c_str()); //here could happen a shit
		cout << keyword << endl;
		system("pause");
		keyword = delete_end_gaps(keyword);
		keyword = replace(keyword, "+", "_");
		fread(useless, 1, 1, input);
		fread(u_num, 8, 1, input);
		num = (double)*u_num;
		fread(u_dtype, 1, 4, input);
		dtype = uint8ToChar(u_dtype, 4);
		fread(useless, 1, 1, input);
		switch (*dtype)
		{
		case 'INTE':
			wsize = 4;
			readData(num, wsize, 'd', &allParametres, keyword, input, counter); //SHIT!!! 
		case 'REAL':
			wsize = 4;
			readData(num, wsize, 'd', &allParametres, keyword, input, counter);
		case 'DOUB':
			wsize = 8;
			readData(num, wsize, 'd', &allParametres, keyword, input, counter);
		case 'LOGI':
			wsize = 4;
			readData(num, wsize, 'b', &allParametres, keyword, input, counter);
		case 'CHAR':
			num = num * 8;
			wsize = 1;
			readData(num, wsize, 'c', &allParametres, keyword, input, counter);
		}
		fread(useless, 1, 1, input);
		counter++;
	}
	for (size_t i = 0; i < 24; i++)
	{
		allParametres[i].printMe();
	}
	//string str, str2;
	//str = "a123wfgafgnghwndawd     a  ";
	//str2 = delete_end_gaps(str);
	//cout << str2;
	fclose(input);
	system("pause");
	return 0;
}

string delete_end_gaps(string s)
{
	while (s.back() == ' ')
	{
		s.pop_back();
	}
	return s;
}

string replace(string s, string old_sym, string new_sym)
{
	size_t found = 0, i = 0;
	for (size_t i = 0; i < s.length(); i = i + found + 1)
	{
		found = s.find(old_sym, i);
		s.replace(found, 1, new_sym);
	}
	//do
	//{
	//	found = s.find(initial_sym, i);
	//	s.replace(found, 1, final_sym);
	//	i = i + found + 1;
	//} while (i < s.length());
	return s;
}

char * uint8ToChar(uint8_t *connectString, size_t connectLength)
{
	char* cs = new char[connectLength];
	for (uint32_t i = 0; i < connectLength; i++)
		cs[i] = (char)connectString[i];
	return cs;
}

void readData(double num, size_t size, char flag, Data* allParametres[], string keyword, FILE* input, size_t counter)
{
	double remnum, bufnum;
	double *buflen, *dbuf;
	char *cbuf;
	bool *bbuf;
	size_t pos = 0, n = 0, row = 0;
	remnum = num;
	for (size_t i = 0; i < 24; i++)
	{
		if (allParametres[i]->datumName == keyword)
		{
			counter = i;
			row = 1;
		}
		else
		{
			allParametres[counter]->datumName = keyword;
		}
	}
	vector<double>::iterator itd;
	vector<char>::iterator itc;
	vector<bool>::iterator itb;
	itd = allParametres[counter]->vecDoubleVal.at(row).begin();
	itc = allParametres[counter]->vecCharVal.at(row).begin();
	itb = allParametres[counter]->vecBoolVal.at(row).begin();

	while (remnum > 0)
	{
		fread(buflen, 8, 1, input);
		bufnum = (int)*buflen / size;
		switch (flag)
		{
		case 'd':
			fread(dbuf, size, bufnum, input);
			allParametres[counter]->vecDoubleVal.at(row).insert(itd, (int)bufnum, *dbuf);
			itd = itd + bufnum;
		case 'c':
			fread(cbuf, size, bufnum, input);
			allParametres[counter]->vecCharVal.at(row).insert(itc, (int)bufnum, *cbuf);
			itc = itc + bufnum;
		case 'b':
			fread(bbuf, size, bufnum, input);
			allParametres[counter]->vecBoolVal.at(row).insert(itb, (int)bufnum, *bbuf);
			itb = itb + bufnum;
		}
		fread(dbuf, 1, 1, input);
		remnum = remnum - bufnum;
	}
}

