# 3DViewer
In this project I’ll  implement an application for viewing 3D wireframe models in C++ in the object-oriented programming paradigm.

## Contents

1. [Chapter I](#chapter-i) \
   1.1. [About project](#information) \
   1.2. [Bonus settings and record](#bonuses) \
2. [Chapter II](#chapter-ii) \
    1.1 [How to install](#install) \
    1.2 [How to check](#tests)
3. [Chapter III](#chapter-iii) \
   3.1. [Overview](#overview)


# Chapter I

## Information

- The program developed in C++ language with Qt framework and Opengl library;

- Model loading
    - The application allows loading 3D wireframe models from .obj files(vertices and surfaces list support only);

- The program provides the ability to:
    - Translate the model by a given distance in relation to the X, Y, Z axes;
    - Rotate the model by a given angle in relation to its X, Y, Z axes;
    - Scale the model by a given value.

- The graphical user interface must contain:
    - A button to select the model file and a field to output its name("File" action in main menubar);
    - A visualisation area for the wireframe model(in main window);
    - Button/buttons and input fields for translating the model(sliders and editLines);
    - Button/buttons and input fields for rotating the model(sliders and editLines);
    - Button/buttons and input fields for scaling the model(sliders and editLines);
    - Information about the uploaded model - file name, number of vertices and edges(Labels).

- The program correctly processes and allows user to view models with details up to 100, 1000, 10,000, 100,000, 1,000,000  vertices without freezing (a freeze is an interface inactivity of more than 0.5 seconds).

## Bonuses

### Custom preferences

- The program allows customizing some preferences:
    - The type of projection (parallel and central).
    - Setting up the type (solid, dashed), color and thickness of the edges, display method (none, circle, square), color and size of the vertices.
    - Choosing the background color.
    - Settings must be saved between program restarts.

### Screenshot and Gifs 

- Also the program allows:
    - Saving the captured (rendered) images as bmp and jpeg files.
    - Recording small screencasts - the current custom affine transformation of the loaded object into gif-animation (640x480, 10fps, 5s) by a special button.


# Chapter II

## Install

1. Clone repository `git clone <repository-url>`.
2. Navigate to `src/` directory.
3. Run `make install` or just `make` to install the project.
4. Run `make run` to start the project.

## Tests 

1. Run unit tests: `make test`.
2. Run code coverage with unit tests: `make gcov`.

> **Note:** If you want to install project on macOs add this string in View/3DViewer.pro: `greaterThan(QT_MAJOR_VERSION, 4): QT +=  3dextras openglwidgets`.

# Chapter III

## Overview

### Installing

![Installing](./dvi/media/Installing.gif)


### Overview of basic actions

![Basic actions](./dvi/media/Overview1.gif)


### How to create a GIF

![Create GIF](./dvi/media/Man.gif)


### Conservation of settings

![Settings saved](./dvi/media/SaveSettings.gif)