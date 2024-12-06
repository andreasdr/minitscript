![LOGO](https://raw.githubusercontent.com/andreasdr/minitscript/master/resources/github/minitscript-logo.png)

# 1. Flow Control - Loops

forTime and forCondition are loops and belong to flow control also:

forTime takes a single argument "time" - it repeats the loop until "time" in milliseconds has passed since the initialization of the loop. See an example of forTime() below: 
```
...
	$i = 0
	forTime(2000)
		console.printLine($i, ": Hello World")
		script.wait(500)
		++$i
	end
...
```

forCondition takes a single boolean value as argument. The loop will be executed as long as the argument is true. In this case "$i < 5" translates to "lesser($i, 5)" which results in a boolean value, which is used as the forCondition argument. See example of forCondition() below.
```
...
	$i = 0
	forCondition($i < 5)
		console.printLine("$ = ", $i)
		++$i
	end
...
```

TODO: for, break, continue

# 2. Links

## 2.1. Language Documentation
 - [Syntax](./README-Syntax.md)
 - [Data types](./README-DataTypes.md)
 - [Flow control - if, elseif, else](./README-FlowControl-Conditions.md)
 - [Flow Control - switch, case, default](./README-FlowControl-Conditions2.md)
 - [Flow Control - loops](./README-FlowControl-Loops.md)
 - [Classes](./README-Classes.md)
 - [Classes API](./README-Classes-API.md)
 - [Functions](./README-Functions.md)
 - [BuiltIn functions](./README-BuiltIn-Functions.md)
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
