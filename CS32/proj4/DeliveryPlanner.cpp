#include "provided.h"
#include <vector>
#include <list>
#include <map>
using namespace std;

string compassDir(const double& direction);

class DeliveryPlannerImpl
{
public:
	DeliveryPlannerImpl(const StreetMap* sm);
	~DeliveryPlannerImpl();
	DeliveryResult generateDeliveryPlan(
		const GeoCoord& depot,
		const vector<DeliveryRequest>& deliveries,
		vector<DeliveryCommand>& commands,
		double& totalDistanceTravelled) const;
private:
	const StreetMap* m_streetMap;
	const PointToPointRouter* m_ptpr;
	DeliveryOptimizer* m_dO;
};

DeliveryPlannerImpl::DeliveryPlannerImpl(const StreetMap* sm)
{
	m_streetMap = sm;
	m_ptpr = new PointToPointRouter(sm);
	m_dO = new DeliveryOptimizer(sm);
}

DeliveryPlannerImpl::~DeliveryPlannerImpl()
{
	delete m_ptpr;
	delete m_dO;
}

DeliveryResult DeliveryPlannerImpl::generateDeliveryPlan(
	const GeoCoord& depot,
	const vector<DeliveryRequest>& deliveries,
	vector<DeliveryCommand>& commands,
	double& totalDistanceTravelled) const
{
	//First, reorder the order of the delivery requests (using the DeliveryOptimizer class) to 
	//optimize / reduce the total travel distance.
	double oldCrow, newCrow;
	vector<DeliveryRequest> modifiableDeliveries;
	for (size_t m = 0; m < deliveries.size(); m++)
		modifiableDeliveries.push_back(deliveries[m]);
	m_dO->optimizeDeliveryOrder(depot, modifiableDeliveries, oldCrow, newCrow);

	//generating point to point routes and saving them in totalRoute;
	list<StreetSegment> route;
	list<StreetSegment> totalRoute;
	totalDistanceTravelled = 0;
	double tD;

	//from depot to first location
	DeliveryResult res = m_ptpr->generatePointToPointRoute(depot, modifiableDeliveries[0].location, route, tD);
	totalDistanceTravelled += tD;
	if (res == BAD_COORD)
		return BAD_COORD;
	else if (res == NO_ROUTE)
		return NO_ROUTE;
	list<StreetSegment>::iterator it;
	for (it = route.begin(); it != route.end(); it++) {
		totalRoute.push_back(*it);
	}
	route.clear();

	//from location to location
	size_t i;
	for (i = 0; i < modifiableDeliveries.size() - 1; i++) {
		res = m_ptpr->generatePointToPointRoute(modifiableDeliveries[i].location, modifiableDeliveries[i + 1].location, route, tD);
		totalDistanceTravelled += tD;
		if (res == BAD_COORD)
			return BAD_COORD;
		else if (res == NO_ROUTE)
			return NO_ROUTE;
		for (it = route.begin(); it != route.end(); it++) {
			totalRoute.push_back(*it);
		}
		route.clear();
	}

	//from last location to depot
	res = m_ptpr->generatePointToPointRoute(modifiableDeliveries[modifiableDeliveries.size() - 1].location, depot, route, tD);
	totalDistanceTravelled += tD;
	if (res == BAD_COORD)
		return BAD_COORD;
	else if (res == NO_ROUTE)
		return NO_ROUTE;
	for (it = route.begin(); it != route.end(); it++) {
		totalRoute.push_back(*it);
	}
	route.clear();

	//convert to delivery commands
	commands.clear();
	string name = "";
	string prevName = "";
	double distance = 0;
	double direction;
	string dirString;
	double angle;
	string ofTurn = "";
	DeliveryCommand dC;
	int deliveryIterator = 0;
	list<StreetSegment>::iterator prevSeg;
	for (it = totalRoute.begin(); it != totalRoute.end(); it++) {
		name = it->name;
		//if segments continue down same road or if it's the first segment, calculate direction and distance
		if (name == prevName || prevName == "") {
			if (prevName == "")
				direction = angleOfLine(*it);
			distance += distanceEarthMiles(it->start, it->end);
		}

		//determing compass direction from double-valued direction
		dirString = compassDir(direction);

		//if finished with list, conclude with a proceed command
		if (++it == totalRoute.end()) {
			dC.initAsProceedCommand(dirString, name, distance);
			commands.push_back(dC);
			break;
		}
		it--;

		//determine if a delivery is in order
		//if so, conclude ongoing segments with a proceed and add a delivery.
		if (deliveryIterator < modifiableDeliveries.size() && it->end == modifiableDeliveries[deliveryIterator].location) {
			GeoCoord repeatingLocation = modifiableDeliveries[deliveryIterator].location;
			dC.initAsProceedCommand(dirString, name, distance);
			commands.push_back(dC);
			while (repeatingLocation == modifiableDeliveries[deliveryIterator].location) {
				dC.initAsDeliverCommand(modifiableDeliveries[deliveryIterator].item);
				deliveryIterator++;
				commands.push_back(dC);
				if (deliveryIterator == modifiableDeliveries.size()) {
					break;
				}
			}
			distance = 0;
			prevName = "";
			continue;
		}

		//if a turn is needed
		if (name != prevName && prevName != "") {
			//make proceed command to conclude previous segments
			dC.initAsProceedCommand(dirString, prevName, distance);
			commands.push_back(dC);
			distance = 0;

			//find angle between segments of different streets and recalculate direction and distance of segment
			//for turn command
			angle = angleBetween2Lines(*prevSeg, *it);
			direction = angleOfLine(*it);
			dirString = compassDir(direction);
			distance += distanceEarthMiles(it->start, it->end);
			if (angle < 1 || angle > 359) {
				prevName = name;
				continue;
			}
			else if (angle < 180 && angle >= 1)
				ofTurn = "left";
			else if (angle <= 359 && angle >= 180)
				ofTurn = "right";
			dC.initAsTurnCommand(ofTurn, name);
			commands.push_back(dC);
		}
		prevName = name;
		prevSeg = it;
	}

	if (deliveryIterator == modifiableDeliveries.size())
		return DELIVERY_SUCCESS;
	return NO_ROUTE;
}

//******************** DeliveryPlanner functions ******************************

// These functions simply delegate to DeliveryPlannerImpl's functions.
// You probably don't want to change any of this code.

DeliveryPlanner::DeliveryPlanner(const StreetMap* sm)
{
	m_impl = new DeliveryPlannerImpl(sm);
}

DeliveryPlanner::~DeliveryPlanner()
{
	delete m_impl;
}

DeliveryResult DeliveryPlanner::generateDeliveryPlan(
	const GeoCoord& depot,
	const vector<DeliveryRequest>& deliveries,
	vector<DeliveryCommand>& commands,
	double& totalDistanceTravelled) const
{
	return m_impl->generateDeliveryPlan(depot, deliveries, commands, totalDistanceTravelled);
}

string compassDir(const double& direction) {
	if (direction >= 0 && direction < 22.5)
		return "east";
	else if (direction >= 22.5 && direction < 67.5)
		return "northeast";
	else if (direction >= 67.5 && direction < 112.5)
		return "north";
	else if (direction >= 112.5 && direction < 157.5)
		return "northwest";
	else if (direction >= 157.5 && direction < 202.5)
		return "west";
	else if (direction >= 202.5 && direction < 247.5)
		return "southwest";
	else if (direction >= 247.5 && direction < 292.5)
		return "south";
	else if (direction >= 292.5 && direction < 337.5)
		return "southeast";
	return "east";
}
