GLFW Compile&Build: http://www.glfw.org/docs/latest/compile_guide.html

Command to build test.cpp: `g++ test.cpp -o test -I /usr/local/include -L /usr/local/lib -lglfw3 -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo`  
Command to ubild simple_window.cpp: `g++ simple_window.cpp -o simple_window -I /usr/local/include -L /usr/local/lib -lglfw3 -lGLEW -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo`  
Command to ubild triangle.cpp: `g++ triangle.cpp -o triangle -std=c++11 -I /usr/local/include -L /usr/local/lib -lglfw3 -lGLEW -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo`  
---
How to compile GLFW:
1. Clone repo git@github.com:glfw/glfw.git
2. `cd glfw`
3. `cmake .`
4. `make`
5. `make install`

How to install GLEW: `brew install glew`
