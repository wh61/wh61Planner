//
//  mapbuilder.hpp
//  wh61Planning
//
//  Created by Wu Hong on 1/15/16.
//  Copyright © 2016 Wu Hong. All rights reserved.
//

#ifndef mapbuilder_hpp
#define mapbuilder_hpp

#include <stdio.h>
#include <vector>
#include "datastructs.h"

using namespace std;

class MapBuilder {
public:
    void build(const vector<vector<int> >& raw_image, int height, int width, int sz,
               double max_val, double min_val,
               vector<vector<double> >& s_vec, vector<vector<Vector> >& v_vec);
private:
    vector<vector<int> > rawImage2RawMap(const vector<vector<int> >& raw_image, int heigh, int width, int sz);
    vector<Point> getEdgePoints(const vector<vector<int> >& raw_map);
    vector<vector<pair<Point, Point> > > find2endPoint(const vector<vector<int> >& raw_map
                                                       , const vector<Point>& edge);
    void getSVec(const vector<vector<pair<Point, Point> > >& two_point_map, vector<vector<double> >& s_vec, double max_val, double min_val);
    void getVVec(const vector<vector<pair<Point, Point> > >& two_point_map, vector<vector<Vector> >& v_vec);
    static const double inf;
};


#endif /* mapbuilder_hpp */
