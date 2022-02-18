# Basler Pylon SDK
## CMake implementation and OpenCV example
Basler Pylon SDK delivers with only Makefile and Visual Studio support.
On a Basler Github project tough they used a CMake module to find the SDK ([Basler CMake module](https://github.com/basler/pylon-ros-camera/blob/master/pylon_camera/cmake/FindPylon.cmake)). 


To compile pylon based project using CMake use the **FindPylon.cmake** module and reference it on the *CMAKE_MODULE_PATH* of yours CMakeLists. 
From default Pylon in installed in /opt/pylon/. If not change folder reference in the cmake module. 

### Opencv
Moreover in the pylon examples there is no clear use of OpenCV, and transfering from the two images domain, so the grab example use OpenCV for image conversion and visualization. 

- **GrabCV:** Simple example that grab frame, convert it and show using CV functions. 
- **GrabEvery:** The camera grabs a frame every 5 seconds using SoftwareTrigger. 

### Camera emulation
```bash
export PYLON_CAMEMU=1
```

## Requirements
- Pylon SDK (v6.3.0)
- OpenCV (v4.5.5)

## Compile examples
```bash
    mkdir build && cd build
    cmake ..
    cmake --build .
```
