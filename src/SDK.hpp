#ifndef SDK_HPP
#define SDK_HPP

#include <stddef.h>

struct CCommand;

typedef void *(*CreateInterfaceFn)(const char *name, int *ret);
typedef void *(*InstantiateInterfaceFn)();
typedef void (*CommandCallback)(const CCommand &args);

class IServerPluginCallbacks {
public:
	virtual bool Load(CreateInterfaceFn interfaceFactory, CreateInterfaceFn gameServerFactory) = 0;
	virtual void Unload() = 0;
	virtual void Pause() = 0;
	virtual void UnPause() = 0;
	virtual const char *GetPluginDescription() = 0;
	virtual void LevelInit(const char *mapName) = 0;
	virtual void ServerActivate(void *edictList, int edictCount, int clientMax) = 0;
	virtual void GameFrame(bool simulating) = 0;
	virtual void LevelShutdown() = 0;
	virtual void ClientActive(void *entity) = 0;
	virtual void ClientDisconnect(void *entity) = 0;
	virtual void ClientPutInServer(void *entity, const char *playername) = 0;
	virtual void SetCommandClient(int index) = 0;
	virtual void ClientSettingsChanged(void *edict) = 0;
	virtual int ClientConnect(bool *allowConnect, void *entity, const char *name, const char *address, char *reject, int maxrejectlen) = 0;
	virtual int ClientCommand(void *entity, const void *&args) = 0;
	virtual int NetworkIDValidated(const char *userName, const char *networkID) = 0;
	virtual void OnQueryCvarValueFinished(int cookie, void *playerEntity, int status, const char *cvarName, const char *cvarValue) = 0;
};

class IEngineTool {
public:
	virtual ~IEngineTool();

	virtual int GetToolCount() const = 0;
	virtual const char *GetToolName(int index) const = 0;
	virtual void SwitchToTool(int index) const = 0;
	virtual bool IsTopmostTool(const void *sys) const = 0;
	virtual const void *GetToolSystem(int index) const = 0;
	virtual void *GetTopmostTool() = 0;
	virtual void ShowCursor(bool show) = 0;
	virtual bool IsCursorVisible() const = 0;

	virtual void GetServerFactory(CreateInterfaceFn &factory) = 0;
	virtual void GetClientFactory(CreateInterfaceFn &factory) = 0;
	virtual float GetSoundDuration(const char *name) = 0;
	virtual bool IsSoundStillPlaying(int guid) = 0;
	virtual int StartSound() = 0; // NOTE: INCORRECT SIGNATURE
	virtual void StopSoundByGuid(int guid) = 0;
	virtual float GetSoundDuration(int guid) = 0;
	virtual bool IsLoopingSound(int guid) = 0;
	virtual void ReloadSound(const char *sample) = 0;
	virtual void StopAllSounds() = 0;
	virtual float GetMono16Samples() = 0; // NOTE: INCORRECT SIGNATURE
	virtual void SetAudioState() = 0; // NOTE: INCORRECT SIGNATURE
	virtual void Command(const char *cmd) = 0;
	virtual void Execute() = 0;
	virtual const char *GetCurrentMap() = 0;
	virtual void ChangeToMap(const char *mapname) = 0;
	virtual bool IsMapValid(const char *mapname) = 0;
	virtual void RenderView() = 0; // NOTE: INCORRECT SIGNATURE
	virtual bool IsInGame() = 0;
	virtual bool IsConnected() = 0;
	virtual int GetMaxClients() = 0;
	virtual bool IsGamePaused() = 0;
	virtual void SetGamePaused(bool paused) = 0;
	virtual float GetTimescale() = 0;
	virtual void SetTimescale(float scale) = 0;
	virtual float GetRealTime() = 0;
	virtual float GetRealFrameTime() = 0;
	virtual float Time() = 0;
	virtual float HostFrameTime() = 0;
	virtual float HostTime() = 0;
	virtual int HostTick() = 0;
	virtual int HostFrameCount() = 0;
	virtual float ServerTime() = 0;
	virtual float ServerFrameTime() = 0;
	virtual int ServerTick() = 0;
	virtual float ServerTickInterval() = 0;
	virtual float ClientTime() = 0;
	virtual float ClientFrameTime() = 0;
	virtual int ClientTick() = 0;
	virtual void SetClientFrameTime(float frametime) = 0;
	virtual void ForceUpdateDuringPause() = 0;
	virtual void *GetModel() = 0; // NOTE: INCORRECT SIGNATURE
	virtual void *GetStudioModel() = 0; // NOTE: INCORRECT SIGNATURE
	virtual void Con_NPrintf(int pos, const char *fmt, ...) = 0;
	virtual void Con_NXPrintf(const void *info, const char *fmt, ...) = 0; // NOTE: INCORRECT SIGNATURE
	virtual void GetGameDir(char *gamedir, int maxLength) = 0;
	virtual void GetScreenSize(int &width, int &height) = 0;
	// NOTE: THERE IS MORE
};

