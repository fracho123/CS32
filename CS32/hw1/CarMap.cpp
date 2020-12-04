#include "CarMap.h"
#include <iostream>

CarMap::CarMap() {       // Create an empty car map.
}
bool CarMap::addCar(std::string license) {
	return m_cars.insert(license, 0);
}
// If a car with the given license plate is not currently in the map, 
// and there is room in the map, add an entry for that car recording
// that it has been driven 0 miles, and return true.  Otherwise,
// make no change to the map and return false.

double CarMap::miles(std::string license) const {
	double miles;
	if (m_cars.contains(license)) {
		m_cars.get(license, miles);
		return miles;
	}
	return -1;
}
// If a car with the given license plate is in the map, return how
// many miles it has been driven; otherwise, return -1.

bool CarMap::drive(std::string license, double distance) {
	double miles;
	if (!m_cars.contains(license) || distance < 0)
		return false;
	m_cars.get(license, miles);
	miles += distance;
	return m_cars.update(license, miles);
}
// If no car with the given license plate is in the map or if
// distance is negative, make no change to the map and return
// false.  Otherwise, increase by the distance parameter the number
// of miles the indicated car has been driven and return true.

int CarMap::fleetSize() const {
	return m_cars.size();
}
// Return the number of cars in the CarMap.

void CarMap::print() const {
	std::string lic;
	double mil;
	for (int i = 0; i < m_cars.size(); i++) {
		m_cars.get(i, lic, mil);
		std::cout << lic << " " << mil << std::endl;
	}
}
// Write to cout one line for every car in the map.  Each line
// consists of the car's license plate, followed by one space,
// followed by the number of miles that car has been driven.  Write
// no other text.  The lines need not be in any particular order.