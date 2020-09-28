// ----- TCACTarget ----------------------------------------------------------
struct TCACTarget
{
	const char *SenderName;		// T_SENDER.F_NAME - "AREA", "APPEAL" etc.
	const char *ExecSBuild;		// AppName.Exe SpecialBuild - "Areal/District", "Appealative
	const char *BaseAlias;		// BDE ALIAS / ODBC - "SAREAL", SAPPEAL" etc.
	const char *UserName;		// "�������", ����������" � �.�.
	const char *auserName;		// "��������", "�����������" � �.�.

	int GetVersion(TCACDataBase *dataBase) const;	// 0 on error
};
