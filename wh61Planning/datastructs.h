//
//  datastructs.h
//  wh61Planning
//
//  Created by Wu Hong on 1/15/16.
//  Copyright © 2016 Wu Hong. All rights reserved.
//

#ifndef datastructs_h
#define datastructs_h

struct Point {
    Point(int x_ = 0, int y_ = 0) : x(x_), y(y_) {}
    int x, y;
    bool operator<(const Point& rhs) const {
        if (x == rhs.x) return x < rhs.x;
        return x < rhs.x;
    }
};



 

struct Vector {
    double x, y;
    Vector(double x_ = 0, double y_ = 0) : x(x_), y(y_) {}
};


#endif /* datastructs_h */
