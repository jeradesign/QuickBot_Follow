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

const int morph_size = 3;
const float minDeltaX = 0.05;
const float minDeltaY = 0.05;

using namespace std;
using namespace cv;

bool _inited;

void findPerson(cv::Mat mat1, cv::Mat ycbcr) {
  cv::Mat equalized;
    
  double min, max;
    
  MatIterator_<uint16_t> it, end;
  for( it = mat1.begin<uint16_t>(), end = mat1.end<uint16_t>(); it != end; ++it) {
    if (*it < 13500) {
      *it = 13500;
    }
  }
    
  cv::minMaxLoc(mat1, &min, &max);
  fprintf(stderr, "min: %f max: %f", min, max);
  normalize(mat1, equalized, 0, 16384, cv::NORM_MINMAX);
  equalized.convertTo(equalized, CV_8UC1, 0.015625);
    
  Mat ycrcb(ycbcr.rows, ycbcr.cols, CV_8UC3);
    
  int from_to[] = { 0,0, 1,2, 2,1 };
  cv::mixChannels(&ycbcr, 1, &ycrcb, 1, from_to, 3);
  Mat rgbMat;
  cvtColor(ycrcb, rgbMat, CV_YCrCb2RGB);
    
  Mat mask;
  resize(equalized, mask, rgbMat.size());
    
  threshold(mask, mask, 32, 255, THRESH_BINARY);
    
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
  fprintf(stderr, "Biggest area %d", biggestArea);
  if (biggestArea > 0) {
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
        
    fprintf(stderr, "%f, %f", deltaX, deltaY);
    // [_galileo panBy:deltaY * 180];
    // [_galileo tiltBy:deltaX * 180];
  } else {
    // [_galileo panBy:0];
    // [_galileo tiltBy:0];
  }
    
  // [self matReady:rgbMat];
}
