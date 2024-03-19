![LOGO](https://raw.githubusercontent.com/Mindty-Kollektiv/minitscript/master/resources/github/minitscript-logo.png)

# 1. Data Types and Variables 

Default MinitScript works with the following data types:
- boolean
- integer
- float
- string
- byte array
- array
- map
- set  

Variable identifiers always start with a "$". Constants need also be prefixed with a "$" and should be named with uppercase letters like "$PI = 3.14".
You can manually set variables with the setVariable("$variableName", ...) or constants with setConstant("$CONSTANT", ...) methods.

## 4.1. Primitive data types

The following primitive data types are available: boolean, integer, float, string.

Variables of those types can be assigned implicitly, which means the parser know about the data type by given value:

```
...
	$boolean = true
	$boolean = false
...
```

```
...
	$integer = 123
...
```

```
...
	$float = 456.789
...
```

```
...
	$string = "This is my mighty string"
...
```

The primitive data types can also be assigned by using initialization methods that return explicitly those primitive data types:

```
...
	$boolean = boolean(true)
	$boolean = boolean(false)
...
```

```
...
	$integer = integer(123)
...
```

```
...
	$float = float(456.789)
...
```

```
...
	$string = string("This is my mighty string")
...
```

# 2. Links

## 2.1. Language Documentation
 - [Syntax](./README-Syntax.md)
 - [DataTypes and variables](./README-DataTypes%20and%20Variables.md)
 - [Flow control - If, elseif, else](./README-FlowControl-Conditions.md)
 - [Flow Control - Switch, Case, Default](./README-FlowControl-Conditions2.md)
 - [Flow Control - Loops](./README-FlowControl-Loops.md)
 - [Classes](./README-Classes.md)
 - [Classes API](./README-Classes-API.md)
 - [Functions](./README-Functions.md)
 - [BuiltIn Functions](./README-BuiltIn-Functions.md)
 - [Functions](./README-Functions.md)
 - [Operators](./README-Operators.md)
 - [Syntax](./README-Events.md)
 - [Events](./README-Constants.md)

## 2.2. Other Links

- MinitScript, see [README.md](./README.md)
- MinitScript - How to build, see [README-BuildingHowTo.md](./README-BuildingHowTo.md)
- MinitScript - How to use, see [README-Tools.md](./README-Tools.md)
- MinitScript - ToDo list, see [README-ToDo.md](./README-ToDo.md)
- The Mindty Kollektiv [Discord Server](https://discord.gg/Na4ACaFD)
