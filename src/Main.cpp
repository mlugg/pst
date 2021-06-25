#include "Main.hpp"
#include "SDK.hpp"
#include "Timer.hpp"
#include "Utils.hpp"

#include <string.h>
#include <stdlib.h>

CON_COMMAND(pst_start, "pst_start [ticks] - manually start the timer") {
	int ticks = 0;

	if (args.argc > 1) {
		ticks = atoi(args.argv[1]);
	}

	Timer::Start(ticks);
}

CON_COMMAND(pst_stop, "pst_stop - manually stop the timer") {
	Timer::Stop();
}


ICvar *g_cvar;
IEngineTool *g_engineTool;
IServerTools *g_serverTools;
IServerGameDLL *g_serverGameDLL;

int g_entParentOffset = -1;
CEntInfo *g_gladosBody;
void *g_player;

static bool IsOnMap(const char *map) {
	const char *name = g_engineTool->GetCurrentMap();

	// Ignore the first 5 character if they're 'maps/' or 'maps\'
	if (!strncmp(name, "maps/", 5) || !strncmp(name, "maps\\", 5)) name += 5;

	// Ignore the last 4 characters '.bsp'
	size_t len = strlen(name) - 4;

	return !strncmp(name, map, len);
};

static int FindOffset(SendTable *table, const char *field) {
	for (size_t i = 0; i < table->nprops; ++i) {
		SendProp *prop = &table->props[i];

		if (!strcmp(prop->varName, field)) {
			return prop->offset;
		}

		if (prop->type == SendPropType::DataTable) {
			int off = FindOffset(prop->dataTable, field);
			if (off != -1) {
				return prop->offset + off;
			}
		}
	}

	return -1;
}

CON_COMMAND(pst_set_offset_target, "DEBUG COMMAND, TO BE REMOVED") {
	if (args.argc <= 1) return;
	for (ServerClass *curClass = g_serverGameDLL->GetAllServerClasses(); curClass; curClass = curClass->next) {
		if (!strcmp(curClass->networkName, "CPortal_Player")) {
			g_entParentOffset = FindOffset(curClass->table, args.argv[1]);
			return;
		}
	}
}

static void InitOffsets() {
	for (ServerClass *curClass = g_serverGameDLL->GetAllServerClasses(); curClass; curClass = curClass->next) {
		if (!strcmp(curClass->networkName, "CPortal_Player")) {
			g_entParentOffset = FindOffset(curClass->table, "m_flAnimTime");
			return;
		}
	}
}

class PST : public IServerPluginCallbacks {
public:
	virtual bool Load(CreateInterfaceFn interfaceFactory, CreateInterfaceFn gameServerFactory) {
		g_cvar = (ICvar *)interfaceFactory("VEngineCvar004", nullptr);
		if (!g_cvar) return false;

		g_engineTool = (IEngineTool *)interfaceFactory("VENGINETOOL003", nullptr);
		if (!g_engineTool) return false;

		g_serverTools = (IServerTools *)gameServerFactory("VSERVERTOOLS001", nullptr);
		if (!g_serverTools) return false;

		g_serverGameDLL = (IServerGameDLL *)gameServerFactory("ServerGameDLL008", nullptr);
		if (!g_serverGameDLL) return false;

		g_cvarIdentifier = g_cvar->AllocateDLLIdentifier();

		for (size_t i = 0; i < ConCommandBase::commandCount; ++i) {
			ConCommandBase *command = ConCommandBase::allCommands[i];
			g_cvar->RegisterConCommand(command);
		}

		InitOffsets();
		//if (g_entParentOffset == -1) return false;

		return true;
	}

	virtual void Unload() {
		g_cvar->UnregisterConCommands(g_cvarIdentifier);
	}

	virtual void Pause() {
	}

	virtual void UnPause() {
	}

	virtual const char *GetPluginDescription() {
		return "PST: Portal Speedrun Timer";
	}

	virtual void LevelInit(const char *mapName) {
	}

	virtual void ServerActivate(void *edictList, int edictCount, int clientMax) {
	}

	virtual void GameFrame(bool simulating) {
		if (!simulating) Timer::AddPauseTick();

		// temporary debug shit
		g_engineTool->Con_NPrintf(2, "0x%8x", *(int *)((char *)g_player + g_entParentOffset));

		if (simulating && g_gladosBody && !g_gladosBody->entity) {
			g_gladosBody = nullptr;
			Timer::Stop();
		}

		g_engineTool->Con_NPrintf(0, "%s", Utils::FormatTime(Timer::GetTicks()).c_str());
	}

	virtual void LevelShutdown() {
		g_player = nullptr;
		Timer::Pause();
	}

	virtual void ClientActive(void *entity) {
		g_player = entity;

		Timer::Resume();

		if (IsOnMap("testchmb_a_00")) {
			if (GetServerTick() == 4260) {
				Timer::Start(3533);
			}
		}

		g_gladosBody = nullptr;
		if (IsOnMap("escape_02")) {
			for (CEntInfo *ent = g_serverTools->FirstEntity(); ent; ent = g_serverTools->NextEntity(ent)) {
				char buf[16] = {0};
				g_serverTools->GetKeyValue(ent, "targetname", buf, sizeof buf);
				if (!strcmp(buf, "glados_body")) {
					g_gladosBody = ent;
				}
			}
		}
	}

	virtual void ClientDisconnect(void *entity) {
	}

	virtual void ClientPutInServer(void *entity, const char *playername) {
	}

	virtual void SetCommandClient(int index) {
	}

	virtual void ClientSettingsChanged(void *edict) {
	}

	virtual int ClientConnect(bool *allowConnect, void *entity, const char *name, const char *address, char *reject, int maxrejectlen) {
		return 0;
	}

	virtual int ClientCommand(void *entity, const void *&args) {
		return 0;
	}

	virtual int NetworkIDValidated(const char *userName, const char *networkID) {
		return 0;
	}

	virtual void OnQueryCvarValueFinished(int cookie, void *playerEntity, int status, const char *cvarName, const char *cvarValue) {
	}

};

PST g_pst;

extern "C" void *CreateInterface(const char *name, int *ret) {
	if (!strcmp(name, "ISERVERPLUGINCALLBACKS002")) {
		if (ret) *ret = 0;
		return (IServerPluginCallbacks *)&g_pst;
	}

	if (ret) *ret = 1;
	return nullptr;
}

uint32_t GetServerTick() {
	return g_engineTool->ServerTick();
}
