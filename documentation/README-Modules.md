![LOGO](https://raw.githubusercontent.com/andreasdr/minitscript/master/resources/github/minitscript-logo.png)

# 1. Modules

MinitScript supports module script files, in short modules, including the transpilation of them.

## 1.1. Creating a module

You are declaring a script file as module script file by the "module" statement.
Module script files are only allowed to have functions, callables and stacklets.

This is module_1.tscript, which will be used in module-test.tscript in section 1.2.

```
module

# this function is provided by our module_1.tscript
function: module1Function()
	console.printLine("-----------------")
	console.printLine("module1Function")
	console.printLine("-----------------")
end
```

## 1.2. Using a module

You are including a module by the "use: " statement. After the colon the relative path to the script module file is required.
Note that modules can also include modules.  

See a module-test.tscript, which uses module_1.tscript from the section 1.1. above.

```
use: module_1.tscript

# initialize
on: initialize
	console.printLine("-------------------------")
	console.printLine("module-test: Initialize")
	console.printLine("-------------------------")
	console.printLine()
end

# if no condition is met, nothing will be executed, lol :D
on: nothing
	console.printLine("-----------------------")
	console.printLine("module-test: Nothing")
	console.printLine("-----------------------")
	console.printLine()
	# call module 1 functions
	console.printLine("main: Calling module1Function() from module_1.tscript")
	module1Function()
	# done
	script.stop()
end

# an error has occurred
on: error
	console.printLine("--------------------")
	console.printLine("module-test: Error")
	console.printLine("--------------------")
	console.printLine("An error occurred")
	script.stop()
end
```

# 2. Links

## 2.1. Language Documentation
- [Syntax](./README-Syntax.md)
- [DataTypes](./README-DataTypes.md)
- [Flow control - if, elseif, else](./README-FlowControl-Conditions.md)
- [Flow Control - switch, case, default](./README-FlowControl-Conditions2.md)
- [Flow Control - loops](./README-FlowControl-Loops.md)
- [Functions](./README-Functions.md)
- [Lambda Functions](./README-Lambda-Functions.md)
- [Classes](./README-Classes.md)
- [Modules](./README-Modules.md)
- [BuiltIn functions](./README-BuiltIn-Functions.md)
- [Classes API](./README-Classes-API.md)
- [Operators](./README-Operators.md)
- [Constants](./README-Constants.md)

## 2.2. Other Links

- MinitScript, see [README.md](../README.md)
- MinitScript - How to build, see [README-BuildingHowTo.md](../README-BuildingHowTo.md)
- MinitScript - How to use, see [README-Tools.md](../README-Tools.md)
- The Mindty Kollektiv [Discord Server](https://discord.gg/Na4ACaFD)
