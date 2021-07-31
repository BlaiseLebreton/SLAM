#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include "struct.h"
#include "lidar.h"

using namespace cv;
using namespace std;

#define WIDTH 250

int main(int argc, char** argv) {

  if (Lidar_Initialize() == 1) {
    return 1;
  }
  else {
    printf("Lidar initialized\n");
  }

  namedWindow ("LIDAR Datas", WINDOW_NORMAL);
  resizeWindow("LIDAR Datas", WIDTH, WIDTH);
  Mat data = Mat::zeros(Size(WIDTH, WIDTH), CV_8UC1);
  double scale = WIDTH/(16.0);


  for(;;)	{
    vector<LidarPoint> Scan = Lidar_GetScan();
    data = 0;

    for (int s = 0; s < Scan.size(); s++) {
      float x = Scan[s].r*cos(Scan[s].a)*scale + WIDTH/2.0;
      float y = Scan[s].r*sin(Scan[s].a)*scale + WIDTH/2.0;
      data.at<uchar>(y, x) = 255;
    }

    imshow("LIDAR Datas", data);
    if (waitKey(1) == 27) {
      break; // stop capturing by pressing ESC
    }
  }
  Lidar_Shutdown();

  return 0;
}
