//stenography.cpp
//Shane Tachick
//February 23, 2015
//Program that will hopefully hide data inside of a PPM image file

#include <iostream>
using std::endl;
using std::cout;
using std::cin;
#include <fstream>
using std::stringstream;
using std::ifstream;
using std::ofstream;
#include <string>
using std::string;
#include <vector>
using std::vector;
#include <math.h>
#include <cstdlib>
using std::rand;
using std::srand;
#include <ctime>
using std::time;
#include <bitset>
using std::bitset;
#include <math.h>
using std::pow;

//a tuple of color nodes, 3 ints, red green and blue values in that order
struct redGreenBlue
{
	int red;
	int green;
	int blue;
};

//takes a character and returns the integer ascii value of it
int convertToInt(char M)
{
	return (int)M;
}

//takes a .ppm file and loads it into a workable vector format
void loadVector(vector<redGreenBlue> &wordlist, string filename, string &header, int &x, int &y, int &colors)
{
	wordlist.clear();

	redGreenBlue temp;
	int tempRed;
	int tempGreen;
	int tempBlue;
	ifstream file(filename);
	getline(file, header);
	file >> x;
	file >> y;
	file >> colors;
	while (file >> tempRed >> tempGreen >> tempBlue)
	{
		temp.red = tempRed;
		temp.green = tempGreen;
		temp.blue = tempBlue;
		wordlist.push_back(temp);
	}
}

//takes a .txt file (hopefully) and uses it to make a vector of ints that correspond to the message
void messageToVector(string filename, vector <redGreenBlue> &charMap)
{
	charMap.clear();
	string message = "";
	redGreenBlue temp;

	//opens the file containing the message to be hidden, assumes proper input
	ifstream inFile(filename);
	string tempString;
	while (!inFile.eof())
	{
		getline(inFile, tempString);
		message = message + tempString + " ";
	}
	// uncomment this to test what the message is
	//cout << message << endl;

	for (size_t i = 0; i < message.size(); i += 3)
	{
		temp.red = (int)message[i];
		if (message.size() > i + 1)
			temp.green = (int)message[i + 1];
		else
			temp.green = 0;
		if (message.size() > i + 2)
			temp.blue = (int)message[i + 2];
		else
			temp.blue = 0;
		charMap.push_back(temp);
	}
}

//takes a .txt filename and converts it into a .ppm image where the characters are converted
//into their ascii decimal value and then used for the RGB values in the picture.
void garbageImage(string message)
{
	vector <redGreenBlue> garbage;
	messageToVector(message, garbage);

	//converts from double to int with data lost, add 1 to makeup for the lost fraction.
	int x = sqrt(garbage.size()) + 1;
	size_t counter = 0;

	ofstream out("tricksymessage.ppm");
	out << "P3\n"
		<< x << " " << x << "\n"
		<< "255\n";
	for (int i = 0; i < x; i++)
	{
		for (int j = 0; j < x; j++)
		{
			if (counter < garbage.size())
			{
				out << garbage[counter].red << " ";
				out << garbage[counter].green << " ";
				out << garbage[counter].blue << "    ";
				counter++;
			}
			else
				out << " 0   0   0     ";
		}
		out << "\n";
	}
}

//prints what the PPM file looks like to the console, mostly used for debugging and presenting
void printVector(vector <redGreenBlue> toPrint, string header, int x, int y, int colors)
{
	int counter = 0;
	cout << header << endl;
	cout << x << " " << y << endl;
	cout << colors << endl;
	for (int i = 0; i < y; i++)
	{
		for (int j = 0; j < x; j++)
		{
			cout << toPrint[counter].red << " ";
			cout << toPrint[counter].green << " ";
			cout << toPrint[counter].blue << "    ";
			counter++;
		}
		cout << endl;
	}
}

//takes a vector of redGreenBlue's and breaks it into a plain vector of ints, used for
//working with 2 vectors simultaniously to make the math of it simpler
void cleanerVector(vector <redGreenBlue> &incoming, vector <int> &cleanerInput)
{
	for (size_t i = 0; i < incoming.size(); i++)
	{
		cleanerInput.push_back(incoming[i].red);
		cleanerInput.push_back(incoming[i].green);
		cleanerInput.push_back(incoming[i].blue);
	}
}

