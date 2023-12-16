This is the home of the MiniScript language. This document and repository is WIP. Expect a BETA release around 2024-01-15. 

# 1. Introduction

MiniScript was developed as part of our [TDME2 3D Engine](https://github.com/Mindty-Kollektiv/tdme2) to match
the requirements for high performance script execution, as well as straight forward game logic scripting.

MiniScript might borrow some ideas from JavaScript, Kotlin, PHP and even C++, which you might like and find familier.
Also note the focus on easy integration into other products and easy customizability and extendability.

## 1.1. Open source

- MiniScript is open source
  - please check the [license](https://github.com/Mindty-Kollektiv/miniscript/blob/master/LICENSE)
  - you find the source code at [https://github.com/Mindty-Kollektiv/miniscript](https://github.com/Mindty-Kollektiv/miniscript) 
  - here you find developer [documentation including collaboration graphs and API](https://www.mindty.com/products/miniscript/documentation/)

## 1.2. Language features

- Introduction to MiniScript language features:
  - very small implementation of a scripting language
  - runs on every CPU, OS, ... due to its simplicity, so its highly portable
  - can be easily extended by writing state machine machine states and script methods in C++ as well as custom data types
  - built-in data types: null, boolean, integer, float, string, byte array, array, map and set
  - when calling script C++ methods or script functions with arguments it does optionally use references or value by copy
  - supports functions and recursion
  - supports inline/lamda functions
  - supports operators by operator to method mapping by a preprocessor run
  - supports loops and conditions
  - supports programming with classes style programming
    - for built-in datatypes: string, byte array, array, map and set
    - for script classes/objects
    - for custom data types
  - supports event like programming
  - unicode support via UTF8
  - can be transpiled to C++

# 2. Flow control

## 2.1. If, elseif, else and end

If, elseif, else and end are flow control methods. Please see the usage below.

See an example of if and end:
```
...
	$i = 1
	if ($i == 1)
		console.log("i -> 1")
	end
...
```

See an example of if and else and end:
```
...
	$i = 2
	if ($i == 1)
		console.log("i -> 1")
	else
		console.log("else: ", $i)
	end
...
```

See an example of if, elseif, else and end:

```
...
	$i = 2
	console.log($i, ":")
	if ($i == 0)
		console.log("i -> 0")
	elseif ($i == 1)
		console.log("i -> 1")
	elseif ($i == 2)
		console.log("i -> 2")
	elseif ($i == 3)
		console.log("i -> 3")
	else
		console.log("else: ", $i)
	end
...
```

## 2.2. forTime, forCondition

forTime and forCondition are loops and belong to flow control also:

forTime takes a single argument "time" - it repeats the loop until "time" in milliseconds has passed since the initialization of the loop. See an example of forTime() below: 
```
...
	$i = 0
	forTime(2000)
		console.log($i, ": Hello World")
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
		console.log("$ = ", $i)
		++$i
	end
...
```

# 3. Functions/Callables

See this example that shows functions and recursion.

Argument variables, parsed in function declarations are populated in function context with corresponding values.
```
...
# function of recursive factorial computation
function: factorial($value)
	console.log("factorial(): $value = " + $value)
	if ($value == 0) 
		return(1)
	end
	return($value * factorial($value - 1))
end
...
	console.log("factorial(5) = " + factorial(5))
...
```

If a argument(variable) is prefixed with a & operator in the function declaration, the variable will not be copied into the function arguments, 
but a reference will be created, means if this variable changes within the function it will also change in the parent variable scope.
See &$b and &$c.

Be aware that value by copy variables usually require more instantiation time due to the copy that needs to be made of the variable from parent variable scope to function variable scope. 
```
...
# function to test references in user functions
function: assignTest($a, &$b, &$c)
	$a = "a"
	$b = "b"
	$c = "c"
end
...
	$a = "0"
	$b = "1"
	$c = "2"
	console.log("assignTest(): pre: $a = " + $a + ", $b = " + $b + ", $c = " + $c)
	assignTest($a, $b, $c)
	console.log("assignTest(): post: $a = " + $a + ", $b = " + $b + ", $c = " + $c)
...
``` 

Global variables can always be accessed by using the "$GLOBAL." accessor.
By default variables are read from current context and if they have not been found from root context.
So to be sure to use a global variable in function scope, just use the "$GLOBAL." accessor.
```
...
# function to test global variable access
function: globalVariableTest()
	console.log("globalVariableTest(): $GLOBAL.globalTest = " + $GLOBAL.globalTest)
	$GLOBAL.globalTest = "Been there, done that, got the t-shirt"
end
...
	$globalTest = "Global Test Variable"
	console.log("globalVariableTest(): pre: $globalTest = " + $globalTest)
	globalVariableTest()
	console.log("globalVariableTest(): post: $globalTest = " + $globalTest)
...
```

A special type of functions are callables. Callables are functions that are used to interact between MiniScript scripts.
 
Despite the fact that a callable function of a script can be called from another script, 
they have the limitation that they must not contain MiniScript C++ method calls that require a context function.

Context functions are functions that require a special context. You can ignore this fow now. 
Default MiniScript does not provide script methods by C++ that require a context. 

```
...
# call this callable to select this unit 
callable: select()
	$GLOBAL.selectMode = "select"
end
...
# call this callable to unselect this unit 
callable: unselect()
	$GLOBAL.selectMode = "unselect"
end
...
# update engine (context) function
function: updateEngine()
	...
	# selection
	if ($GLOBAL.selectMode == "select")
		engine.entity.setEnabled(logic.getId(), true, "selection")
		$GLOBAL.selectMode = "none"
	elseif ($GLOBAL.selectMode == "unselect")
		engine.entity.setEnabled(logic.getId(), false, "selection")
		$GLOBAL.selectMode = "none"
	end
	...
end
...
```

# 4. Data types and variables

Default MiniScript works with the following data types:
- boolean
- integer
- float
- string
- byte array
- array
- map
- set  

Variable identifiers always start with a "$". Constants need also be prefixed with a "$" and should be named with uppercase letters like "$PI = 3.14".
You can manually set variables with the setVariable("$variableName", ...) or constants with setConstant("$CONSTANT", ...) method.

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
	$boolean = bool(true)
	$boolean = bool(false)
...
```

```
...
	$integer = int(123)
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

## 4.2. Byte arrays

A byte array is a collection/sequence of byte values which can be accessed by indices. Bytes are the smallest atomic values a CPU does handle.
A byte has a value of 0...255. Using bit math you can also manipulate byte values at bit scope.

Usually byte arrays can be used to exchange/construct network packets/streams, texture data, mesh data, ...
Also, using a byte array instead of a generic array for byte storage, results in using much less memory space. 

Initializing a byte array by constructor:

```
...
	$byteArray = ByteArray()
...
```

... or initialize and push values to it:
```
...
	$byteArray = ByteArray(1, 2, 3)
...
```

Pushing values using ByteArray::push():
```
...
	$byteArray->push(5, 6, 7)
...
```

Pushing values using [] operator:
```
...
	$byteArray[] = 8
	$byteArray[] = 9
	$byteArray[] = 10
...
```

Iterating byte arrays using ByteArray::length() and ByteArray::get():
```
...
	$i = 0
	forCondition($i < $byteArray->length())
		console.log($i + ": " + $byteArray->get($i))
		++$i
	end
...
```

Iterating byte arrays using ByteArray::length() and [] operator:
```
...
	$i = 0
	forCondition($i < $byteArray->length())
		console.log($i + ": " + $byteArray[$i])
		++$i
	end
...
```

Removing from byte arrays using a index with ByteArray::remove():
```
...
	$byteArray->remove(2)
...
```

## 4.3. Arrays

An array is a collection/sequence of values which can be accessed by indices.

Initializing an array by array initializer:

```
...
	$array = [1, 2, 3]
...
```

Initializing an array by constructor:

```
...
	$array = Array()
...
```

... or initialize and push values to it:
```
...
	$array = Array(1, 2, 3)
...
```

Pushing values using Array::push():
```
...
	$array->push(5, 6, 7)
...
```

Pushing values using [] operator:
```
...
	$array[] = 8
	$array[] = 9
	$array[] = 10
...
```

Removing values from arrays using Array::removeOf():
```
...
	$array->removeOf(6)
	$array->removeOf(7)
...
```

Removing from arrays using a index with Array::remove():
```
...
	$array->remove(2)
...
```
Iterating arrays using Array::length() and Array::get():
```
...
	$i = 0
	forCondition($i < $array->length())
		console.log($i + ": " + $array->get($i))
		++$i
	end
...
```

Iterating arrays using Array::length() and [] operator:
```
...
	$i = 0
	forCondition($i < $array->length())
		console.log($i + ": " + $array[$i])
		++$i
	end
...
```

Iterating arrays using Array::forEach() and a lamda function
```
...
	$array->forEach(($value) -> { console.log($value) })
...
```

## 4.4. Maps

A map is key, value pair storage using a underlying hash map. Keys can only exist once in a map.

Initializing a map by map initializer
```
...
	$map = {"test1": 123, "test2": 456, "test3": [1, 2, 3], "test4": "Yaaaa"}
...
```

Initializing a map by map constructor:
```
...
	$map = Map()
...
```

Setting map key, value pairs using Map::set():
```
...
	$map->set("test1", 123)
	$map->set("test2", 456)
	$map->set("test3", array(1, 2, 3))
	$map->set("test4", "Yaaaa")
...
```

Removing from map using Map::remove() and a given key:
```
...
	$map->remove("test2")
...
```

Reading values from map using Map::get() and given keys:
```
...
	console.log("map value for test1 key using map.get(): ", $map->get("test1"))
	console.log("map value for test2 key using map.get(): ", $map->get("test2"))
	console.log("map value for test3 key using map.get(): ", $map->get("test3"))
	console.log("map value for test4 key using map.get(): ", $map->get("test4"))
...
```

Reading values from map using dot operator:
```
...
	console.log("map value for test1 using map dot operator: ", $map.test1)
	console.log("map value for test2 using map dot operator: ", $map.test2)
	console.log("map value for test3 using map dot operator: ", $map.test3)
	console.log("map value for test4 using map dot operator: ", $map.test4)
...
```

Setting key, value pairs to map using dot operator:
```
...
	$map.test6 = 666
	$map.test7 = 770
...
```

Reading map keys:
```
...
	console.log("map keys: ", $map->getKeys())
...
```

Reading map values:
```
...
	console.log("map values: ", $map->getValues())
...
```

Reading all keys and values from map using Map::get() and Map::getKeys()
```
...
	$mapKeys = $map->getKeys()
	$i = 0
	forCondition($i < $mapKeys->length())
		console.log($mapKeys[$i] + " = " + $map->get($mapKeys[$i]))
		++$i
	end
...
```

Iterating maps using Map::forEach() and a lamda function
```
...
	$map->forEach(($key, $value) -> { console.log($key + " = " + $value) })
...
```

## 4.5. Sets

A set is key storage using a underlying hash set. Keys can only exist once in a set.

Initializing a set by set initializer
```
...
	$set = {"test1", "test2", "test3"}
...
```

Initializing a set by set constructor
```
...
	$set = Set()
...
```

Inserting keys into set using Set::insert():
```
...
	$set->insert("test1")
	$set->insert("test2")
	$set->insert("test3")
...
```

Removing keys from set using Set::remove():
```
...
	$set->remove("test2")
...
```

Checking if keys exist in set using Set::has() and given keys:
```
...
	console.log("set does have test1 key using set.has(): ", $set->has("test1"))
	console.log("set does have test2 key using set.has(): ", $set->has("test2"))
	console.log("set does have test3 key using set.has(): ", $set->has("test3"))
	console.log("set does have test4 key using set.has(): ", $set->has("test4"))
	console.log("set does have test5 key using set.has(): ", $set->has("test5"))
...
```


Checking if keys exist in set using dot operator and given keys:
```
...
	console.log("set key for test1 using set dot operator: ", $set.test1)
	console.log("set key for test2 using set dot operator: ", $set.test2)
	console.log("set key for test3 using set dot operator: ", $set.test3)
	console.log("set key for test4 using set dot operator: ", $set.test4)
	console.log("set key for test5 using set dot operator: ", $set.test5)
...
```

Inserting/Removing set keys by using dot operator and boolean assignment:
```
...
	$set.test6 = true
	$set.test7 = true
	$set.test8 = false
	$set.test9 = true
...
```

Reading all keys as array from set:
```
...
	console.log("set keys: ", $set->getKeys())
...
```

Iterating sets using Set::forEach() and a lamda function
```
...
	$set->forEach(($key) -> { console.log($key) })
...
```

## 4.6. Classes

Classes in MiniScript can be represented by maps, a constructed map with function assignments and/or definitions can be called object, see map section in 4.4.

Creating a object in MiniScript works by using map initializer plus () -> methodName function assignment, or () -> { console.dump($this) } inline function definition.
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
				console.log(
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
	console.log(
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
	console.log(
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

# 5. Program structure and flow

... TODO ...

# 6. MiniScript classes

## 6.1. String class

| &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Table of methods &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; |
|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| &nbsp;                                    |
| <b>STATIC METHODS</b>                     |
| Create string                                                                                    |
| <sub><b>static</b> String($string: String): String</sub>                                         |
| Concatenate strings                                                                              |
| <sub><b>static</b> String::concatenate(...): String</sub>                                        |
| Create string from byte array                                                                    |
| <sub><b>static</b> String::fromByteArray($byteArray: ByteArray): String</sub>                    |
| Create spaces as string                                                                          |
| <sub><b>static</b> String::space([$spaces: Integer]): String</sub>                               |
| &nbsp;                                    |
| <b>NON STATIC METHODS</b>                 |
| Return character of string at given position                                                     |
| <sub>charAt($index: Integer): String</sub>                                                       |
| Test if string ends with specific string                                                         |
| <sub>endsWith($suffix: String): Boolean</sub>                                                    |
| Test if strings matches ignoring case sensitivity                                                |
| <sub>equalsIgnoreCase($other: String): Boolean</sub>                                             |
| Return first index of specific string in string                                                  |
| <sub>firstIndexOf($what: String[, $beginIndex: Integer]): Integer</sub>                          |
| Return index of specific string in string                                                        |
| <sub>indexOf($what: String[, $beginIndex: Integer]): Integer</sub>                               |
| Test if string value is empty                                                                    |
| <sub>isEmpty(): Boolean</sub>                                                                    |
| Test if string value is a float number                                                           |
| <sub>isFloat(): Boolean</sub>                                                                    |
| Test if string value is a integer number                                                         |
| <sub>isInteger(): Boolean</sub>                                                                  |
| Return last index of specific string in string                                                   |
| <sub>lastIndexOf($what: String[, $beginIndex: Integer]): Integer</sub>                           |
| Return string length                                                                             |
| <sub>length(): Integer</sub>                                                                     |
| Pad string left                                                                                  |
| <sub>padLeft($by: String, $toLength: Integer): String</sub>                                      |
| Pad string right                                                                                 |
| <sub>padRight($by: String, $toLength: Integer): String</sub>                                     |
| RegEx match                                                                                      |
| <sub>regexMatch($pattern: String): Boolean</sub>                                                 |
| RegEx replace                                                                                    |
| <sub>regexReplace($pattern: String, $by: String): String</sub>                                   |
| Replace specific string in string with given string                                              |
| <sub>replace($what: String, $by: String[, $beginIndex: Integer]): String</sub>                   |
| Test if given string starts with specific string                                                 |
| <sub>startsWith($prefix: String): Boolean</sub>                                                  |
| Return substring of string                                                                       |
| <sub>substring($beginIndex: Integer[, $endIndex: Integer]): String</sub>                         |
| Convert string to byte array                                                                     |
| <sub>toByteArray(): ByteArray</sub>                                                              |
| Compute lower case string of string                                                              |
| <sub>toLowerCase(): String</sub>                                                                 |
| Compute upper case string of string                                                              |
| <sub>toUpperCase(): String</sub>                                                                 |
| Tokenize string                                                                                  |
| <sub>tokenize($delimiters: String): Array</sub>                                                  |
| Trim string                                                                                      |
| <sub>trim(): String</sub>                                                                        |

## 6.2. Byte array class

| &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Table of methods &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; |
|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| &nbsp;                                    |
| <b>STATIC METHODS</b>                     |
| Create a byte array                                                                              |
| <sub><b>static</b> ByteArray(...): ByteArray</sub>                                               |
| &nbsp;                                    |
| <b>NON STATIC METHODS</b>                 |
| Append another byte array to this byte array                                                     |
| <sub>appendByteArray(&$other: ByteArray): Null</sub>                                             |
| Clear byte array                                                                                 |
| <sub>clear(): Null</sub>                                                                         |
| Extract a byte array from this byte array                                                        |
| <sub>extractByteArray($index: Integer, $length: Integer): ByteArray</sub>                        |
| Get byte array entry                                                                             |
| <sub>get($index: Integer): Integer</sub>                                                         |
| Get byte array length                                                                            |
| <sub>length(): Integer</sub>                                                                     |
| Add entry to byte array                                                                          |
| <sub>push(...): Null</sub>                                                                       |
| Remove entry from byte array                                                                     |
| <sub>remove($index: Integer): Null</sub>                                                         |
| Set byte array entry                                                                             |
| <sub>set($index: Integer, $value: Integer): Null</sub>                                           |

## 6.3. Array class

| &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Table of methods &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; |
|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| &nbsp;                                    |
| <b>STATIC METHODS</b>                     |
| Create array                                                                                     |
| <sub><b>static</b> Array(...): Array</sub>                                                       |
| &nbsp;                                    |
| <b>NON STATIC METHODS</b>                 |
| Clear array                                                                                      |
| <sub>clear(): Null</sub>                                                                         |
| Iterate array values, by using a (Lamda) function                                                |
| <sub>forEach($function: Function[, &$cookie: Mixed]): Null</sub>                                 |
| Iterate range of array values, by using a (Lamda) function                                       |
| <sub>forRange($function: Function, $beginIndex: Integer[, $count: Integer[, $step: Integer[, &$cookie: Mixed]]]): Null</sub>|
| Get array entry                                                                                  |
| <sub>get($index: Integer): Mixed</sub>                                                           |
| Get array index by value                                                                         |
| <sub>indexOf($value: String[, $beginIndex: Integer]): Integer</sub>                              |
| Get array length                                                                                 |
| <sub>length(): Integer</sub>                                                                     |
| Add entry to array                                                                               |
| <sub>push(...): Null</sub>                                                                       |
| Remove array entry by index                                                                      |
| <sub>remove($index: Integer): Null</sub>                                                         |
| Remove array entry by value                                                                      |
| <sub>removeOf($value: String[, $beginIndex: Integer]): Null</sub>                                |
| Reverse array                                                                                    |
| <sub>reverse(): Null</sub>                                                                       |
| Set array entry                                                                                  |
| <sub>set($index: Integer, $value: Mixed): Null</sub>                                             |
| Sort array                                                                                       |
| <sub>sort($function: Function): Null</sub>                                                       |

## 6.4. Map class

| &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Table of methods &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; |
|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| &nbsp;                                    |
| <b>STATIC METHODS</b>                     |
| Create map                                                                                       |
| <sub><b>static</b> Map(): Map</sub>                                                              |
| &nbsp;                                    |
| <b>NON STATIC METHODS</b>                 |
| Clear map                                                                                        |
| <sub>clear(): Null</sub>                                                                         |
| Iterate map key and value pairs, by using a (Lamda) function                                     |
| <sub>forEach($function: Function[, &$cookie: Mixed]): Null</sub>                                 |
| Get map value by key                                                                             |
| <sub>get($key: String): Mixed</sub>                                                              |
| Get map keys                                                                                     |
| <sub>getKeys(): Array</sub>                                                                      |
| Get map values                                                                                   |
| <sub>getValues(): Array</sub>                                                                    |
| Has entry by key                                                                                 |
| <sub>has($key: String): Boolean</sub>                                                            |
| Remove map entry                                                                                 |
| <sub>remove($key: String): Null</sub>                                                            |
| Set map entry                                                                                    |
| <sub>set($key: String, $value: Mixed): Null</sub>                                                |

## 6.5. Set class

| &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Table of methods &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; |
|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| &nbsp;                                    |
| <b>STATIC METHODS</b>                     |
| Create set                                                                                       |
| <sub><b>static</b> Set(): Set</sub>                                                              |
| &nbsp;                                    |
| <b>NON STATIC METHODS</b>                 |
| Clear set                                                                                        |
| <sub>clear(): Null</sub>                                                                         |
| Iterate set keys, by using a (Lamda) function                                                    |
| <sub>forEach($function: Function[, &$cookie: Mixed]): Null</sub>                                 |
| Get set keys                                                                                     |
| <sub>getKeys(): Array</sub>                                                                      |
| Has key in set                                                                                   |
| <sub>has($key: String): Boolean</sub>                                                            |
| Insert key into set                                                                              |
| <sub>insert($key: String): Null</sub>                                                            |
| Remove key from set                                                                              |
| <sub>remove($key: String): Null</sub>                                                            |

# 7. MiniScript Methods

## 7.1. Base methods

| &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Table of methods &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; |
|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| Set constant                                                                                     |
| <sub>setConstant($constant: String, $value: Mixed): Mixed</sub>                                  |
| Get variable                                                                                     |
| <sub>getVariable($variable: String): Mixed</sub>                                                 |
| Set variable                                                                                     |
| <sub>setVariable($variable: String, $value: Mixed): Mixed</sub>                                  |
| Unset variable                                                                                   |
| <sub>unsetVariable($variable: String): Null</sub>                                                |
| Add                                                                                              |
| <sub>add($a: Mixed, $b: Mixed): Mixed</sub>                                                      |
| Logical and                                                                                      |
| <sub>and($a: Boolean, $b: Boolean): Boolean</sub>                                                |
| Bitwise and                                                                                      |
| <sub>bitwiseAnd($a: Integer, $b: Integer): Integer</sub>                                         |
| Bitwise not                                                                                      |
| <sub>bitwiseNot($value: Integer): Integer</sub>                                                  |
| Bitwise or                                                                                       |
| <sub>bitwiseOr($a: Integer, $b: Integer): Integer</sub>                                          |
| Bitwise xor                                                                                      |
| <sub>bitwiseXor($a: Integer, $b: Integer): Integer</sub>                                         |
| Create bool                                                                                      |
| <sub>bool($bool: Boolean): Boolean</sub>                                                         |
| Divide                                                                                           |
| <sub>div($a: Mixed, $b: Mixed): Mixed</sub>                                                      |
| Else                                                                                             |
| <sub>else(): Null</sub>                                                                          |
| Else if                                                                                          |
| <sub>elseif($condition: Boolean): Null</sub>                                                     |
| End                                                                                              |
| <sub>end(): Null</sub>                                                                           |
| Equals                                                                                           |
| <sub>equals($a: Mixed, $b: Mixed): Boolean</sub>                                                 |
| For condition                                                                                    |
| <sub>forCondition($condition: Boolean): Null</sub>                                               |
| For time                                                                                         |
| <sub>forTime($time: Integer): Null</sub>                                                         |
| Greater                                                                                          |
| <sub>greater($a: Mixed, $b: Mixed): Boolean</sub>                                                |
| Greater equals                                                                                   |
| <sub>greaterEquals($a: Mixed, $b: Mixed): Boolean</sub>                                          |
| If                                                                                               |
| <sub>if($condition: Boolean): Null</sub>                                                         |
| Create integer                                                                                   |
| <sub>int($int: Integer): Integer</sub>                                                           |
| Lesser                                                                                           |
| <sub>lesser($a: Mixed, $b: Mixed): Boolean</sub>                                                 |
| Lesser equals                                                                                    |
| <sub>lesserEquals($a: Mixed, $b: Mixed): Boolean</sub>                                           |
| Compute modulo                                                                                   |
| <sub>mod($value: Number, $range: Number): Number</sub>                                           |
| Multiply                                                                                         |
| <sub>mul($a: Mixed, $b: Mixed): Mixed</sub>                                                      |
| Logical not                                                                                      |
| <sub>not($bool: Boolean): Boolean</sub>                                                          |
| Logical not equal                                                                                |
| <sub>notEqual($a: Mixed, $b: Mixed): Boolean</sub>                                               |
| Logical or                                                                                       |
| <sub>or($a: Boolean, $b: Boolean): Boolean</sub>                                                 |
| Prefix decrement                                                                                 |
| <sub>prefixDecrement(&$variable: Integer): Integer</sub>                                         |
| Prefix increment                                                                                 |
| <sub>prefixIncrement(&$variable: Integer): Integer</sub>                                         |
| Return from function with optional return value                                                  |
| <sub>return([$value: Mixed]): Null</sub>                                                         |
| Subtract                                                                                         |
| <sub>sub($a: Mixed, $b: Mixed): Mixed</sub>                                                      |

## 7.2. Application methods

| &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Table of methods &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; |
|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| Execute Application                                                                              |
| <sub>application.execute($command: String): String</sub>                                         |

## 7.3. Console methods

| &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Table of methods &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; |
|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| Pretty print variable to console                                                                 |
| <sub>console.dump($value: Mixed): Null</sub>                                                     |
| Print to console                                                                                 |
| <sub>console.log(...): Null</sub>                                                                |

## 7.4. Script container and interoperability methods

| &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Table of methods &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; |
|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| Returns if a script has a specific callable function                                             |
| <sub>context.script.hasCallable($scriptId: String, $callable: String): Boolean</sub>             |
| Returns all script ids of its context                                                            |
| <sub>context.script.getScriptIds(): Array</sub>                                                  |
| Call a script callable function                                                                  |
| <sub>context.script.call($scriptId: String, $callable: String, ...): Mixed</sub>                 |
| Load a script into context                                                                       |
| <sub>context.script.loadScript($scriptId: String, $pathName: String, $fileName: String[, $verbose: Boolean]): Null</sub>|
| Remove a script from context                                                                     |
| <sub>context.script.removeScript($scriptId: String): Null</sub>                                  |

## 7.5. Cryptography Base64 methods

| &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Table of methods &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; |
|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| Decode a Base64 encoded string                                                                   |
| <sub>cryptography.base64.decode($value: String): String</sub>                                    |
| Encode a string using Base64                                                                     |
| <sub>cryptography.base64.encode($value: String): String</sub>                                    |

## 7.6. Cryptography SHA256 methods

| &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Table of methods &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; |
|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| Hash a string using SHA256                                                                       |
| <sub>cryptography.sha256.encode($value: String): String</sub>                                    |

## 7.7. File System methods

| &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Table of methods &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; |
|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| Get canonical URI                                                                                |
| <sub>filesystem.getCanonicalURI($pathName: String, $fileName: String): ?String</sub>             |
| Get file content as byte array                                                                   |
| <sub>filesystem.getContent($pathName: String, $fileName: String): ?ByteArray</sub>               |
| Set file content from byte array                                                                 |
| <sub>filesystem.setContent($pathName: String, $fileName: String, $content: ByteArray): Boolean</sub>|
| Get file content as string                                                                       |
| <sub>filesystem.getContentAsString($pathName: String, $fileName: String): ?String</sub>          |
| Get file content as string array                                                                 |
| <sub>filesystem.getContentAsStringArray($pathName: String, $fileName: String): ?Array</sub>      |
| Set file content from string                                                                     |
| <sub>filesystem.setContentFromString($pathName: String, $fileName: String, $content: String): Boolean</sub>|
| Set file content from string array                                                               |
| <sub>filesystem.setContentFromStringArray($pathName: String, $fileName: String, $content: Array): Boolean</sub>|
| Get current working path name                                                                    |
| <sub>filesystem.getCurrentWorkingPathName(): ?String</sub>                                       |
| Returns if given URI is a drive name(applies to Microsoft Windows only)                          |
| <sub>filesystem.isDrive($uri: String): Boolean</sub>                                             |
| Extracts file name from given URI                                                                |
| <sub>filesystem.getFileName($uri: String): String</sub>                                          |
| Returns file size from file                                                                      |
| <sub>filesystem.getFileSize($pathName: String, $fileName: String): ?Integer</sub>                |
| Returns if given URI is a path                                                                   |
| <sub>filesystem.isPath($uri: String): ?Boolean</sub>                                             |
| Extracts path name from given URI                                                                |
| <sub>filesystem.getPathName($uri: String): String</sub>                                          |
| Change current working path                                                                      |
| <sub>filesystem.changePath($pathName: String): Boolean</sub>                                     |
| Compose URI from given path name and file name                                                   |
| <sub>filesystem.composeURI($pathName: String, $fileName: String): String</sub>                   |
| Create path                                                                                      |
| <sub>filesystem.createPath($pathName: String): Boolean</sub>                                     |
| Returns if URI does exist                                                                        |
| <sub>filesystem.exists($uri: String): ?Boolean</sub>                                             |
| List folder                                                                                      |
| <sub>filesystem.list($pathName: String): ?Array</sub>                                            |
| Move file/folder from a location to another location                                             |
| <sub>filesystem.move($uriFrom: String, $uriTo: String): Boolean</sub>                            |
| Removes a file with given file name                                                              |
| <sub>filesystem.removeFile($pathName: String, $fileName: String): Boolean</sub>                  |
| Removes a file extension from given file name                                                    |
| <sub>filesystem.removeFileExtension($fileName: String): String</sub>                             |
| Removes a path from file system                                                                  |
| <sub>filesystem.removePath($pathName: String, $recursive: String): Boolean</sub>                 |
| Renames a file/folder                                                                            |
| <sub>filesystem.rename($fileNameFrom: String, $fileNameTo: String): Boolean</sub>                |

## 7.8. Float methods

| &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Table of methods &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; |
|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| Create float                                                                                     |
| <sub>float($float: Float): Float</sub>                                                           |
| Convert integer bit representation of float to float                                             |
| <sub>float.fromIntValue($int: Integer): Float</sub>                                              |
| Convert float to integer bit representation of float                                             |
| <sub>float.toIntValue($float: Float): Integer</sub>                                              |

## 7.9. JSON methods

| &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Table of methods &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; |
|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| Deserialize JSON                                                                                 |
| <sub>json.deserialize($json: String): Mixed</sub>                                                |
| Serialize JSON                                                                                   |
| <sub>json.serialize($value: Mixed): String</sub>                                                 |

## 7.10. Math methods

| &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Table of methods &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; |
|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| Degree to radian factor                                                                          |
| <sub>math.DEG2RAD(): Float</sub>                                                                 |
| Epsilon                                                                                          |
| <sub>math.EPSILON(): Float</sub>                                                                 |
| G                                                                                                |
| <sub>math.G(): Float</sub>                                                                       |
| PI                                                                                               |
| <sub>math.PI(): Float</sub>                                                                      |
| Return number as positive number                                                                 |
| <sub>math.abs($value: Number): Number</sub>                                                      |
| Return number to be positive within given range                                                  |
| <sub>math.absmod($value: Number, $range: Number): Number</sub>                                   |
| Compute acos                                                                                     |
| <sub>math.acos($x: Float): Float</sub>                                                           |
| Compute asin                                                                                     |
| <sub>math.asin($x: Float): Float</sub>                                                           |
| Compute atan                                                                                     |
| <sub>math.atan($x: Float): Float</sub>                                                           |
| Compute atan2                                                                                    |
| <sub>math.atan2($y: Float, $x: Float): Float</sub>                                               |
| Round float up to next higher integer                                                            |
| <sub>math.ceil($value: Float): Float</sub>                                                       |
| Return number clamped to be in given range                                                       |
| <sub>math.clamp($value: Number, $min: Number, $max: Number): Number</sub>                        |
| Compute acos                                                                                     |
| <sub>math.cos($x: Float): Float</sub>                                                            |
| Compute exp                                                                                      |
| <sub>math.exp($power: Float): Float</sub>                                                        |
| Round float down to next lower integer                                                           |
| <sub>math.floor($value: Float): Float</sub>                                                      |
| Compute log                                                                                      |
| <sub>math.log($value: Float): Float</sub>                                                        |
| Return maximum number of given values                                                            |
| <sub>math.max($value1: Number, $value2: Number): Number</sub>                                    |
| Return minimum number of given values                                                            |
| <sub>math.min($value1: Number, $value2: Number): Number</sub>                                    |
| Compute modulo/remainder                                                                         |
| <sub>math.mod($value: Number, $range: Number): Number</sub>                                      |
| Compute pow                                                                                      |
| <sub>math.pow($base: Number, $power: Number): Number</sub>                                       |
| Create a random number between 0.0 and 1.0                                                       |
| <sub>math.random(): Float</sub>                                                                  |
| Round float up or down to integer                                                                |
| <sub>math.round($value: Float): Float</sub>                                                      |
| Return sign of given number                                                                      |
| <sub>math.sign($value: Number): Number</sub>                                                     |
| Compute sin                                                                                      |
| <sub>math.sin($x: Float): Float</sub>                                                            |
| Compute square root                                                                              |
| <sub>math.sqrt($value: Float): Float</sub>                                                       |
| Compute square product                                                                           |
| <sub>math.square($value: Number): Number</sub>                                                   |
| Compute tan                                                                                      |
| <sub>math.tan($x: Float): Float</sub>                                                            |

## 7.11. HTTP/HTTPS client methods

| &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Table of methods &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; |
|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| Execute a HTTP/HTTPS GET request                                                                 |
| <sub>network.httpclient.get($url: String[, $queryParameters: ?Map[, $headers: ?Map]]): ?Map</sub>|
| Execute a HTTP/HTTPS DELETE request                                                              |
| <sub>network.httpclient.delete($url: String[, $queryParameters: ?Map[, $headers: ?Map]]): ?Map</sub>|
| Execute a HTTP/HTTPS HEAD request                                                                |
| <sub>network.httpclient.head($url: String[, $queryParameters: ?Map[, $headers: ?Map]]): ?Map</sub>|
| Execute a HTTP/HTTPS POST request                                                                |
| <sub>network.httpclient.post($url: String, $data: Mixed[, $queryParameters: ?Map[, $headers: ?Map]]): ?Map</sub>|
| Execute a HTTP/HTTPS PUT request                                                                 |
| <sub>network.httpclient.put($url: String, $data: Mixed[, $queryParameters: ?Map[, $headers: ?Map]]): ?Map</sub>|

## 7.12. Script methods

| &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Table of methods &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; |
|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| Get named conditions                                                                             |
| <sub>script.getNamedConditions(): String</sub>                                                   |
| Returns if script runs natively                                                                  |
| <sub>script.isNative(): Boolean</sub>                                                            |
| Get script variables                                                                             |
| <sub>script.getVariables(): Map</sub>                                                            |
| Call script callable function                                                                    |
| <sub>script.call($function: String, ...): Mixed</sub>                                            |
| Disable a specific named condition                                                               |
| <sub>script.disableNamedCondition($name: String): Null</sub>                                     |
| Emit a condition                                                                                 |
| <sub>script.emit($condition: String): Null</sub>                                                 |
| Enable a specific named condition                                                                |
| <sub>script.enableNamedCondition($name: String): Null</sub>                                      |
| Evaluate a script statement                                                                      |
| <sub>script.evaluate($statement: String): Mixed</sub>                                            |
| Stop script                                                                                      |
| <sub>script.stop(): Null</sub>                                                                   |
| Wait for given milliseconds                                                                      |
| <sub>script.wait($time: Integer): Null</sub>                                                     |
| Wait for condition to happen                                                                     |
| <sub>script.waitForCondition(): Null</sub>                                                       |

## 7.13. Time methods

| &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Table of methods &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; |
|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| Get time as string                                                                               |
| <sub>time.getAsString([$format: String]): String</sub>                                           |
| Get current time in milliseconds                                                                 |
| <sub>time.getCurrentMillis(): Integer</sub>                                                      |

## 7.14. XML methods

| &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Table of methods &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; |
|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| Create XML tag                                                                                   |
| <sub>xml.createTag($name: String[, $attributes: Map[, $innerXML: String]]): String</sub>         |

# 8. Operators

| Op | Method                                                                                      |
|----|---------------------------------------------------------------------------------------------|
| !  | not($bool: Boolean): Boolean                                                                |
| != | notEqual($a: Mixed, $b: Mixed): Boolean                                                     |
| %  | mod($value: Number, $range: Number): Number                                                 |
| &  | bitwiseAnd($a: Integer, $b: Integer): Integer                                               |
| && | and($a: Boolean, $b: Boolean): Boolean                                                      |
| *  | mul($a: Mixed, $b: Mixed): Mixed                                                            |
| +  | add($a: Mixed, $b: Mixed): Mixed                                                            |
| ++ | prefixIncrement(&$variable: Integer): Integer                                               |
| -  | sub($a: Mixed, $b: Mixed): Mixed                                                            |
| -- | prefixDecrement(&$variable: Integer): Integer                                               |
| /  | div($a: Mixed, $b: Mixed): Mixed                                                            |
| <  | lesser($a: Mixed, $b: Mixed): Boolean                                                       |
| <= | lesserEquals($a: Mixed, $b: Mixed): Boolean                                                 |
| =  | setVariable($variable: String, $value: Mixed): Mixed                                        |
| == | equals($a: Mixed, $b: Mixed): Boolean                                                       |
| >  | greater($a: Mixed, $b: Mixed): Boolean                                                      |
| >= | greaterEquals($a: Mixed, $b: Mixed): Boolean                                                |
| \| | bitwiseOr($a: Integer, $b: Integer): Integer                                                |
| \|\|| or($a: Boolean, $b: Boolean): Boolean                                                      |
| ^  | bitwiseXor($a: Integer, $b: Integer): Integer                                               |
| ~  | bitwiseNot($value: Integer): Integer                                                        |

# 9. Other information

## 9.1. Links

- MiniScript, see [README.md](./README.md)
- MiniScript How to build, see [README-BuildingHowTo.md](./README-BuildingHowTo.md)
- The Mindty Kollektiv [Discord Server](https://discord.gg/Na4ACaFD)

# 10. Donation 
- I have to pay my bills, beer and stuff, so if you have any use for this open source project, like educational, productive or fun, ..., consider a donation here [PayPal](https://www.paypal.me/andreasdrewke), here [GitHub Sponsors](https://github.com/sponsors/andreasdr) or via [Amazon Wishlist](https://www.amazon.de/hz/wishlist/ls/250IBUL4JSZVR?ref_=wl_share)
