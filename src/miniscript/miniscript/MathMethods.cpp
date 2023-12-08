#include <span>

#include <miniscript/miniscript.h>
#include <miniscript/math/Math.h>
#include <miniscript/miniscript/MathMethods.h>
#include <miniscript/miniscript/MiniScript.h>
#include <miniscript/utilities/Console.h>

using std::span;

using miniscript::miniscript::MathMethods;

using miniscript::math::Math;
using miniscript::miniscript::MiniScript;
using miniscript::utilities::Console;

void MathMethods::registerDataType(MiniScript::DataType* dataType) {
	dataTypes.push_back(dataType);
}

void MathMethods::registerMethods() {
	// operator methods
	{
		//
		class ScriptMethodAdd: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			ScriptMethodAdd(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_PSEUDO_MIXED, .name = "a", .optional = false, .reference = false },
						{ .type = MiniScript::TYPE_PSEUDO_MIXED, .name = "b", .optional = false, .reference = false }
					},
					MiniScript::TYPE_PSEUDO_MIXED
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "add";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				miniScript->getMathMethods()->add(arguments, returnValue, statement);
			}
			MiniScript::Operator getOperator() const override {
				return MiniScript::OPERATOR_ADDITION;
			}
		};
		miniScript->registerMethod(new ScriptMethodAdd(miniScript));
	}
	{
		//
		class ScriptMethodSub: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			ScriptMethodSub(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_PSEUDO_MIXED, .name = "a", .optional = false, .reference = false },
						{ .type = MiniScript::TYPE_PSEUDO_MIXED, .name = "b", .optional = false, .reference = false }
					},
					MiniScript::TYPE_PSEUDO_MIXED
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "sub";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				miniScript->getMathMethods()->sub(arguments, returnValue, statement);
			}
			MiniScript::Operator getOperator() const override {
				return MiniScript::OPERATOR_SUBTRACTION;
			}
		};
		miniScript->registerMethod(new ScriptMethodSub(miniScript));
	}
	{
		//
		class ScriptMethodMul: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			ScriptMethodMul(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_PSEUDO_MIXED, .name = "a", .optional = false, .reference = false },
						{ .type = MiniScript::TYPE_PSEUDO_MIXED, .name = "b", .optional = false, .reference = false }
					},
					MiniScript::TYPE_PSEUDO_MIXED
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "mul";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				miniScript->getMathMethods()->mul(arguments, returnValue, statement);
			}
			MiniScript::Operator getOperator() const override {
				return MiniScript::OPERATOR_MULTIPLICATION;
			}
		};
		miniScript->registerMethod(new ScriptMethodMul(miniScript));
	}
	{
		//
		class ScriptMethodDiv: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			ScriptMethodDiv(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_PSEUDO_MIXED, .name = "a", .optional = false, .reference = false },
						{ .type = MiniScript::TYPE_PSEUDO_MIXED, .name = "b", .optional = false, .reference = false }
					},
					MiniScript::TYPE_PSEUDO_MIXED
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "div";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				miniScript->getMathMethods()->div(arguments, returnValue, statement);
			}
			MiniScript::Operator getOperator() const override {
				return MiniScript::OPERATOR_DIVISION;
			}
		};
		miniScript->registerMethod(new ScriptMethodDiv(miniScript));
	}
	{
		//
		class ScriptMethodMod: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			ScriptMethodMod(MiniScript* miniScript): MiniScript::Method(
				{
					{ .type = MiniScript::TYPE_PSEUDO_NUMBER, .name = "value", .optional = false, .reference = false },
					{ .type = MiniScript::TYPE_PSEUDO_NUMBER, .name = "range", .optional = false, .reference = false },
				},
				MiniScript::TYPE_PSEUDO_NUMBER
			), miniScript(miniScript) {}
			const string getMethodName() override {
				return "mod";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				if (MiniScript::hasType(arguments, MiniScript::TYPE_INTEGER) == true) {
					int64_t value;
					int64_t range;
					if (MiniScript::getIntegerValue(arguments, 0, value, false) == true &&
						MiniScript::getIntegerValue(arguments, 1, range, false) == true) {
						returnValue.setValue(Math::mod(value, range));
					} else {
						Console::println(getMethodName() + "(): " + miniScript->getStatementInformation(statement) + ": argument mismatch: expected arguments: " + miniScript->getArgumentInformation(getMethodName()));
						miniScript->startErrorScript();
					}
				} else {
					float value;
					float range;
					if (MiniScript::getFloatValue(arguments, 0, value, false) == true &&
						MiniScript::getFloatValue(arguments, 1, range, false) == true) {
						returnValue.setValue(Math::mod(value, range));
					} else {
						Console::println(getMethodName() + "(): " + miniScript->getStatementInformation(statement) + ": argument mismatch: expected arguments: " + miniScript->getArgumentInformation(getMethodName()));
						miniScript->startErrorScript();
					}
				}
			}
			MiniScript::Operator getOperator() const override {
				return MiniScript::OPERATOR_MODULO;
			}
		};
		miniScript->registerMethod(new ScriptMethodMod(miniScript));
	}
	// constants
	{
		//
		class ScriptMethodPI: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			ScriptMethodPI(MiniScript* miniScript): MiniScript::Method({}, MiniScript::TYPE_FLOAT), miniScript(miniScript) {}
			const string getMethodName() override {
				return "math.PI";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				returnValue.setValue(Math::PI);
			}
		};
		miniScript->registerMethod(new ScriptMethodPI(miniScript));
	}
	{
		//
		class ScriptMethodEPSILON: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			ScriptMethodEPSILON(MiniScript* miniScript): MiniScript::Method({}, MiniScript::TYPE_FLOAT), miniScript(miniScript) {}
			const string getMethodName() override {
				return "math.EPSILON";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				returnValue.setValue(Math::EPSILON);
			}
		};
		miniScript->registerMethod(new ScriptMethodEPSILON(miniScript));
	}
	{
		//
		class ScriptMethodDEG2RAD: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			ScriptMethodDEG2RAD(MiniScript* miniScript): MiniScript::Method({}, MiniScript::TYPE_FLOAT), miniScript(miniScript) {}
			const string getMethodName() override {
				return "math.DEG2RAD";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				returnValue.setValue(Math::DEG2RAD);
			}
		};
		miniScript->registerMethod(new ScriptMethodDEG2RAD(miniScript));
	}
	{
		//
		class ScriptMethodG: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			ScriptMethodG(MiniScript* miniScript): MiniScript::Method({}, MiniScript::TYPE_FLOAT), miniScript(miniScript) {}
			const string getMethodName() override {
				return "math.G";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				returnValue.setValue(Math::G);
			}
		};
		miniScript->registerMethod(new ScriptMethodG(miniScript));
	}
	{
		//
		class ScriptMethodAcos: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			ScriptMethodAcos(MiniScript* miniScript): MiniScript::Method(
				{
					{ .type = MiniScript::TYPE_FLOAT, .name = "x", .optional = false, .reference = false },
				},
				MiniScript::TYPE_FLOAT), miniScript(miniScript) {}
			const string getMethodName() override {
				return "math.acos";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				float x;
				if (MiniScript::getFloatValue(arguments, 0, x, false) == true) {
					returnValue.setValue(Math::acos(x));
				} else {
					Console::println(getMethodName() + "(): " + miniScript->getStatementInformation(statement) + ": argument mismatch: expected arguments: " + miniScript->getArgumentInformation(getMethodName()));
					miniScript->startErrorScript();
				}
			}
		};
		miniScript->registerMethod(new ScriptMethodAcos(miniScript));
	}
	{
		//
		class ScriptMethodAsin: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			ScriptMethodAsin(MiniScript* miniScript): MiniScript::Method(
				{
					{ .type = MiniScript::TYPE_FLOAT, .name = "x", .optional = false, .reference = false },
				},
				MiniScript::TYPE_FLOAT), miniScript(miniScript) {}
			const string getMethodName() override {
				return "math.asin";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				float x;
				if (MiniScript::getFloatValue(arguments, 0, x, false) == true) {
					returnValue.setValue(Math::asin(x));
				} else {
					Console::println(getMethodName() + "(): " + miniScript->getStatementInformation(statement) + ": argument mismatch: expected arguments: " + miniScript->getArgumentInformation(getMethodName()));
					miniScript->startErrorScript();
				}
			}
		};
		miniScript->registerMethod(new ScriptMethodAsin(miniScript));
	}
	{
		//
		class ScriptMethodAtan: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			ScriptMethodAtan(MiniScript* miniScript): MiniScript::Method(
				{
					{ .type = MiniScript::TYPE_FLOAT, .name = "x", .optional = false, .reference = false },
				},
				MiniScript::TYPE_FLOAT), miniScript(miniScript) {}
			const string getMethodName() override {
				return "math.atan";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				float x;
				if (MiniScript::getFloatValue(arguments, 0, x, false) == true) {
					returnValue.setValue(Math::atan(x));
				} else {
					Console::println(getMethodName() + "(): " + miniScript->getStatementInformation(statement) + ": argument mismatch: expected arguments: " + miniScript->getArgumentInformation(getMethodName()));
					miniScript->startErrorScript();
				}
			}
		};
		miniScript->registerMethod(new ScriptMethodAtan(miniScript));
	}
	{
		//
		class ScriptMethodAtan2: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			ScriptMethodAtan2(MiniScript* miniScript): MiniScript::Method(
				{
					{ .type = MiniScript::TYPE_FLOAT, .name = "y", .optional = false, .reference = false },
					{ .type = MiniScript::TYPE_FLOAT, .name = "x", .optional = false, .reference = false },
				},
				MiniScript::TYPE_FLOAT), miniScript(miniScript) {}
			const string getMethodName() override {
				return "math.atan2";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				float y;
				float x;
				if (MiniScript::getFloatValue(arguments, 0, y, false) == true &&
					MiniScript::getFloatValue(arguments, 1, x, false) == true) {
					returnValue.setValue(Math::atan2(y, x));
				} else {
					Console::println(getMethodName() + "(): " + miniScript->getStatementInformation(statement) + ": argument mismatch: expected arguments: " + miniScript->getArgumentInformation(getMethodName()));
					miniScript->startErrorScript();
				}
			}
		};
		miniScript->registerMethod(new ScriptMethodAtan2(miniScript));
	}
	{
		//
		class ScriptMethodTan: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			ScriptMethodTan(MiniScript* miniScript): MiniScript::Method(
				{
					{ .type = MiniScript::TYPE_FLOAT, .name = "x", .optional = false, .reference = false },
				},
				MiniScript::TYPE_FLOAT), miniScript(miniScript) {}
			const string getMethodName() override {
				return "math.tan";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				float x;
				if (MiniScript::getFloatValue(arguments, 0, x, false) == true) {
					returnValue.setValue(Math::tan(x));
				} else {
					Console::println(getMethodName() + "(): " + miniScript->getStatementInformation(statement) + ": argument mismatch: expected arguments: " + miniScript->getArgumentInformation(getMethodName()));
					miniScript->startErrorScript();
				}
			}
		};
		miniScript->registerMethod(new ScriptMethodTan(miniScript));
	}
	{
		//
		class ScriptMethodCos: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			ScriptMethodCos(MiniScript* miniScript): MiniScript::Method(
				{
					{ .type = MiniScript::TYPE_FLOAT, .name = "x", .optional = false, .reference = false },
				},
				MiniScript::TYPE_FLOAT), miniScript(miniScript) {}
			const string getMethodName() override {
				return "math.cos";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				float x;
				if (MiniScript::getFloatValue(arguments, 0, x, false) == true) {
					returnValue.setValue(Math::cos(x));
				} else {
					Console::println(getMethodName() + "(): " + miniScript->getStatementInformation(statement) + ": argument mismatch: expected arguments: " + miniScript->getArgumentInformation(getMethodName()));
					miniScript->startErrorScript();
				}
			}
		};
		miniScript->registerMethod(new ScriptMethodCos(miniScript));
	}
	{
		//
		class ScriptMethodSin: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			ScriptMethodSin(MiniScript* miniScript): MiniScript::Method(
				{
					{ .type = MiniScript::TYPE_FLOAT, .name = "x", .optional = false, .reference = false },
				},
				MiniScript::TYPE_FLOAT), miniScript(miniScript) {}
			const string getMethodName() override {
				return "math.sin";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				float x;
				if (MiniScript::getFloatValue(arguments, 0, x, false) == true) {
					returnValue.setValue(Math::sin(x));
				} else {
					Console::println(getMethodName() + "(): " + miniScript->getStatementInformation(statement) + ": argument mismatch: expected arguments: " + miniScript->getArgumentInformation(getMethodName()));
					miniScript->startErrorScript();
				}
			}
		};
		miniScript->registerMethod(new ScriptMethodSin(miniScript));
	}
	{
		//
		class ScriptMethodFloor: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			ScriptMethodFloor(MiniScript* miniScript): MiniScript::Method(
				{
					{ .type = MiniScript::TYPE_FLOAT, .name = "value", .optional = false, .reference = false },
				},
				MiniScript::TYPE_FLOAT), miniScript(miniScript) {}
			const string getMethodName() override {
				return "math.floor";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				float value;
				if (MiniScript::getFloatValue(arguments, 0, value, false) == true) {
					returnValue.setValue(Math::floor(value));
				} else {
					Console::println(getMethodName() + "(): " + miniScript->getStatementInformation(statement) + ": argument mismatch: expected arguments: " + miniScript->getArgumentInformation(getMethodName()));
					miniScript->startErrorScript();
				}
			}
		};
		miniScript->registerMethod(new ScriptMethodFloor(miniScript));
	}
	{
		//
		class ScriptMethodCeil: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			ScriptMethodCeil(MiniScript* miniScript): MiniScript::Method(
				{
					{ .type = MiniScript::TYPE_FLOAT, .name = "value", .optional = false, .reference = false },
				},
				MiniScript::TYPE_FLOAT), miniScript(miniScript) {}
			const string getMethodName() override {
				return "math.ceil";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				float value;
				if (MiniScript::getFloatValue(arguments, 0, value, false) == true) {
					returnValue.setValue(Math::ceil(value));
				} else {
					Console::println(getMethodName() + "(): " + miniScript->getStatementInformation(statement) + ": argument mismatch: expected arguments: " + miniScript->getArgumentInformation(getMethodName()));
					miniScript->startErrorScript();
				}
			}
		};
		miniScript->registerMethod(new ScriptMethodCeil(miniScript));
	}
	{
		//
		class ScriptMethodRound: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			ScriptMethodRound(MiniScript* miniScript): MiniScript::Method(
				{
					{ .type = MiniScript::TYPE_FLOAT, .name = "value", .optional = false, .reference = false },
				},
				MiniScript::TYPE_FLOAT), miniScript(miniScript) {}
			const string getMethodName() override {
				return "math.round";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				float value;
				if (MiniScript::getFloatValue(arguments, 0, value, false) == true) {
					returnValue.setValue(Math::round(value));
				} else {
					Console::println(getMethodName() + "(): " + miniScript->getStatementInformation(statement) + ": argument mismatch: expected arguments: " + miniScript->getArgumentInformation(getMethodName()));
					miniScript->startErrorScript();
				}
			}
		};
		miniScript->registerMethod(new ScriptMethodRound(miniScript));
	}
	{
		//
		class ScriptMethodSqrt: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			ScriptMethodSqrt(MiniScript* miniScript): MiniScript::Method(
				{
					{ .type = MiniScript::TYPE_FLOAT, .name = "value", .optional = false, .reference = false },
				},
				MiniScript::TYPE_FLOAT), miniScript(miniScript) {}
			const string getMethodName() override {
				return "math.sqrt";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				float value;
				if (MiniScript::getFloatValue(arguments, 0, value, false) == true) {
					returnValue.setValue(Math::sqrt(value));
				} else {
					Console::println(getMethodName() + "(): " + miniScript->getStatementInformation(statement) + ": argument mismatch: expected arguments: " + miniScript->getArgumentInformation(getMethodName()));
					miniScript->startErrorScript();
				}
			}
		};
		miniScript->registerMethod(new ScriptMethodSqrt(miniScript));
	}
	{
		//
		class ScriptMethodRandom: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			ScriptMethodRandom(MiniScript* miniScript): MiniScript::Method({}, MiniScript::TYPE_FLOAT), miniScript(miniScript) {}
			const string getMethodName() override {
				return "math.random";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				returnValue.setValue(Math::random());
			}
		};
		miniScript->registerMethod(new ScriptMethodRandom(miniScript));
	}
	{
		//
		class ScriptMethodExp: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			ScriptMethodExp(MiniScript* miniScript): MiniScript::Method(
				{
					{ .type = MiniScript::TYPE_FLOAT, .name = "power", .optional = false, .reference = false },
				},
				MiniScript::TYPE_FLOAT), miniScript(miniScript) {}
			const string getMethodName() override {
				return "math.exp";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				float power;
				if (MiniScript::getFloatValue(arguments, 0, power, false) == true) {
					returnValue.setValue(Math::exp(power));
				} else {
					Console::println(getMethodName() + "(): " + miniScript->getStatementInformation(statement) + ": argument mismatch: expected arguments: " + miniScript->getArgumentInformation(getMethodName()));
					miniScript->startErrorScript();
				}
			}
		};
		miniScript->registerMethod(new ScriptMethodExp(miniScript));
	}
	{
		//
		class ScriptMethodLog: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			ScriptMethodLog(MiniScript* miniScript): MiniScript::Method(
				{
					{ .type = MiniScript::TYPE_FLOAT, .name = "value", .optional = false, .reference = false },
				},
				MiniScript::TYPE_FLOAT), miniScript(miniScript) {}
			const string getMethodName() override {
				return "math.log";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				float value;
				if (MiniScript::getFloatValue(arguments, 0, value, false) == true) {
					returnValue.setValue(Math::log(value));
				} else {
					Console::println(getMethodName() + "(): " + miniScript->getStatementInformation(statement) + ": argument mismatch: expected arguments: " + miniScript->getArgumentInformation(getMethodName()));
					miniScript->startErrorScript();
				}
			}
		};
		miniScript->registerMethod(new ScriptMethodLog(miniScript));
	}
	{
		//
		class ScriptMethodSign: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			ScriptMethodSign(MiniScript* miniScript): MiniScript::Method(
				{
					{ .type = MiniScript::TYPE_PSEUDO_NUMBER, .name = "value", .optional = false, .reference = false },
				},
				MiniScript::TYPE_PSEUDO_NUMBER),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "math.sign";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				int64_t intValue;
				float floatValue;
				if (MiniScript::hasType(arguments, MiniScript::TYPE_FLOAT) == true && MiniScript::getFloatValue(arguments, 0, floatValue, false) == true) {
					returnValue.setValue(Math::sign(floatValue));
				} else
				if (MiniScript::getIntegerValue(arguments, 0, intValue, false) == true) {
					returnValue.setValue(Math::sign(intValue));
				} else {
					Console::println(getMethodName() + "(): " + miniScript->getStatementInformation(statement) + ": argument mismatch: expected arguments: " + miniScript->getArgumentInformation(getMethodName()));
					miniScript->startErrorScript();
				}
			}
		};
		miniScript->registerMethod(new ScriptMethodSign(miniScript));
	}
	{
		//
		class ScriptMethodSquare: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			ScriptMethodSquare(MiniScript* miniScript): MiniScript::Method(
				{
					{ .type = MiniScript::TYPE_PSEUDO_NUMBER, .name = "value", .optional = false, .reference = false },
				},
				MiniScript::TYPE_PSEUDO_NUMBER),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "math.square";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				int64_t intValue;
				float floatValue;
				if (MiniScript::hasType(arguments, MiniScript::TYPE_FLOAT) == true && MiniScript::getFloatValue(arguments, 0, floatValue, false) == true) {
					returnValue.setValue(Math::square(floatValue));
				} else
				if (MiniScript::getIntegerValue(arguments, 0, intValue, false) == true) {
					returnValue.setValue(Math::square(intValue));
				} else {
					Console::println(getMethodName() + "(): " + miniScript->getStatementInformation(statement) + ": argument mismatch: expected arguments: " + miniScript->getArgumentInformation(getMethodName()));
					miniScript->startErrorScript();
				}
			}
		};
		miniScript->registerMethod(new ScriptMethodSquare(miniScript));
	}
	{
		//
		class ScriptMethodMin: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			ScriptMethodMin(MiniScript* miniScript): MiniScript::Method(
				{
					{ .type = MiniScript::TYPE_PSEUDO_NUMBER, .name = "value1", .optional = false, .reference = false },
					{ .type = MiniScript::TYPE_PSEUDO_NUMBER, .name = "value2", .optional = false, .reference = false },
				},
				MiniScript::TYPE_PSEUDO_NUMBER),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "math.min";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				int64_t intValue1;
				int64_t intValue2;
				float floatValue1;
				float floatValue2;
				if (arguments.size() != 2) {
					Console::println(getMethodName() + "(): " + miniScript->getStatementInformation(statement) + ": argument mismatch: expected arguments: " + miniScript->getArgumentInformation(getMethodName()));
					miniScript->startErrorScript();
				} else
				if (MiniScript::hasType(arguments, MiniScript::TYPE_FLOAT) == true &&
					MiniScript::getFloatValue(arguments, 0, floatValue1, false) == true &&
					MiniScript::getFloatValue(arguments, 1, floatValue2, false) == true) {
					returnValue.setValue(Math::min(floatValue1, floatValue2));
				} else
				if (MiniScript::getIntegerValue(arguments, 0, intValue1, false) == true &&
					MiniScript::getIntegerValue(arguments, 1, intValue2, false) == true) {
					returnValue.setValue(Math::min(intValue1, intValue2));
				} else {
					Console::println(getMethodName() + "(): " + miniScript->getStatementInformation(statement) + ": argument mismatch: expected arguments: " + miniScript->getArgumentInformation(getMethodName()));
					miniScript->startErrorScript();
				}
			}
		};
		miniScript->registerMethod(new ScriptMethodMin(miniScript));
	}
	{
		//
		class ScriptMethodMax: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			ScriptMethodMax(MiniScript* miniScript): MiniScript::Method(
				{
					{ .type = MiniScript::TYPE_PSEUDO_NUMBER, .name = "value1", .optional = false, .reference = false },
					{ .type = MiniScript::TYPE_PSEUDO_NUMBER, .name = "value2", .optional = false, .reference = false },
				},
				MiniScript::TYPE_PSEUDO_NUMBER),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "math.max";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				int64_t intValue1;
				int64_t intValue2;
				float floatValue1;
				float floatValue2;
				if (arguments.size() != 2) {
					Console::println(getMethodName() + "(): " + miniScript->getStatementInformation(statement) + ": argument mismatch: expected arguments: " + miniScript->getArgumentInformation(getMethodName()));
					miniScript->startErrorScript();
				} else
				if (MiniScript::hasType(arguments, MiniScript::TYPE_FLOAT) == true &&
					MiniScript::getFloatValue(arguments, 0, floatValue1, false) == true &&
					MiniScript::getFloatValue(arguments, 1, floatValue2, false) == true) {
					returnValue.setValue(Math::max(floatValue1, floatValue2));
				} else
				if (MiniScript::getIntegerValue(arguments, 0, intValue1, false) == true &&
					MiniScript::getIntegerValue(arguments, 1, intValue2, false) == true) {
					returnValue.setValue(Math::max(intValue1, intValue2));
				} else {
					Console::println(getMethodName() + "(): " + miniScript->getStatementInformation(statement) + ": argument mismatch: expected arguments: " + miniScript->getArgumentInformation(getMethodName()));
					miniScript->startErrorScript();
				}
			}
		};
		miniScript->registerMethod(new ScriptMethodMax(miniScript));
	}
	{
		//
		class ScriptMethodAbs: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			ScriptMethodAbs(MiniScript* miniScript): MiniScript::Method(
				{
					{ .type = MiniScript::TYPE_PSEUDO_NUMBER, .name = "value", .optional = false, .reference = false },
				},
				MiniScript::TYPE_PSEUDO_NUMBER),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "math.abs";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				int64_t intValue;
				float floatValue;
				if (MiniScript::hasType(arguments, MiniScript::TYPE_FLOAT) == true && MiniScript::getFloatValue(arguments, 0, floatValue, false) == true) {
					returnValue.setValue(Math::abs(floatValue));
				} else
				if (MiniScript::getIntegerValue(arguments, 0, intValue, false) == true) {
					returnValue.setValue(Math::abs(intValue));
				} else {
					Console::println(getMethodName() + "(): " + miniScript->getStatementInformation(statement) + ": argument mismatch: expected arguments: " + miniScript->getArgumentInformation(getMethodName()));
					miniScript->startErrorScript();
				}
			}
		};
		miniScript->registerMethod(new ScriptMethodAbs(miniScript));
	}
	{
		//
		class ScriptMethodClamp: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			ScriptMethodClamp(MiniScript* miniScript): MiniScript::Method(
				{
					{ .type = MiniScript::TYPE_PSEUDO_NUMBER, .name = "value", .optional = false, .reference = false },
					{ .type = MiniScript::TYPE_PSEUDO_NUMBER, .name = "min", .optional = false, .reference = false },
					{ .type = MiniScript::TYPE_PSEUDO_NUMBER, .name = "max", .optional = false, .reference = false },
				},
				MiniScript::TYPE_PSEUDO_NUMBER),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "math.clamp";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				int64_t intValueA;
				int64_t intValueB;
				int64_t intValueC;
				float floatValueA;
				float floatValueB;
				float floatValueC;
				if (arguments.size() != 3) {
					Console::println(getMethodName() + "(): " + miniScript->getStatementInformation(statement) + ": argument mismatch: expected arguments: " + miniScript->getArgumentInformation(getMethodName()));
					miniScript->startErrorScript();
				} else
				if (MiniScript::hasType(arguments, MiniScript::TYPE_FLOAT) == true &&
					MiniScript::getFloatValue(arguments, 0, floatValueA, false) == true &&
					MiniScript::getFloatValue(arguments, 1, floatValueB, false) == true &&
					MiniScript::getFloatValue(arguments, 2, floatValueC, false) == true) {
					returnValue.setValue(Math::clamp(floatValueA, floatValueB, floatValueC));
				} else
				if (MiniScript::getIntegerValue(arguments, 0, intValueA, false) == true &&
					MiniScript::getIntegerValue(arguments, 1, intValueB, false) == true &&
					MiniScript::getIntegerValue(arguments, 2, intValueC, false) == true) {
					returnValue.setValue(Math::clamp(intValueA, intValueB, intValueC));
				} else {
					Console::println(getMethodName() + "(): " + miniScript->getStatementInformation(statement) + ": argument mismatch: expected arguments: " + miniScript->getArgumentInformation(getMethodName()));
					miniScript->startErrorScript();
				}
			}
		};
		miniScript->registerMethod(new ScriptMethodClamp(miniScript));
	}
	{
		//
		class ScriptMethodPow: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			ScriptMethodPow(MiniScript* miniScript): MiniScript::Method(
				{
					{ .type = MiniScript::TYPE_PSEUDO_NUMBER, .name = "base", .optional = false, .reference = false },
					{ .type = MiniScript::TYPE_PSEUDO_NUMBER, .name = "power", .optional = false, .reference = false },
				},
				MiniScript::TYPE_PSEUDO_NUMBER),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "math.pow";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				int64_t intValueBase;
				int64_t intValuePower;
				float floatValueBase;
				float floatValuePower;
				if (arguments.size() != 2) {
					Console::println(getMethodName() + "(): " + miniScript->getStatementInformation(statement) + ": argument mismatch: expected arguments: " + miniScript->getArgumentInformation(getMethodName()));
					miniScript->startErrorScript();
				} else
				if (MiniScript::hasType(arguments, MiniScript::TYPE_FLOAT) == true &&
					MiniScript::getFloatValue(arguments, 0, floatValueBase, false) == true &&
					MiniScript::getFloatValue(arguments, 1, floatValuePower, false) == true) {
					returnValue.setValue(Math::pow(floatValueBase, floatValuePower));
				} else
				if (MiniScript::getIntegerValue(arguments, 0, intValueBase, false) == true &&
					MiniScript::getIntegerValue(arguments, 1, intValuePower, false) == true) {
					returnValue.setValue(static_cast<int64_t>(Math::pow(intValueBase, intValuePower)));
				} else {
					Console::println(getMethodName() + "(): " + miniScript->getStatementInformation(statement) + ": argument mismatch: expected arguments: " + miniScript->getArgumentInformation(getMethodName()));
					miniScript->startErrorScript();
				}
			}
		};
		miniScript->registerMethod(new ScriptMethodPow(miniScript));
	}
	{
		//
		class ScriptMethodMod: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			ScriptMethodMod(MiniScript* miniScript): MiniScript::Method(
				{
					{ .type = MiniScript::TYPE_PSEUDO_NUMBER, .name = "value", .optional = false, .reference = false },
					{ .type = MiniScript::TYPE_PSEUDO_NUMBER, .name = "range", .optional = false, .reference = false },
				},
				MiniScript::TYPE_PSEUDO_NUMBER
			), miniScript(miniScript) {}
			const string getMethodName() override {
				return "math.mod";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				if (MiniScript::hasType(arguments, MiniScript::TYPE_INTEGER) == true) {
					int64_t value;
					int64_t range;
					if (MiniScript::getIntegerValue(arguments, 0, value, false) == true &&
						MiniScript::getIntegerValue(arguments, 1, range, false) == true) {
						returnValue.setValue(Math::mod(value, range));
					} else {
						Console::println(getMethodName() + "(): " + miniScript->getStatementInformation(statement) + ": argument mismatch: expected arguments: " + miniScript->getArgumentInformation(getMethodName()));
						miniScript->startErrorScript();
					}
				} else {
					float value;
					float range;
					if (MiniScript::getFloatValue(arguments, 0, value, false) == true &&
						MiniScript::getFloatValue(arguments, 1, range, false) == true) {
						returnValue.setValue(Math::mod(value, range));
					} else {
						Console::println(getMethodName() + "(): " + miniScript->getStatementInformation(statement) + ": argument mismatch: expected arguments: " + miniScript->getArgumentInformation(getMethodName()));
						miniScript->startErrorScript();
					}
				}
			}
		};
		miniScript->registerMethod(new ScriptMethodMod(miniScript));
	}
	{
		//
		class ScriptMethodAbsMod: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			ScriptMethodAbsMod(MiniScript* miniScript): MiniScript::Method(
				{
					{ .type = MiniScript::TYPE_PSEUDO_NUMBER, .name = "value", .optional = false, .reference = false },
					{ .type = MiniScript::TYPE_PSEUDO_NUMBER, .name = "range", .optional = false, .reference = false },
				},
				MiniScript::TYPE_PSEUDO_NUMBER
			), miniScript(miniScript) {}
			const string getMethodName() override {
				return "math.absmod";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				if (MiniScript::hasType(arguments, MiniScript::TYPE_INTEGER) == true) {
					int64_t value;
					int64_t range;
					if (MiniScript::getIntegerValue(arguments, 0, value, false) == true &&
						MiniScript::getIntegerValue(arguments, 1, range, false) == true) {
						returnValue.setValue(Math::absmod(value, range));
					} else {
						Console::println(getMethodName() + "(): " + miniScript->getStatementInformation(statement) + ": argument mismatch: expected arguments: " + miniScript->getArgumentInformation(getMethodName()));
						miniScript->startErrorScript();
					}
				} else {
					float value;
					float range;
					if (MiniScript::getFloatValue(arguments, 0, value, false) == true &&
						MiniScript::getFloatValue(arguments, 1, range, false) == true) {
						returnValue.setValue(Math::absmod(value, range));
					} else {
						Console::println(getMethodName() + "(): " + miniScript->getStatementInformation(statement) + ": argument mismatch: expected arguments: " + miniScript->getArgumentInformation(getMethodName()));
						miniScript->startErrorScript();
					}
				}
			}
		};
		miniScript->registerMethod(new ScriptMethodAbsMod(miniScript));
	}
}

