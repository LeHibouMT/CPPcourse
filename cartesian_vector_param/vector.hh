#pragma once

#include <ostream>
#include <initializer_list>
#include <cassert>
#include "config.h"
#include "vector.cpp"

using namespace std;

class Vector
{
public:
    // DO NOT CHANGE THIS
    Vector(const Vector&) = default;
    Vector& operator=(const Vector&) = default;
    ~Vector() = default; // destructor
    //

    // Add suitable constructors
    Vector() = default; // default contructor, which create a null vector
    Vector(initializer_list<int> list); // parameterized constructor
    Vector(value n); // another type of parameterized constructor
    Vector(value* n); // another type of parameterized constructor
    // possibly more

// Public Member functions here
    value getvalue(int n); // getter
    void changevalues(value* value); // changer
    // More to go
    // + functions
    Vector operator+(const value value);
    Vector operator+(const Vector vector);
    Vector& operator+=(const value value);
    Vector& operator+=(const Vector vector);
    // - functions
    Vector operator-(const value value);
    Vector operator-(const Vector vector);
    Vector& operator-=(const value value);
    Vector& operator-=(const Vector vector);
    // * functions
    Vector operator*(const value value);
    value operator*(const Vector vector);
    Vector& operator*=(const value value);
    value operator*=(const Vector vector);
    // [] functions
    value operator[](value value) const;
    value& operator[](value value);

private:
    // Private Member functions here (if necessary)
    // Member variables are ALWAYS private, and they go here
    value value_[NDIM];
};

// Nonmember function operators go here
std::ostream& operator<<(std::ostream& ostream, Vector vector);