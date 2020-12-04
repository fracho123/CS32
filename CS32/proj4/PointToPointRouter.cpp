#include "provided.h"
#include <list>
#include <stack>
#include <set>
#include <map>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

class PointToPointRouterImpl
{
public:
	PointToPointRouterImpl(const StreetMap* sm);
	~PointToPointRouterImpl();
	DeliveryResult generatePointToPointRoute(
		const GeoCoord& start,
		const GeoCoord& end,
		list<StreetSegment>& route,
		double& totalDistanceTravelled) const;
private:
	const StreetMap* m_streetMap;

	struct branchingSegment {
		StreetSegment seg;
		size_t previous;
	};
};

PointToPointRouterImpl::PointToPointRouterImpl(const StreetMap* sm)
{
	m_streetMap = sm;
}

PointToPointRouterImpl::~PointToPointRouterImpl()
{
}

DeliveryResult PointToPointRouterImpl::generatePointToPointRoute(const GeoCoord& start, const GeoCoord& end, list<StreetSegment>& route, double& totalDistanceTravelled) const
{
	//queue method of path finding
	queue<branchingSegment> toDo;
	//a vector of branching segments to make them easily accessible for other segments
	vector<branchingSegment> savedSegments;
	//vector of visited coordinates
	vector<GeoCoord> visited;
	visited.push_back(start);

	if (start == end) {
		route.clear();
		totalDistanceTravelled = 0;
		return DELIVERY_SUCCESS;
	}


	//get segments that branch off from the starting coordinate
	vector<StreetSegment> segs;
	if (!m_streetMap->getSegmentsThatStartWith(start, segs))
		return BAD_COORD;
	//push these segments' end coordinates to visited and push them to queue
	for (size_t i = 0; i < segs.size(); i++) {
		GeoCoord star(segs[i].start.latitudeText, segs[i].start.longitudeText);
		GeoCoord visit(segs[i].end.latitudeText, segs[i].end.longitudeText);
		visited.push_back(visit);
		branchingSegment temp;
		StreetSegment segmen(star, visit, segs[i].name);
		temp.seg = segmen;
		temp.previous = -1;
		toDo.push(temp);
	}
	segs.clear();

	while (!toDo.empty())
	{
		//pop top off for processing
		savedSegments.push_back(toDo.front());
		branchingSegment curr = savedSegments[savedSegments.size() - 1];
		size_t point = savedSegments.size() - 1;
		toDo.pop();

		//if end found
		if (curr.seg.end == end) {
			totalDistanceTravelled = 0;
			route.clear();
			size_t root = curr.previous;

			route.push_front(curr.seg);
			totalDistanceTravelled += distanceEarthMiles(curr.seg.start, curr.seg.end);

			while (root != -1) {
				route.push_front(savedSegments[root].seg);
				totalDistanceTravelled += distanceEarthMiles(savedSegments[root].seg.start, savedSegments[root].seg.end);
				root = savedSegments[root].previous;
			}
			return DELIVERY_SUCCESS;
		}

		//find branching segments from top segment
		if (!m_streetMap->getSegmentsThatStartWith(curr.seg.end, segs))
			return BAD_COORD;

		//check to see if the end coordinates of each branching segment has been visited yet
		size_t j;
		for (size_t i = 0; i < segs.size(); i++) {
			for (j = 0; j < visited.size(); j++) {
				if (segs[i].end == visited[j])
					break;
			}
			//if the end coordinates havent been visited yet, we can add it
			if (j == visited.size()) {
				GeoCoord star(segs[i].start.latitudeText, segs[i].start.longitudeText);
				GeoCoord visit(segs[i].end.latitudeText, segs[i].end.longitudeText);
				visited.push_back(visit);
				branchingSegment temp;
				StreetSegment segmen(star, visit, segs[i].name);
				temp.seg = segmen;
				temp.previous = point;
				toDo.push(temp);
			}
		}
		segs.clear();

	}
	return NO_ROUTE;
}

//******************** PointToPointRouter functions ***************************

// These functions simply delegate to PointToPointRouterImpl's functions.
// You probably don't want to change any of this code.

PointToPointRouter::PointToPointRouter(const StreetMap* sm)
{
	m_impl = new PointToPointRouterImpl(sm);
}

PointToPointRouter::~PointToPointRouter()
{
	delete m_impl;
}

DeliveryResult PointToPointRouter::generatePointToPointRoute(
	const GeoCoord& start,
	const GeoCoord& end,
	list<StreetSegment>& route,
	double& totalDistanceTravelled) const
{
	return m_impl->generatePointToPointRoute(start, end, route, totalDistanceTravelled);
}
