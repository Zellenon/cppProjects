//
// Created by boofsnorf on 2/18/20.
//

#ifndef HW1_RECTANGLE_H
#define HW1_RECTANGLE_H


class Rectangle {
public:
    explicit Rectangle(double _x=0., double _y=0., double _w=1., double _h=1);

    // Getters and Setters
    const double getX();
    const double getY();
    const double getW();
    const double getH();
    const double area(); // Convenience function, just returns m_w * m_h
    void setX(const double _x);
    void setY(const double _y);
    void setW(const double _w); // Cannot be <0
    void setH(const double _h); // Cannot be <0

    const double intersectArea(Rectangle& a); // Returns the area covered by both rectangles
    const double intersectArea(Rectangle& a, Rectangle& b); // Returns the area covered by all three rectangles
    const double unionArea(Rectangle& a); // Returns total area covered by all rectangles. Overlap is not counted twice.
    const double unionArea(Rectangle& a, Rectangle& b); // Returns total area covered by all rectangles. Overlap is not counted twice.

    void print(); // Simple testing function which prints all 4 member variables' value.
private:
    double m_x, m_y;
    double m_w, m_h;
};


#endif //HW1_RECTANGLE_H
