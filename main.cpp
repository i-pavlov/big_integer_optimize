//
// Created by Иван Павлов on 31.05.15.
//

#include <iostream>
#include "big_integer.h"
#include "so_vector.h"
#include <memory>
#include <vector>

using namespace std;


int main() {
    big_integer x = big_integer("1000000000000000000");
    big_integer y = x;
    big_integer z = x + 5;
    cout << x.elements.vector.elements->use_count() << endl;
    int xx = 3;
    int yx = 3;
    cout << x << endl;
    cout << y << endl;
    return 0;
}