#include <cstdio>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>
#include "blackboard.hpp"
#include "calibrator.hpp"

using namespace std;

int thresh = 10;
IplImage* img = 0;
IplImage* img0 = 0;
CvMemStorage* storage = 0;
CvPoint calib[4];
/*
//experimental fuction
IplImage* Experiment(IplImage* img)
{
    CvSeq* contours;
    CvSeq* result;
    CvMemStorage *storage = cvCreateMemStorage(0);
    IplImage* ret = cvCreateImage(cvGetSize(img), 8, 1);
    //cvCvtColor(img, temp, CV_BGR2GRAY);
    //cvCvtColor(img, temp, CV_RGB2GRAY);
    cvFindContours(img, storage, &contours);
	if( contours ){
		cvDrawContours(
			ret,
			contours,
			cvScalarAll(255),
			cvScalarAll(255),
			100 );
	}

    cvReleaseMemStorage(&storage);

    return ret;
}
*/

double angle( CvPoint* pt1, CvPoint* pt2, CvPoint* pt0 )
{
    double dx1 = pt1->x - pt0->x;
    double dy1 = pt1->y - pt0->y;
    double dx2 = pt2->x - pt0->x;
    double dy2 = pt2->y - pt0->y;
    return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}

IplImage* DetectAndDrawQuads(IplImage* img)
{
    CvSeq* contours;
    CvSeq* result;
    CvMemStorage *storage = cvCreateMemStorage(0);
    IplImage* ret = cvCreateImage(cvGetSize(img), 8, 1);
    //IplImage* temp = cvCreateImage(cvGetSize(img), 8, 1);
    //cvCvtColor(img, temp, CV_BGR2GRAY);
    //cvCvtColor(img, temp, CV_RGB2GRAY);
    //cvThreshold(img, temp, 128, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
    //cvFindContours(img, storage, &contours);
    cvFindContours(img, storage, &contours, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0));
    /*
    while(contours)
    {
        //cvDrawContours(ret, contours, CV_RGB( rand()&255, rand()&255, rand()&255 ), CV_RGB(0,0,0), -1, CV_FILLED, 8, cvPoint(0,0));
        cvDrawContours(
			ret,
			contours,
			cvScalarAll(255),
			cvScalarAll(255),
			100 );
        *//*
        result = cvApproxPoly(contours, sizeof(CvContour), storage, CV_POLY_APPROX_DP, cvContourPerimeter(contours)*0.02, 0);
         if(result->total==4)
        {
            cout << "4" << endl;
            CvPoint *pt[4];
            for(int i=0;i<4;i++)
                pt[i] = (CvPoint*)cvGetSeqElem(result, i);

            cvLine(ret, *pt[0], *pt[1], cvScalar(255));
            cvLine(ret, *pt[1], *pt[2], cvScalar(255));
            cvLine(ret, *pt[2], *pt[3], cvScalar(255));
            cvLine(ret, *pt[3], *pt[0], cvScalar(255));

        }
    contours = contours->h_next;
    }*/
     while( contours )
            {
                double s,t;
                // approximate contour with accuracy proportional
                // to the contour perimeter
                result = cvApproxPoly( contours, sizeof(CvContour), storage,
                    CV_POLY_APPROX_DP, cvContourPerimeter(contours)*0.02, 0 );
                // square contours should have 4 vertices after approximation
                // relatively large area (to filter out noisy contours)
                // and be convex.
                // Note: absolute value of an area is used because
                // area may be positive or negative - in accordance with the
                // contour orientation
                if( result->total == 4 &&
                    //cvContourArea(result,CV_WHOLE_SEQ,0) > 1000 && //comment by laciKE because compilation error
		    cvContourArea(result,CV_WHOLE_SEQ) > 1000 &&
                    cvCheckContourConvexity(result) )
                {
                    s = 0;

                    for(int  i = 0; i < 5; i++ )
                    {
                        // find minimum angle between joint
                        // edges (maximum of cosine)
                        if( i >= 2 )
                        {
                            t = fabs(angle(
                            (CvPoint*)cvGetSeqElem( result, i ),
                            (CvPoint*)cvGetSeqElem( result, i-2 ),
                            (CvPoint*)cvGetSeqElem( result, i-1 )));
                            s = s > t ? s : t;
                        }
                    }

                    // if cosines of all angles are small
                    // (all angles are ~90 degree) then write quandrange
                    // vertices to resultant sequence
                    if( s < 0.3 )
                        {
                        CvPoint *pt[4];
                        for(int i=0;i<4;i++){
                            pt[i] = (CvPoint*)cvGetSeqElem(result, i);
                            CvPoint* Tmp = (CvPoint*)cvGetSeqElem(result, i);
			    calib[i] = *Tmp;
                            }
                        cvLine(ret, *pt[0], *pt[1], cvScalar(255));
                        cvLine(ret, *pt[1], *pt[2], cvScalar(255));
                        cvLine(ret, *pt[2], *pt[3], cvScalar(255));
                        cvLine(ret, *pt[3], *pt[0], cvScalar(255));
                        }

                }

                // take the next contour
                contours = contours->h_next;
            }

    //cvReleaseImage(&temp);
    cvReleaseMemStorage(&storage);

    return ret;
}

