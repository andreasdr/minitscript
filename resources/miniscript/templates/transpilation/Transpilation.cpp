#include "{$class-name}.h"

#include <miniscript/miniscript.h>
#include <miniscript/miniscript/MiniScript.h>

#include <{$base-class-header}>

#undef MINISCRIPT_METHODUSAGE_COMPLAIN
#undef MINISCRIPT_METHODUSAGE_COMPLAIN2
#define MINISCRIPT_METHODUSAGE_COMPLAIN(methodName) miniScript->complain(methodName, statement); miniScript->startErrorScript(); return;
#define MINISCRIPT_METHODUSAGE_COMPLAIN2(methodName, returnValue) miniScript->complain(methodName, statement); miniScript->startErrorScript(); return returnValue;

/*__MINISCRIPT_TRANSPILEDMINISCRIPTCODE_INCLUDES_START__*/
/*__MINISCRIPT_TRANSPILEDMINISCRIPTCODE_INCLUDES_END__*/

/*__MINISCRIPT_TRANSPILEDMINISCRIPTCODE_USINGS_START__*/
/*__MINISCRIPT_TRANSPILEDMINISCRIPTCODE_USINGS_END__*/

{$class-name}::{$class-name}(): {$base-class}() {
	initializeNative();
}

/*__MINISCRIPT_TRANSPILEDMINISCRIPTCODE_DEFINITIONS_START__*/
/*__MINISCRIPT_TRANSPILEDMINISCRIPTCODE_DEFINITIONS_END__*/
