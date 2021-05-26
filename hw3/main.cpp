/*
 * Dog Class Experimentation
 * Graham Scott
 * 02.10.2020
 * Version 1.0.0
 * Implements and tests 4 instances of the Dog class, printing expected and experimental values.
 * Accepts no arguments and simply returns 0.
 */

#include <iostream>
#include <fstream>
#include <vector>
#include "Rectangle.h"

using namespace std;

int main() {
    /*
     * Constructor Testing.
     * r1 tests defaults
     * r2 is a 'normal' call, with valid valued provided for all arguments
     * r3 tests handling of negative width/height
     * r4 is another 'normal' rectangle, large enough to overlap with every other rectangle for testing of union/intersection.
     */
    Rectangle r1;
    Rectangle r2(1,0,2,2);
    Rectangle r3(0.5,1,-1,-1);
    Rectangle r4(-2,-2,4,4);

    cout << endl << "TEST 1: Constructor" << endl;
    cout << "Expected X/Y/W/H values for r1: 0/0/1/1" << endl << "Real Values:" << endl;
    r1.print();
    cout << "Expected X/Y/W/H values for r2: 1/0/2/2" << endl << "Real Values:" << endl;
    r2.print();
    cout << "Expected X/Y/W/H values for r3: 0.5/1/1/1" << endl << "Real Values:" << endl;
    r3.print();
    cout << "Expected X/Y/W/H values for r4: -2/-2/4/4" << endl << "Real Values:" << endl;
    r4.print();

    /*
     * Setter Testing
     * Tests setters by swapping the values of th rectangles
     * r1 <-> r4
     * r2 <-> r3
     */
    r4.setX(0);
    r4.setY(0);
    r4.setW(1);
    r4.setH(1);
    r3.setX(1);
    r3.setY(0);
    r3.setW(2);
    r3.setH(2);
    r2.setX(0.5);
    r2.setY(1);
    r2.setW(1);
    r2.setH(1);
    r1.setX(-2);
    r1.setY(-2);
    r1.setW(4);
    r1.setH(4);

    cout << endl << "TEST 2: Setters" << endl;
    cout << "Expected X/Y/W/H values for r1: -2/-2/4/4" << endl << "Real Values:" << endl;
    r1.print();
    cout << "Expected X/Y/W/H values for r2: 0.5/1/1/1" << endl << "Real Values:" << endl;
    r2.print();
    cout << "Expected X/Y/W/H values for r3: 1/0/2/2" << endl << "Real Values:" << endl;
    r3.print();
    cout << "Expected X/Y/W/H values for r4: 0/0/1/1" << endl << "Real Values:" << endl;
    r4.print();

    // Intersection Testing
    // Tests the intersection of each rectangle with the rectangle that comes after it

    cout << endl << "TEST 3: Intersection" << endl;
    cout << "Expected 1 for intersection of r1-r2. Got " << r1.intersectArea(r2) << endl;
    cout << "Expected 0.5 for intersection of r2-r3. Got " << r2.intersectArea(r3) << endl;
    cout << "Expected 0 for intersection of r3-r4. Got " << r3.intersectArea(r4) << endl;
    cout << "Expected 1 for intersection of r4-r1. Got " << r4.intersectArea(r1) << endl;

    cout << endl << "TEST 3.5: 3-way Intersection" << endl;
    cout << "Expected 0.5 for intersection of r1-r2-r3. Got " << r1.intersectArea(r2,r3) << endl;
    cout << "Expected 0 for intersection of r2-r3-r4. Got " << r2.intersectArea(r3,r4) << endl;
    cout << "Expected 0 for intersection of r3-r4-r1. Got " << r3.intersectArea(r4,r1) << endl;
    cout << "Expected 0 for intersection of r4-r3-r1. Got " << r4.intersectArea(r1,r2) << endl;

    cout << endl << "TEST 4: Union" << endl;
    cout << "Expected 16 for union of r1-r2. Got " << r1.unionArea(r2) << endl;
    cout << "Expected 4.5 for union of r2-r3. Got " << r2.unionArea(r3) << endl;
    cout << "Expected 5 for union of r3-r4. Got " << r3.unionArea(r4) << endl;
    cout << "Expected 16 for union of r4-r1. Got " << r4.unionArea(r1) << endl;

    cout << endl << "TEST 4.5: 3-way Union" << endl;
    cout << "Expected 18 for union of r1-r2-r3. Got " << r1.unionArea(r2,r3) << endl;
    cout << "Expected 5.5 for union of r2-r3-r4. Got " << r2.unionArea(r3,r4) << endl;
    cout << "Expected 18 for union of r3-r4-r1. Got " << r3.unionArea(r4,r1) << endl;
    cout << "Expected 16 for union of r4-r1-r2. Got " << r4.unionArea(r1,r2) << endl;
    
    /*
     * File Input Testing
     * Reads a file ('data.txt') and creates a vector of rectangles based off the numbers inside
     * It then tests the intersection and union with the 2 rectangles after it in the list
     */

    vector<Rectangle> fileRects;
    ifstream rectfile("data.txt");
    if (rectfile.is_open()) {
        vector<double> args;
        while (rectfile.good()) { // Puts every number individually in the vector "args"
            double n = 0;
            rectfile >> n;
            args.push_back(n);
        }
        rectfile.close();

        // Takes the first 4 arguments, makes a rectangle with them, then deletes them
        // Stops when there are fewer than 4 arguments
        while (args.size() >= 4) {
            fileRects.push_back(Rectangle(args.at(0),args.at(1),args.at(2),args.at(3)));
            for (int i = 0; i < 4; i++)
                args.erase(args.begin());
        }

        cout << endl << "TEST 5: Reading Rectangles from a file" << endl;
        for(Rectangle rect: fileRects) {
            rect.print();
            cout << "--------------" << endl;
        }

        for (int i = 2; i < fileRects.size(); i++) {
            cout << "Intersection of rectangles " << (i-2) << ", " << (i-1) << ", and " << i << ": "
            << fileRects.at(i-2).intersectArea(fileRects.at(i-1), fileRects.at(i)) << endl;
            cout << "Union of rectangles " << (i-2) << ", " << (i-1) << ", and " << i << ": "
                 << fileRects.at(i-2).unionArea(fileRects.at(i-1), fileRects.at(i)) << endl;
        }
    } else
        cout << "Failed to open file" << endl;

}