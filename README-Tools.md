![LOGO](https://raw.githubusercontent.com/Mindty-Kollektiv/minitscript/master/resources/github/minitscript-logo.png)

# 1. Introduction to MinitScript tools

The MinitScript tool that you might use most is the MinitScript interpreter. 

MinitScript tools also help with transpiling MinitScript scripts to C++ and generating executable (main) files. 
If using multiple scripts, it also helps with generating a library C++ file, which is required in this case.

If you want your scripts working in native mode(which involves transpilation and such) 
you also need to create a GNU make file or a Microsoft Compiler(MSC) nmake file. 

MinitScript tools also help with generating documentation.

See section 1.x to have a description of each tool. 

If you just want to have your script(s) working in native mode, you can fast forward to section 2. and use the minitscriptbuild tool. 

## 1.1. minitscript

minitscript is the MinitScript interpreter tool, which means scripts are translated to a syntax tree, which then gets executed. This can be called script interpretion.
The minitscript tool can take the following arguments:

### 1.1.1. Show usage help

```
minitscript --help
```  

### 1.1.2. Show version

```
minitscript --version
```  

### 1.1.3. Interpret a script 

Interpret a script and show additional script information, which can be handy if debugging a script

```
minitscript --verbose test.tscript
```  

### 1.1.4. Interpret a script and pass arguments to it 

Interpret a script and pass arguments to it, which can be read by application.getArguments() script method call

```
minitscript test.tscript --arguments --help
```  

### 1.1.5. Just interpret a script

```
minitscript test.tscript
```  

### 1.1.6. Without arguments 

minitscript can also be ran without arguments at all. This way you can feed minitscript tool with pipe input using '<'

```
minitscript < test.tscript
```  

### 1.1.7. Prompt 

You do not need to pipe a file into minitscript tool. So if you start minitscript tool without arguments you can enter a (partial) script and exit with 
control-z(Windows) and control-d(unixes and others) 

```
minitscript
```

Now you can enter e.g.

	console.printLine(1*2+3)
	
and press control-z(Windows) and control-d(unixes and others) to exit the prompt and execute the script.

## 1.2. minitscripttranspiler

To transpile or translate a MinitScript script code to C++ code you can use the "minitscripttranspiler" tool.
The basic usage looks like the following:

```
minitscripttranspiler test.tscript tsrc/test
```

This will create a C++ header and definition file, in this case tsrc/test.h and tsrc/test.cpp, from test.tscript.
Note that the C++ class name of the transpilation unit is derived from tsrc/test, in this case "test".

If you have extended MinitScript, you can also add the corresponding C++ MinitScript extension files like this:

```
minitscripttranspiler test.tscript tsrc/test src/MinitScriptExtension.cpp
```

## 1.3. minitscriptuntranspiler

To "untranspile" a MinitScript C++ code you can use the "minitscriptuntranspiler" tool.
The basic usage looks like the following:

```
minitscriptuntranspiler test.tscript tsrc/test
```

This will keep the C++ header and definition file, in this case tsrc/test.h and tsrc/test.cpp, but remove the
transpilation part.

## 1.4. minitscriptmain

There are 3 types of MinitScript C++ files. Ordinary script files, main files and library files.
Main files are application entry points, and result in a executable binary file.
To have a application with multiple translated MinitScript scripts, you need to create a library C++ file.

To create a main file, you can use the "minitscriptmain" tool.
The basic usage looks like the following:

```
minitscriptmain test.tscript test tsrc\Main-main.cpp
```

The first argument is our main MinitScript script file, the second argument is the class name of our C++ transpilation unit, 
which gets derived from our transpilation file name, which we used in 1.2. 
The third argument ist the main C++ file, that we want to create for our MinitScript application.
Please note that main file name, needs to end with an "-test.cpp".
 
Optional command line arguments are:
- --use-libray - Issues to use a MinitScript library C++ file, if you have multiple script files tranpiled into C++ (see 1.5.)
- --native-only - By default, scripts gets interpreted also in native mode, if they have changed, so if you only want to compile and do not want to use optional interpretion you can use the "--native-only" argument

## 1.5. minitscriptlibrary

If you have multiple transpiled MinitScript scripts in your MinitScript application, you need to create a MinitScript library C++ file.
Internally it is used to resolve script path and file names to C++ transpilation unit class names.

The usage looks like the following:

```
minitscriptlibrary test1.tscript test1 test2.tscript test2 [testN.tscript testN] tsrc/library.cpp
```

The first argument is the first script file name, the second argument the corresponding class name.
The third argument is the second script file name, the fourth argument the corresponding class name.
You can add more script file name and corresponding class names pairs. 
The last argument is the Library C++ file.

Optional command line arguments are:
- --native-only - By default, scripts gets interpreted also in native mode, if they have changed, so if you only want to compile and do not want to use optional interpretion you can use the "--native-only" argument

Please see a complete example here:

```
# create target C++ transpilation source folder
mkdir tsrc 
# transpile test1.tscript in tsrc/test1.h/.cpp, with class name test1
miniscripttranspiler test1.tscript tsrc/test1
# transpile test2.tscript in tsrc/test2.h/.cpp, with class name test2
miniscripttranspiler test2.tscript tsrc/test2
# create a MinitScript C++ library file for test1.tscript and test2.tscript into tsrc/library.cpp
miniscriptlibrary test1.tscript test1 test2.tscript test2 tsrc/library.cpp
# generate a MinitScript C++ main file from test1.tscript and class name test1 into tsrc/Main-main.cpp
miniscriptmain --use-library test1.tscript test1 tsrc/Main-main.cpp
# generate Linux/BSD/MacOSX makefile from folder tsrc into TMakefile
miniscriptmakefile tsrc TMakefile
# compile
make -f TMakefile
```

Note: Things are a lot easier if using the MinitScript build script, see 2. 

## 1.6. minitscriptmakefile(Linux/BSD/MacOSX,...)

To be able to compile transpiled MinitScript scripts, main and library files, you can use the "minitscriptmakefile" tool.
It generates a Makefile, which can be used for the GNU make tool(Linux, ...) or gmake tool(BSD, ...), which is used on Unix like Operating Systems. 

The basic usage looks like the following:

```
minitscriptmakefile tsrc TMakefile
```

The first argument is the path name, where we transpiled our scripts to, and where we added optional MinitScript main files and a MinitScript library C++ file.
The second argument is just the Makefile file name.

To create a build of your MinitScript application you can just run "make -f TMakefile" on Linux or "gmake -f TMakefile" on BSD. 

You can also generate a .so(shared object file for Linux/BSD/MacOSX) file or a .dll(dynamic link library for Windows/MINGW) file, to be embedded in other C++ applications.
In this case the first argument needs to be "--library". 

Please see an example below:

```
minitscriptmakefile --library tsrc TMakefile
```

## 1.7. minitscriptnmakefile(Windows)

To be able to compile transpiled MinitScript scripts, main and library files on Windows, you can use the "minitscriptnmakefile" tool.
It generates a Makefile, which can be used with the Microsoft Compiler(MSC) nmake tool. 

The basic usage looks like the following:

```
minitscriptnmakefile tsrc TMakefile.nmake
```

The first argument is the path name, where we transpiled our scripts to, and where we added optional MinitScript main files and a MinitScript library C++ file.
The second argument is just the Makefile file name.

To create a build of your MinitScript application you can just run "nmake -f TMakefile.nmake". 

You can also generate a .dll(dynamic link library for Windows) file, to be embedded in other C++ applications.
In this case the first argument needs to be "--library". 

Please see an example below:

```
minitscriptnmakefile --library tsrc TMakefile
```

## 1.8. minitscriptdocumentation

"minitscriptdocumentation" is the MinitScript documentation tool. It requires to pass the type of documentation to be generated, 
which can be one of the following: classes, methods, variables, operators, keys or keywords.

Optionally you can pass the main heading index as first argument by adding "--heading=X", whereas X is the main heading index, which needs to be a integer value.

See an example below:
```
minitscriptdocumentation --heading=6 classes
```

# 2. How to use the minitscriptbuild tool

If you want to have your script(s) executed in native mode, 
which requires transpilation of MinitScript scripts to C++ and generation of main, library C++ files and make files, you can use the minitscriptbuild tool.

Optional command line arguments are:
- --application - Build a application that can be executed from your Operating System
- --library - Builds a .so(shared object file for Linux/BSD/MacOSX) file or a .dll(dynamic link library for Windows/MINGW) file, that can be embedded in other C++ applications
- --native-only - By default, scripts gets interpreted also in native mode, if they have changed, so if you only want to compile and do not want to use optional interpretion you can use the "--native-only" argument

The basic usage looks like:
 
```
minitscriptbuild main.tscript [script1.tscript] [script2.tscript]
```

This example command compiles all the enlisted scripts and builds them into an executable application. 
In this particular case, main.tscript is the main script which can be executed, script1.tscript and script2.tscript are examples for optional additional scripts that can be used e.g. from the main script.

If there is no --application or --library command line argument, building applications is the default behaviour of minitscriptbuild tool. 

If you want to have your scripts executed native only or do not want to ship the script files in your application installation package, you can e.g. use the following command: 

```
minitscriptbuild --native-only main.tscript script1.tscript script2.tscript
```

If you want to build a .so(shared object file for Linux/BSD/MacOSX) file or a .dll(dynamic link library for Windows/MINGW) file, that can be embedded in other C++ applications, 
see an example below:  

```
minitscriptbuild --library script1.tscript script2.tscript
```

# 3. Other information

- this document and the Makefile build system itself is still WIP

## 3.1. Links

- MinitScript, see [README.md](./README.md)
- MinitScript - How to build, see [README-BuildingHowTo.md](./README-BuildingHowTo.md)
- MinitScript - How to use, see [README-Tools.md](./README-Tools.md)
- MinitScript - ToDo list, see [README-ToDo.md](./README-Tools.md)
- The Mindty Kollektiv [Discord Server](https://discord.gg/Na4ACaFD)
