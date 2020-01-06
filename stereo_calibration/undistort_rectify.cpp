#include <opencv2/core/core.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <iostream>
#include "popt_pp.h"

using namespace std;
using namespace cv;
cv::Mat show_Isaline(Mat In_left_img,Mat In_right_img,Size In_imagesize,Rect In_left,Rect In_right)
{
       Mat canvas;
       double sf;
       int w, h;
       sf = 600. / MAX(In_imagesize.width, In_imagesize.height);
       //printf()
       w = cvRound(In_imagesize.width * sf);
       h = cvRound(In_imagesize.height * sf);
      canvas.create(h, w * 2, CV_8UC3);
       /*左图像画到画布上*/
       Mat canvasPart = canvas(Rect(w*0, 0, w, h));								//得到画布的一部分
       resize(In_left_img, canvasPart, canvasPart.size(), 0, 0, INTER_AREA);		//把图像缩放到跟canvasPart一样大小
       Rect vroiL(cvRound(In_left.x*sf), cvRound(In_left.y*sf),				//获得被截取的区域
                 cvRound(In_left.width*sf), cvRound(In_left.height*sf));
       rectangle(canvasPart,vroiL, Scalar(0, 0, 255), 3, 8);						//画上一个矩形
      // cout << "Painted ImageL" << endl;
       /*右图像画到画布上*/
       canvasPart = canvas(Rect(w, 0, w, h));										//获得画布的另一部分
       resize(In_right_img, canvasPart, canvasPart.size(), 0, 0, INTER_LINEAR);
       Rect vroiR(cvRound(In_right.x * sf), cvRound(In_right.y*sf),
                 cvRound(In_right.width * sf), cvRound(In_right.height * sf));
       rectangle(canvasPart, vroiR, Scalar(0, 255, 0), 3, 8);
     //  cout << "Painted ImageR" << endl;
       /*画上对应的线条*/
       for (int i = 0; i < canvas.rows;i+=16)
         line(canvas, Point(0, i), Point(canvas.cols, i), Scalar(0, 255, 0), 1, 8);
      return canvas.clone();
   //printf(grayImageL.)
   //cout << "wait key" << endl;
}

int main(int argc, char const *argv[])
{
  char* leftimg_filename;
  char* rightimg_filename;
  char* calib_file;
  char* leftout_filename;
  char* rightout_filename;

  static struct poptOption options[] = {
    { "leftimg_filename",'l',POPT_ARG_STRING,&leftimg_filename,0,"Left imgage path","STR" },
    { "rightimg_filename",'r',POPT_ARG_STRING,&rightimg_filename,0,"Right image path","STR" },
    { "calib_file",'c',POPT_ARG_STRING,&calib_file,0,"Stereo calibration file","STR" },
    { "leftout_filename",'L',POPT_ARG_STRING,&leftout_filename,0,"Left undistorted imgage path","STR" },
    { "rightout_filename",'R',POPT_ARG_STRING,&rightout_filename,0,"Right undistorted image path","STR" },
    POPT_AUTOHELP
    { NULL, 0, 0, NULL, 0, NULL, NULL }
  };

  POpt popt(NULL, argc, argv, options, 0);
  int c;
  while((c = popt.getNextOpt()) >= 0) {}

  Mat R1, R2, P1, P2, Q;
  Mat K1, K2, R;
  Vec3d T;
  Mat D1, D2;
  Mat img1 = imread(leftimg_filename, CV_LOAD_IMAGE_COLOR);
  Mat img2 = imread(rightimg_filename, CV_LOAD_IMAGE_COLOR);

  cv::FileStorage fs1(calib_file, cv::FileStorage::READ);
  fs1["K1"] >> K1;
  fs1["K2"] >> K2;
  fs1["D1"] >> D1;
  fs1["D2"] >> D2;
  fs1["R"] >> R;
  fs1["T"] >> T;

  fs1["R1"] >> R1;
  fs1["R2"] >> R2;
  fs1["P1"] >> P1;
  fs1["P2"] >> P2;
  fs1["Q"] >> Q;

  cv::Mat lmapx, lmapy, rmapx, rmapy;
  cv::Mat imgU1, imgU2,matAdd1;

  cv::initUndistortRectifyMap(K1, D1, R1, P1, img1.size(), CV_32F, lmapx, lmapy);
  cv::initUndistortRectifyMap(K2, D2, R2, P2, img2.size(), CV_32F, rmapx, rmapy);
  cv::remap(img1, imgU1, lmapx, lmapy, cv::INTER_LINEAR);
  cv::remap(img2, imgU2, rmapx, rmapy, cv::INTER_LINEAR);
  
  addWeighted(imgU1,0.5,imgU2,0.5,0,matAdd1);
  cv::Mat dst=show_Isaline(imgU1,imgU2,imgU2.size(),cv::Rect(0,0,imgU1.cols,imgU1.rows),cv::Rect(0,0,imgU2.cols,imgU2.rows));
  imwrite(leftout_filename, imgU1);
  imwrite(rightout_filename, imgU2);
  imwrite("matAdd.jpg", matAdd1);
 imwrite("dst.jpg", dst);
  //waitKey(30);
  return 0;
}
