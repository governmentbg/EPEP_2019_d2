//{{TDialog = TConnectDialog}}
class TConnectDialog : public TDialog
{
public:
	TConnectDialog(TWindow* parent, const TCACTarget *tTarget, int tVersion);
	virtual ~TConnectDialog();

protected:
	enum
	{
		DBT_BDE_INTERBASE = 0,		// must be 0
		DBT_ODBC_FIREBIRD,
		DBT_ODBC_POSTGRES
	};

	enum
	{
		TARGET_STEP = 2,
		ALIAS_COUNT = DBT_COUNT * 4
	};

	const TCACTarget *const target;
	const int version;
	TIArray<TCACConnect> Connects;
	bool duplicates;

	TComboBox *alias;
	TEdit *value;
	char tValue[DBIMAXSCFLDLEN];
	mstr tNames[ALIAS_COUNT];		// 2016:188

	TCACConnect *NewConnect(const char *name, int baseType);
	void Load(int index, char *value);
	void Commit(TCACConnect *connect, int index);
	virtual bool IsValid();

//{{TConnectDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TConnectDialogVIRTUAL_END}}

//{{TConnectDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
	void AliasCBNSelChange();
//{{TConnectDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TConnectDialog);
};    //{{TConnectDialog}}
