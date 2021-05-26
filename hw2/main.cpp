/*
 * Dog Class Experimentation
 * Graham Scott
 * 02.10.2020
 * Version 1.0.0
 * Implements and tests 4 instances of the Dog class, printing expected and experimental values.
 * Accepts no arguments and simply returns 0.
 */

#include <iostream>

using namespace std;

class Dog {
public:
    void addBirthday();

    // setters
    void setMonths(int monthsToSet);
    // getters
    int getMonths();
    double getHumanYears();
    double getDogYears();
    string getStage() const;

    explicit Dog(int _months = 1);

private:
    int months;
};

// Simple setter function, prevents ages under 1 month.
void Dog::setMonths(int monthsToSet) {
    if (monthsToSet < 1)
        months = 1;
    else
        months = monthsToSet;
}

// Adds 1 year (12 months) to the age of the dog.
void Dog::addBirthday() {
    months += 12;
}

// Classifies the dog's developmental stage based off months alive. Returns a single word.
string Dog::getStage() const {
    string stage;
    if (months < 11) {
        stage = "Puppy";
    } else if (months < 25) {
        stage = "Adolescence";
    } else if (months < 80) {
        stage = "Adulthood";
    } else {
        stage = "Senior";
    }

    return stage;
}

// Standard getter, returns dog age in months
int Dog::getMonths() {
    return months;
}

// Derived getter, returns dog age in standard years (months/12)
double Dog::getHumanYears() {
    return months/12.;
}

// Derived getter, returns dog age in 'dog years', approx 7x shorter than human years
double Dog::getDogYears() {
    return months*7./12.;
}

// Default constructor, ensures age is at least one month.
Dog::Dog(int _months) {
    setMonths(_months);
}

int main() {
    /* Creates 4 dogs. One is meant to simulate a 'normal' case (Dog Classic™), one tests the default constructor (Unaged Dog),
     * one tests handling of negative numbers (Unborn Dog), and one is given 500 months to observe the behavior of
     * higher-than-normal numbers.  */
    Dog classic(9);
    Dog unaged;
    Dog unborn(-5);
    Dog old(500);

    //Checking the assigned ages
    cout << "Dog Classic instantiated with 9 months. Returned value of getMonths(): " << classic.getMonths() << endl;
    cout << "Unaged Dog instantiated without months. Returned value of getMonths(): " << unaged.getMonths() << endl;
    cout << "Unborn Dog instantiated with -5 months. Returned value of getMonths(): " << unborn.getMonths() << endl;
    cout << "Old Dog instantiated with 500 months. Returned value of getMonths(): " << old.getMonths() << endl << endl;

    // Adds 12 to the ages of all dogs.
    classic.addBirthday(), unaged.addBirthday(), unborn.addBirthday(), old.addBirthday();
    cout << "All dog ages increased by 1 year (12 months)." << endl;
    cout << "Dog Classic age increased by 1 year (9+12=21). Returned value of getMonths(): " << classic.getMonths() << endl;
    cout << "Unaged Dog age increased by 1 year (1+12=13). Returned value of getMonths(): " << unaged.getMonths() << endl;
    cout << "Unborn Dog age increased by 1 year (1+12=13). Returned value of getMonths(): " << unborn.getMonths() << endl;
    cout << "Old Dog age increased by 1 year (500+12=512). Returned value of getMonths(): " << old.getMonths() << endl << endl;

    // Testing Growth Stages
    cout << "Dog Classic expected stage: Adolescence. Returned stage: " << classic.getStage() << endl;
    cout << "Unaged Dog expected stage: Adolescence. Returned stage: " << unaged.getStage() << endl;
    cout << "Unborn Dog expected stage: Adolescence. Returned stage: " << unborn.getStage() << endl;
    cout << "Old Dog age expected stage: Senior. Returned stage: " << old.getStage() << endl << endl;

    // Converting months to years
    cout << "Dog Classic expected years: 1.75. Returned years: " << classic.getHumanYears() << endl;
    cout << "Unaged Dog expected years: 1.08333. Returned years: " << unaged.getHumanYears() << endl;
    cout << "Unborn Dog expected years: 1.08333. Returned years: " << unborn.getHumanYears() << endl;
    cout << "Old Dog age expected years: 42.6666. Returned years: " << old.getHumanYears() << endl << endl;

    // Converting months to dog years
    cout << "Dog Classic expected dog years: 12.25. Returned years: " << classic.getDogYears() << endl;
    cout << "Unaged Dog expected dog years: 7.58333. Returned years: " << unaged.getDogYears() << endl;
    cout << "Unborn Dog expected dog years: 7.58333. Returned years: " << unborn.getDogYears() << endl;
    cout << "Old Dog age expected dog years: 298.6666. Returned years: " << old.getDogYears() << endl << endl;


    return 0;
}