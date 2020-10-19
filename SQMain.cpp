#include "SQMain.h"
#include "SQConsts.h"
#include "SQFuncs.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <stdarg.h>
#include <string>
#include <fstream>
using namespace std;

PluginFuncs* VCMP;
HSQUIRRELVM v;
HSQAPI sq;
void OnSquirrelScriptLoad() {
	// See if we have any imports from Squirrel
	size_t size;
	int32_t sqId = VCMP->FindPlugin(const_cast<char*>("SQHost2"));
	const void** sqExports = VCMP->GetPluginExports(sqId, &size);

	// We do!
	if (sqExports != NULL && size > 0) {
		// Cast to a SquirrelImports structure
		SquirrelImports** sqDerefFuncs = (SquirrelImports**)sqExports;

		// Now let's change that to a SquirrelImports pointer
		SquirrelImports* sqFuncs = (SquirrelImports*)(*sqDerefFuncs);

		// Now we get the virtual machine
		if (sqFuncs) {
			// Get a pointer to the VM and API
			v = *(sqFuncs->GetSquirrelVM());
			sq = *(sqFuncs->GetSquirrelAPI());

			// Register functions
			RegisterFuncs(v);
			// Register constants
			RegisterConsts(v);
		}
	}
	else
		OutputError("Failed to attach to SQHost2.");
}

uint8_t OnPluginCommand(uint32_t type, const char* text) {
	switch (type) {
	case 0x7D6E22D8:
		OnSquirrelScriptLoad();
		break;
	default:
		break;
	}
	return 1;
}


extern "C" unsigned int VcmpPluginInit(PluginFuncs* pluginFuncs, PluginCallbacks* pluginCalls, PluginInfo* pluginInfo) {
    VCMP = pluginFuncs;

	// Plugin information
	pluginInfo->pluginVersion = 0x110;
	pluginInfo->apiMajorVersion = PLUGIN_API_MAJOR;
	pluginInfo->apiMinorVersion = PLUGIN_API_MINOR;
	strcpy(pluginInfo->name, "squirrel_logfile");
	pluginCalls->OnPluginCommand = OnPluginCommand;
	return 1;
}