class CEntInfo {
public:
	void *entity;
	int serial;
	CEntInfo *prev, *next;
};

class IServerTools {
public:
	virtual ~IServerTools();
	virtual void *GetIServerEntity(void *clientEntity) = 0;
	virtual bool SnapPlayerToPosition() = 0; // NOTE: INCORRECT SIGNATURE
	virtual bool GetPlayerPosition() = 0; // NOTE: INCORRECT SIGNATURE
	virtual bool SetPlayerFOV() = 0; // NOTE: INCORRECT SIGNATURE
	virtual int GetPlayerFOV(void *clientPlayer) = 0;
	virtual bool IsInNoClipMode(void *clientPlayer) = 0;
	virtual CEntInfo *FirstEntity() = 0;
	virtual CEntInfo *NextEntity(CEntInfo *entity) = 0;
	virtual void *FindEntityByHammerID(int hammerId) = 0;
	virtual bool GetKeyValue(void *entity, const char *field, char *value, int maxLen) = 0;
	// NOTE: THERE IS MORE
};

struct SendTable;

enum class SendPropType {
	Int,
	Float,
	Vector,
	VectorXY,
	String,
	Array,
	DataTable,
	Int64,
	NUMSendPropTypes,
};

struct SendProp {
	void *VMT; // fuck these methods i dont care bout em
	void *recvProp;
	SendPropType type;
	int bits;
	float lowValue;
	float highValue;
	SendProp *arrayProp;
	void *arrayLengthProxy;
	int elements;
	int elementStride;
	char *excludeDTName;
	char *parentArrayPropName;
	char *varName;
	float highLowMul;
	int flags;
	void *proxyFn;
	void *dataTableProxyFn;
	SendTable *dataTable;
	int offset;
	const void *extraData;
};

struct SendTable {
	SendProp *props;
	int nprops;
	char *netTableName;
	void *precalc;
};

struct ServerClass {
	char *networkName;
	SendTable *table;
	ServerClass *next;
	int classId;
	int instanceBaselineIndex;
};

class IServerGameDLL {
public:
	virtual bool DLLInit() = 0; // NOTE: INCORRECT SIGNATURE
	virtual bool ReplayInit(CreateInterfaceFn replayFactory) = 0;
	virtual bool GameInit() = 0;
	virtual bool LevelInit() = 0; // NOTE: INCORRECT SIGNATURE
	virtual void ServerActivate() = 0; // NOTE: INCORRECT SIGNATURE
	virtual void GameFrame(bool simulating) = 0;
	virtual void PreClientUpdate(bool simulating) = 0;
	virtual void LevelShutdown() = 0;
	virtual void GameShutdown() = 0;
	virtual void DLLShutdown() = 0;
	virtual float GetTickInterval() const = 0;
	virtual ServerClass *GetAllServerClasses() = 0;
	// NOTE: THERE IS MORE
};