//takes a .txt file message and a PPM image and generates a code specific to that image for the message
//and outputs that code to a text file
void generateCode(vector <redGreenBlue> incomingPPM, vector <redGreenBlue> incomingTXT)
{
	vector <string> temp;
	vector <int> cleanerInput;
	ofstream out("codedmessage.txt");
	int PPMindex = 0;
	int startPos = 0;
	int offset = 0;

	cleanerVector(incomingTXT, cleanerInput);

	//takes 'characters' from cleanerInput and searches incomingPPM for a match, then outputs the index
	for (size_t i = 0; i < cleanerInput.size(); i++)
	{
		
		PPMindex = rand();
		PPMindex %= incomingPPM.size();
		startPos = PPMindex;
		offset = 0;
		
		while (true)
		{
			if (PPMindex == incomingPPM.size() - 1)
				PPMindex = 0;
			else
				PPMindex++;

			if (startPos == PPMindex)
				offset++;

			if ((incomingPPM[PPMindex].red - offset) == (cleanerInput[i]))
			{
				out << PPMindex << " " << offset << " " << "r" << "\n";
				offset = 0;
				PPMindex = startPos;
				break;
			}
			if ((incomingPPM[PPMindex].green - offset) == (cleanerInput[i]))
			{
				out << PPMindex << " " << offset << " " << "g" << "\n";
				offset = 0;
				PPMindex = startPos;
				break;
			}
			if ((incomingPPM[PPMindex].blue - offset) == (cleanerInput[i]))
			{
				out << PPMindex << " " << offset << " " << "b" << "\n";
				offset = 0;
				PPMindex = startPos;
				break;
			}
		}
	}

	//uncomment to see what is actually in the vector cleanerInput
	/*
	for (int i = 0; i < cleanerInput.size(); i++)
	{
	cout << (char)cleanerInput[i];
	}
	*/
}

//takes a coded garbage image and decodes it back to the original text 
void decodeImage(string codedPPMfile)
{
	vector <redGreenBlue> codedPPM;
	int dontCareX;
	int dontCareY;
	int dontCareColors;
	string dontCareHeader;
	ofstream out("decodedgarbage.txt");
	loadVector(codedPPM, codedPPMfile, dontCareHeader, dontCareX, dontCareY, dontCareColors);
	for (size_t i = 0; i < codedPPM.size(); i++)
	{
		out << (char)codedPPM[i].red;
		out << (char)codedPPM[i].green;
		out << (char)codedPPM[i].blue;
	}
}

//takes a text file that has been coded using a specific PPM image and decodes it back to its original message
void decodeCode(string inputTXT, vector <redGreenBlue> basePPM)
{

	ifstream file(inputTXT);
	ofstream out("decoded.txt");
	int tempIndex;
	int tempOffset;
	char tempChar;

	while (file >> tempIndex >> tempOffset >> tempChar)
	{
		if (tempChar == 'r')
			out << (char)(basePPM[tempIndex].red - tempOffset);
		if (tempChar == 'g')
			out << (char)(basePPM[tempIndex].green - tempOffset);
		if (tempChar == 'b')
			out << (char)(basePPM[tempIndex].blue - tempOffset);
	}
}

//takes and int and returns its 8-bit binary representation
bitset<8> toBinary(int number)
{
	bitset<8> temp;
	int index = 0;
	while (number >= 1)
	{
		temp[index] = number % 2;
		number /= 2;
		index++;
	}


	/* Uncomment for sanity checking
	for (size_t i = 0; i < temp.size(); i++)
	{
	cout << temp[i];
	}
	*/
	return temp;
}

//converts a vector of ints representing a message into a complimentary vector of binary values
//uncomment the print statements for sanity checking
void convertMSGtoBinary(vector<redGreenBlue> &incomingTXT, vector <bitset<8>> &messageInBinary)
{
	messageInBinary.clear();

	for (size_t i = 0; i < incomingTXT.size(); i++)
	{
		//cout << incomingTXT[i].red << endl;
		messageInBinary.push_back(toBinary(incomingTXT[i].red));
		//cout << endl;
		//cout << incomingTXT[i].green << endl;
		messageInBinary.push_back(toBinary(incomingTXT[i].green));
		//cout << endl;
		//cout << incomingTXT[i].blue << endl;
		messageInBinary.push_back(toBinary(incomingTXT[i].blue));
		//cout << endl;
	}
}

