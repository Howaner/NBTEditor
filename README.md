# NBTEditor (1.0)
A nbt editor software to edit minecraft binary files. Currently it can be used to edit .dat and .schematic files.  
![NBTEditor](Image.png)

## Download
- Windows Portable: http://dl.howaner.de/NBTEditor/1.0/NBTEditor.zip
- Windows Installer: http://dl.howaner.de/NBTEditor/1.0/NBTEditor.msi
- Ubuntu 16.04 Deb: http://dl.howaner.de/NBTEditor/1.0/nbteditor_1.0-1_amd64.deb


## Used frameworks
The software is written in C++ with the Qt5 framework and Zlib.

## How to compile it on linux
```
apt-get install build-essential qt5-default qttools5-dev-tools cmake zlib1g-dev
mkdir build
cmake ..
make
./nbteditor
```

## How to compile it on windows
1. Install qt5 developer framework with mingw-32 compiler
2. Install cmake
3. Compile static zlib library
4. Follow linux introductions