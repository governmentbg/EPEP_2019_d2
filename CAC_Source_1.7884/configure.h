struct TSettingData
{
	const char *section;
	const char *name;
	long flags;
	char type;
	long min;
	long max;
	void *value;
	const char *text1;
	const char *text2;
	const char *text3;
	const char *_default;
};

struct TDoubleExt;

class TConfigure
{
public:
	TConfigure();
	~TConfigure() { delete[] buffer; }

	static void Reset() { ForEach(Reset); }
	static void Load(const char *configName);
	static void Apply() { ForEach(ApplyA); }
	static const char *Invalid(const TSettingData *settingData, const char *value);
	static void AddError(const char *text);
	static void AddError(const char *section, const char *name, const char *text);
	static void ShowErrors();
	static void GetGlobal(const char *section, const char *name, char *value);
	static bool GetCurrent(const char *section, const char *name, bool showErrors);
	static bool GetCurrent(const char *section, const char *name, long _default, bool showErrors);
#if TESTVER
	static bool GetCurrent(const char *section, const char *name, const char *_default, bool showErrors);
#else  // TESTVER
#endif  // TESTVER
	static void LoadCache(TGroupArray *settings, const char *section, TGArray<TConfigData> *exclude);
	static const TSettingData *Find(const char *section, const char *name, const char *oper = NULL);
	static void Update(const char *section, const char *name, const char *value);		// local only

private:
	static mstr errors;
	static char *buffer;

	static bool AllowLocal(const TSettingData *settingData);
	static char *Buffer(const TSettingData *settingData);
	static void ForEach(void (*action)(TSettingData *settingData));
#if TESTVER
#else  // TESTVER
	static bool GetCurrent(const char *section, const char *name, const char *_default, bool showErrors);
#endif  // TESTVER
	static void Init(TSettingData *settingData);
	static void Reset(TSettingData *settingData);
	static bool Try(const char *configName, const TSettingData *settingData, char *buffer);
	static bool Apply(TSettingData *settingData);
	static void ApplyA(TSettingData *settingData);
	static void GetValue(TSettingData *settingData);
	static const char *ParseElectricTypes(const char *value, TDoubleExt *exts);
};

bool edit_config_data(TWindow *parent, TConfigData *configData, TGArray<TConfigData> *exclude);
void configure(TWindow *parent);