void convertPasswordToBinary(string password, vector <bitset<8>> &binaryPassword)
{
	binaryPassword.clear();

	for (size_t i = 0; i < password.size(); i++)
	{
		binaryPassword.push_back(password[i]);
	}

	//uncomment to sanity check your password
	/*
	for (size_t i = 0; i < binaryPassword.size(); i++)
	{
		cout << binaryPassword[i] << endl;
	}
	*/
}

void xorMessageAndPassword(vector <bitset<8>> &messageInBinary, vector <bitset<8>> binaryPassword)
{
	for (size_t i = 0; i < messageInBinary.size(); i++)
	{
		//uncomment to sanity check the logic
		/*
		cout << "mes: " << messageInBinary[i] << endl;
		cout << "Pas: " << binaryPassword[i % binaryPassword.size()] << endl;
		cout << "Xor: " << (messageInBinary[i] ^ binaryPassword[i % binaryPassword.size()]) << endl;
		cout << endl;
		*/

		messageInBinary[i] = messageInBinary[i] ^ binaryPassword[i % binaryPassword.size()];
	}
}

void createBinaryCodedPPM(vector <bitset<8>> & messageInBinary, vector <redGreenBlue> &incomingPPM, string newImage, string header, int x, int y, int colors)
{
	vector <int> cleanerPPM;

	cleanerVector(incomingPPM, cleanerPPM);

	if (cleanerPPM.size() < (messageInBinary.size() * 8))
	{
		cout << "This picture is too small to encode this message" << endl;
		return;
	}

	//sanity checking on binary message
	/*
	cout << "Message going into the file" << endl;
	for (size_t i = 0; i < messageInBinary.size(); i++)
	{
		cout << messageInBinary[i] << endl;
	}
	*/

	ofstream out(newImage);
	int binCounter = 0;
	int charCounter = 0;

	for (size_t i = 0; i < cleanerPPM.size(); i++)
	{
		if (binCounter >= 8)
		{
			binCounter = 0;
			charCounter++;
		}
		if (charCounter >= messageInBinary.size())
			break;

		if (((cleanerPPM[i] % 2 == 0) || (cleanerPPM[i] == 0)) && (messageInBinary[charCounter].test(binCounter)))
		{
			if (cleanerPPM[i] == 0)
				cleanerPPM[i]++;
			else
				cleanerPPM[i]--;
		}
		else if ((cleanerPPM[i] % 2 == 1) && !(messageInBinary[charCounter].test(binCounter)))
		{
			if (cleanerPPM[i] == 255)
				cleanerPPM[i]--;
			else
				cleanerPPM[i]++;
		}
		binCounter++;
	}

	int counter = 0;
	out << "P3\n"
		<< x << " " << y << "\n"
		<< colors << "\n";
	for (size_t j = 0; j < y; j++)
	{
		for (size_t i = 0; i < x; i++)
		{
			out << cleanerPPM[counter] << " ";
			out << cleanerPPM[counter + 1] << " ";
			out << cleanerPPM[counter + 2] << "    ";
			counter += 3;
		}
		out << "\n";
	}
}

void decodeBinary(string codedPPM, vector <bitset<8>> binaryPassword)
{
	string stringDontCareHeader;
	int X;
	int Y;
	int intDontCareColors;
	vector <redGreenBlue> temp;
	vector <int> workingVector;
	vector <bitset<8>> binMSG;
	int charCounter = 0;
	int binCounter = 0;
	bitset<8> tempBitset;

	loadVector(temp, codedPPM, stringDontCareHeader, X, Y, intDontCareColors);
	cleanerVector(temp, workingVector);

	for (size_t i = 0; i < workingVector.size(); i++)
	{
		if (workingVector[i] % 2 == 1)
			tempBitset[binCounter] = 1;
		else
			tempBitset[binCounter] = 0;

		binCounter++;

		if (binCounter > 7)
		{
			binCounter = 0;
			charCounter++;
			binMSG.push_back(tempBitset);
		}
	}

	//sanity checking for the binary message coming out of the file
	/*
	cout << "Binary coming from the coded image" << endl;
	for (size_t i = 0; i < binMSG.size(); i++)
	{
	cout << binMSG[i] << endl;
	}
	*/

	xorMessageAndPassword(binMSG, binaryPassword);

	ofstream out("binaryMessage.txt");
	for (size_t i = 0; i < binMSG.size(); i++)
	{
		int temp = 0;

		for (size_t j = 0; j < 8; j++)
		{
			if (binMSG[i].test(j) == true)
				temp += pow(2, j);
		}
		out << (char)temp;
	}
}