void MathMethods::mul(const span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) {
	if (arguments.size() != 2) {
		Console::println("mul(): " + miniScript->getStatementInformation(statement) + ": argument mismatch: expected arguments: " + miniScript->getArgumentInformation("mul"));
		miniScript->startErrorScript();
		//
		return;
	}
	// custom data types
	for (const auto dataType: dataTypes) {
		if (dataType->mul(miniScript, arguments, returnValue, statement) == true) return;
	}
	// float
	if (MiniScript::hasType(arguments, MiniScript::TYPE_FLOAT) == true) {
		float a;
		float b;
		if (MiniScript::getFloatValue(arguments, 0, a, false) == true &&
			MiniScript::getFloatValue(arguments, 1, b, false) == true) {
			returnValue.setValue(a * b);
			//
			return;
		} else  {
			Console::println("mul(): " + miniScript->getStatementInformation(statement) + ": argument mismatch: expected arguments: " + miniScript->getArgumentInformation("mul"));
			miniScript->startErrorScript();
			//
			return;
		}
	} else {
		// int
		int64_t a;
		int64_t b;
		if (MiniScript::getIntegerValue(arguments, 0, a, false) == true &&
			MiniScript::getIntegerValue(arguments, 1, b, false) == true) {
			returnValue.setValue(a * b);
			//
			return;
		} else  {
			Console::println("mul(): " + miniScript->getStatementInformation(statement) + ": argument mismatch: expected arguments: " + miniScript->getArgumentInformation("mul"));
			miniScript->startErrorScript();
			//
			return;
		}
	}
}