// fuction from examples squares.c
// returns sequence of squares detected on the image.
// the sequence is stored in the specified memory storage
CvSeq* findSquares4( IplImage* img, CvMemStorage* storage )
{
    CvSeq* contours;
    int i, c, l, N = 11;
    CvSize sz = cvSize( img->width & -2, img->height & -2 );
    IplImage* timg = cvCloneImage( img ); // make a copy of input image
    IplImage* gray = cvCreateImage( sz, 8, 1 );
    IplImage* pyr = cvCreateImage( cvSize(sz.width/2, sz.height/2), 8, 3 );
    IplImage* tgray;
    CvSeq* result;
    double s, t;
    // create empty sequence that will contain points -
    // 4 points per square (the square's vertices)
    CvSeq* squares = cvCreateSeq( 0, sizeof(CvSeq), sizeof(CvPoint), storage );

    // select the maximum ROI in the image
    // with the width and height divisible by 2
    cvSetImageROI( timg, cvRect( 0, 0, sz.width, sz.height ));

    // down-scale and upscale the image to filter out the noise
    cvPyrDown( timg, pyr, 7 );
    cvPyrUp( pyr, timg, 7 );
    tgray = cvCreateImage( sz, 8, 1 );

    // find squares in every color plane of the image
    for( c = 0; c < 3; c++ )
    {
        // extract the c-th color plane
        cvSetImageCOI( timg, c+1 );
        cvCopy( timg, tgray, 0 );

        // try several threshold levels
        for( l = 0; l < N; l++ )
        {
            // hack: use Canny instead of zero threshold level.
            // Canny helps to catch squares with gradient shading
            if( l == 0 )
            {
                // apply Canny. Take the upper threshold from slider
                // and set the lower to 0 (which forces edges merging)
                cvCanny( tgray, gray, 0, thresh, 5 );
                // dilate canny output to remove potential
                // holes between edge segments
                cvDilate( gray, gray, 0, 1 );
                //cout << "a" << endl;
            }
            else
            {
                // apply threshold if l!=0:
                //     tgray(x,y) = gray(x,y) < (l+1)*255/N ? 255 : 0
            //    cout << "b" << endl;
                cvThreshold( tgray, gray, (l+1)*255/N, 255, CV_THRESH_BINARY );
            }

            // find contours and store them all as a list
            cvFindContours( gray, storage, &contours, sizeof(CvContour),
                CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0) );

            // test each contour
            while( contours )
            {
                // approximate contour with accuracy proportional
                // to the contour perimeter
                result = cvApproxPoly( contours, sizeof(CvContour), storage,
                    CV_POLY_APPROX_DP, cvContourPerimeter(contours)*0.02, 0 );
                // square contours should have 4 vertices after approximation
                // relatively large area (to filter out noisy contours)
                // and be convex.
                // Note: absolute value of an area is used because
                // area may be positive or negative - in accordance with the
                // contour orientation
                if( result->total == 4 &&
                    //cvContourArea(result,CV_WHOLE_SEQ,0) > 100 && //comment by laciKE because compilation error
 		    cvContourArea(result,CV_WHOLE_SEQ) > 100 &&
                    cvCheckContourConvexity(result) )
                {
                    s = 0;

                    for( i = 0; i < 5; i++ )
                    {
                        // find minimum angle between joint
                        // edges (maximum of cosine)
                        if( i >= 2 )
                        {
                            t = fabs(angle(
                            (CvPoint*)cvGetSeqElem( result, i ),
                            (CvPoint*)cvGetSeqElem( result, i-2 ),
                            (CvPoint*)cvGetSeqElem( result, i-1 )));
                            s = s > t ? s : t;
                        }
                    }

                    // if cosines of all angles are small
                    // (all angles are ~90 degree) then write quandrange
                    // vertices to resultant sequence
                    if( s < 0.3 )
                        for( i = 0; i < 4; i++ )
                            cvSeqPush( squares,
                                (CvPoint*)cvGetSeqElem( result, i ));
                }

                // take the next contour
                contours = contours->h_next;
            }
        }
    }

    // release all the temporary images
    cvReleaseImage( &gray );
    cvReleaseImage( &pyr );
    cvReleaseImage( &tgray );
    cvReleaseImage( &timg );

    return squares;
}


