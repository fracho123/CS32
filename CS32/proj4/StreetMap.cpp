#include "provided.h"
#include "ExpandableHashMap.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <functional>
using namespace std;


unsigned int hasher(const GeoCoord& g)
{
	return std::hash<string>()(g.latitudeText + g.longitudeText);
}

class StreetMapImpl
{
public:
	StreetMapImpl();
	~StreetMapImpl();
	bool load(string mapFile);
	bool getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const;
private:
	ExpandableHashMap<GeoCoord, vector<StreetSegment>> ehm;
};

StreetMapImpl::StreetMapImpl()
{
}

StreetMapImpl::~StreetMapImpl()
{
}

bool StreetMapImpl::load(string mapFile)
{
	ifstream file(mapFile);
	// Test for failure to open
	if (!file)
		return false;

	string line;
	string name = "";
	int expectedSegments;
	string ss, se, es, ee;
	bool expectName = true;
	bool expectNSegments = false;
	bool expectSegments = false;


	//while loop utilizes boolean variables to determine what variables to expect on each line;
	while (getline(file, line))
	{
		istringstream iss(line);
		
		//if name of street, save it
		if (expectName == true) {
			expectName = false;
			expectNSegments = true;
			for (size_t k = 0; k < line.length(); k++)
				name += line[k];
			continue;
		}

		//if number of segments, save it
		if (expectNSegments == true) {
			expectNSegments = false;
			expectSegments = true;
			iss >> expectedSegments;
			continue;
		}
		
		//if segments, store their values
		if (expectSegments == true) {
			expectedSegments--;
			iss >> ss;
			iss >> se;
			iss >> es;
			iss >> ee;
			
			GeoCoord coordinate1{ ss, se };
			GeoCoord coordinate2{ es, ee };

			//add segment pointers to vector
			StreetSegment composedSeg1{ coordinate1, coordinate2, name };
			//reverse segment
			StreetSegment composedSeg2{ coordinate2, coordinate1, name };
			
			//find locations of keys in hashmap
			vector<StreetSegment>* locat1 = ehm.find(coordinate1);
			vector<StreetSegment>* locat2 = ehm.find(coordinate2);
			
			//if not found, allocate new one
			if (locat1 == nullptr) {
				vector<StreetSegment> tem1;
				tem1.push_back(composedSeg1);
				ehm.associate(coordinate1, tem1);
			}
			//if found, push to vector
			else {
				(*locat1).push_back(composedSeg1);
			}
			
			//same as before but for reverse segment
			if (locat2 == nullptr) {
				vector<StreetSegment> tem2;
				tem2.push_back(composedSeg2);
				ehm.associate(coordinate2, tem2);
			}
			else {
				(*locat2).push_back(composedSeg2);
			}
			
			if (expectedSegments == 0) {
				expectSegments = false;
				expectName = true;
				name = "";
			}
		}
	}
	return true;
}

bool StreetMapImpl::getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const {
	const vector<StreetSegment>* ss  = ehm.find(gc);
	if (ss == nullptr)
		return false;
	else
		segs = *ss;
	return true;
}

//******************** StreetMap functions ************************************

// These functions simply delegate to StreetMapImpl's functions.
// You probably don't want to change any of this code.

StreetMap::StreetMap()
{
	m_impl = new StreetMapImpl;
}

StreetMap::~StreetMap()
{
	delete m_impl;
}

bool StreetMap::load(string mapFile)
{
	return m_impl->load(mapFile);
}

bool StreetMap::getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const
{
	return m_impl->getSegmentsThatStartWith(gc, segs);
}