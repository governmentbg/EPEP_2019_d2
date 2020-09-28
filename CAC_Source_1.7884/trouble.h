struct TElectricTrouble
{
	TElectricTrouble(TScaleDialog *tParent, int tType, bool tExtraDB = false);
	~TElectricTrouble();

	void BackupNames();
	void Enable(bool enable);
	void ActChanged();
	void ClearAct();
	void IndexChanged(char indexKind);
	void Swap();

	void Name(const TRCDKeyContainer *container, const CDate &date);
	void Name(const TRCDKeyContainer *container) { Name(container, electric_date(container)); }
	bool Move(const TRCDKeyContainer *container, const CDate &date);
	bool Move(const TRCDKeyContainer *container) { return Move(container, electric_date(container)); }
	void PostMove(const TRCDKeyContainer *container, const CDate &date, char targetKind = '\0');
	void SetPortreg(const TRCDKeyContainer *container, const CDate &date, bool tPortreg, bool edit);
	void Edit(bool update);
	bool IsValid();

	int type;
	TButton *swap;
	TStringFace *name[TROUBLE_COUNT];
	TButton *attach;
	bool extraDB;
	TRegisterAct *act;
	bool primary;

protected:
	TScaleDialog *parent;
	bool portreg;

	int FxType(int type);

private:
	char backup[TROUBLE_COUNT][SIZE_OF_PATH];
};
