//
// Created by Graham on 5/3/20.
//

#include "MyList.h"
#include <string>
#include <iostream>

using namespace std;

MyList::MyList(Node *start) {
	m_startPtr = start;
	m_currentPtr = start;
}

bool MyList::empty() const {
	return m_startPtr == nullptr;
}

void MyList::print() const {
	string msg;
	if (empty()) {
		msg = "List is empty";
	} else {
		Node* node = m_startPtr;
		while (node != nullptr) {
			msg += node->getDescription() + "\n";
			node = node->getNextPtr();
		}
	}
	cout << msg << endl;
}

void MyList::insert(Node *node) {
	Node* i = m_startPtr;
	if (i == nullptr) {
		m_startPtr = node;
		return;
	}
	if (*node < *i) {
		node->setNextPtr(i);
		m_startPtr = node;
		return;
	}
	while (i->getNextPtr() != nullptr) {
		if (*node < *i->getNextPtr()) {
			node->setNextPtr(i->getNextPtr());
			i->setNextPtr(node);
			return;
		}
		i = i->getNextPtr();
	}
	i->setNextPtr(node);
	return;
}

void MyList::remove(Node node) {
	Node* i = m_startPtr;
	if (i == nullptr)
		return;
	if (*i == node) {
		m_startPtr = i->getNextPtr();
		delete i;
		return;
	}
	while (i->getNextPtr() != nullptr) {
		if (*i->getNextPtr() == node) {
			Node* temp = i->getNextPtr();
			i->setNextPtr(i->getNextPtr()->getNextPtr());
			delete temp;
			return;
		}
		i = i->getNextPtr();
	}
}

void MyList::clear() {
	if (m_startPtr == nullptr) return;
	cout << m_startPtr->recursiveDelete();
	delete m_startPtr;
	m_startPtr = nullptr;
	return;
}

const Node *MyList::getCurrentNode() {
	return m_currentPtr;
}

void MyList::advanceToNextNode() {
	if (m_currentPtr != nullptr)
		m_currentPtr = m_currentPtr->getNextPtr();
}

void MyList::resetCurrentPtr() {
	m_currentPtr = m_startPtr;
}