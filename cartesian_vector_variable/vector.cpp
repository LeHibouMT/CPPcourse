#include <string>
#include "vector.hh"
#include "config.h"
#include <ostream>
#include <initializer_list>

using namespace std;
//template <class T> // We can use this to avoid redifine data types in function

Vector::Vector(const Vector& vector) {
    this->sizet = vector.sizet;
    this->value_ = std::make_unique<value[]>(this->sizet);
    for (int i = 0; i < this->sizet; i++) {
        this->value_[i] = vector.value_[i];
    }
}

Vector& Vector::operator=(const Vector& vector) {
    this->sizet = vector.sizet;
    this->value_ = make_unique<value[]>(this->sizet);
    for (int i = 0; i < this->sizet; i++) {
        this->value_[i] = vector.value_[i];
    }
    return *this;
}

// constructors 

Vector::Vector(size_t size) {
    this->sizet = size;
    this->value_ = make_unique<value[]>(this->sizet);
    for (int i = 0; i < this->sizet; i++) {
        this->value_[i] = 0;
    }
}

Vector::Vector(initializer_list<value> list) {
    const value* v = data(list);
    this->sizet = list.size();
    this->value_ = make_unique<value[]>(this->sizet);
    for (int i = 0; i < this->sizet; i++) {
        this->value_[i] = v[i];
    }
}

size_t Vector::size() const { // return size
    return this->sizet;
}

// public functions

Vector& Vector::operator+=(const Vector& rhs) {
    if (this->sizet != rhs.sizet) {
        throw runtime_error("Incompatible size");
    }

    for (int i = 0; i < this->sizet; i++) {
        this->value_[i] = this->value_[i] + rhs.value_[i];
    }
    return *this;
};

Vector& Vector::operator-=(const Vector& rhs) {
    if (this->sizet != rhs.sizet) {
        throw runtime_error("Incompatible size");
    }

    for (int i = 0; i < this->sizet; i++) {
        this->value_[i] = this->value_[i] - rhs.value_[i];
    }
    return *this;
}

Vector& Vector::operator+=(value v) {
    for (int i = 0; i < this->sizet; i++) {
        this->value_[i] = this->value_[i] + v;
    }
    return *this;
}

Vector& Vector::operator*=(value v) {
    for (int i = 0; i < this->sizet; i++) {
        this->value_[i] = this->value_[i] * v;
    }
    return *this;
}

Vector Vector::operator+(const Vector& rhs) const {
    if (this->sizet != rhs.sizet) {
        throw std::runtime_error("Incompatible size");
    }

    Vector v = Vector(this->sizet);
    for (int i = 0; i < this->sizet; i++) {
        v.value_[i] = this->value_[i] + rhs.value_[i];
    }
    return v;
}

Vector Vector::operator+(value v) const {
    Vector vector = Vector(this->sizet);
    for (int i = 0; i < this->sizet; i++) {
        vector.value_[i] = this->value_[i] + v;
    }
    return vector;
}

value Vector::operator*(const Vector& rhs) const {
    value v{0};
    for (int i = 0; i < this->sizet; i++) {
        v += (this->value_[i] * rhs.value_[i]);
    }
    return v;
}

Vector Vector::operator*(value v) const {
    Vector vector = Vector(this->sizet);
    for (int i = 0; i < this->sizet; i++) {
        vector.value_[i] = this->value_[i] * v;
    }
    return vector;
}

value& Vector::operator[](size_t idx) {
    return this->value_[idx];
}

value Vector::operator[](size_t idx) const {
    return this->value_[idx];
}

// Nonmember functions

Vector operator*(const value& s, const Vector& v) {
    Vector vector = v;
    vector *= s;
    return vector;
}

Vector operator+(const value& s, const Vector& v) {
    Vector vector = v;
    vector += s;
    return vector;
}

std::ostream& operator<<(std::ostream& ostream, const Vector& v) {
    ostream << "{";
    for (int i = 0; i < v.size() - 1; i++) {
        ostream << v[i] << ",";
    }

    ostream << v[v.size() - 1] << "}";
    return ostream;
}