void MathMethods::div(const span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) {
	if (arguments.size() != 2) {
		Console::println("div(): " + miniScript->getStatementInformation(statement) + ": argument mismatch: expected arguments: " + miniScript->getArgumentInformation("div"));
		miniScript->startErrorScript();
		//
		return;
	}
	// custom data types
	for (const auto dataType: dataTypes) {
		if (dataType->div(miniScript, arguments, returnValue, statement) == true) return;
	}
	// float
	if (MiniScript::hasType(arguments, MiniScript::TYPE_FLOAT) == true) {
		float a;
		float b;
		if (MiniScript::getFloatValue(arguments, 0, a, false) == true &&
			MiniScript::getFloatValue(arguments, 1, b, false) == true) {
			returnValue.setValue(a / b);
			//
			return;
		} else  {
			Console::println("div(): " + miniScript->getStatementInformation(statement) + ": argument mismatch: expected arguments: " + miniScript->getArgumentInformation("div"));
			miniScript->startErrorScript();
			//
			return;
		}
	} else {
		// int
		int64_t a;
		int64_t b;
		if (MiniScript::getIntegerValue(arguments, 0, a, false) == true &&
			MiniScript::getIntegerValue(arguments, 1, b, false) == true) {
			returnValue.setValue(a / b);
			//
			return;
		} else  {
			Console::println("div(): " + miniScript->getStatementInformation(statement) + ": argument mismatch: expected arguments: " + miniScript->getArgumentInformation("div"));
			miniScript->startErrorScript();
			//
			return;
		}
	}
}

