//
// Created by Graham on 5/3/20.
//

#ifndef COVID_19_NODE_H
#define COVID_19_NODE_H

#include <string>


class Node {
private:
	float m_latitude, m_longitude;
	int m_day, m_month, m_year;
	Node *m_nextPtr;

public:
	explicit Node(float lat = 0, float lng = 0, int day = 0, int mnt = 0, int yer = 0, Node* nxt = nullptr);

	void reconstruct(float lat = 0, float lng = 0, int day = 0, int mnt = 0, int yer = 0, Node* nxt = nullptr);

	float getLongitude() const;
	float getLatitude() const;
	int getDay() const;
	int getMonth() const;
	int getYear() const;
	Node *getNextPtr() const;
	std::string getDescription() const;

	void setLongitude(float lng);
	void setLatitude(float lat);
	void setDay(int day);
	void setMonth(int mnt);
	void setYear(int year);
	void setNextPtr(Node* ptr);

	std::string recursiveDelete();

	bool operator==(const Node &rhs) const;
	bool operator!=(const Node &rhs) const;
	bool operator<(const Node &rhs) const;
	bool operator>(const Node &rhs) const;
	bool operator<=(const Node &rhs) const;
	bool operator>=(const Node &rhs) const;
};


#endif //COVID_19_NODE_H
