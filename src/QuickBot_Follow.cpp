#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <cstring>
#include <cassert>
#include <opencv2/core/core.hpp>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "Lepton_Frame.h"
#include "Lepton_Driver.h"
#include "OpenCV_Adapter.h"
#include "Person_Finder.h"

int socket_fd;
struct sockaddr_in servaddr;

static void setupConnection(char *hostname)
{
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(11539);
    servaddr.sin_addr.s_addr = inet_addr(hostname);
    connect(socket_fd, (const sockaddr*)&servaddr, sizeof(servaddr));
}

int main(int argc, char **argv)
{
	if (argc < 2) {
		return -1;
	}

	setupConnection(argv[1]);

	setupLepton();

	for (;;) {
        uint8_t *frame = nextFrame();
        if (!checkFrame()) {
            continue;
        }
        cv::Mat img = convertData(frame);
        // cv::Mat processed = img.clone();
        float deltax, deltay;
        cv::Mat processed = findPerson(img, deltax, deltay);

        if (deltay == 0.0) {
            printf("stop\n"); fflush(stdout);
        } else if (deltax == 0.0) {
            printf("straight\n"); fflush(stdout);
        } else if (deltax > 0.0) {
            printf("right\n"); fflush(stdout);
        } else {
            printf("left\n"); fflush(stdout);
        }
        
        // fprintf(stderr, "deltax = %f, deltay = %f\n", deltax, deltay);
        // cv::Mat processed(SCAN_LINES, SCAN_COLUMNS, CV_8UC3, cv::Scalar(255,0,0));
        // fprintf(stderr, "isContinuous = %d\n", processed.isContinuous());
        write(socket_fd, processed.data, SCAN_LINES * SCAN_COLUMNS * 3);
	}
}
