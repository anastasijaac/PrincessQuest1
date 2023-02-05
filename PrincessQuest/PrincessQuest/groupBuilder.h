#ifndef GROUPBUILDER
#define GROUPBUILDER

#include "Globale.h"
#include "group.h"
#include "groupBox.h"
#include "groupPosition.h"
#include "groupNumber.h"
#include "groupString.h"

class GroupBuilder
{

public:
	
	static const bool savedInGroups; 

	
	static Group* buildGroup(DataGroupType dataType);
	
	static void buildGroups(list<DataGroupType> groupTypes, list<Group*> &groups);

	
	static Group* addGroupStringToGroup(string name, list<Group*> &groups);
	
	static void loadGroups(ifstream &file, list<Group*> &groups);
	
	static Group* findGroupByName(string str, list<Group*> &groups);

};

#endif