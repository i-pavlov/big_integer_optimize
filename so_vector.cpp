//
//  vector.cpp
//  big_integer
//
//  Created by Иван Павлов on 15.06.15.
//  Copyright (c) 2015 Иван Павлов. All rights reserved.
//

#include "so_vector.h"
#include <vector>
#include <cassert>
#include <iostream>

using namespace std;

so_vector::so_vector() {
    length = 0;
}
so_vector::so_vector(so_vector const& other) {
    
    if(other.length == 0) {
        vector.value = 0;
    }
        
    if(other.length == 1) {
        vector.value = other.vector.value;
    }
    if(other.length > 1) {
        vector.elements = new shared_ptr<std::vector<uint32_t> > (new std::vector<uint32_t> ());
        *vector.elements = *other.vector.elements;
    }
    length = other.length;
}

void so_vector::pop_back() {
    assert(length > 0);
    if(length > 1) {
        if(!vector.elements->unique())
            vector.elements->reset(new std::vector<uint32_t>(**vector.elements));
        (*vector.elements)->pop_back();
    }
    if(length == 2) {
        uint32_t last = (*vector.elements)->at(0);
        delete vector.elements;
        vector.value = last;
    }
    length--;
}

uint32_t so_vector::back() {
    assert(length > 0);
    if(length == 1)
        return vector.value;
    else
        return (*vector.elements)->back();
}

void so_vector::push_back(uint32_t push_value) {
    if(length == 0) {
        vector.value = push_value;
        length = 1;
        return;
    }
    if(length == 1) {
        uint32_t last = vector.value;
        vector.elements = new shared_ptr<std::vector<uint32_t> >(new std::vector<uint32_t> ());
        (*vector.elements)->push_back(last);
    }
    if(!vector.elements->unique())
        vector.elements->reset(new std::vector<uint32_t>(**vector.elements));
    (*vector.elements)->push_back(push_value);
    length++;
}

size_t so_vector::size() const {
    return length;
}

uint32_t &so_vector::operator [] (size_t position) {
    assert(position < length);
    if(length == 1)
        return vector.value;
    else {
        if(!vector.elements->unique())
            vector.elements->reset(new std::vector<uint32_t>(**vector.elements));
        return (*vector.elements)->at(position);
    }
}

uint32_t so_vector::operator [] (size_t position) const {
    assert(position < length);
    if(length == 1)
        return vector.value;
    else
        return (*vector.elements)->at(position);
}

void so_vector::resize(size_t size) {
    resize(size, 0);
}

void so_vector::resize(size_t size, const uint32_t &val) {
    if(size == 0)
        if(length > 1)
            delete vector.elements;
    if(size == 1) {
        if(length == 0)
            vector.value = val;
        if(length > 1) {
            uint32_t last = (*vector.elements)->at(0);
            delete vector.elements;
            vector.value = last;
        }
    }
    if(size > 1) {
        uint32_t last = 0;
        if(length == 1)
        {
            last = vector.value;
            vector.elements = new shared_ptr<std::vector<uint32_t> > (new std::vector<uint32_t> ());
            (*vector.elements)->push_back(last);
        }
        if(!vector.elements->unique())
            vector.elements->reset(new std::vector<uint32_t>(**vector.elements));
        (*vector.elements)->resize(size, val);
    }
    length = size;
}

void so_vector::clear() {
    if(length > 1) {
        if(!vector.elements->unique())
            vector.elements->reset(new std::vector<uint32_t>(**vector.elements));
        delete vector.elements;
    }
    length = 0;
}
so_vector& so_vector::operator = (so_vector const& other) {
    if(length > 1 && !vector.elements->unique())
            vector.elements->reset(new std::vector<uint32_t>(**vector.elements));
    if(other.length == 0)
        clear();
    if(other.length == 1) {
        if(length >= 1)
            delete vector.elements;
        vector.value = other.vector.value;
    }
    if(other.length > 1) {
        if(length <= 1)
            vector.elements = new shared_ptr<std::vector<uint32_t> > (new std::vector<uint32_t> ());
        *vector.elements = *other.vector.elements;
    }
    length = other.length;
    return *this;
}

so_vector& so_vector::operator = (std::vector<uint32_t> const& other) {
    if(length > 1 && !vector.elements->unique())
        vector.elements->reset(new std::vector<uint32_t>(**vector.elements));
    if(other.size())
        clear();
    if(other.size() == 1) {
        if(length >= 1)
            delete vector.elements;
        vector.value = other.at(0);
    }
    if(other.size() > 1) {
        if(length <= 1)
            vector.elements = new shared_ptr<std::vector<uint32_t> > (new std::vector<uint32_t> ());
        *(*vector.elements) = other;
    }
    length = other.size();
    return *this;
}
