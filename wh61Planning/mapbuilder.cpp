//
//  mapbuilder.cpp
//  wh61Planning
//
//  Created by Wu Hong on 1/15/16.
//  Copyright © 2016 Wu Hong. All rights reserved.
//

#include "mapbuilder.hpp"
#include <cassert>
#include <array>
#include <cmath>
#include <algorithm>

const double MapBuilder::inf = 1000000000;

void MapBuilder::build(const vector<vector<int> >& raw_image, int height, int width, int sz,
                       double max_val, double min_val,
                       vector<vector<double> >& s_vec, vector<vector<Vector> >& v_vec) {
    vector<vector<int> > raw_map = rawImage2RawMap(raw_image, height, width, sz);
    vector<Point> edge_point = getEdgePoints(raw_map);
    vector<vector<pair<Point, Point> > > two_point_map = find2endPoint(raw_map, edge_point);
    getSVec(two_point_map, s_vec, max_val, min_val);
    getVVec(two_point_map, v_vec);
}

vector<vector<int> > MapBuilder::rawImage2RawMap(const vector<vector<int> >& raw_image, int height, int width, int sz) {
    printf("running rawImage2RawMap\n");
    assert(height % sz == 0);
    assert(width % sz == 0);
    int raw_image_r = static_cast<int>(raw_image.size());
    assert(raw_image_r != 0);
    int raw_image_c = static_cast<int>(raw_image[0].size());
    int r = height / sz, c = width / sz;
    vector<vector<int> > result;
    result.assign(r, vector<int>(c, 0));
    for (int i = 0; i < r; ++i) {
        for (int j = 0; j < c; ++j) {
            int i_ = i * raw_image_r / r;
            int j_ = j * raw_image_c / c;
            result[i][j] = raw_image[i_][j_];
        }
    }
    return result;
}

vector<Point> MapBuilder::getEdgePoints(const vector<vector<int> >& raw_map) {
    printf("running getEdgePoints: ");
    size_t r = raw_map.size();
    size_t c = raw_map[0].size();
    int K = 3;
    vector<Point> vec;
    for (int i = 0; i < r; ++i) {
        for (int j = 0; j < c; ++j) {
            if (raw_map[i][j] == 0) continue;
            bool flag = false;
            for (int x = i - K; x <= i + K; ++x) {
                for (int y = j - K; y <= j + K; ++y) {
                    if (x < 0 || x >= r || y < 0 || y >= c) {
                        continue;
                    } else {
                        if (raw_map[x][y] != raw_map[i][j]) flag = true;
                    }
                }
            }
            if (flag) vec.push_back(Point(i, j));
        }
    }
    int point_cnt = static_cast<int>(vec.size());
    printf("%d\n", point_cnt);
    sort(vec.begin(), vec.end());
    return vec;
}

vector<vector<pair<Point, Point> > > MapBuilder::find2endPoint(const vector<vector<int> >& raw_map
                                                               , const vector<Point>& edge) {
    printf("Begin to run find2endPoint\n");
    int r = static_cast<int>(raw_map.size());
    assert(r != 0);
    int c = static_cast<int>(raw_map[0].size());
    vector<vector<pair<Point, Point> > > result(r, vector<pair<Point, Point> >(c));
    for (int i = 0; i < r; ++i) {
        for (int j = 0; j < c; ++j) {
            if (raw_map[i][j] != 0) continue;
            std::array<double, 4> num = {inf, inf, inf, inf};
            std::array<Point, 4> p;
            
            /*
            
            int sz = static_cast<int>(edge.size());
            for (int k = 0; k < sz; ++k) {
                int x = edge[k].x, y = edge[k].y;
                int val = raw_map[x][y];
                double dist = hypot(i - x, j - y);
                if (dist < num[val]) {
                    num[val] = dist;
                    p[val] = Point(x, y);
                }
            }
            */
            
            int low_index = max(0, i - r / 10), up_index = min(i + r / 10, r - 1);
            vector<Point>::const_iterator iter1 = lower_bound(edge.begin(), edge.end(), Point(low_index, 0)) , iter2 = upper_bound(edge.begin(), edge.end(), Point(up_index, 0));
            for (auto iter = iter1; iter != iter2; ++iter) {
                int x = iter->x, y = iter->y;
                int val = raw_map[x][y];
                double dist = hypot(i - x, j - y);
                if (dist < num[val]) {
                    num[val] = dist;
                    p[val] = Point(x, y);
                }

            }
            
            double min1 = inf, min2 = inf, index1 = 0, index2 = 0;
            for (int k = 1; k < 4; ++k) {
                if (num[k] < min1) {
                    min2 = min1;
                    index2 = index1;
                    min1 = num[k];
                    index1 = k;
                } else if (num[k] < min2) {
                    min2 = num[k];
                    index2 = k;
                }
            }
            if (index1 > index2) swap(index1, index2);
            result[i][j] = make_pair(p[index1], p[index2]);
        }
    }
    printf("finish running find2endpoint\n");
    return result;
}

void MapBuilder::getSVec(const vector<vector<pair<Point, Point> > >& two_point_map, vector<vector<double> >& s_vec, double max_val, double min_val) {
    int r = static_cast<int>(two_point_map.size());
    assert(r != 0);
    int c = static_cast<int>(two_point_map[0].size());
    s_vec.assign(r, vector<double>(c, 0));
    int error_cnt = 0, total_cnt = 0;
    for (int i = 0; i < r; ++i) {
        for (int j = 0; j < c; ++j) {
            Point p1 = two_point_map[i][j].first, p2 = two_point_map[i][j].second;
            if (p1.x == 0 && p2.x == 0 && p1.y == 0 && p2.y == 0) {
                s_vec[i][j] = inf;
                continue;
            }
            double dist1 = hypot(i - p1.x, j - p1.y), dist2 = hypot(i - p2.x, j - p2.y);
            double dist_total = hypot(p1.x - p2.x, p1.y - p2.y);
            if (abs(dist1 + dist2 - dist_total) > 0.02 * dist_total) {
                //printf("dist1 + dist2 != dist_total\n");
                error_cnt++;
            }
            total_cnt++;
            double dist = min(dist1, dist2);
            double per = dist / ((dist1 + dist2) / 2);
            double val = max_val - (max_val - min_val) * per;
            s_vec[i][j] = val;
        }
    }
    printf("error_cnt/total_cnt: %d/%d\n", error_cnt, total_cnt);
}

void MapBuilder::getVVec(const vector<vector<pair<Point, Point> > >& two_point_map, vector<vector<Vector> >& v_vec) {
    int r = static_cast<int>(two_point_map.size());
    assert(r != 0);
    int c = static_cast<int>(two_point_map[0].size());
    v_vec.assign(r, vector<Vector>(c));
    for (int i = 0; i < r; ++i) {
        for (int j = 0; j < c; ++j) {
            Point p1 = two_point_map[i][j].first, p2 = two_point_map[i][j].second;
            if (p1.x == 0 && p2.x == 0 && p1.y == 0 && p2.y == 0) {
                continue;
            }
            double x = p2.x - p1.x, y = p2.y - p1.y;
            double new_x = -y, new_y = x, scale = hypot(new_x, new_y);
            new_x /= scale;
            new_y /= scale;
            v_vec[i][j] = Vector(new_x, new_y);
        }
    }
}