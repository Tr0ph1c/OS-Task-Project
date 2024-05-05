# Linux Group / User System Manager with GUI

## Description
A program with graphical user interface that can be used to manage groups and users on a linux based system without
the hassle of learning commands or memorizing them. The program uses GLFW+OpenGL to render the GUI and 
ImGui for the GUI elements. The interface is relatively straight forward and easy to use. The program uses
existing commands like `useradd` `groupadd` etc..

## Dependencies
* OpenGL3
* GLFW `sudo apt-get install libglfw3-dev`
* ImGui -> binaries included with the repo

## How to build
clone the repository<br>
`git clone https://github.com/Tr0ph1c/OS-Task-Project.git`<br>
unpack dependencies.tar.gz<br>
`tar -xf dependencies.tar.gz`<br>
build with the script<br>
`./build.sh`<br>
run with sudo<br>
`sudo ./main`
