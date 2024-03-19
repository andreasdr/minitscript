![LOGO](https://raw.githubusercontent.com/Mindty-Kollektiv/minitscript/master/resources/github/minitscript-logo.png)

# 1. Flow Control - Switch, case and default

Switch, case, and default are flow control methods too. Please see the usage below.

See an example:

```
...
	#
	$i = 1
	switch($i)
		case(0)
			console.printLine("$i = 0")
		end
		case(1)
			console.printLine("$i = 1")
		end
		case(2)
			console.printLine("$i = 2")
		end
		default
			console.printLine("default: $i = " + $i)
		end
	end
...
```

Please note, default block is optional.

# 2. Links

## 2.1. Language Documentation
 - [Syntax](./README-Syntax.md)
 - [Data types and variables](./README-DataTypes-and-Variables.md)
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
