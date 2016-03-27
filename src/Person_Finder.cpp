/*
 * Person Finder for QuickBot_Follow
 * (Code based on CVFHeatTracker.mm from the HeatKey repo.)
 *
 * John Brewer 9/15/15
 * Copyright 2015 Jera Design LLC
 * All rights reserved
 */

#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

static const int morph_size = 3;
static const float minDeltaX = 0.05;
static const float minDeltaY = 0.05;

static const int min_temp = 8000;
static const int max_temp = 8500;
static const int human_temp = 8250;

static const int min_area = 100;

using namespace std;
using namespace cv;

bool _inited;

Mat findPerson(Mat mat1) {
    Mat equalized;
    
    double min, max;
    
    MatIterator_<uint16_t> it, end;
    for( it = mat1.begin<uint16_t>(), end = mat1.end<uint16_t>(); it != end; ++it) {
        if (*it < min_temp) {
            *it = min_temp;
        } else if (*it > max_temp) {
            *it = min_temp;
        }
    }
    
    double alpha = 255.0 / (max_temp - min_temp);
    double beta = alpha * -min_temp;
    
    // fprintf(stderr, "min_temp = %f, max_temp = %f\n", alpha * min_temp + beta, alpha * max_temp + beta);
    mat1.convertTo(equalized, CV_8UC1, alpha, beta);
    
    // fprintf(stderr, "equalized.rows = %d, equalized.cols = %d\n", equalized.rows, equalized.cols);
    Mat rgbMat(equalized.rows, equalized.cols, CV_8UC3);
    cvtColor(equalized, rgbMat, CV_GRAY2RGB);
    
#if 1
    Mat mask;
    
    // adaptiveThreshold(equalized, mask, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 7, 0);
    threshold(equalized, mask, alpha * human_temp + beta, 255, THRESH_BINARY + THRESH_OTSU);
    
    // cvtColor(mask, rgbMat, CV_GRAY2RGB);

    vector<vector<cv::Point> > contours;
    findContours(mask, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
    vector<cv::Point> biggestContour;
    int biggestArea = 0;
    for (const auto& contour : contours) {
        int currentArea = contourArea(contour);
        if (currentArea > biggestArea) {
            biggestArea = currentArea;
            biggestContour = contour;
        }
    }
    // fprintf(stderr, "Biggest area %d\n", biggestArea);
    if (biggestArea > min_area) {
        Moments mu = moments(biggestContour);
        int x = mu.m10/mu.m00;
        int y = mu.m01/mu.m00;
        Scalar green(0, 255, 0);
        drawContours(rgbMat, vector<vector<cv::Point> >{biggestContour}, -1, green);
        Scalar blue(0, 0, 255);
        int right = rgbMat.cols;
        int bottom = rgbMat.rows;
        cv::line(rgbMat, cv::Point(0,y), cv::Point(right,y), cv::Scalar(0, 0 , 255));
        cv::line(rgbMat, cv::Point(x,0), cv::Point(x,bottom), cv::Scalar(0, 0 , 255));
        int matX = rgbMat.cols / 2;
        int matY = rgbMat.rows / 2;
        
        float deltaX = (float)(x - matX) / rgbMat.cols;
        float deltaY = -(float)(y - matY) / rgbMat.rows;
        
        if (fabs(deltaX) < minDeltaX) {
            deltaX = 0;
        }
        if (fabs(deltaY) < minDeltaY) {
            deltaY = 0;
        }
        
        // fprintf(stderr, "%f, %f\n", deltaX, deltaY);
        // [_galileo panBy:deltaY * 180];
        // [_galileo tiltBy:deltaX * 180];
    } else {
        // [_galileo panBy:0];
        // [_galileo tiltBy:0];
    }
#endif

    return rgbMat;
}
