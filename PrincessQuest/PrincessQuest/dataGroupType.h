#ifndef DATAGROUPTYPE_H
#define DATAGROUPTYPE_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>


using namespace std;

class DataGroupType{
public:
	string groupName{};
	int dataType{};
	bool singleItem{};

	static void saveRSDataGroupType(ofstream& file, DataGroupType dataGroupType) {};
	static DataGroupType loadRSDataGroupType(ifstream& file) {};

	static const int DATATYPE_STRING = 0, DATATYPE_POSITION = 1, DATATYPE_BOX = 2, DATATYPE_NUMBER = 3;

};

#endif