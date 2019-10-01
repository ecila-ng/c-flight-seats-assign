//************************************  PROGRAM IDENTIFICATION  ***************************************
//*                                                                                                   *
//*   PROGRAM FILE NAME:  program1.cpp          ASSIGNMENT #:  1            Grade: _________          *
//*                                                                                                   *
//*   PROGRAM AUTHOR: ________________________________________________________                        *
//*                                     Tran Nguyen                                                   *
//*                                                                                                   *
//*   COURSE #:  CSC 36000 11                              DUE DATE:  February 1, 2019                *
//*                                                                                                   *
//*****************************************************************************************************
//***********************************  PROGRAM DESCRIPTION  *******************************************
//*                                                                                                   *
//*   PROCESS: This program assign seats to passengers for Southern Comfort Airlines. It reads in     *
//*   passenger data then assign seats for them on various flights according to the rules.            *
//*   #Rule 1: Specific seat requests are honored first.                                              * 
//*   #Rule 2: If a requested seat is already assigned to another passenger, the person should be     *
//*    place in the same row, if possible, starting from the left side (lowest seat number).          *
//*   #Rule 3: If the requested row is full, the passenger should be seated as far forward as         *
//*    possible in the requested column but within the requested section.                             *
//*   #Rule 4: If the request using Rule 2 or Rule 3 cannot be honored, the passenger should be       * 
//*    assigned the first available seat starting from the front and left of the requested section    *
//*    going across each row until a vacant seat is found.                                            * 
//*   #Rule 5: First Class and Coach request must be honored.  If there are no seats in the requested *
//*    section, the passenger's Boarding Number must go on a waiting list for the next flight.        * 
//*                                                                                                   * 
//*****************************************************************************************************

#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cstdlib>
#define PAGELINES 50 //Number of lines in one page

using namespace std;

struct passenger {
	int boardNum;
	int flightNum;
	char reqSect;
	int reqRow;
	char reqCol;

	int colIndex;
	int flightIndex;

};

//Declaration
int seatingChart[8][10][3];
passenger waitingList[8][50];
int waitListSize[8];


int isOccupied;

int checkOccupied(int flight, int row, int col);
int findFlights(int flightNum);
int findCol(char reqCol);

void initialize();
void assignSeat(passenger P);

passenger readFile(ifstream &inFile);
void printOutput(ofstream &outFile);



//*************************************  FUNCTION HEADER  **********************************************
void Header(ofstream &Outfile) {
	// Receives – the output file               
	// Task - Prints the output preamble               
	// Returns - Nothing   
	Outfile << setw(30) << "Tran Nguyen";
	Outfile << setw(17) << "CSC 36000";
	Outfile << setw(15) << "Section 11" << endl;
	Outfile << setw(30) << "Spring 2018";
	Outfile << setw(20) << "Assignment #1" << endl;
	Outfile << setw(35) << "-----------------------------------";
	Outfile << setw(35) << "-----------------------------------" << endl << endl;
	return;
}

//*************************************  FUNCTION FOOTER  **********************************************
void Footer(ofstream &Outfile) {
	// Receives – the output file               
	// Task - Prints the output salutation               
	// Returns - Nothing   Outfile << endl;   
	Outfile << setw(35) << " --------------------------------- " << endl;
	Outfile << setw(35) << "|      END OF PROGRAM OUTPUT      |" << endl;
	Outfile << setw(35) << " --------------------------------- " << endl;
	return;
}


int main()
{
	ifstream inFile;
	ofstream outFile("output.txt", ios::out);
	initialize();

	inFile.open("data1.txt");
	passenger newPassenger;

	while (inFile.good()) {
		newPassenger = readFile(inFile);
		assignSeat(newPassenger);
	}

	inFile.close();

	Header(outFile); //Place header at the beginning of output
	printOutput(outFile);

	outFile.close();

	return 0;
}

int findCol(char reqCol) {
	switch (reqCol) {
	case 'L':
		return 0;
		break;
	case 'M':
		return 1;
		break;
	case 'R':
		return 2;
		break;
	}
}

int findFlights(int flightNum) {
	switch (flightNum)
	{
	case 1010: //To Memphis, TN
		return 0;
		break;
	case 1015: //From Memphis, TN
		return 1;
		break;
	case 1020: //To Little Rock, AK
		return 2;
		break;
	case 1025: //From Little Rock, AK
		return 3;
		break;
	case 1030: //To Shreveport, LA
		return 4;
		break;
	case 1035: //From Shreveport, LA
		return 5;
		break;
	case 1040: //To Huntsville, AL
		return 6;
		break;
	case 1045: //From Huntsville, AL
		return 7;
		break;
	default:
		return -1;
		break;
	}


}

passenger readFile(ifstream &inFile)
{
	//Open the input file and start reading all the data into appropriate arrays
	passenger newPassenger;
	inFile >> newPassenger.boardNum >> newPassenger.flightNum >> newPassenger.reqSect;
	inFile >> newPassenger.reqRow >> newPassenger.reqCol;
	newPassenger.flightIndex = findFlights(newPassenger.flightNum);
	newPassenger.colIndex = findCol(newPassenger.reqCol);
	return newPassenger;

}

