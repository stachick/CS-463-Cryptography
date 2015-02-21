// vigCipher.cpp
// Shane Tachick
// 1/22/2015
// Cryptography
// imports a wordlist and performs a Vignere Polyalphabetic Cipher on those words to find combinations that output a
// ciphertext that starts with 'moose'

#include <vector>
using std::vector;
#include <iostream>
using std::cin;
using std::cout;
using std::endl;
#include <string>
using std::string;
#include <fstream>
using std::ifstream;
using std::fstream;
#include <utility>
using std::pair;


bool loadVector(vector<string> &wordlist)
{
	ifstream infile("wordlist.txt");
	string current;
	while (true)
	{
		getline(infile, current);
		if (!infile)
		{
			if (infile.eof())
			{
				cout << "----------------------------" << endl;
				cout << "end of file reached, groovy." << endl;
				std::cout<<"file size:\t"<<wordlist.size()<<std::endl;
				return true;
			}
			else
			{
				cout << "danger will robinson, danger!" << endl;
				return false;
			}
		}
		wordlist.push_back(current);
	}
}

int main()
{
	vector<string> wordlist;
	loadVector(wordlist);
	pair <string, string> correctMatch;
	vector< pair<string, string> > combinations;

	string ciphertext;
	string plainText;
	string key;
	string desiredCiphertext = "moose";

	cout << "test 1" << endl;

	for (int i = 0; i < wordlist.size(); ++i)
	{
		plainText = wordlist[i];
		//cout << "test 2" << endl;
		if (plainText.length() >= desiredCiphertext.length())
		{
			for (int j = 0; j < wordlist.size(); ++j)
			{
				key = wordlist[j];

				while (key.length() < plainText.length())
				{
					key += key;
				}
				//cout << key << endl;
				//cout << "test 3" << endl;

				ciphertext.resize(plainText.length(), 'A');

				for (int k = 0; k < plainText.length(); ++k)
					ciphertext[k] = (plainText[k]-'a'+key[k]-'a')%26+'a';

				if (ciphertext.length() != desiredCiphertext.length())
					ciphertext.resize(desiredCiphertext.length());

				if (desiredCiphertext == ciphertext)
				{
					correctMatch.first = plainText;
					correctMatch.second = key;
					combinations.push_back(correctMatch);
					cout << "test 7" << endl;

					cout << plainText << endl;
					cout << key << endl;
				}
			}
		}
	}

	cout << "test 6" << endl;
	for (int i = 0; i < combinations.size(); ++i)
	{
		cout << "Plain Text: " << combinations[i].first << endl;
		cout << "Key:        " << combinations[i].second << endl;
		cout << endl;
	}
}