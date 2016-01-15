//
//  imageprocess.hpp
//  wh61Planning
//
//  Created by Wu Hong on 1/14/16.
//  Copyright © 2016 Wu Hong. All rights reserved.
//

#ifndef imageprocess_hpp
#define imageprocess_hpp

#include <stdio.h>
#include <vector>
#include <cv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;


class ImageProcess {
public:
    vector<vector<int> > processImageWrapper(const string& dir);
    void showImage(const vector<vector<int> >&);
    void setMinDistance(int dist) {
        min_dist = dist;
    }
private:
    vector<vector<int> > processImage(const cv::Mat& input_image);
    vector<vector<int> > getBGR(const cv::Mat& input_image, char type);
    void imageDenoise(vector<vector<int> > &vec);
    int min_dist;
    cv::Vec3b target;
    cv::Mat result;
};



#endif /* imageprocess_hpp */