void waitOnUser()
{
	char pause;
	cout << endl;
	cout << "Press enter to continue" << endl;
	cin.ignore();
	cin.get(pause);
}

int main()
{
	string inputPPM;
	string inputTXT;
	string newImage;
	string header;
	string codedPPM;
	string password;
	int menuOption = -1;
	int x;
	int y;
	int colors;
	srand(time(0));

	vector <bitset<8>> messageInBinary;
	vector <bitset<8>> binaryPassword;
	vector <redGreenBlue> incomingPPM;
	vector <redGreenBlue> incomingTXT;


	while (menuOption != 0)
	{
		cout << "Which module do you want to run?\n" <<
			"0. Press 0 to exit\n" <<
			"1. Load a PPM image and print it to the console.\n" <<
			"2. Generate a random image using your message as the Red, Green, and Blue values for each pixel.\n" <<
			"3. Generate a coded message from a PPM image that can be decoded using the same image.\n" <<
			"4. Decodes a message that was encoded using option 4.\n" <<
			"5. Alter a PPM image to hide your message by manipulating the least significant bits of the RGB values for each pixel.\n"
			"6. Decodes a message that was encoded using option 5.\n" << endl;

		cout << endl << "Enter your choice." << endl;
		cin >> menuOption;

		switch (menuOption)
		{
		case 1:
			//loads a PPM image and prints its contents
			cout << "What is the name of the PPM image to load?" << endl;
			cin >> inputPPM;
			loadVector(incomingPPM, inputPPM, header, x, y, colors);
			printVector(incomingPPM, header, x, y, colors);

			waitOnUser();
			break;

		case 2:
			//takes a TXT file and generates a garbage PPM image from it
			cout << "What is the name of the TXT file you want to disguise?" << endl;
			cin >> inputTXT;
			garbageImage(inputTXT);
			cout << "What is the name of the disquised PPM file you want to decode?" << endl;
			cin >> inputPPM;
			decodeImage(inputPPM);

			waitOnUser();
			break;

		case 3:
			//takes a message and a PPM image and generates a coded message specific to that image
			cout << "What is the name of the PPM image to load?" << endl;
			cin >> inputPPM;
			cout << "What is the message you want disguised?" << endl;
			cin >> inputTXT;
			loadVector(incomingPPM, inputPPM, header, x, y, colors);
			messageToVector(inputTXT, incomingTXT);
			generateCode(incomingPPM, incomingTXT);

			waitOnUser();
			break;

		case 4:
			//Takes a base picture and a coded message and decodes it back into the original message
			cout << "What is the name of the coded file?" << endl;
			cin >> inputTXT;
			cout << "What is the name of the base picture?" << endl;
			cin >> inputPPM;
			loadVector(incomingPPM, inputPPM, header, x, y, colors);
			decodeCode(inputTXT, incomingPPM);

			waitOnUser();
			break;

		case 5:
			//Takes a picture and a message and encodes the message into the picture by modifying the 
			//least significant bits in that picture
			cout << "What is the name of the PPM image to load?" << endl;
			cin >> inputPPM;
			cout << "What is the message you want disguised?" << endl;
			cin >> inputTXT;
			cout << "What is your password?" << endl;
			cin >> password;
			cout << "What do you want the duplicate image named?" << endl;
			cin >> newImage;
			loadVector(incomingPPM, inputPPM, header, x, y, colors);
			messageToVector(inputTXT, incomingTXT);
			convertMSGtoBinary(incomingTXT, messageInBinary);
			convertPasswordToBinary(password, binaryPassword);
			xorMessageAndPassword(messageInBinary, binaryPassword);
			createBinaryCodedPPM(messageInBinary, incomingPPM, newImage, header, x, y, colors);

			waitOnUser();
			break;

		case 6:
			cout << "What is the name of the binary coded PPM image?" << endl;
			cin >> codedPPM;
			cout << "What is your password?" << endl;
			cin >> password;
			convertPasswordToBinary(password, binaryPassword);
			decodeBinary(codedPPM, binaryPassword);

			waitOnUser();
			break;

		default:
			break;
		}
	}
	return 0;
}
