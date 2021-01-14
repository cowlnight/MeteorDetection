---
title: 'MeteorDetection'
disqus: hackmd
---

Meteor Detection 
===
![downloads](https://img.shields.io/github/downloads/atom/atom/total.svg)
![build](https://img.shields.io/appveyor/ci/:user/:repo.svg)
![chat](https://img.shields.io/discord/:serverId.svg)


This is a basic approach for meteor detection. There are 2 versions (Cpp version and Python version) for this task.


## Environment 
OS: Windows 10 (Visual Studio 2017)
OpenCV

## Setup （Windows 10）
Working Directory: `$(ProjectDir)..\Assets`
Include Directories: `..\include;$(IncludePath)`
Library Directories: `..\lib;$(LibraryPath)`

Library for Debug mode:
```
opencv_world344d.lib
libfftw3-3_win64.lib
lapack_win64_MTd.lib
blas_win64_MTd.lib
```

Library for Release mode:
```
opencv_world344.lib
libfftw3-3_win64.lib
lapack_win64_MT.lib
blas_win64_MT.lib
```
Precompiled header file: `pch.h`

## Files
### Projects
`BatchDetection`: project for batch running
`FindMeteor`: project for meteor detection

### Preparation files
* `pch.h`    : Precompiled header file  (includes header files for C++ utility library, STL library, OpenCV library, Extension library, Forward declaration and Type definition).
    *If you use some new library (not included in this file), it will be added in proper area.* 
* `pch.cpp`  : Source file corresponding to pre-compiled header; necessary for compilation to succeed.
* `PreDefine/AppDeclare.h` : Forward declarations.
    *All the class you need in this project needs to be fisrt declared in this file.*
* `PreDefine/AppTypeDef.h` : Application typedef declaration.
* `utils.h` : Header file for declaring public function.
* `utils.cpp` : Source file of public function.

### Data files
* `MeteorData.h` : `CMeteorData` (class) for all public data.   
 *If you want to add some public data for your phase, you can add it in **YOURPHASE**/**YOURPHASE**Data.h*
   
                 
### Interface File
* AppAlgoITF.h : IAppAlgoITF(interface class) for algorithm interface.

### Algorithm
*Declare the private variable and function you need in YOUR header file, and implement your algorithm in source file of your own section.*
*  `Algorithm/MetDetectionAlgo.h` : `CLablDetection`(class) for meteor detection algorithm.
* `Algorithm/MetDetectionAlgo.cpp` : Implementation of meteor detection algorithm.


### Main Function
* `FindMeteor.cpp` : This file contains the 'main' function. Program execution begins and ends there.


## How to Use
1. Unzip `release.zip`.
2. Put images to `data` folder, keep the path as:
    * `data/FOLDER/IMAGE.jpg`
3. Run `BatchDetection.exe` for excution.

## How to Build the Code
1. Unzip `Assets` and `Include` folders.
2. Following `Files` illustration to build Visual studio project.

## Example


| Images | Detection output | 
| -------- | -------- | 
|![](https://i.imgur.com/LG4nT44.jpg) | ![](https://i.imgur.com/1LBiImk.jpg)| 
| ![](https://i.imgur.com/CjI0CJh.jpg)|![](https://i.imgur.com/nGza3a6.jpg) | 
|![](https://i.imgur.com/11x5L2X.jpg)|![](https://i.imgur.com/BcSsjJ2.jpg)|

## Limitation
1. This project can not identify the difference between airplane and meteor. 
2. The light spot may create noise and get the wrong output.
3. The project just solve the specific source image(because we create the mask for this type image). If you want to use images from other camera or device, you need to rewrite the mask code.