// the function draws all the squares in the image
IplImage* drawSquares( IplImage* img, CvSeq* squares )
{
    CvSeqReader reader;
    IplImage* cpy = cvCloneImage( img );
    int i;

    // initialize reader of the sequence
    cvStartReadSeq( squares, &reader, 0 );

    // read 4 sequence elements at a time (all vertices of a square)
    for( i = 0; i < squares->total; i += 4 )
    {
        CvPoint pt[4], *rect = pt;
        int count = 4;

        // read 4 vertices
        CV_READ_SEQ_ELEM( pt[0], reader );
        CV_READ_SEQ_ELEM( pt[1], reader );
        CV_READ_SEQ_ELEM( pt[2], reader );
        CV_READ_SEQ_ELEM( pt[3], reader );

        // draw the square as a closed polyline
        cvPolyLine( cpy, &rect, &count, 1, 1, CV_RGB(0,255,0), 3, CV_AA, 0 );
    }
    return cpy;
    // show the resultant image
    //cvShowImage( blackBoard->blackBoardWindow, cpy );
    cvReleaseImage( &cpy );
}


//calibration of desktop
//result will be stored in calibrationData (vertices of desktop's rectangle in clockwise order from top-left vertex)
//return 1 iff success, otherwise 0
//(Tono)
int Calibrator::calibrate() {
	IplImage* desktop = cvCreateImage(cvSize(
			blackBoard->desktopDrawer->desktopWidth,
			blackBoard->desktopDrawer->desktopHeight), IPL_DEPTH_8U, 3);

	//show white screen for autocalibration
	int W = desktop->widthStep;
	int H = desktop->height;
	int x, y;

	for (y = 0; y < H; y++)
		for (x = 0; x < W; x++ ) {
			desktop->imageData[y * W + x] = 255;
		}

	cvShowImage(blackBoard->blackBoardWindow, desktop);

    //wait 250ms
	cvWaitKey(250);

	//capture frame from webcam
	IplImage* frame = cvQueryFrame(blackBoard->webcam);
	if (!frame)
		return 0;

    // some times frames for initializing webcam
    for (int i=0; i<30; i++) {
    frame = cvQueryFrame(blackBoard->webcam);
        if (!frame)
          return 0;
    }

    //frame in gray scale
    IplImage* frame_gray = cvCreateImage(cvSize(frame->width,frame->height),frame->depth,1);

    //frame1_gray = cvCloneImage(frame);
    cvCvtColor( frame, frame_gray, CV_BGR2GRAY );
    //cvCvtColor( frame, frame_gray, CV_RGB2GRAY );

    /*
    //experiment with only white&black colour
    W = frame_gray->widthStep;
	H = frame_gray->height;
    for (y = 0; y < H; y++)
		for (x = 0; x < W; x++) {
            if (frame_gray->imageData[y * W + x] >99) {frame_gray->imageData[y * W + x]=255;}
            else frame_gray->imageData[y * W + x] = 0;
     }
    */

    IplImage* frame_bw = cvCreateImage(cvGetSize(frame_gray),IPL_DEPTH_8U,1);
    cvThreshold(frame_gray, frame_bw, 128, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);

    //show frame1_gray
	cvShowImage(blackBoard->webcamWindow,frame_bw);
    cerr << "frame" << endl;

    //wait 5s
	cvWaitKey(5000);
    /*
    //show black screen for autocalibration
    W = desktop->widthStep;
	H = desktop->height;
    for (y = 0; y < H; y++)
		for (x = 0; x < W; x += 3) {
			desktop->imageData[y * W + x + 2] = 0;
			desktop->imageData[y * W + x + 1] = 0;
			desktop->imageData[y * W + x] = 0;
		}

	cvShowImage(blackBoard->blackBoardWindow, desktop);
	cvWaitKey(1000);

    IplImage * frame2 = cvQueryFrame(blackBoard->webcam);
	if (!frame2)
		return 0;

    for (int i=0; i<30; i++) {
    frame2 = cvQueryFrame(blackBoard->webcam);
        if (!frame2)
          return 0;
    }



    //IplImage* frame2_diff = cvCreateImage(cvSize(frame2->width,frame2->height),frame2->depth,1);

    //cvThreshold(frame2_diff,frame_finish,20,255,CV_THRESH_BINARY);

    //cvCvtColor( frame2, frame2_gray, CV_BGR2GRAY );
    //frame2_gray = cvCloneImage(frame2);
    cvShowImage(blackBoard->webcamWindow,frame2_diff);
    cvWaitKey(10000);
    W = frame2_gray->widthStep;
	H = frame2_gray->height;
    for (y = 0; y < H; y++){
		for (x = 0; x < W; x++){
		    char c = frame2_gray->imageData[y * W + x ];
            printf("%u ",c);
            }
        printf("\n");
    }
    */
    /*
    //experiment with White&Black colour
    for (y = 0; y < H; y++)
		for (x = 0; x < W; x+=3) {

            if ((frame2_gray->imageData[y * W + x ]<60) || (frame2_gray->imageData[y * W + x + 2]<60) || (frame2_gray->imageData[y * W + x + 1]<60)) {
                frame2_gray->imageData[y * W + x + 1] = 0;
                frame2_gray->imageData[y * W + x + 2] = 0;
                frame2_gray->imageData[y * W + x] = 0;
                }
            else {
                frame2_gray->imageData[y * W + x + 1] = 255;
                frame2_gray->imageData[y * W + x + 2] = 255;
                frame2_gray->imageData[y * W + x ] = 255;
            }
     }

    cvShowImage(blackBoard->webcamWindow,frame2_gray);
    cerr << "second frame" << endl;
    //cerr << frame << " " << frame2 << endl;

    cvWaitKey(5000);

    // xor frame1_gray and frame2_gray
    W = frame1_gray->widthStep;
	H = frame1_gray->height;

    for (y = 0; y < H; y++)
		for (x = 0; x < W; x++) {
            frame1_gray->imageData[y * W + x] ^= frame2_gray->imageData[y * W + x];
		}

    cvShowImage(blackBoard->blackBoardWindow,frame1_gray);
    cvWaitKey(10000);
    */

    // copy from squares.c example from opencv
    storage = cvCreateMemStorage(0);
    //cvShowImage(blackBoard->blackBoardWindow,drawSquares(frame, findSquares4( frame, storage ) ));
    //cvShowImage(blackBoard->blackBoardWindow,DetectAndDrawQuads(frame));
    cvShowImage(blackBoard->blackBoardWindow, DetectAndDrawQuads(frame_bw));
    cvWaitKey(5000);
	//TODO calibration
    // manual calibration //comment by  because Segmentation Fault
    //osetrit poradie vrcholov!!!!
      int minxy=500600;
      int mini=0;
      for(int i = 0; i<4; i++)
      {
        if(calib[i].x+calib[i].y<minxy){
		minxy=calib[i].x+calib[i].y;
		mini=i;
	}
      }
	int smer;
	if(calib[(mini+1)%4].x>calib[mini].x)
		smer=1;
	else
		smer=-1;
      //calibrationData.vertex[0] = calib[0];
      //cerr << 0 << ": " << calibrationData.vertex[0].x << " " <<calibrationData.vertex[0].y << endl;
      for(int i=0; i<4; i++) {
      calibrationData.vertex[i] = calib[(8+mini+smer*i)%4];
      cerr << i << ": " << calibrationData.vertex[i].x << " " <<calibrationData.vertex[i].y << endl;
      }
    
    // manual calibration
	//calibrationData.vertex[0] = cvPoint(40, 40);
	//calibrationData.vertex[1] = cvPoint(600, 40);
	//calibrationData.vertex[2] = cvPoint(600, 440);
	//calibrationData.vertex[3] = cvPoint(40, 440);
    

	cvReleaseImage(&desktop);
	cvReleaseImage(&frame_gray);
	//cvReleaseImage(&frame2_gray);
	cerr << "calibration end" << endl;
	return 1;
}

Calibrator::Calibrator(BlackBoard* parent) {
	blackBoard = parent;
}

Calibrator::~Calibrator() {

}