void MathMethods::add(const span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) {
	if (arguments.size() != 2) {
		Console::println(miniScript->getStatementInformation(statement) + ": argument mismatch: expected arguments: " + miniScript->getArgumentInformation("add"));
		miniScript->startErrorScript();
		return;
	}
	// string concatenation
	if (MiniScript::hasType(arguments, MiniScript::TYPE_STRING) == true) {
		string result;
		for (auto i = 0; i < arguments.size(); i++) {
			result+= arguments[i].getValueAsString();
		}
		returnValue.setValue(result);
		//
		return;
	}
	// custom data types
	for (const auto dataType: dataTypes) {
		if (dataType->add(miniScript, arguments, returnValue, statement) == true) return;
	}
	// float
	if (MiniScript::hasType(arguments, MiniScript::TYPE_FLOAT) == true) {
		float a;
		float b;
		if (MiniScript::getFloatValue(arguments, 0, a, false) == true &&
			MiniScript::getFloatValue(arguments, 1, b, false) == true) {
			returnValue.setValue(a + b);
			//
			return;
		} else  {
			Console::println("add(): " + miniScript->getStatementInformation(statement) + ": argument mismatch: expected arguments: " + miniScript->getArgumentInformation("add"));
			miniScript->startErrorScript();
			//
			return;
		}
	} else {
		// int
		int64_t a;
		int64_t b;
		if (MiniScript::getIntegerValue(arguments, 0, a, false) == true &&
			MiniScript::getIntegerValue(arguments, 1, b, false) == true) {
			returnValue.setValue(a + b);
			//
			return;
		} else  {
			Console::println("add(): " + miniScript->getStatementInformation(statement) + ": argument mismatch: expected arguments: " + miniScript->getArgumentInformation("add"));
			miniScript->startErrorScript();
			//
			return;
		}
	}

}

