![LOGO](https://raw.githubusercontent.com/andreasdr/minitscript/master/resources/github/minitscript-logo.png)

# 1. Operators

| Op | Method                                                                                      |
|----|---------------------------------------------------------------------------------------------|
| !  | not($boolean: Boolean[, $operator: Integer]): Boolean                                       |
| != | notEqual($a: Mixed, $b: Mixed[, $operator: Integer]): Boolean                               |
| %  | mod($value: Number, $range: Number[, $operator: String]): Number                            |
| &  | bitwiseAnd($a: Integer, $b: Integer[, $operator: Integer]): Integer                         |
| && | and($a: Boolean, $b: Boolean[, $operator: Integer]): Boolean                                |
| *  | mul($a: Mixed, $b: Mixed[, $operator: String]): Mixed                                       |
| +  | add($a: Mixed, $b: Mixed[, $operator: Integer]): Mixed                                      |
| ++ | postfixIncrement(&$variable: Integer[, $operator: Integer]): Integer                        |
| ++ | prefixIncrement(&$variable: Integer[, $operator: Integer]): Integer                         |
| -  | sub($a: Mixed, $b: Mixed[, $operator: String]): Mixed                                       |
| -- | postfixDecrement(&$variable: Integer[, $operator: Integer]): Integer                        |
| -- | prefixDecrement(&$variable: Integer[, $operator: Integer]): Integer                         |
| /  | div($a: Mixed, $b: Mixed[, $operator: String]): Mixed                                       |
| <  | lesser($a: Mixed, $b: Mixed[, $operator: Integer]): Boolean                                 |
| <= | lesserEquals($a: Mixed, $b: Mixed[, $operator: Integer]): Boolean                           |
| =  | setVariable($variable: String, $value: Mixed[, $operator: Integer]): Mixed                  |
| == | equals($a: Mixed, $b: Mixed[, $operator: Integer]): Boolean                                 |
| >  | greater($a: Mixed, $b: Mixed[, $operator: Integer]): Boolean                                |
| >= | greaterEquals($a: Mixed, $b: Mixed[, $operator: Integer]): Boolean                          |
| \| | bitwiseOr($a: Integer, $b: Integer[, $operator: Integer]): Integer                          |
| \|\|| or($a: Boolean, $b: Boolean[, $operator: Integer]): Boolean                                |
| ^  | bitwiseXor($a: Integer, $b: Integer[, $operator: Integer]): Integer                         |
| ~  | bitwiseNot($value: Integer[, $operator: Integer]): Integer                                  |

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
