![LOGO](https://raw.githubusercontent.com/andreasdr/minitscript/master/resources/github/minitscript-logo.png)

# 1. MinitScript 2.0: new language features draft

## 1.1. Inline comments with #- $x = 1 -#

```
    function: bubbleSortA(&$array)
    	for ($i = 1, $i < $array->getSize()#-+ 1-#, ++$i)
    		for ($j = 0, $j < $array->getSize() - $i, ++$j)
                #-
    			if ($array[$j] > $array[$j + 1])
    				swap($array[$j], $array[$j + 1])
    			end
                -#
    		end
    	end
    end
```

## 1.2. Enums

```
    enum Colors
        $RED = 0
        $GREEN = 1
        $BLUE = 2
    end

    ...
    $color = Colors.$RED
    switch($color)
        case(Colors.$RED)
            console.printLine("RED")
        end        
        case(Colors.$GREEN)
            console.printLine("GREEN")
        end
        case(Colors.$BLUE)
            console.printLine("BLUE")
        end
        default
            console.printLine("No specific color selected")
        end
    end
```

## 1.3. Structures, which are compound data types, including sub data type definitions, used for data validation

```
    # maybe, not sure if required 100%
    structure CarData
        $tireCount: Integer
        $color: Integer
    end
```

## 1.4. Data objects, which can optionally be validated

```
    # maybe, but looks good already
    data Car [validated-by CarData]
        private $tireCount  = 4
        private $color      = "red"
    end
```

## 1.5. Class interfaces

```
    interface CarInterface
        constructor($tireCount, $color)

        destructor()

        method: dump()

        operator: +: ($a, $b)

        operator: +=: ($b)
    end
```

## 1.6. Classes with optional interface and data validation

```
    class Car[ implements CarInterface] [validated-by CarData]
        private $tireCount  = 4
        private $color      = "red"

        constructor($tireCount, $color)
            $this.tireCount = $tireCount
            $this.color = $color
        end

        destructor()
            $this.tireCount = null
            $this.color = null      
        end

        method: dump()
            console.printLine(
                "$this.tireCount = $tireCount, " +
                "$this.color = $color"
            )
        end

        operator: +: ($a, $b)
            return Auto(
                $a.tireCount + $b.tireCount,
                $a.color + "/" +  $b.color
            )
        end  

        operator: +=: ($b)
            $this.tireCount+= $b.tireCount,
            $this.color+= "/" +  $b.color
        end  
    
    end

    #
    $car = Car(4, Colors.$RED)
    $car->dump()
    #
    $car = Car(4, Colors.$RED) + Car(4, Colors.$BLUE)
```

## 1.7. Static member variables
    - maybe by mapping to global variables
    - Class::$classProperty would map to $$.___Class_classProperty

```
    class Class
        static private $classProperty  = 4
    end
```

## 1.8. Static member methods

```
    class Class
        static method: classMethod($a, $b)
            console.printLine($a + " / " + $b)
        end
    end
```

# 2. Links

- MinitScript, see [README.md](./README.md)
- MinitScript - How to build, see [README-BuildingHowTo.md](./README-BuildingHowTo.md)
- MinitScript - How to use, see [README-Tools.md](./README-Tools.md)
- The Mindty Kollektiv [Discord Server](https://discord.gg/Na4ACaFD)
