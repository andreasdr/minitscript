![LOGO](https://raw.githubusercontent.com/andreasdr/minitscript/master/resources/github/minitscript-logo.png)

# 1. Data types, variables and constants

Default MinitScript works with the following data types:
- boolean
- integer
- float
- string
- array
- map
- set
- ...

Variable and constant identifiers always start with a "$".
You can manually set variables with the setVariable("$variableName", ...) or constants with setConstant("$CONSTANT", ...) methods.

## 1.1. Primitive data types

The following primitive data types are available: boolean, integer, float.

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

## 1.2. Additional data types

Additionally we have string, array, set and map datatypes. See some initialization examples below:

See how to initialize a string.

```
...
	$string = "This is my mighty string"
...
```

See how to initialize an array.

```
...
	$array = ["a", "b", "c"]
...
```

See how to initialize a set.

```
...
	$set = {"a", "b", "c"}
...
```

See how to initialize a map.

```
...
	$map = {"a": 1, "b": 2, "c": 3}
...
```

For more information about how to work with those data types, check [Classes API](./README-Classes-API.md).

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
