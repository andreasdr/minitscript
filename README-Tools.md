# 1. How to use MinitScript tools?

## 1.1. "minitscript" tool

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

You dont need to pipe a file into minitscript tool. So if you start minitscript tool without arguments you can enter a (partial) script and exit with 
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
 
## 1.5. minitscriptlibrary
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
minitscriptnmakefile tsrc TMakefile
```

The first argument is the path name, where we transpiled our scripts to, and where we added optional MinitScript main files and a MinitScript library C++ file.
The second argument is just the Makefile file name.

To create a build of your MinitScript application you can just run "nmake -f TMakefile". 

You can also generate a .dll(dynamic link library for Windows) file, to be embedded in other C++ applications.
In this case the first argument needs to be "--library". 

Please see an example below:

```
minitscriptnmakefile --library tsrc TMakefile
```

## 1.8. minitscriptdocumentation

The "minitscriptdocumentation" tool is experimental. 

Basically it shows MinitScript classes, methods, constants and operators.
Optionally it can show documentation keys also, that can be used to help with documenting MinitScript methods.

# 2. How to use MinitScript build script?

# 3. Other information

## 3.1. Links

- MinitScript, see [README.md](./README.md)
- MinitScript - How to build, see [README-BuildingHowTo.md](./README-BuildingHowTo.md)
- MinitScript - How to use, see [README-Tools.md](./README-Tools.md)
- The Mindty Kollektiv [Discord Server](https://discord.gg/Na4ACaFD)
