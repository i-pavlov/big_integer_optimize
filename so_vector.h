//
//  vector.h
//  big_integer
//
//  Created by Иван Павлов on 15.06.15.
//  Copyright (c) 2015 Иван Павлов. All rights reserved.
//

#ifndef __big_integer__vector__
#define __big_integer__vector__

#include <stdio.h>
#include <vector>
#include <memory>

using namespace std;
union data {
    uint32_t value;
    shared_ptr<vector<uint32_t> >* elements;
};

struct so_vector {
    so_vector();
    so_vector(so_vector const& other);
    void push_back(uint32_t value);
    void pop_back();
    void resize(size_t size);
    void resize(size_t size, const uint32_t &val);
    void clear();
    size_t size() const;
    uint32_t back();
    uint32_t &operator [] (size_t pos);
    uint32_t operator [] (size_t pos) const;
    so_vector& operator = (so_vector const& other);
    so_vector& operator = (std::vector<uint32_t> const& other);

    size_t length;
    data vector;
};

#endif /* defined(__big_integer__vector__) */
