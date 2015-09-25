/*
 * OpenCV Adapter for Lepton Data Sender
 *
 * John Brewer 9/22/15
 * Copyright 2015 Jera Design LLC
 * All Rights Reseverd
 */

#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "Lepton_Frame.h"

using namespace cv;

Mat convertData(uint8_t *recvFrame) {
    Mat temp(SCAN_LINES, SCAN_COLUMNS, CV_16UC1, recvFrame + 4, LINE_SIZE);
    Mat img = temp.clone();
    MatIterator_<uint16_t> it, end;
    for( it = img.begin<uint16_t>(), end = img.end<uint16_t>(); it != end; ++it) {
        // fprintf(stderr, "old *it = %x, ", *it);
        *it = 256 * (*it % 256) + (*it / 256);
        // fprintf(stderr, "new *it = %x\n", *it);
    }
    return img;
}
