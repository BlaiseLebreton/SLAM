#include "/home/pi/LidarSDK/include/CYdLidar.h"
#include <iostream>
#include "../struct.h"

using namespace std;
using namespace ydlidar;

#if defined(_MSC_VER)
#pragma comment(lib, "ydlidar_driver.lib")
#endif

CYdLidar laser;

int Lidar_Initialize() {
  string port;
  int ac;
  char* av[3];
  init(ac, av);

  map<string, string> ports = YDlidarDriver::lidarPortList();
  map<string, string>::iterator it;

  if (ports.size() == 1) {
    port = ports.begin()->second;
  }
  else {
    int id = 0;

    for (it = ports.begin(); it != ports.end(); it++) {
      printf("%d. %s\n", id, it->first.c_str());
      id++;
    }

    if (ports.empty()) {
      printf("Not Lidar was detected. Please enter the lidar serial port:");
      cin >> port;
    } else {
      while (ok()) {
        printf("Please select the lidar port:");
        string number;
        cin >> number;

        if ((size_t)atoi(number.c_str()) >= ports.size()) {
          continue;
        }

        it = ports.begin();
        id = atoi(number.c_str());

        while (id) {
          id--;
          it++;
        }

        port = it->second;
        break;
      }
    }
  }


  if (!ok()) {
    return 0;
  }

  laser.setSerialPort(port);
  laser.setSerialBaudrate(115200);
  laser.setFixedResolution(false);
  laser.setReversion(false);
  laser.setInverted(false);
  laser.setAutoReconnect(true);
  laser.setSingleChannel(true);
  laser.setLidarType(TYPE_TOF);
  laser.setMaxAngle(180);
  laser.setMinAngle(-180);
  laser.setMinRange(0.1);
  laser.setMaxRange(8.0);
  laser.setScanFrequency(8.0);

  vector<float> ignore_array;
  ignore_array.clear();
  laser.setIgnoreArray(ignore_array);

  bool ret = laser.initialize();

  if (ret) {
    ret = laser.turnOn();
  }

  // Test get data
  bool hardError;
  LaserScan scan;
  if (laser.doProcessSimple(scan, hardError)) {
    printf("LIDAR OK\n");
  }
  else {
    printf("LIDAR NOK\n");
  }
  return 0;
}

vector<LidarPoint> Lidar_GetScan() {
  vector<LidarPoint> MyScan;
  if (!ok()) {
    return MyScan;
  }

  bool hardError;
  LaserScan scan;

  if (laser.doProcessSimple(scan, hardError)) {
    for(int i = 0; i < scan.points.size(); i++) {
      MyScan.push_back({
        // scan.points[i].range*cos(scan.points[i].angle+M_PI/2.0),
        // scan.points[i].range*sin(scan.points[i].angle+M_PI/2.0),
        scan.points[i].angle+M_PI/2.0,
        scan.points[i].range
      });
    }
  }
  else {
    fprintf(stderr, "Failed to get Lidar Data\n");
    fflush(stderr);
  }
  return MyScan;
}

int Lidar_Shutdown() {
  laser.turnOff();
  laser.disconnecting();
  return 0;
}
