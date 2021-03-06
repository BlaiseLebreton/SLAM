CXX = g++
CXXFLAGS = -g -latomic -Wno-psabi
OBJDIR = /obj
BINARY = SLAM

default:
	@make -s $(BINARY)

$(BINARY): main.o lidar/lidar.o /home/pi/LidarSDK/build/libydlidar_driver.a
	@echo "Compiling $(BINARY)"
	@$(CXX) $^ -o $@ $(CXXFLAGS) -lpthread  `pkg-config --cflags --libs opencv4`

main.o: main.cpp lidar/lidar.o
	@echo "Compiling main.cpp"
	@$(CXX) -c $< -o $@ $(CXXFLAGS) `pkg-config --cflags --libs opencv4`

lidar/lidar.o : lidar/lidar.cpp struct.h
	@echo "Compiling lidar.cpp"
	@$(CXX) -c $< -o $@ $(CXXFLAGS) -I/home/pi/LidarSDK/include

clean:
	@echo "Cleaning files"
	@rm -f *.o lidar/*.o
	@make -s