extern int g_cvarIdentifier;

class ConCommandBase {
public:
	ConCommandBase(const char *name, const char *helpString, int flags)
		: next(nullptr)
		, registered(false)
		, name(name)
		, helpString(helpString)
		, flags(flags)
	{
		ConCommandBase::allCommands[ConCommandBase::commandCount++] = this;
	}

	virtual ~ConCommandBase() { }

	virtual bool IsCommand() = 0;

	virtual bool IsFlagSet(int flag) const {
		return this->flags & flag;
	}

	virtual void AddFlags(int flags) {
		this->flags |= flags;
	}

	virtual const char *GetName() const {
		return this->name;
	}

	virtual const char *GetHelpText() const {
		return this->helpString;
	}

	virtual bool IsRegistered() const {
		return this->registered;
	}

	virtual int GetDLLIdentifier() const {
		return g_cvarIdentifier;
	}

	virtual void Create(const char *name, const char *helpString = nullptr, int flags = 0) { }

	virtual void Init() { }

	ConCommandBase *next;
	bool registered;
	const char *name;
	const char *helpString;
	int flags;

	static ConCommandBase *allCommands[];
	static size_t commandCount;
};

class ConCommand : public ConCommandBase {
public:
	ConCommand(const char *name, const char *helpString, int flags, CommandCallback cbk)
		: ConCommandBase(name, helpString, flags)
		, callback(cbk)
		, completionCallback(nullptr)
		, hasCompletionCallback(0)
		, usingNewCommandCallback(1)
		, usingCommandCallbackInterface(0)
	{ }

	virtual ~ConCommand() { }

	virtual bool IsCommand() { return true; }

	virtual int AutoCompleteSuggest(const char *partial, void *commands) { return 0; }

	virtual bool CanAutoComplete() { return false; }

	virtual void Dispatch(const CCommand &cmd) { this->callback(cmd); }

	CommandCallback callback;
	void *completionCallback;

	bool hasCompletionCallback : 1;
	bool usingNewCommandCallback : 1;
	bool usingCommandCallbackInterface : 1;
};

class ICvar {
public:
	virtual bool Connect(CreateInterfaceFn factory) = 0;
	virtual void Disconnect() = 0;
	virtual void *QueryInterface(const char *interfaceName) = 0;
	virtual int Init() = 0;
	virtual void Shutdown() = 0;

	virtual int AllocateDLLIdentifier() = 0;
	virtual void RegisterConCommand(ConCommandBase *commandBase) = 0;
	virtual void UnregisterConCommand(ConCommandBase *commandBase) = 0;
	virtual void UnregisterConCommands(int id) = 0;
	virtual const char *GetCommandLineValue(const char *varName) = 0;
	virtual ConCommandBase *FindCommandBase(const char *name) = 0;
	virtual const ConCommandBase *FindCommandBase(const char *name) const = 0;
	// NOTE: THERE IS MORE
};

struct CCommand {
	int argc;
	int argv0Size;
	char argSBuffer[512];
	char argvBuffer[512];
	const char *argv[64];
};

#define FL_EDICT_FULL (1<<2)

struct IServerUnknown {
	virtual ~IServerUnknown() {}
	virtual void SetRefEHandle(const int &handle) = 0;
	virtual const int &GetRefEHandle() const = 0;
	virtual void *GetCollideable() = 0;
	virtual void *GetNetworkable() = 0;
	virtual void *GetBaseEntity() = 0;
};

struct edict_t {
	int stateFlags;
	int networkSerialNumber;
	void *networkable;
	IServerUnknown *unk;
};

#define CON_COMMAND(name, desc) \
	static void name##_callback(const CCommand &); \
	static ConCommand name(#name, desc, 0, &name##_callback); \
	static void name##_callback(const CCommand &args)

#endif
