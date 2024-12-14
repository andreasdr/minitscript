![LOGO](https://raw.githubusercontent.com/andreasdr/minitscript/master/resources/github/minitscript-logo.png)

# 1. Flow Control - If, elseif and else

If, elseif and else are flow control methods. Please see the usage below.

See an example of if:
```
...
	$i = 1
	if ($i == 1)
		console.printLine("i -> 1")
	end
...
```

See an example of if and else:
```
...
	$i = 2
	if ($i == 1)
		console.printLine("i -> 1")
	else
		console.printLine("else: ", $i)
	end
...
```

See an example of if, elseif and else:

```
...
	$i = 2
	console.printLine($i, ":")
	if ($i == 0)
		console.printLine("i -> 0")
	elseif ($i == 1)
		console.printLine("i -> 1")
	elseif ($i == 2)
		console.printLine("i -> 2")
	elseif ($i == 3)
		console.printLine("i -> 3")
	else
		console.printLine("else: ", $i)
	end
...
```

# 2. Links

## 2.1. Language Documentation
- [Syntax](./README-Syntax.md)
- [DataTypes](./README-DataTypes.md)
- [Flow control - if, elseif, else](./README-FlowControl-Conditions.md)
- [Flow Control - switch, case, default](./README-FlowControl-Conditions2.md)
- [Flow Control - loops](./README-FlowControl-Loops.md)
- [Classes](./README-Classes.md)
- [Classes API](./README-Classes-API.md)
- [Functions](./README-Functions.md)
- [BuiltIn functions](./README-BuiltIn-Functions.md)
- [Modules](./README-Modules.md)
- [Operators](./README-Operators.md)
- [Constants](./README-Constants.md)

## 2.2. Other Links

- MinitScript, see [README.md](../README.md)
- MinitScript - How to build, see [README-BuildingHowTo.md](../README-BuildingHowTo.md)
- MinitScript - How to use, see [README-Tools.md](../README-Tools.md)
- The Mindty Kollektiv [Discord Server](https://discord.gg/Na4ACaFD)
