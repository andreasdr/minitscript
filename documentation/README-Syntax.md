![LOGO](https://raw.githubusercontent.com/andreasdr/minitscript/master/resources/github/minitscript-logo.png)

# 1. Syntax

## 1.1. Introduction

MinitScript takes a simple approach regarding its syntax. There are no syntax keywords or similar. 
Everything, despite variables, constants and operators, is build around the idea of methods, functions and callables, 
which are identified by a name, including some namespace information, and arguments surrounded by brackets like:

E.g. namespace.functionName($arg0, ..., $argN)

You can simply list statements line by line.

```
	console.printLine("Hi there. Who has some serious interest in MinitScripting?")
	console.printLine("Me!")
	console.printLine("Me not!")
```

You can also separate statements using the semicolon.

```
	# one line, one statement
	console.printLine("Hi there. Who has some serious interest in MinitScripting?")
	# see line below with multiple statements
	console.printLine("Me!"); console.printLine("Me not!"); console.printLine("Never ever!")
```

This can also be used like:

```
	$array = [1,2,3]
	forEach($entry in $array); console.printLine($entry); end
```

## 1.2. Methods, functions and callables.

A method is provided by a C++ class and its member method, hence the name. 
In the documentation you find them in [BuiltIn functions](./documentation/README-BuiltIn-Functions.md) section.
This sounds a bit confusing, but makes sense.

Methods (and possibly a custom data type) can also be used to generate a classes like API, just look in the [Classes API](./documentation/README-Classes-API.md) section.

Function and callables are defined by the scripts itself. Callables are basically public functions for script interoperability with some more safety checks.
See section [Functions](./documentation/README-Functions.md)

In combination with map datatype, you can also generate classes like objects in MinitScript language. See section [Classes](./documentation/README-Classes.md).

## 1.3. Top level script syntax

At top level MinitScript can take the following syntax:
- event like approach:
  - on: ...
  - on-enabled: ...
- functions
  - function: ...
  - callable: ...
  - stacklet: ...

First see the event like approach, which we might change still. This is WIP.
The event like approach was invented to have the ability to have a simple state machine and action graph available as feasible script interpretation.

Initialization looks like:

```
on: initialize
	...
	console.printLine("Nothing's going on here, I need to get initialized")
	$initialization = true
	...
end	
```

If there was no event matched before in script event order, "nothing" gets executed, see below

```
on: nothing
	...
	console.printLine("Nothing's going on here, I am bored. Am I at least initialized?")
	if ($initialization == true)
		console.printLine("I am initialized")
	else
		console.printLine("I am NOT initialized")
	end
	script.stop()
	...
end	
```

In case of error see the error event.

```
on: error
	...
	console.printLine("There was an error. I am excited")
	console.printLine("Message: " + script.error.getMessage())
	console.printLine("Statement: " + script.error.getStatement())
	script.stop()
	...
end	
```

"on: initialize", "on: nothing" and "on: error" are default events. They just have a name by convention and are emitted, if required.
Events get more interesting with conditions and a specific order to match a required state machine or graph.

We might add a section for events later.

At top level script scope we still can define functions like:

```
function: factorial($value)
	if ($value == 0)
		return(1)
	end
	return($value * factorial($value - 1))
end
``` 

and callables like:

```
callable: helloWorldFunction()
	console.printLine("helloWorldFunction(): Hello world")
end
```

To have more information, I would suggest you to read the following sections next or on demand.
- [Data types, variables and constants](./README-DataTypes.md)
- [Flow control - if, elseif and else](./README-FlowControl-Conditions.md)
- [Flow control - switch, case and default](./README-FlowControl-Conditions2.md)
- [Flow control - forTime, forCondition, for and forEach](./README-FlowControl-Loops.md)
- [Functions](./README-Functions.md)
- [Lambda Functions](./README-Lambda-Functions.md)
- [Classes](./README-Classes.md)
- [Modules](./README-Modules.md)

# 2. Links

## 2.1. Language documentation
- [Syntax](./README-Syntax.md)
- [Data types, variables and constants](./README-DataTypes.md)
- [Flow control - if, elseif and else](./README-FlowControl-Conditions.md)
- [Flow control - switch, case and default](./README-FlowControl-Conditions2.md)
- [Flow control - forTime, forCondition, for and forEach](./README-FlowControl-Loops.md)
- [Functions](./README-Functions.md)
- [Lambda Functions](./README-Lambda-Functions.md)
- [Classes](./README-Classes.md)
- [Modules](./README-Modules.md)
- [Built-in functions](./README-BuiltIn-Functions.md)
- [Built-in classes](./README-BuiltIn-Classes.md)
- [Operators](./README-Operators.md)
- [Constants](./README-Constants.md)

## 2.2. Other links

- MinitScript, see [README.md](../README.md)
- MinitScript - How to build, see [README-BuildingHowTo.md](../README-BuildingHowTo.md)
- MinitScript - How to use, see [README-Tools.md](../README-Tools.md)
- The Mindty Kollektiv [Discord Server](https://discord.gg/Na4ACaFD)
