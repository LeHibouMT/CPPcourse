#include <string>
#include "vector.hh"
#include "config.h"
#include <ostream>
#include <initializer_list>

using namespace std;
//template <class T> // We can use this to avoid redifine data types in function

// constructors 

Vector::Vector() {
    for (int i = 0; i < NDIM; i++) {
        value_[i] = 0;
    }
}

Vector::Vector(initializer_list<value> list) {
    auto* p = data(list); 
    for (int i = 0; i < NDIM; i++) {
        value_[i] = p[i];
    }
}

Vector::Vector(value val) {
    for (int i = 0; i < NDIM; i++) {
        value_[i] = val;
    }
};

Vector::Vector(value* val) {
    for (int i = 0; i < NDIM; i++) {
        value_[i] = val[i];
    }
};

// public functions

value Vector::getvalue(int n) {
    return this->value_[n];
}

void Vector::changevalues(value* val) {
    for (int i = 0; i < NDIM; i++) {
        this->value_[i] = val[i];
    }
}

// + functions

Vector Vector::operator+(const value val) {
    auto v = Vector{};
    for (int i = 0; i < NDIM; i++) {
        v.value_[i] = this->value_[i] + val;
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

Vector& Vector::operator+=(const value val) {
    return *this + val;
}

Vector& Vector::operator+=(const Vector vector) {
    return *this + vector;
}

// - functions

Vector Vector::operator-(const value val) {
    return *this + (-1 * val);
}

Vector Vector::operator-(const Vector vector) {
    auto v = Vector{};
    for (int i = 0; i < NDIM; i++) {
        v.value_[i] = this->value_[i] - vector.value_[i];
    }
    return v;
}

Vector& Vector::operator-=(const value val) {
    return *this - val;
}

Vector& Vector::operator-=(const Vector vector) {
    return *this - vector;
}

// * functions

Vector Vector::operator*(const value val) {
    auto v = Vector{};
    for (int i = 0; i < NDIM; i++) {
        v.value_[i] = this->value_[i] * val;
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

Vector& Vector::operator*=(const value val) {
    return *this * val;
}

value Vector::operator*=(const Vector vector) {
    return *this * vector;
}

// [] functions

value Vector::operator[](value val) const {
    return this->value_[val];
}

value& Vector::operator[](value val) {
    return this->value_[val];
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