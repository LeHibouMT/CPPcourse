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

// + functions

Vector& Vector::operator+=(const Vector& rhs) {
    if (rhs.sizet != this->sizet) {
        throw runtime_error("Incompatible size");
    }

    for (int i = 0; i < this->sizet; i++) {
        this->value_[i] = this->value_[i] + rhs.value_[i];
    }
    return *this;
};

Vector& Vector::operator-=(const Vector& rhs) {
    if (rhs.sizet != this->sizet) {
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