void assignSeat(passenger P) {
	if (P.boardNum < 0) {
		return;
	}

	isOccupied = checkOccupied(P.flightIndex, P.reqRow, P.colIndex);

	if (isOccupied == 0) {
		seatingChart[P.flightIndex][P.reqRow - 1][P.colIndex] = P.boardNum;
		return;
	}

	//Rule 2: Same row, change col

	for (int c = 0; c < 3; c++) {
		isOccupied = checkOccupied(P.flightIndex, P.reqRow - 1, c);
		if (isOccupied == 0) {
			seatingChart[P.flightIndex][P.reqRow - 1][c] = P.boardNum;
			return;
		}
	}


	//Rule 3: Same col, change row, still in section
	//and Rule 4 : Place in whatever seat is left but within the same section

	//check Section
	//******First Class section
	if (P.reqSect == 'F') {
		for (int r = 0; r < 3; r++) {
			isOccupied = checkOccupied(P.flightIndex, r, P.colIndex);
			if (isOccupied == 0) {
				seatingChart[P.flightIndex][r][P.colIndex] = P.boardNum;
				return;
			}
		}

		//If still not seated, place in whatever seat is left
		for (int r = 0; r < 3; r++) {
			for (int c = 0; c < 3; c++) {
				isOccupied = checkOccupied(P.flightIndex, r, c);
				if (isOccupied == 0) {
					seatingChart[P.flightIndex][r][c] = P.boardNum;
					return;
				}
			}
		}


		//If still not seated, place in the waiting list

		waitingList[P.flightIndex][waitListSize[P.flightIndex]] = P;
		waitListSize[P.flightIndex]++;
		return;

	}

	//******Coach Section
	else if (P.reqSect == 'C') {
		for (int r = 3; r < 10; r++) {
			isOccupied = checkOccupied(P.flightIndex, r, P.colIndex);
			if (isOccupied == 0) {
				seatingChart[P.flightIndex][r][P.colIndex] = P.boardNum;
				return;
			}
		}

		//If still not seated, place in whatever seat is left

		for (int r = 3; r < 10; r++) {
			for (int c = 0; c < 3; c++) {
				isOccupied = checkOccupied(P.flightIndex, r, c);
				if (isOccupied == 0) {
					seatingChart[P.flightIndex][r][c] = P.boardNum;
					return;
				}
			}
		}


		//If still not seated, place in the waiting list

		waitingList[P.flightIndex][waitListSize[P.flightIndex]] = P;
		waitListSize[P.flightIndex]++;
		return;

	}

}

int checkOccupied(int flight, int row, int col) {
	if (seatingChart[flight][row - 1][col] == -999) {
		return 0;
	}
	else {
		return 1;
	}
}

void initialize() {
	for (int f = 0; f < 8; f++) {
		for (int r = 0; r < 10; r++) {
			for (int c = 0; c < 3; c++) {
				seatingChart[f][r][c] = -999;
			}
		}

	}
}

void printOutput(ofstream &outFile) {
	int numLines; //number of lines 

	for (int f = 0; f < 8; f++) {
		numLines = 21;

		switch (f)
		{
		case 0:
			numLines += 4;
			outFile << "Flight 1010" << "\t\t\t\t\t" << "FROM: Jackson, Mississippi" << endl;
			outFile << "\t\t\t\t\t\t" << "TO: Memphis, Tennessee" << endl;
			outFile << endl;
			break;
		case 1:
			outFile << "Flight 1015" << "\t\t\t\t\t" << "FROM: Memphis, Tennessee" << endl;
			outFile << "\t\t\t\t\t\t" << "TO: Jackson, Mississippi" << endl;
			outFile << endl;
			break;
		case 2:
			outFile << "Flight 1020" << "\t\t\t\t\t" << "FROM: Jackson, Mississippi" << endl;
			outFile << "\t\t\t\t\t\t" << "TO: Little Rock, Arkansas" << endl;
			outFile << endl;
			break;
		case 3:
			outFile << "Flight 1025" << "\t\t\t\t\t" << "FROM: Little Rock, Arkansas" << endl;
			outFile << "\t\t\t\t\t\t" << "TO: Jackson, Mississippi" << endl;
			outFile << endl;
			break;
		case 4:
			outFile << "Flight 1030" << "\t\t\t\t\t" << "FROM: Jackson, Mississippi" << endl;
			outFile << "\t\t\t\t\t\t" << "TO: Shreveport, Louisiana" << endl;
			outFile << endl;
			break;
		case 5:
			outFile << "Flight 1035" << "\t\t\t\t\t" << "FROM: Shreveport, Louisiana" << endl;
			outFile << "\t\t\t\t\t\t" << "TO: Jackson, Mississippi" << endl;
			outFile << endl;
			break;
		case 6:
			outFile << "Flight 1040" << "\t\t\t\t\t" << "FROM: Jackson, Mississippi" << endl;
			outFile << "\t\t\t\t\t\t" << "TO: Huntsville, Alabama" << endl;
			outFile << endl;
			break;
		case 7:
			numLines += 4;
			outFile << "Flight 1045" << "\t\t\t\t\t" << "FROM: Huntsville, Alabama" << endl;
			outFile << "\t\t\t\t\t\t" << "TO: Jackson, Mississippi" << endl;
			outFile << endl;
			Footer(outFile);
			break;
		}

		for (int r = 0; r < 10; r++) {
			for (int c = 0; c < 3; c++) {
				outFile << "\t" << seatingChart[f][r][c];
			}
			outFile << endl;
		}
		outFile << endl;
		outFile << "WAITING LIST" << endl;

		if (waitListSize[f] == 0) {
			outFile << endl;
			outFile << "\tThere is no waiting list for this flight." << endl;
		}
		else {
			for (int w = 0; w < waitListSize[f]; w++) {
				if (w % 10 == 0) {
					numLines++;
					outFile << endl;
				}
				outFile << "\t" << waitingList[f][w].boardNum;
			}
			outFile << endl;
		}
		outFile << endl;
		outFile << "END OF SEATING CHART" << endl;

		//New page starts
		for (int x = numLines; x < PAGELINES; x++) {
			outFile << endl;
		}

	}
}
