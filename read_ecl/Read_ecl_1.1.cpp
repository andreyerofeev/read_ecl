#include<stdio.h>
#include<fstream>
#include<iostream>
#include<vector>
#include<cstdio>
#include<string>

const int maxkeyWordLength = 8;

using namespace std;

struct Data
{
	vector< vector<double> > vecDoubleVal;
	vector< vector<char> > vecCharVal;
	vector< vector<bool> > vecBoolVal;
	string valName;
	void printMe();
};

string delete_end_gaps(string s);
string replace(string s, string old_sym, string new_sym);
char * uint8ToChar(uint8_t *connectString, size_t connectLength);

int main()
{
	char *filename;
	char c_keyWord[9], numC[400], dtypeC[5];
	uint8_t dtype[5];
	uint16_t num;
	uint32_t skipWord;
	string keyWord;
	size_t counter;
	filename = "6dp_sint_oil_res1.txt";
	ifstream in;
	in.open(filename, ios::binary | ios::in);
	in.read((char *)&skipWord, 4); //SHIT!!! because int32 has 4 bytes in matlab and we need to skip 4 bytes
	//cout << skipWord << endl;
	counter = 0;
	//cout << c_keyWord[7] << endl;
	//while (!in.eof())
	{
		counter += 1;
		for (size_t i = 0; i < maxkeyWordLength; i++)
		{
			in.read(&c_keyWord[i], 1);
			//if (c_keyWord[i] != ' ' & c_keyWord[i] != '+')
			//{
			//	keyWord[i] = c_keyWord[i];
			//}
			//if (c_keyWord[i] == '+')
			//{
			//	keyWord[i] = '_';
			//}
			//cout << c_keyWord[i] << endl;
			//cout << c_keyWord << endl;
			//cout << endl;
		}
		
		keyWord = string(c_keyWord);
		//cout << keyWord << '!' << endl;
		//cout << c_keyWord << endl;
		keyWord = delete_end_gaps(keyWord);
		//keyWord = replace(keyWord, "+", "_");
		cout << keyWord << endl;
		//for (size_t i = 0; i < 3; i++)
		//{
		//	in.read(&numC[i], 1);
		//	cout << numC[i] << " " ;
		//}
		//cout << endl << "numC = " << numC << "!" << endl;
		in.read((char *)&num, 2);
		double dnum;
		dnum = (double)num;
		cout << "dnum=" << num << endl;
		in.read((char *)&num, 2);
		dnum = (double)num;
		cout << "dnum=" << num << endl;
		for (size_t i = 0; i < 4; i++)
		{
			in.read((char *)&dtype[i], 1);
			cout << dtype[i] << " ";
		}
		//dtypeC = (char)dtype;
		cout << endl;
		cout << dtype << endl;
		//in.read((char *)&num, 1);
		//cout << num << endl;

	}
	in.close();
	system("pause");
	return(0);
}

void Data::printMe()
{
	//realization
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
	size_t found = 0;
	for (size_t i = 0; i < s.length(); i = i + 1 + found)
	{
		found = s.find(old_sym, i);
		s.replace(found, 1, new_sym);
	}
	return s;
}

char * uint8ToChar(uint8_t *connectString, size_t connectLength)
{
	char* cs = new char[connectLength];
	for (uint32_t i = 0; i < connectLength; i++)
		cs[i] = (char)connectString[i];
	return cs;
}