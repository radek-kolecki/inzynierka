#include <iostream>
#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <assert.h>
#include "a.h"
#include "featuresDemo.h"
#include "drawFlow.h"

using namespace std;
using namespace cv;

typedef vector <Point2f> myPoints;

char key;
//IplImage* srcGray;
Mat frame, frameGray, frame1, frame2, frame1Gray, frame2Gray, frameC, pyramid1, pyramid2;
myPoints corners, corners2;

int main(){

	cvNamedWindow("Camera_Output", CV_WINDOW_AUTOSIZE); //Create window
	cvNamedWindow("Camera_Output1", CV_WINDOW_AUTOSIZE); //Create window
	cvNamedWindow("Camera_Output2", CV_WINDOW_AUTOSIZE); //Create window
	
	// open the default camera
	VideoCapture cap(0); 
	if(!cap.isOpened()){
		fprintf(stderr, "Error: Can't open video.\n");
		return -1;
	}

	CvSize optical_flow_window = cvSize(3,3);
	CvTermCriteria optical_flow_termination_criteria = cvTermCriteria( CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, .3 );
	vector <uchar> status;

	/* The i-th element of this array is the error in the optical flow for the i-th
	feature
	* of frame1 as found in frame 2. If the i-th feature was not found (see the
	array above)
	* I think the i-th entry in this array is undefined.
	*/
	Mat err;

	//main loop
	while(1){

		cap >> frame1; // get a new frame from camera
		//cvtColor(frame1, frame1Gray, CV_BGR2GRAY);
		imshow("Camera_Output", frame1);
		usleep(100);

		cap >> frame2; // get a new frame from camera
		//cvtColor(frame2, frame2Gray, CV_BGR2GRAY);
		goodFeaturesToTrack_Demo( 0, 0, frame1, "Camera_Output1", corners);
		assert(corners.size() > 0);

		cvtColor(frame1, frame1Gray, CV_BGR2GRAY);
		cvtColor(frame2, frame2Gray, CV_BGR2GRAY);
		calcOpticalFlowPyrLK(frame1Gray, frame2Gray, corners,
			corners2, status, 
			err, optical_flow_window, 5, 
			optical_flow_termination_criteria, 0, 1e-4);

		frameC = frame2.clone();
		drawFlow(frameC, status, corners, corners2, "Camera_Output2");
		//imshow("Camera_Output", frame2);
		//imshow("Camera_Output1", frameC);

		key = cvWaitKey(10); //Capture Keyboard stroke
		if (char(key) == 27){
			break; //If you hit ESC key loop will break.
		}
	}

	cvDestroyWindow("Camera_Output"); //Destroy Window
	cvDestroyWindow("Camera_Output1"); //Destroy Window
	cvDestroyWindow("Camera_Output2"); //Destroy Window
	return 0;
}
