// ----- TSubpoenaVariantGroup -----------------------------------------------
// 2008:169 LPR: multi-person subpoena
class TSubpoenaVariantGroup : public TDummyGroup
{
public:
	TSubpoenaVariantGroup(TSideWind *tSideWind, TGArray<TLinkWind> *tAllLinks, TGArray<TSideWind> *tSideWinds,
		TGArray<TUCNGroup> *tAffecteds, TRCDKeyContainer *container, const TLawsuit *tLawsuit);
	virtual ~TSubpoenaVariantGroup();

	void OppositeLawers(mstr &m);
	static void Affected(TGArray<TUCNGroup> *affecteds, const TBaseWind *baseWind);
	void Affected(TBaseWind *baseWind) { Affected(affecteds, baseWind); }

	char subpoenaKind;
	TGArray<TLinkWind> links;	// selected only
	TLinkWind preLink;
	long flags;
	long flagsX;
	TSideWind *const sideWind;
	// 2013:337 for legal aid
	TGArray<TLegalSide> laids;	// selected only
	TGArray<TSideWind> sameSides;
	TGArray<TSideWind> opposites;
	TGArray<TSideWind> thirdSides;
	TGArray<TSideWind> *const allSides;			// 2014:296 exec list, printsub cache
	char *const categories;					// 2014:338 exec list, printsub cache

	TGArray<TLinkWind> *const allLinks;
	TGArray<TUCNGroup> *const affecteds;
	TExecList *execList;
	const char category;

	// 2015:076 compute links on choose subpoena kind
	int notifySelect;
	void FillLinks(TGArray<TLinkWind> *tLinks, bool allowLinks);
	const TLawsuit *lawsuit;	// 2016:243 for e-portal
	mstr receiver;			// 2016:244 for e-portal
	CDate subpoenaDate;		// 2017:058 +related
	CDate validDate;			// 2017:213 +related

protected:
	bool lawer;
	bool crime;
};

void process_subpoenas(TWindow* parent, TRCDKeyContainer* container, char status, TSideWind *sideWind, int sideIndex,
	TGArray<TLinkWind> *links, TGArray<TSideWind> *allSides, TGArray<TUCNGroup> *affecteds, const TLawsuit *lawsuit);
