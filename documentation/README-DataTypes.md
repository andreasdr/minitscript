![LOGO](https://raw.githubusercontent.com/andreasdr/minitscript/master/resources/github/minitscript-logo.png)

# 1. Data Types

Default MinitScript works with the following data types:
- boolean
- integer
- float
- string
- byte array
- array
- map
- set  

Variable and constant identifiers always start with a "$".
You can manually set variables with the setVariable("$variableName", ...) or constants with setConstant("$CONSTANT", ...) methods.

## 2. Primitive data types

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
- [Syntax](./documentation/README-Syntax.md)
- [DataTypes](./documentation/README-DataTypes.md)
- [Flow control - if, elseif, else](./documentation/README-FlowControl-Conditions.md)
- [Flow Control - switch, case, default](./documentation/README-FlowControl-Conditions2.md)
- [Flow Control - loops](./documentation/README-FlowControl-Loops.md)
- [Classes](./documentation/README-Classes.md)
- [Classes API](./documentation/README-Classes-API.md)
- [Functions](./documentation/README-Functions.md)
- [BuiltIn functions](./documentation/README-BuiltIn-Functions.md)
- [Modules](./documentation/README-Modules.md)
- [Operators](./documentation/README-Operators.md)
- [Constants](./documentation/README-Constants.md)

## 2.2. Other Links

- MinitScript, see [README.md](./README.md)
- MinitScript - How to build, see [README-BuildingHowTo.md](./README-BuildingHowTo.md)
- MinitScript - How to use, see [README-Tools.md](./README-Tools.md)
- The Mindty Kollektiv [Discord Server](https://discord.gg/Na4ACaFD)