void MathMethods::sub(const span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) {
	if (arguments.size() != 2) {
		Console::println("sub(): " + miniScript->getStatementInformation(statement) + ": argument mismatch: expected arguments: " + miniScript->getArgumentInformation("sub"));
		miniScript->startErrorScript();
		//
		return;
	}
	// custom data types
	for (const auto dataType: dataTypes) {
		if (dataType->sub(miniScript, arguments, returnValue, statement) == true) return;
	}
	// float
	if (MiniScript::hasType(arguments, MiniScript::TYPE_FLOAT) == true) {
		float a;
		float b;
		if (MiniScript::getFloatValue(arguments, 0, a, false) == true &&
			MiniScript::getFloatValue(arguments, 1, b, false) == true) {
			returnValue.setValue(a - b);
			//
			return;
		} else  {
			Console::println("sub(): " + miniScript->getStatementInformation(statement) + ": argument mismatch: expected arguments: " + miniScript->getArgumentInformation("sub"));
			miniScript->startErrorScript();
			//
			return;
		}
	} else {
		// int
		int64_t a;
		int64_t b;
		if (MiniScript::getIntegerValue(arguments, 0, a, false) == true &&
			MiniScript::getIntegerValue(arguments, 1, b, false) == true) {
			returnValue.setValue(a - b);
			//
			return;
		} else  {
			Console::println("sub(): " + miniScript->getStatementInformation(statement) + ": argument mismatch: expected arguments: " + miniScript->getArgumentInformation("sub"));
			miniScript->startErrorScript();
			//
			return;
		}
	}
}
