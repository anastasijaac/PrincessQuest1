#include "groupBuilder.h"
#include "Globale.h"

const bool GroupBuilder::savedInGroups = false;

Group* GroupBuilder::buildGroup(DataGroupType dataType)
{
	Group* group = nullptr;

	if (dataType.dataType == DataGroupType::DATATYPE_NUMBER)
	{
		group = new GroupNumber(dataType);
	}
	else if (dataType.dataType == DataGroupType::DATATYPE_POSITION)
	{
		group = new GroupPosition(dataType);
	}
	else if (dataType.dataType == DataGroupType::DATATYPE_BOX)
	{
		group = new GroupBox(dataType);
	}
	else{
		group = new GroupString(dataType);
	}

	return group;
}

void GroupBuilder::buildGroups(list<DataGroupType> groupTypes, list<Group*> &groups){
	for (list<DataGroupType>::iterator dgt = groupTypes.begin(); dgt != groupTypes.end(); dgt++)
	{
		Group *group = buildGroup((*dgt));
		groups.push_back(group);
	}
}

Group* GroupBuilder::addGroupStringToGroup(string name, list<Group*> &groups){
	DataGroupType dgt;
	dgt.dataType = DataGroupType::DATATYPE_STRING;
	dgt.groupName = name;
	dgt.singleItem = false;
	Group *group = new GroupString(dgt);
	groups.push_back(group); 
	return group;
}
void GroupBuilder::loadGroups(ifstream &file, list<Group*> &groups) {
	//liest jede gruppe in dem file: 
	while (!file.eof())
	{

		//nächste Zeile von der Datei
		int positionBeforeRead = static_cast<int>(file.tellg());
		string line;
		getline(file, line);
		if (line.empty() || line == "")
			break; 
		if (savedInGroups)
		{
			//such den semicolon 
			int pos = line.find(":", 0);
			if (pos == -1)
			{
				file.seekg(positionBeforeRead);
				break; 
			}
			Group *group = findGroupByName(line.substr(0, pos), groups);
			if (group == nullptr){
				//findet die Gruppe nicht speichert aber die daten
				group = addGroupStringToGroup(line.substr(0, pos), groups);
			}
			string numStr = line.substr(pos + 1, line.length() - pos + 2);
			stringstream ss;
			ss << numStr << endl;
			int num;
			ss >> num;

			for (int i = 0; i < num; i++)
			{
				if (!file.good())
					break;

				getline(file, line);

				group->addToGroup(line);
			}
				 
		}
		else
		{
			int pos = line.find(":", 0);
			if (pos == -1){
				file.seekg(positionBeforeRead);
				break; 
			}
			cout << "sub: " << line.substr(0, pos) << endl;
			Group *group = findGroupByName(line.substr(0, pos), groups);
			if (group == NULL){
				group = addGroupStringToGroup(line.substr(0, pos), groups);
			}
			//säubert den String damit Daten eingefügt werden können
			line = Globale::clipOffDataHeader(line); //lösche den gruppennamen
			group->addToGroup(line);
			
		}
	}

}

Group* GroupBuilder::findGroupByName(string str, list<Group*> &groups){
	for (list<Group*>::iterator group = groups.begin(); group != groups.end(); group++)
	{
		if (str == (*group)->type.groupName)
			return (*group);
	}

	return NULL;
}