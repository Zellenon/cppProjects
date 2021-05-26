/**
 * hw7 implementation
 * @file main.cpp
 * @author Mitchell Hall and Gary E. Christensen
 */

#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include "MyList.h"

using namespace std;

/**
 * Unit tests to be made by students
 * @return True if all unit tests pass, false if a uni tests fails
 */
bool unitTests() {
    //////////////////////////////////////////////////////////////////////////////////////////////
    /// STUDENT - Add your unit tests here and return true or false if unit tests run properly ///
    //////////////////////////////////////////////////////////////////////////////////////////////


    return true;
}

int main() {

    // Change unitTesting to false to enable graphics
    bool unitTesting = false;

    if (unitTesting) {
    	// Setup
		MyList myList;
		string tmpString;
		float longitude, latitude;
		int month, day, year;
		Node *tmpNode;
		const char* testAnswers[2] = {"false","true"};

		// Testing Node Functions
		cout << "Node Comparators Testing:" << endl;
		Node* test1 = new Node(0,0,1,1,1);
		Node* test2 = new Node(0,0,2,2,2);
		cout << test1->getDescription() << " < " << test2->getDescription() << ": " << testAnswers[*test1<*test2] << endl;

		test1->reconstruct(0,0,1,1,1);
		test2->reconstruct(0,0,2,0,2);
		cout << test1->getDescription() << " < " << test2->getDescription() << ": " << testAnswers[*test1<*test2] << endl;

		test1->reconstruct(0,0,1,1,3);
		test2->reconstruct(0,0,2,0,2);
		cout << test1->getDescription() << " < " << test2->getDescription() << ": " << testAnswers[*test1<*test2] << endl;

		test1->reconstruct(0,0,2,2,2);
		test2->reconstruct(0,0,2,3,2);
		cout << test1->getDescription() << " < " << test2->getDescription() << ": " << testAnswers[*test1<*test2] << endl;

		// Testing List Functions
		cout << "Testing Default List:" << endl;
		myList.print();
		cout << "Adding Node with all defaults" << endl;
		myList.insert(new Node);
		cout << "List after adding default Node:" << endl;
		myList.print();
		cout << "List after adding 2 more non-default nodes:" << endl;
		myList.insert(new Node(0,0,3,3,3));
		myList.insert(new Node(0,0,4,4,4));
		myList.print();
		cout << "List after adding a node in the middle:" << endl;
		myList.insert(new Node(0,0,2,2,2));
		myList.insert(new Node(0,0,2,3,2));
		myList.insert(new Node(0,0,2,2,3));
		myList.print();
		cout << "First node in the list:" << endl;
		myList.resetCurrentPtr();
		cout << myList.getCurrentNode()->getDescription() << endl;
		cout << "Removing 2/2/3" << endl;
		myList.remove(Node(0,0,2,2,3));
		cout << "List after removal:" << endl;
		myList.print();


		cout << "Testing List Clearing:" << endl;
		myList.clear();
		cout << "List after clearing:" << endl;
		myList.print();

		// Loading the test data into the list
		cout << "Loading Test Data" << endl;
    	ifstream fin("data/confirmed_cases.txt");
		if (fin.fail()) {
			cout << "Error: could not open data/confirmed_cases.txt" << endl;
			return EXIT_FAILURE;
		}
		getline(fin, tmpString);        // Get rid of first line
		while (fin.good()) {
			fin >> latitude >> longitude >> month >> day >> year;
			tmpNode = new Node(latitude, longitude, day, month, year);
			myList.insert(tmpNode);
		}
		fin.close();
		unitTests();
		cout << "Uncomment the next line to print Test Data" << endl;
		//myList.print();

		cout << "Turn unit testing off to enable graphics." << endl;

    } else {

        // State enumeration
        enum SimulationState {
            Start, Running, Stop, Done
        };
        SimulationState state = Start;

        // Textures and sprites
        sf::Texture mapTexture;
        if (!mapTexture.loadFromFile("images/map.jpg")) { return EXIT_FAILURE; }
        sf::Sprite mapSprite(mapTexture);

        // Font and text
        sf::Font font;
        if (!font.loadFromFile("fonts/ObliviousFont.ttf")) { return EXIT_FAILURE; }
        sf::Text dateText("", font, 28), infoText("", font, 28);
        infoText.setPosition(5, 500);
        dateText.setPosition(5, 0);
        infoText.setFillColor(sf::Color::Black);
        dateText.setFillColor(sf::Color::Black);

        // Variable declarations
        sf::CircleShape circle(2, 10);
        circle.setOrigin(2, 2);
        circle.setFillColor(sf::Color(0, 0, 255, 100));

        const Node * curNode;
        const Node * dateNode;
        MyList myList;
        string tmpString;
        float longitude, latitude;
        int month, day, year;
        Node *tmpNode;

        // Load Cases
        ifstream fin("data/confirmed_cases.txt");
        if (fin.fail()) {
            cout << "Error: could not open data/confirmed_cases.txt" << endl;
            return EXIT_FAILURE;
        }
        getline(fin, tmpString);        // Get rid of first line
        while (fin.good()) {
            fin >> latitude >> longitude >> month >> day >> year;
            tmpNode = new Node(latitude, longitude, day, month, year);
            myList.insert(tmpNode);
        }
        fin.close();

        // Create window and eventr
        sf::RenderWindow app(sf::VideoMode(1080, 540), "COVID-19");
        app.setFramerateLimit(10);
        sf::Event e{};

        // Window icon
        sf::Image icon;
        if (!icon.loadFromFile("images/icon.png")) { return EXIT_FAILURE; }
        app.setIcon(64, 64, icon.getPixelsPtr());

        // Main application loop
        while (app.isOpen()) {

            // Event handler
            while (app.pollEvent(e)) {
                switch (e.type) {
                    case sf::Event::Closed:
                        app.close();
                        return 0;

                    case sf::Event::KeyPressed:
                        if (e.key.code == sf::Keyboard::Space) {
                            if (state == Running) {
                                state = Stop;
                                infoText.setString("Pause - Press 'SPACE' to Resume");
                                app.setTitle("COVID-19  |  Paused");
                            } else if (state == Stop) {
                                state = Running;
                                infoText.setString("Play - Press 'SPACE' to Pause");
                                app.setTitle("COVID-19  |  Running");
                            }
                        }
                        if (state == Done && e.key.code == sf::Keyboard::R) {
                            state = Start;
                        }
                        break;

                    case sf::Event::LostFocus:
                        if (state == Running) {
                            state = Stop;
                            infoText.setString("Pause - Press 'SPACE' to Resume");
                            app.setTitle("COVID-19  |  Paused");
                        }
                        break;

                    default:
                        break;
                }
            }

            // State Machine
            switch (state) {
                case Start:
                    myList.resetCurrentPtr();
                    curNode = myList.getCurrentNode(); // set the dateNode to the first day in the file
                    dateNode = curNode;
                    infoText.setString("Play - Press 'SPACE' to Pause");
                    app.setTitle("COVID-19  |  Playing");
                    state = Running;
                    break;

                case Running:
                    app.clear();
                    app.draw(mapSprite);

                    myList.resetCurrentPtr(); // reset currentPtr to start of list
                    curNode = myList.getCurrentNode();
                    while (curNode != nullptr && *curNode <= *dateNode) {        // Draw all cases
                        circle.setPosition(float(curNode->getLongitude() + 180) * 3.0f,
                                           float(540 - ((curNode->getLatitude() + 90) * 3.0f)));
                        app.draw(circle);
                        myList.advanceToNextNode();
                        curNode = myList.getCurrentNode();
                    }

                    if (curNode == nullptr) {        // If end of list
                        state = Done;
                        infoText.setString("Done - Press 'R' to Restart");
                        app.setTitle("COVID-19  |  Done");
                    } else {      // If not end of list
                        dateText.setString(to_string(curNode->getMonth()) + "/" +
                                           to_string(curNode->getDay()) + "/" +
                                           to_string(curNode->getYear()));
                        myList.advanceToNextNode();
                        dateNode = curNode;
                        myList.resetCurrentPtr(); // reset current pointer to start of list
                    }
                    app.draw(dateText);
                    app.draw(infoText);
                    app.display();
                    break;

                case Stop:
                    app.clear();
                    app.draw(mapSprite);

                    myList.resetCurrentPtr();
                    curNode = myList.getCurrentNode();
                    while (curNode != nullptr && *curNode <= *dateNode) {        // Draw all cases
                        circle.setPosition(float(curNode->getLongitude() + 180) * 3.0f,
                                           float(540 - ((curNode->getLatitude() + 90) * 3.0f)));
                        app.draw(circle);
                        myList.advanceToNextNode();
                        curNode = myList.getCurrentNode();
                    }
                    app.draw(dateText);
                    app.draw(infoText);
                    app.display();
                    break;

                default:
                    break;
            }
        }

    }

    return 0;
}
