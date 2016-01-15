//
//  main.cpp
//  wh61Planning
//
//  Created by Wu Hong on 1/14/16.
//  Copyright © 2016 Wu Hong. All rights reserved.
//

#include <iostream>
#include "imageprocess.hpp"
#include "mapbuilder.hpp"
#include "datastructs.h"
#include <vector>
#include <cstdio>


int main() {
    // insert code here...
    string dir = "road.jpg";
    ImageProcess img;
    ImageProcess *pimg = &img;
    vector<vector<int> > result = pimg->processImageWrapper(dir);
#ifdef DEBUG
    size_t r = result.size(), c = 0;
    if (r > 0) c = result[0].size();
    int cnt = 0;
    for (int i = 0;  i < r; ++i) {
        for (int j = 0; j < c; ++j) {
            if (result[i][j] == -1) {
                cnt++;
            }
        }
    }
    printf("r * c = %d, -1: %d\n", static_cast<int>(r * c), cnt);
    pimg->showImage(result);
#endif
    MapBuilder mb;
    MapBuilder *pmb = &mb;
    vector<vector<Vector> > v_vec;
    vector<vector<double> > s_vec;
    pmb->build(result, 8500, 8000, 10, 10.0, 1.0, s_vec, v_vec);
    
    return 0;
}
