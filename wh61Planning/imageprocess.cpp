//
//  imageprocess.cpp
//  wh61Planning
//
//  Created by Wu Hong on 1/14/16.
//  Copyright © 2016 Wu Hong. All rights reserved.
//

#include "imageprocess.hpp"
#include <vector>
#include <queue>


vector<vector<int> > ImageProcess::processImageWrapper(const string& dir) {
    cv::Mat image = cv::imread(dir.c_str());
    if (!image.data) {
        printf("No Image!");
        return vector<vector<int> >();
    }
    vector<vector<int> > result = processImage(image);
    return result;
}

void ImageProcess::showImage(const vector<vector<int> >& vec) {
    cv::Mat image;
    size_t r = vec.size();
    if (r == 0) return;
    size_t c = vec[0].size();
    image.create(static_cast<int>(r), static_cast<int>(c), CV_8UC1);
    for (int i = 0; i < r; ++i) {
        for (int j = 0; j < c; ++j) {
            int val = vec[i][j], new_val = 255;
            if (val == 1) new_val = 144;
            if (val == 2) new_val = 77;
            if (val == 3) new_val = 0;
            image.at<uchar>(i,j) = new_val;
        }
    }
    cv::namedWindow("wh61test1");
    cv::imshow("wh61test1", image);
    cv::waitKey(15000);
}





vector<vector<int> > ImageProcess::processImage(const cv::Mat &input_image) {
    vector<vector<int> > image_map;
    int r = input_image.rows, c = input_image.cols;
    image_map.assign(r, vector<int>(c, 0));
    int mass_cnt = 0;
    vector<vector<int> > vec_B = getBGR(input_image, 'B');
    vector<vector<int> > vec_G = getBGR(input_image, 'G');
    vector<vector<int> > vec_R = getBGR(input_image, 'R');
    
    for (int x = 0; x < r; ++x) {
        for (int y = 0; y < c; ++y) {
            if (image_map[x][y] == 0) {
                image_map[x][y] = ++mass_cnt;
                static int dx[] = {0, 1, 0, -1}, dy[] = {-1, 0, 1, 0};
                int std_B = vec_B[x][y], std_G = vec_G[x][y], std_R = vec_R[x][y];
                queue<pair<int, int> > q;
                q.push(make_pair(x, y));
                int point_count = 0;
                vector<pair<int, int> > p_vec;
                while (!q.empty()) {
                    int cur_x = q.front().first, cur_y = q.front().second;
                    p_vec.push_back(q.front());
                    q.pop();
                    point_count++;
                    for (int i = 0; i < 4; ++i) {
                        int new_x = cur_x + dx[i], new_y = cur_y + dy[i];
                        if (new_x < 0 || new_x >= r || new_y < 0 || new_y >= c) {
                            continue;
                        }
                        if (image_map[new_x][new_y] != 0) continue;
                        int R = vec_R[new_x][new_y], G = vec_G[new_x][new_y], B = vec_B[new_x][new_y];
                        if (abs(R - std_R) < 10 && abs(B - std_B) < 10 && abs(G - std_G) < 10) {
                            image_map[new_x][new_y] = mass_cnt;
                            q.push(make_pair(new_x, new_y));
                        }
                    }
                }
                if (point_count > 200) {
#ifdef DEBUG
                    printf("mass No.: %d  BGR: %d %d %d  point count: %d\n", mass_cnt, std_B, std_G, std_R, point_count);
#endif
                } else {
                    for (auto pr : p_vec) {
                        int x = pr.first, y = pr.second;
                        image_map[x][y] = -1;
                    }
                    mass_cnt--;
                }
            }
        }
    }
#ifdef DEBUG
    printf("%d\n", mass_cnt);
#endif
    imageDenoise(image_map);
    return image_map;
}

vector<vector<int> > ImageProcess::getBGR(const cv::Mat& image, char type) {
    int index = 0;
    if (type == 'G') index = 1;
    if (type == 'R') index = 2;
    int r = image.rows, c = image.cols;
    vector<vector<int> > result(r, vector<int>(c, 0));
    for (int i = 0; i < r; ++i) {
        for (int j = 0; j < c; ++j) {
            result[i][j] = image.at<cv::Vec3b>(i, j)[index];
        }
    }
    return result;
}

void ImageProcess::imageDenoise(vector<vector<int> > &vec) {
    static int dx[] = {0, 1, 0, -1};
    static int dy[] = {1, 0, -1, 0};
    size_t r = vec.size();
    for (int i = 0; i < r; ++i) {
        size_t c = vec[i].size();
        for (int j= 0; j < c; ++j) {
            bool ok = false;
            for (int k = 0; k < 1000; ++k) {
                for (int h = 0; h < 4; ++h) {
                    int x = i + dx[h] * k, y = j + dy[h] * k;
                    if (x < 0 || x >= r || y < 0 || y >= c) {
                        continue;
                    }
                    if (vec[x][y] > 0) {
                        vec[i][j] = vec[x][y];
                        ok = true;
                        break;
                    }
                }
                if (ok) break;
            }
        }
    }
    for (int i = 0; i < r; ++i) {
        size_t c = vec[i].size();
        for (int j = 0; j < c; ++j) {
            if (vec[i][j] == 2)
                vec[i][j] = 0;
            else if (vec[i][j] == 4)
                vec[i][j] = 2;
        }
    }
    /*for (int i = 0; i < r; ++i) {
        size_t c = vec[i].size();
        for (int j = 0; j < c; ++j) {
            int cnt1 = 0, cnt2 = 0;
            for (int x = i - 20; x <= i + 20; ++x) {
                for (int y = j - 20; y <= j + 20; ++y) {
                    if (x < 0 || x >= r || y < 0 || y >= c) {
                        continue;
                    }
                    if (vec[x][y] == vec[i][j]) {
                        cnt1++;
                    } else {
                        cnt2++;
                    }
                }
            }
            double percent = static_cast<double>(cnt1) / (cnt1 + cnt2);
            if (percent < 0.1) {
                printf("noise");
            }
        }
    }
     */
}