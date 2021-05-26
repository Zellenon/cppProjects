//
// Created by Graham on 5/3/20.
//

#ifndef COVID_19_MYLIST_H
#define COVID_19_MYLIST_H

#include "Node.h"

class MyList {
private:
	Node* m_startPtr;
	Node* m_currentPtr;
public:
	explicit MyList(Node* start = nullptr);
	bool empty() const;
	void print() const;
	void insert(Node* node);
	void remove(Node node);
	void clear();
	const Node * getCurrentNode();
	void advanceToNextNode();
	void resetCurrentPtr();
};


#endif //COVID_19_MYLIST_H
