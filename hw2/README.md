# HW2

Your Name (Please replace with your name.)

Your SBU ID (Please replace with your 9-digit SBU ID.)

Your Email (Please replace with your email.)

## Overview

- Implemented a sample modern OpenGL program with GLFW as the windowing toolkit. 
- Implemented some basic geometries, including lines, triangles, and circles. Circles are implemented with tessellation shaders. 

## Notes

- All README files for future homework should also comply with the same format as this one. 
- This program template is just for your reference. Please feel free to code your own program (i.e., not using this template). However, the user interface (mouse and keyboard functionalities) should be the same as specified in the homework manual. 
- Please submit either the C++ version or the Python version (but **not both**), and **comply with the submission requirements as detailed on the [TA Help Page](https://www3.cs.stonybrook.edu/~xihan1/courses/cse528/ta_help_page.html)**. Plesase cut and paste this README (together with your answers for the non-programming part) into either `cpp/` or `py/`, rename the directory as instructed by the TA Help Page, and submit via Brightspace. 
- Please also make sure you have checked all implemented features with "x"s in the Markdown table below. As speficied on the TA Help Page, only checked features will be considered for grading!

## Hints on The Template

- Suggested order to read and understand this program: 
  - GLFW callbacks;
  - Triangle (ignore the code related to the self-spin effect);
  - Triangle (with self-spin; involves transformation matrices);
  - Circles (involves tessellation shaders, which are not necessary in the first half of this course). 
- In this program, the circle parameters are passed into tessellation shaders via generic vertex attribute arrays. 
  Note how this differs from the "pass-by-shader-uniforms" method for the sphere example; 
- Please do remember to play with the program as guided by the comments in the tessellation evaluation shader;
- If this program does not work on your VMWare virtual environment, 
  please try to [disable the 3D acceleration feature](https://kb.vmware.com/s/article/59146). 

## Dependencies

- OpenGL (Required for Both Versions):
```bash
sudo add-apt-repository ppa:kisak/kisak-mesa
sudo apt update
sudo apt-get dist-upgrade
sudo reboot
```
- Further Needed for the C/C++ Version: 
  - [GLAD](https://glad.dav1d.de/)
    - Configuration w.r.t. results of `sudo glxinfo | grep "OpenGL`
    - Command `glxinfo` needs `mesa-utils`
  - Remaining dependencies could be installed via apt:
  ```bash
  apt install libopencv-dev libglm-dev libglew-dev libglfw3-dev mesa-utils libx11-dev libxi-dev libxrandr-dev
  ```
- Further Needed for the Python Version (from PyPI):
```bash
pip install numpy PyOpenGL PyGLM glfw
```

## Compile & Run

- C/C++ Version (Run inside `cpp/`): 
```bash
mkdir build
cd build
cmake -DMAKE_BUILD_TYPE=Release ..
make 
cd ..
./build/hw2
```
- Python Version. Run inside `py/`, and replace "py3" with your own conda env name (if using the provided VMWare Virtual Machine, **no replacement** is needed here):
```bash
conda activate py3
python main.py
```

## Features Implemented

Check all features implemented with "x" in "[ ]"s. 
Features or parts left unchecked here won't be graded! 

- [ ] 1. One Segment of Cubic Bezier Spline
- [ ] 2. Piecewise C(2) Cubic Bezier Spline
  - [ ] Base Spline Creation And Display
  - [ ] Control Node Dragging
  - [ ] Control Node Insertion
  - [ ] Control Node Deletion
  - [ ] Save to File
  - [ ] Load from File
- [ ] 3. Catmull-Rom Spline
  - [ ] Base Spline Creation And Display
  - [ ] Interpolation Point Dragging
  - [ ] Interpolation Point Insertion
  - [ ] Interpolation Point Deletion
  - [ ] Save to File
  - [ ] Load from File
- [ ] 4. Extension to 3D (BONUS)
  - [ ] Tracking Ball
  - [ ] One Segment of Cubic Bezier Spline
  - [ ] Piecewise C(2) Bezier Spline
  - [ ] Catmull-Rom Spline

## Usage

- If you have implemented extra functionalities not mentioned in the manual, you may specify them here.
- If your program failed to obey the required mouse/keyboard gestures, you may also specify your own setting here. In this case, penalties may apply.
