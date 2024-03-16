![LOGO](https://raw.githubusercontent.com/Mindty-Kollektiv/minitscript/master/resources/github/minitscript-logo.png)

# 1. How to build MinitScript?

## 1.1. FreeBSD(tested on FreeBSD 14.0)

### 1.1.1 Install packages

```bash
su
pkg install git
pkg install gmake
pkg install openssl
exit
```

### 1.1.2. Clone repository

```bash
git clone https://github.com/Mindty-Kollektiv/minitscript
cd minitscript
```

### 1.1.3. Build

Note: HARDWARE_THREADS should be replaced with the number of your CPU hardware threads, e.g. if you have 2 cores and hyperthreading, you can use 4

```bash
gmake clean && gmake -j HARDWARE_THREADS mains
```

## 1.2. NetBSD(tested on NetBSD 10.0 RC1)

### 1.2.1 Install packages

```bash
su
pkg_add git
pkg_add gmake
pkg_add openssl
exit
```

### 1.2.2. Clone repository

```bash
git clone https://github.com/Mindty-Kollektiv/minitscript
cd minitscript
```

### 1.2.3. Build

Note: HARDWARE_THREADS should be replaced with the number of your CPU hardware threads, e.g. if you have 2 cores, you can use 2.

```bash
gmake clean && gmake -j HARDWARE_THREADS mains
```

## 1.3. OpenBSD(tested on OpenBSD 7.4)

### 1.3.1 Install packages

```bash
su
pkg_add git
pkg_add gmake
pkg_add openssl
exit
```

### 1.3.2. Clone repository

```bash
git clone https://github.com/Mindty-Kollektiv/minitscript
cd minitscript
```

### 1.3.3. Build

Note: HARDWARE_THREADS should be replaced with the number of your CPU hardware threads, e.g. if you have 2 cores, you can use 2.

```bash
gmake clean && gmake -j HARDWARE_THREADS mains CXX=clang++ CC=clang
```

## 1.4. Ubuntu(tested on Ubuntu 22.04)

### 1.4.1 Install packages

```bash
sudo apt-get install git
sudo apt-get install make
sudo apt-get install g++
sudo apt-get install libssl-dev
```

### 1.4.2. Clone repository

```bash
git clone https://github.com/Mindty-Kollektiv/minitscript
cd minitscript
```

### 1.4.3. Build

Note: HARDWARE_THREADS should be replaced with the number of your CPU hardware threads, e.g. if you have 2 cores and hyperthreading, you can use 4

```bash
make clean && make -j HARDWARE_THREADS mains
```

## 1.5. Mac OS X

### 1.5.1 Install packages

You have to install XCode or XCode command line tools.

### 1.5.2. Clone repository

```bash
git clone https://github.com/Mindty-Kollektiv/minitscript
cd minitscript
```

### 1.5.3. Build

Note: HARDWARE_THREADS should be replaced with the number of your CPU hardware threads, e.g. if you have 2 cores and hyperthreading, you can use 4

```bash
make clean && make -j HARDWARE_THREADS mains
```

## 1.6. Windows MSYS2/MINGW64(tested on Windows 10 Pro)

### 1.6.1 Install packages

Install 64 Bit MSYS2 from https://www.msys2.org/, e.g. msys2-x86_64-20190524.exe, then update your MSYS2 installation and install packages.

```bash 
pacman -Syu
pacman -S git
pacman -S make
pacman -S mingw-w64-x86_64-gcc
pacman -S mingw-w64-x86_64-openssl
```

### 1.6.2. Clone repository

```bash
git clone https://github.com/Mindty-Kollektiv/minitscript
cd minitscript
```

### 1.6.3. Build

Note: HARDWARE_THREADS should be replaced with the number of your CPU hardware threads, e.g. if you have 2 cores and hyperthreading, you can use 4

```bash
make clean && make -j HARDWARE_THREADS mains
```

## 1.7. Windows MSC/X64(tested on Windows 10 Pro)

### 1.7.1 Install packages

Install Visual Studio 2022 Community Edition for C++ development, including MSVC v143 and Windows 10 SDK(10.0.19041.0).
Be aware that the makefile uses the default installation paths for libraries and headers.

### 1.7.2. Download or clone repository

Use a GIT client with the following URL: https://github.com/Mindty-Kollektiv/minitscript and clone to a folder into your home folder like "minitscript" OR 
send your browser to "https://github.com/Mindty-Kollektiv/minitscript" and click "Clone or download/Download ZIP" and extract ZIP somewhere in your home folder, e.g. "minitscript".

### 1.7.3. Build

Open your Visual Studio 2022 Community command line interface app for native X64 builds and execute the following commands. 
You need to adjust "minitscript" if you used a different folder. Also you need to extract ext/windows-msc/tools.zip to ext/windows-msc using 7zip or manually.

```
cd %HOMEPATH%/minitscript
cd ./ext/windows-msc && 7z x tools.zip && cd ../..
nmake -f Makefile.nmake
```

This will build MinitScript executables into your "minitscript" folder. You need to copy DLLs still to this folder, just that those dependencies are found.
You need to adjust "minitscript" if you used a different folder.

```
cd %HOMEPATH%\minitscript
.\scripts\windows-prepare-platform-libs.bat
copy .\platform\libs\* .
copy .\lib\* .
```

### 1.7.4. Note

- All *.EXE files are generated in your "minitscript" folder root. You should just be able to launch them from there. So you can ignore 2.1, 2.2 and 2.3.
- The MSC build process is WIP

# 2. Running a MinitScript binary

You find binaries in
- ./bin/minitscript/tools/

## 2.1. Unix-like platforms

On Unix like platforms you need to set up LD_LIBRARY_PATH, just that MinitScript libraries are found.

```bash
$ pwd
/usr/home/andreas/Development/mindty.com/minitscript
$ export LD_LIBRARY_PATH=`pwd`/lib
```

## 2.2. MacOSX

On MacOSX you need to set up DYLD_LIBRARY_PATH, just that MinitScript libraries are found.

```bash
$ pwd
/usr/home/andreas/Development/mindty.com/minitscript
$ export DYLD_LIBRARY_PATH=`pwd`/lib
```

## 2.3. MSYS2/MINGW64

On MSYS2/MINGW64 you need to copy shared objects files to MinitScript folder, just that MinitScript libraries are found.

```bash
$ pwd
/usr/home/andreas/Development/mindty.com/minitscript
$ cp ./lib/*.dll .
```

## 2.4. Execute MinitScript binaries

You have to execute the binaries from minitscript root folder like:

```bash
$ pwd
/usr/home/andreas/Development/mindty.com/minitscript
$ ./bin/minitscript/tools/minitscript resources/minitscript/tests/base-test.tscript
```

# 3. Other information

## 3.1. Links

- MinitScript, see [README.md](./README.md)
- MinitScript - How to build, see [README-BuildingHowTo.md](./README-BuildingHowTo.md)
- MinitScript - How to use, see [README-Tools.md](./README-Tools.md)
- MinitScript - ToDo list, see [README-ToDo.md](./README-ToDo.md)
- The Mindty Kollektiv [Discord Server](https://discord.gg/Na4ACaFD)

## 3.2. Additional notes

- TODO: updated build + install documentation
- Build instructions will follow for Haiku
