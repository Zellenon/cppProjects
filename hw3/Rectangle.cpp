//
// Created by boofsnorf on 2/18/20.
//

#include "Rectangle.h"
#include <iostream>
#include <algorithm>
#include <fstream>

using namespace std;

Rectangle::Rectangle(double _x, double _y, double _w, double _h) {
    setX(_x);
    setY(_y);
    setW(_w);
    setH(_h);
}

const double Rectangle::getX() {
    return m_x;
}

const double Rectangle::getY() {
    return m_y;
}

const double Rectangle::getW() {
    return m_w;
}

const double Rectangle::getH() {
    return m_h;
}

void Rectangle::setX(const double _x) {
    m_x = _x;
}

void Rectangle::setY(const double _y) {
    m_y = _y;
}

void Rectangle::setW(const double _w) {
    if (_w >= 0) {
        m_w = _w;
    } else {
        m_w = 1;
    }
}

void Rectangle::setH(const double _h) {
    if (_h>=0) {
        m_h = _h;
    } else {
        m_h = 1;
    }
}

void Rectangle::print() {
    cout << "X Position: " << getX() << endl;
    cout << "Y Position: " << getY() << endl;
    cout << "Width: " << getW() << endl;
    cout << "Height: " << getH() << endl;
}

const double Rectangle::area() {
    return m_w*m_h;
}

const double Rectangle::intersectArea(Rectangle &a) {
    // Both 'width' and 'height' in this case refer to the width and height of the overlapping area
    double width = min(getX()+getW(),a.getX()+a.getW()) - max(getX(),a.getX());
    double height = min(getY()+getH(),a.getY()+a.getH()) - max(getY(),a.getY());
    return width * height;
}

const double Rectangle::intersectArea(Rectangle &a, Rectangle &b) {
    // Both 'width' and 'height' in this case refer to the width and height of the overlapping area
    double width = min(min(getX()+getW(),a.getX()+a.getW()),b.getX()+b.getW()) - max(max(getX(),a.getX()),b.getX());
    double height = min(min(getY()+getH(),a.getY()+a.getH()),b.getY()+b.getH()) - max(max(getY(),a.getY()),b.getY());
    return width * height;
}

const double Rectangle::unionArea(Rectangle &a) {
    return area()+a.area() - intersectArea(a);
}

const double Rectangle::unionArea(Rectangle &a, Rectangle &b) {
    return area()+a.area()+b.area()+intersectArea(a,b)-intersectArea(a)-intersectArea(b)-a.intersectArea(b);
}
