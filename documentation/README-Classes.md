![LOGO](https://raw.githubusercontent.com/Mindty-Kollektiv/minitscript/master/resources/github/minitscript-logo.png)

# 1. Classes

Classes in MinitScript can be represented by maps, a constructed map with function assignments and/or definitions can be called object, see map section in 4.4.

Creating a object in MinitScript works by using map initializer plus () -> methodName function assignment, or () -> { console.dump($this) } inline function definition.
Please see a example below.

```
...
	#
	$car = {
		# member variables
		wheelCount: 4,
		color: "blue",
		horsePower: 75,
		# member methods
		setWheelCount: ($wheelCount) -> 
			{
				$this.wheelCount = $wheelCount
			},
		setColor: ($color) ->
			{
				$this.color = $color
			},
		setHorsePower: ($horsePower) -> 
			{
				$this.horsePower = $horsePower
			},
		showMeWhatYouGot: () ->
			{
				console.printLine(
					"This amazing car has ", 
					$this.wheelCount,
					" wheels, is colored ", 
					$this.color,
					" with super nice ", 
					$this.horsePower,
					" horses in it"
				)
			},
		getProperties: (&$wheelCount, &$color, &$horsePower) -> 
			{
				$wheelCount = $this.wheelCount
				$color = $this.color
				$horsePower = $this.horsePower
			}
	}
	#
	console.dump($car)
	# arrr, lets see what kind of car we got
	$car->showMeWhatYouGot()
	# i want it in red with 3 wheels and 25 mighty horse power
	$car->setColor("red")
	$car->setWheelCount(3)
	$car->setHorsePower(25)
	# arrr, lets see what kind of car we got now!
	$car->showMeWhatYouGot()
	# lets get the properties
	$wheelCount = null
	$color = null
	$horsePower = null
	$car->getProperties($wheelCount, $color, $horsePower)
	console.printLine(
		"Car properties, wheels: ", 
		$wheelCount,
		", color: ", 
		$color,
		", horse power: ", 
		$horsePower
	)
	#...
```

If you want to assign a object member function later, see this example. Note that the first argument needs to be a reference or value by copy $this variable.
This argument maps to the object that your code is operating on in your object member function.
 
Be aware that value by copy variables usually require more instantiation time due to the copy that needs to be made of the variable from parent variable scope to function variable scope.  

```
...
function: setConvertible(&$this, $convertible)
	$this.convertible = $convertible
end

function: showMeWhatYouGot($this)
	$carType = "car"
	if ($this.convertible == true)
		$carType = "convertible"
	end
	console.printLine(
		"This amazing ", 
		$carType,
		" has ", 
		$this.wheelCount,
		" wheels, is colored ", 
		$this.color,
		" with super nice ", 
		$this.horsePower,
		" horses in it"
	)
end
	...
	#
	$car.convertible = false
	$car.setConvertible = () -> setConvertible
	$car.showMeWhatYouGot = () -> showMeWhatYouGot
	#
	console.dump($car)
	# I want a convertible, who doesn't?
	$car->setConvertible(true)
	# arrr, lets see what kind of car we got
	$car->showMeWhatYouGot()
	#...
```

TODO: private properties, && argument prefix

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
