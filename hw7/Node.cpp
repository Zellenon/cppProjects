//
// Created by Graham on 5/3/20.
//

#include "Node.h"
#include <iostream>

using namespace std;

Node::Node(float lat, float lng, int day, int mnt, int yer, Node* nxt) {
	m_latitude = lat;
	m_longitude = lng;
	m_day = day;
	m_month = mnt;
	m_year = yer;
	m_nextPtr = nxt;
}

void Node::reconstruct(float lat, float lng, int day, int mnt, int yer, Node *nxt) {
	m_latitude = lat;
	m_longitude = lng;
	m_day = day;
	m_month = mnt;
	m_year = yer;
	m_nextPtr = nxt;
}

float Node::getLongitude() const {
	return m_longitude;
}

float Node::getLatitude() const {
	return m_latitude;
}

int Node::getDay() const {
	return m_day;
}

int Node::getMonth() const {
	return m_month;
}

int Node::getYear() const {
	return m_year;
}

Node *Node::getNextPtr() const {
	return m_nextPtr;
}

void Node::setLongitude(const float lng) {
	m_longitude = lng;
}

void Node::setLatitude(const float lat) {
	m_latitude = lat;
}

void Node::setDay(const int day) {
	m_day = day;
}

void Node::setMonth(const int mnt) {
	m_month = mnt;
}

void Node::setYear(const int year) {
	m_year = year;
}

void Node::setNextPtr(Node* ptr) {
	m_nextPtr = ptr;
}

// Puts all Node data into a single line of text (expect for next node pointer)
std::string Node::getDescription() const {
	return to_string(getLatitude()) +","+ to_string(getLongitude()) +" "+ to_string(getDay()) +"/"+ to_string(getMonth()) +"/"+ to_string(getYear());
}

// Deletes all nodes below this one
std::string Node::recursiveDelete() {
	if (m_nextPtr == nullptr) return getDescription() + "\n";
	string msg = m_nextPtr->recursiveDelete() + getDescription() + "\n";
	delete m_nextPtr;
	return msg;
}

bool Node::operator==(const Node &rhs) const {
	return rhs.getDay() == getDay() && rhs.getMonth() == getMonth() && rhs.getYear() == getYear();
}

bool Node::operator!=(const Node &rhs) const {
	return !(*this == rhs);
}

bool Node::operator<(const Node &rhs) const {
	if (m_year < rhs.getYear())
		return true;
	else if (m_year == rhs.getYear()) {
		if (m_month < rhs.getMonth())
			return true;
		else if (m_month == rhs.getMonth())
			if (m_day < rhs.getDay())
				return true;
	}
	return false;
}

bool Node::operator>(const Node &rhs) const {
	if (m_year > rhs.getYear())
		return true;
	else if (m_year == rhs.getYear()) {
		if (m_month > rhs.getMonth())
			return true;
		else if (m_month == rhs.getMonth())
			if (m_day > rhs.getDay())
				return true;
	}
	return false;
}

bool Node::operator<=(const Node &rhs) const {
	return !(*this > rhs);
}

bool Node::operator>=(const Node &rhs) const {
	return !(*this < rhs);
}


#include "Node.h"
