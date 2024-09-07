# HW1

Your Name (Please replace with your name.)

Your SBU ID (Please replace with your 9-digit SBU ID.)

Your Email (Please replace with your email.)

## Overview

- Implemented a sample modern OpenGL program with GLFW as the windowing toolkit. 
- Implemented a naive Bresenham line drawing routine without edge-case handling. 

## Notes

- All README files for future homework should also comply with the same format as this one. 
- This program template is just for your reference. Please feel free to code your own program (i.e., not using this template). However, the user interface (mouse and keyboard functionalities) should be the same as specified in the homework manual. 
- Please submit either the C++ version or the Python version (but **not both**), and **comply with the submission requirements as detailed on the [TA Help Page](https://www3.cs.stonybrook.edu/~xihan1/courses/cse528/ta_help_page.html)**. Plesase cut and paste this README (together with your answers for the non-programming part) into either `cpp/` or `py/`, rename the directory as instructed by the TA Help Page, and submit via Brightspace. 
- Please also make sure you have checked all implemented features with "x"s in the Markdown table below. As speficied on the TA Help Page, only checked features will be considered for grading!

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
pip install PyOpenGL PyGLM glfw
```

## Compile & Run

- C/C++ Version (Run inside `cpp/`): 
```bash
mkdir build
cd build
cmake -DMAKE_BUILD_TYPE=Release ..
make 
cd ..
./build/hw1
```
- Python Version. Run inside `py/`, and replace "py3" with your own conda env name (if using the provided VMWare Virtual Machine, **no replacement** is needed here):
```bash
conda activate py3
python main.py
```

## Features Implemented

Check all features implemented with "x" in "[ ]"s. 
Features or parts left unchecked here won't be graded! 

- [x] 1. Line Segment (Fully Implemented in This Template)
  - [x] 0 <= m <= 1
- [ ] 2. Line Segment
  - [ ] Slope m < -1
  - [ ] -1 <= m < 0
  - [ ] 1 < m
  - [ ] Vertical
- [ ] 3. Ploy-line & Polygon
  - [ ] Poly-line
  - [ ] Polygon
- [ ] 4. Circle & Ellipse
  - [ ] Circle
  - [ ] Ellipse
- [ ] 5. Polynomial Curve (BONUS PART-1)
  - [ ] Line
  - [ ] Quadratic Curve
  - [ ] Cubic Curve
- [ ] 6. Scan-conversion (BONUS PART-2)
  - [ ] Triangle
  - [ ] Convex Polygon
  - [ ] Concave Polygon
  - [ ] Self-intersection detection & report

## Usage

If you have implemented extra functionalities not mentioned in the manual,
you may specify them here.

If your program failed to obey the required mouse/keyboard gestures,
you may also specify your own setting here.
In this case, penalties may apply.

## FAQ: Runtime error "shader file not successfully read"

If you are using CLion or PyCharm, you should set up the working directory of the project.
First click the "hw1 | Debug" (for CLion) or "hw1" (for PyCharm) icon in the top-right corner, 
next click "Edit Configurations...", 
then set up the "Working directory" item to the root of your project, 
i.e., the path to `cpp/` or `py/`
(these directories should be further renamed to `yoursbuid_hwx` as specified above). 
Note that the working directory must be **exactly** root of your project 
(its parent directories, e.g. path to `hw1/`, won't work). 

## Appendix

Please include any other stuff you would like to mention in this section.
E.g., your suggestion on possible combinations of cubic curve parameters in this programming part. 
