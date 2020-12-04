#include "provided.h"
#include <queue>
#include <vector>
using namespace std;

class DeliveryOptimizerImpl
{
public:
	DeliveryOptimizerImpl(const StreetMap* sm);
	~DeliveryOptimizerImpl();
	void optimizeDeliveryOrder(
		const GeoCoord& depot,
		vector<DeliveryRequest>& deliveries,
		double& oldCrowDistance,
		double& newCrowDistance) const;
private:
	const StreetMap* m_streetMap;
};

DeliveryOptimizerImpl::DeliveryOptimizerImpl(const StreetMap* sm)
{
	m_streetMap = sm;
}

DeliveryOptimizerImpl::~DeliveryOptimizerImpl()
{
}

void DeliveryOptimizerImpl::optimizeDeliveryOrder(
	const GeoCoord& depot,
	vector<DeliveryRequest>& deliveries,
	double& oldCrowDistance,
	double& newCrowDistance) const
{
	//calculate the old crow distance
	oldCrowDistance = 0;
	oldCrowDistance += distanceEarthMiles(depot, deliveries[0].location);
	size_t i;
	for (i = 0; i < deliveries.size()-1; i++) {
		oldCrowDistance += distanceEarthMiles(deliveries[i].location, deliveries[i + 1].location);
	}
	oldCrowDistance += distanceEarthMiles(depot, deliveries[i].location);

	//use a queue to simulate prioirty of orders
	//GREEDY method
	queue<GeoCoord> process;
	process.push(depot);
	double minDist = 9999;
	double dist;
	vector<DeliveryRequest>::iterator j, locationOfMin;
	vector<DeliveryRequest> sortedDeliveries;
	//reorder deliveries based on proximity of coordinates
	while (!deliveries.empty()) {
		GeoCoord curr = process.front();
		for (j = deliveries.begin(); j != deliveries.end(); j++) {
			dist = distanceEarthMiles(curr, j->location);
			if (dist < minDist) {
				minDist = dist;
				locationOfMin = j;	
			}
		}
		sortedDeliveries.push_back(*locationOfMin);
		process.push(locationOfMin->location);
		deliveries.erase(locationOfMin);
		minDist = 9999;
	}

	for (size_t l = 0; l < sortedDeliveries.size(); l++) {
		deliveries.push_back(sortedDeliveries[l]);
	}

	//calculate the new crow distance
	newCrowDistance = 0;
	newCrowDistance += distanceEarthMiles(depot, deliveries[0].location);
	size_t k;
	for (k = 0; k < deliveries.size() - 1; k++) {
		newCrowDistance += distanceEarthMiles(deliveries[k].location, deliveries[k + 1].location);
	}
	newCrowDistance += distanceEarthMiles(depot, deliveries[k].location);
}

//******************** DeliveryOptimizer functions ****************************

// These functions simply delegate to DeliveryOptimizerImpl's functions.
// You probably don't want to change any of this code.

DeliveryOptimizer::DeliveryOptimizer(const StreetMap* sm)
{
	m_impl = new DeliveryOptimizerImpl(sm);
}

DeliveryOptimizer::~DeliveryOptimizer()
{
	delete m_impl;
}

void DeliveryOptimizer::optimizeDeliveryOrder(
	const GeoCoord& depot,
	vector<DeliveryRequest>& deliveries,
	double& oldCrowDistance,
	double& newCrowDistance) const
{
	return m_impl->optimizeDeliveryOrder(depot, deliveries, oldCrowDistance, newCrowDistance);
}
