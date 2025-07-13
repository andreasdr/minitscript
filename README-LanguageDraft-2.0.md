![LOGO](https://raw.githubusercontent.com/andreasdr/minitscript/master/resources/github/minitscript-logo.png)

# 1 MinitScript 2.0: new language features draft

- inline comments with #- $x = 1 -#

```
    function: bubbleSortA(&$array)
    	# best code quality, but less performance
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

- enums

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
        #-
        case(Colors.$YELLOW)
            console.printLine("YELLOW")
        end
        -#
        default
            console.printLine("No specific color selected")
        end
    end
```

- structures, which are data only

```
    # maybe, not sure if required 100%
    structure CarData
        $tireCount: Integer
        $color: Integer
    end
```

- structure data validations, which are type hinted interfaces alike

```
    # maybe, but looks good already
    data Car validated-by CarData
        private $tireCount  = 4
        private $color      = "red"
    end
```

- interfaces for class methods

```
    interface CarInterface
        private $tireCount
        private $color

        constructor($tireCount, $color)

        destructor()

        method: dump()

        operator: +: ($a, $b)

        operator: +=: ($b)
    end
```

- classes with optional interface and data validation

```
    class Car[ implements CarInterface [validated-by CarData]
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

- Static member variables
    - maybe by mapping to global variables
    - Class::$classProperty would map to $$.___Class_classProperty

```
    class Class
        static private $classProperty  = 4
    end
```

- Static member methods

```
    class Class
        static method: classMethod($a, $b)
            console.printLine($a + " / " + $b)
        end
    end
```
