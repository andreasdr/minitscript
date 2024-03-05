# 1. How to use MinitScript tools?

## 1.1. "minitscript" tool

minitscript is the MinitScript interpreter tool, which means scripts are translated to a syntax tree, which then gets executed. This can be called script interpretion.
The minitscript tool can take the following arguments:

## 1.1.1. Show usage help

```
minitscript --help
```  

## 1.1.2. Show version

```
minitscript --version
```  

## 1.1.3. Interpret a script 

Interpret a script and show additional script information, which can be handy if debugging a script

```
minitscript --verbose test.tscript
```  

## 1.1.4. Interpret a script and pass arguments to it 

Interpret a script and pass arguments to it, which can be read by application.getArguments() script method call

```
minitscript test.tscript --arguments --help
```  

## 1.1.5. Just interpret a script

```
minitscript test.tscript
```  

## 1.1.6. Without arguments 

minitscript can also be ran without arguments at all. This way you can feed minitscript tool with pipe input using '<'

```
minitscript < test.tscript
```  

## 1.1.7. Prompt 

You dont need to pipe a file into minitscript tool. So if you start minitscript tool without arguments you can enter a (partial) script and exit with 
control-z(Windows) and control-d(unixes and others) 

```
minitscript
```

Now you can enter e.g.

	console.printLine(1*2+3)
	
and press control-z(Windows) and control-d(unixes and others) to exit the prompt and execute the script.

## 1.2. minitscriptdocumentation
## 1.3. minitscriptlibrary
## 1.4. minitscriptmain
## 1.5. minitscriptmakefile
## 1.6. minitscripttranspiler
## 1.7. minitscriptuntranspiler

# 2. How to use MinitScript build script?

# 3. Other information

## 3.1. Links

- MinitScript, see [README.md](./README.md)
- MinitScript - How to build, see [README-BuildingHowTo.md](./README-BuildingHowTo.md)
- MinitScript - How to use, see [README-Tools.md](./README-Tools.md)
- The Mindty Kollektiv [Discord Server](https://discord.gg/Na4ACaFD)
