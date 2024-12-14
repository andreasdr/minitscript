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
- [Syntax](./README-Syntax.md)
- [DataTypes](./README-DataTypes.md)
- [Flow control - if, elseif, else](./README-FlowControl-Conditions.md)
- [Flow Control - switch, case, default](./README-FlowControl-Conditions2.md)
- [Flow Control - loops](./README-FlowControl-Loops.md)
- [Functions](./README-Functions.md)
- [Lambda Functions](./README-Lambda-Functions.md)
- [Classes](./README-Classes.md)
- [BuiltIn functions](./README-BuiltIn-Functions.md)
- [Classes API](./README-Classes-API.md)
- [Modules](./README-Modules.md)
- [Operators](./README-Operators.md)
- [Constants](./README-Constants.md)

## 2.2. Other Links

- MinitScript, see [README.md](../README.md)
- MinitScript - How to build, see [README-BuildingHowTo.md](../README-BuildingHowTo.md)
- MinitScript - How to use, see [README-Tools.md](../README-Tools.md)
- The Mindty Kollektiv [Discord Server](https://discord.gg/Na4ACaFD)
