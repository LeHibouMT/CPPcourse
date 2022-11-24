#include <string>
#include "vector.hh"
#include "config.h"
#include <ostream>

using namespace std;
//template <class T> // We can use this to avoid redifine data types in function

// constructors 

Vector::Vector() {
    for (int i = 0; i < NDIM; i++) {
        value_[i] = 0;
    }
}

Vector::Vector(initializer_list<value> list) {
    auto* p = data(list); // In this case, we need to create a new array as we can't change the size of an array
    for (int i = 0; i < NDIM; i++) {
        value_[i] = p[i];
    }
}

Vector::Vector(value value) {
    for (int i = 0; i < NDIM; i++) {
        value_[i] = value;
    }
};

Vector::Vector(value* value) {
    for (int i = 0; i < NDIM; i++) {
        value_[i] = value[i];
    }
};

// public functions

value Vector::getvalue(int n) {
    return this->value_[n];
}

void Vector::changevalues(value* value) {
    for (int i = 0; i < NDIM; i++) {
        this->value_[i] = value[i];
    }
}

// + functions

Vector Vector::operator+(const value value) {
    auto v = Vector{};
    for (int i = 0; i < NDIM; i++) {
        v.value_[i] = this->value_[i] + value;
    }
    return v;
};

Vector Vector::operator+(const Vector vector) {
    auto v = Vector{};
    for (int i = 0; i < NDIM; i++) {
        v.value_[i] = this->value_[i] + vector.value_[i];
    }
    return v;
};

Vector& Vector::operator+=(const value value) {
    return *this + value;
}

Vector& Vector::operator+=(const Vector vector) {
    return *this + vector;
}

// - functions

Vector Vector::operator-(const value value) {
    return *this + (-1 * value);
}

Vector Vector::operator-(const Vector vector) {
    auto v = Vector{};
    for (int i = 0; i < NDIM; i++) {
        v.value_[i] = this->value_[i] - vector.value_[i];
    }
    return v;
}

Vector& Vector::operator-=(const value value) {
    return *this - value;
}

Vector& Vector::operator-=(const Vector vector) {
    return *this - vector;
}

// * functions

Vector Vector::operator*(const value value) {
    auto v = Vector{};
    for (int i = 0; i < NDIM; i++) {
        v.value_[i] = this->value_[i] * value;
    }
    return v;
}

value Vector::operator*(const Vector vector) {
    value v{0};
    for (int i = 0; i < NDIM; i++) {
        v += (this->value_[i] * vector.value_[i]);
    }
    return v;

}

Vector& Vector::operator*=(const value value) {
    return *this * value;
}

value Vector::operator*=(const Vector vector) {
    return *this * vector;
}

// [] functions

value Vector::operator[](value value) const {
    return this->value_[value];
}

value& Vector::operator[](value value) {
    return this->value_[value];
}

// Nonmember functions

std::ostream& operator<<(std::ostream& ostream, Vector vector) {
    ostream << '{';
    ostream << vector[0];
    for (int i = 1; i < NDIM; i++) {
        ostream << "," << vector[i];
    }
    ostream << '}';
    return ostream;
}