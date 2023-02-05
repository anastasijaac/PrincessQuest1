#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <list>
#include <SDL.h>
#include "dataGroupType.h"

using namespace std;
#ifndef GROUP_H
#define GROUP_H

//ABSTRACT CLASS

class Group{
public:
	DataGroupType type;
	Group(){
	}
	Group(DataGroupType type){
		this->type = type;
	}
	virtual int numberOfDataInGroup() = 0;
	virtual void addToGroup(string str) = 0;


	virtual void draw() = 0;
};

#endif