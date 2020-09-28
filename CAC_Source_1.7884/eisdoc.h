// ----- TEisAlias -----------------------------------------------------------
class TEisAlias : public TLongAlias
{
public:
	TEisAlias(const char *name, long *data);

protected:
	virtual void ExportToControl(TControl *control) const;
};

// ----- MPP -----------------------------------------------------------------
class MPP : public TXMLGroup
{
public:
	MPP();

	CDate dta;
	double rzm;
	long sid;
	long ste;
	long str;
	long vid;
};

// ----- NKZ -----------------------------------------------------------------
class NKZ : public TXMLGroup
{
public:
	NKZ();

	virtual void Print(mstr &m);	// tip + vid

	long iptsrkden;
	CDate iptsrkdta;
	long iptsrkgdn;
	long iptsrkmsc;
	long iptsrksdc;
	long akt;
	CDate aktdta;
	long den;
	long gdn;
	long msc;
	long ncn;
	long rjm;
	double rzm;
	long sdc;
	long sid;
	long str;
	long tip;
	long vid;
};

// ----- IZPNKZ --------------------------------------------------------------
class IZPNKZ : public TXMLObject
{
public:
	IZPNKZ();

	CDate dta;
	long nkzsid;
	long ste;
	long steprc;
	long str;
};

// ----- NPRFZLSTA -----------------------------------------------------------
class NPRFZLSTA : public TXMLObject
{
public:
	NPRFZLSTA();

	long sid;
	CDate dta;
	long kcv;
	long osn;
	long sts;
};

// ----- NPL -----------------------------------------------------------------
class NPL : public TXMLObject
{
public:
	NPL(TGroup *parent);

	long alk;
	long bnz;
	long jiv;
	long lekpsx;
	long lrknrk;
	long mlt;
	long mltot;
	long nkzpdm;
	long nrk;
	long nrkvid;
	long otc;
	long pneprc;
	long pro;
	long prs;
	long rzpnrk;
	long sid;
	long skt;
	long spi;
	long ucl;
	long uclprc;
	long usljiv;
	long vmk;
	long vui;
	long xms;
	long zdrste;
};

// ----- SBC -----------------------------------------------------------------
class SBC : public TXMLObject
{
public:
	SBC();

	long etn;
	long grj;
	long obr;
	long ple;
	long rcd;
	long sid;
	long spj;
	long trd;
	long znq;
	long rge;
};

// ----- VSL -----------------------------------------------------------------
class VSL : public TXMLObject
{
public:
	VSL(TGroup *parent);

	long dlj;
	long ktg;
	long pdl;
	long sid;
	long zvn;
};

// ----- TEisGroup -----------------------------------------------------------
class TEisGroup : public TXMLGroup
{
public:
	TEisGroup(const char *xmlName, const char *groupName, TGroup *(*tClone)());

	virtual void ExactCriteria(msql &m, const char *fields = NULL) = 0;
	virtual void FloatCriteria(msql &m, const char *fields = NULL);

	void LoadFloat(TXMLArray *array, TEisGroup *group, TXMLArray *descript = NULL);

	TRCDKey key;
};

// ----- ADR -----------------------------------------------------------------
class ADR : public TXMLObject
{
public:
	ADR();

	long sid;
	long tip;
	long drj;
	long nsmbgr;
	long rjn;
	long pstkod;
	long mstvid;
	long kodpdl;
	char nsmchj[SIZE_OF_EISPP_STRING];
	char krdtxt[SIZE_OF_EISPP_STRING];
	char nmr[SIZE_OF_EISPP_STRING];
	char blk[SIZE_OF_EISPP_STRING];
	char vhd[SIZE_OF_EISPP_STRING];
	char etj[SIZE_OF_EISPP_STRING];
	char apr[SIZE_OF_EISPP_STRING];
	char msttxt[SIZE_OF_EISPP_STRING];
	long loc;
};

// ----- FZL --------------------------------------------------------------------
class FZL : public TEisGroup
{
public:
	FZL();
	static TGroup *Clone();

	virtual void ExactCriteria(msql &m, const char *fields = NULL);
	void LoadCitizen(const TCitizen *citizen);
	bool SaveCitizen(TCitizen *citizen);
	virtual void Print(mstr &m);
	const char *Invalid();

	IZPNKZ *izpnkz;
	NPRFZLSTA *nprfzlsta;

	TXArray<MPP> *mpp;
	TXArray<NKZ> *nkz;

	long &sid;		// from nprfzlsta
	long grjbgr;
	long grjchj;
	long pol;
	char egn[SIZE_OF_UCN];
	char lnc[SIZE_OF_UCN];
	char ime[SIZE_OF_EISPP_STRING];
	char prz[SIZE_OF_EISPP_STRING];
	char fma[SIZE_OF_EISPP_STRING];
	char imecyr[SIZE_OF_EISPP_STRING];
	char imelat[SIZE_OF_EISPP_STRING];
	CDateTime dtarjd;
	CDateTime dtapnl;

	// internal
	bool dirty;
	NPL *npl;
	VSL *vsl;

protected:
	FZL(const char *xmlName, TGroup *(*tClone)());
	void Init();

	void PrintImeCyr();
	bool SaveFamily(TCitizen *citizen, const char *family) const;
};

// ----- SBH -----------------------------------------------------------------
class SBH : public TXMLObject
{
public:
	SBH();

	long sid;
	long stn;
	char txt[SIZE_OF_EISPP_STRING];
	long vid;
};

// ----- SRK -----------------------------------------------------------------
class SRK : public TXMLObject
{
public:
	SRK();

	void Force();

	short den;
	CDate dta;
	CDate dtadod;
	short gdn;
	short	msc;
	short obt;
	short sdc;
	long sid;
	long vid;
};

// ----- DVJDLO --------------------------------------------------------------
class DVJDLO : public TXMLObject
{
public:
	DVJDLO();

	long sid;
	long prc;
	CDate dta;
	long stripr;
	long strplc;
	long vid;
	char opi[SIZE_OF_EISPP_STRING];
	char dvn[SIZE_OF_EISPP_STRING];
};

// ----- NPRPNESTA -----------------------------------------------------------
class NPRPNESTA : public TXMLObject
{
public:
	NPRPNESTA();

	CDate dta;
	long sts;
};

// ----- PNESTA --------------------------------------------------------------
class PNESTA : public TXMLRequiredObject
{
public:
	PNESTA();

	long kcq;
	long stpdvs;
	long sts;
	CDate stsdta;
};

// ----- DLOSTA --------------------------------------------------------------
class DLOSTA : public TXMLObject
{
public:
	DLOSTA();

	long sid;
	CDate dtasts;
	long sts;
};

// ----- TNmrGroup -----------------------------------------------------------
class TNmrGroup : public TEisGroup
{
public:
	TNmrGroup(const char *xmlName, const char *groupName, TGroup *(*tClone)(), const char *tNmrName, char tLetter);

	char nmr[SIZE_OF_EISPP_NMR];

	virtual bool Insert();

protected:
	const char *nmrName;
	char letter;
};

// ----- PNE_ZVK -------------------------------------------------------------
class PNE_ZVK : public TNmrGroup
{
public:
	PNE_ZVK(const char *xmlName = NULL, TGroup *(*tClone)() = NULL);
	static TGroup *Clone();

	virtual void ExactCriteria(msql &m, const char *fields = NULL);

	long sid;
};

// ----- PNE -----------------------------------------------------------------
class PNE : public PNE_ZVK
{
public:
	PNE();
	static TGroup *Clone();

	virtual void Print(mstr &m);	// npr + kcq

	ADR *adr;
	NPRPNESTA *nprpnesta;
	PNESTA *pnesta;

	long brjnzv;
	long brjrne;
	long brjubt;
	CDateTime dtadod;
	CDateTime dtaotd;
	long otdtip;
	double stinsn;
	double stiobz;
	double stivst;
	char stv[SIZE_OF_EISPP_STRING];
	char txt[SIZE_OF_EISPP_STRING];
	long chs;
	long vmerzk;

	// internal
	bool dirty;

protected:
	PNE(const char *xmlName, TGroup *(*const tClone)());

	void Init();
};

long find_pne_index(TXArray<PNE> *pnes, long pnesid);
PNE *find_pne(TXArray<PNE> *pnes, long pnesid);

// ----- NKZPNE --------------------------------------------------------------
class NKZPNE : public TXMLGroup
{
public:
	NKZPNE();

	virtual void Print(mstr &m);

	long den;
	long gdn;
	long msc;
	double rzm;
	long sdc;
	long sid;
	long vid;
};

// ----- SCQ -----------------------------------------------------------------
class SCQ : public TXMLObject
{
public:
	SCQ();

	TXArray<NKZPNE> *nkzpne;

	CDate dta;
	long osn;
	long rlq;
	long sid;
	long str;
	char stv[SIZE_OF_EISPP_STRING];
	long vid;
};

// ----- NPRFZLPNE -----------------------------------------------------------
class NPRFZLPNE : public TEisGroup
{
public:
	NPRFZLPNE(const char *xmlName = NULL, TGroup *(*tClone)() = NULL);
	static TGroup *Clone();

	virtual void ExactCriteria(msql &m, const char *fields = NULL);
	virtual void FloatCriteria(msql &m, const char *fields = NULL);

	SCQ *scq;
	NPL *npl;
	SBC *sbc;
	VSL *vsl;

	long fzlsid;
	long pnesid;
	long fzlpnesid;
};

long find_nprfzlpne_index(TXArray<NPRFZLPNE> *nprfzlpnes, long pnesid);
NPRFZLPNE *find_nprfzlpne(TXArray<NPRFZLPNE> *nprfzlpnes, long pnesid);

// ----- DLOOSN --------------------------------------------------------------
class DLOOSN : public TXMLGroup
{
public:
	DLOOSN();

	const char *LoadConnect(const TConnect *connect);

	long gdn;
	long ncnone;
	char nmr[SIZE_OF_EISPP_STRING];
	long pmt;
	long sid;
	long sig;
	long sprvid;
	long str;
	long vid;
};

// ----- NFL -----------------------------------------------------------------
class NFL : public TXMLGroup
{
public:
	NFL(const char *xmlName = NULL, TGroup *(*tClone)() = NULL);
	static TGroup *Clone();

	ADR *adr;
	long drj;
	char eik[SIZE_OF_EISPP_STRING];
	long grp;
	long jrdstt;
	char plnnme[SIZE_OF_EISPP_STRING];
	long sid;
	char skrnme[SIZE_OF_EISPP_STRING];
	long vid;
};

// ----- sDLO ----------------------------------------------------------------
class sDLO : public TXMLGroup
{
public:
	sDLO(const char *xmlName = NULL, TGroup *(*tClone)() = sDLO::Clone);
	static TGroup *Clone();

	long gdn;
	long ncnone;
	char nmr[SIZE_OF_EISPP_STRING];
	long pmt;
	long sid;
	long sig;
	long sprvid;
	long str;
	long vid;
};

// ----- DLO -----------------------------------------------------------------
class DLO : public sDLO
{
public:
	DLO();
	static TGroup *Clone();

	void LoadConnects(TRCDKeyContainer *container);

	DLOSTA *dlosta;
	TXArray<FZL> *fzl;
	TXArray<NFL> *nfl;
	TXArray<PNE> *pne;
	TXArray<NPRFZLPNE> *nprfzlpne;
	TXArray<DLOOSN> *dloosn;
};

// ----- NPR -----------------------------------------------------------------
class NPR : public TNmrGroup
{
public:
	NPR();

	virtual void ExactCriteria(msql &m, const char *fields = NULL);
	virtual void FloatCriteria(msql &m, const char *fields = NULL);

	void LoadSideWinds(TRCDKeyContainer *container, TSideWindArray *sideWinds);

	long Generate();
	void InitGenerator();

	DLO *dlo;

	long drj;
	CDate dta;
	long ndzprk;
	long sid;
	long str;
	char opitxt[SIZE_OF_EISPP_STRING];

	// internal
	int fzlmin;
	long scqnmk;
	long nprfzlnmk;

protected:
	long minsid;

	long FetchMinSid(long absmin);
};

// ----- SBE -----------------------------------------------------------------
class SBE : public TXMLGroup
{
public:
	SBE();
	SBE(long vid);

	static long Index(long vid, long sid = 0);	// 2013:009 for non-list sbe-s
	long Index();
	void ApplyDesc(long desc);
	virtual void Print(mstr &m);
	virtual void Write(TEisFile &x);
	void LoadFzlPneNpr();

	SBH *sbh;
	SRK *srk;
	DVJDLO *dvjdlo;
	NPR *npr;
	TXArray<SBE> *sbe;

	CDate dkpdta;
	char dkpnmr[SIZE_OF_EISPP_STRING];
	long dkpstr;
	long dkpvid;
	long sid;
	long sljstr;
	long stpskt;
	long vid;
	char elementType[SIZE_OF_EISPP_STRING];	// "sbereg" | "sbezvk"
	long mnu;

	// internal
	long sbhnmk;
	bool dirty;
	long intvid;

protected:
	void Init();
	long FetchDesc(const char *name);
};

// ----- VHD -----------------------------------------------------------------
class VHD : public TXMLRequiredObject
{
public:
	VHD(SBE *tSbe);

	SBE *sbe;
};

// ----- KST -----------------------------------------------------------------
class KST : public TXMLRequiredObject
{
public:
	KST(SBE *sbe);

	char armStrSid[SIZE_OF_EISPP_STRING];
	char sesSid[SIZE_OF_EISPP_STRING];
	char usrRab[SIZE_OF_EISPP_STRING];
	char usrRej[SIZE_OF_EISPP_STRING];
	char usrSid[SIZE_OF_EISPP_STRING];
	char armSid[SIZE_OF_EISPP_STRING];
	long resSid;
	char lgtSid[SIZE_OF_EISPP_STRING];
	char sbeVid[SIZE_OF_EISPP_STRING];
	CDate sbeDta;
	char okaSid[SIZE_OF_EISPP_STRING];
};

// ----- SubContext ----------------------------------------------------------
enum { SIZE_OF_CONTEXT_DATE = 23 + 1 };

class SubContext : public TXMLRequiredObject
{
public:
	SubContext();

	char delovodenNumber[SIZE_OF_EISPP_STRING];
	char ConfidenceLevel[SIZE_OF_EISPP_STRING];
	long creatorSID;
	char destinSID[SIZE_OF_EISPP_STRING];
	char dispTemplate[SIZE_OF_EISPP_STRING];
	char fileType[SIZE_OF_EISPP_STRING];
	char toDate[SIZE_OF_CONTEXT_DATE];
	char fromDate[SIZE_OF_CONTEXT_DATE];
};

// ----- Argument ------------------------------------------------------------
class Argument : public TXMLRequiredObject
{
public:
	Argument(const char *nprnmr);

	long IDArg;
	char value[SIZE_OF_CONTEXT_DATE];
};

// ----- TIPVHD --------------------------------------------------------------
class TIPVHD : public TXMLRequiredObject
{
public:
	TIPVHD(const char *nprnmr);

	Argument *argument;
	long IDSpravka;
};

// ----- ZQKSPRVHD -----------------------------------------------------------
class ZQKSPRVHD : public TXMLRequiredObject
{
public:
	ZQKSPRVHD(const char *nmrnmr);

	SubContext *subcontext;
	TIPVHD *tipvhd;

	char TypeSpravka[SIZE_OF_EISPP_STRING];
	char ZaqvkaName[SIZE_OF_EISPP_STRING];
	char subType[SIZE_OF_EISPP_STRING];
	long IDZaqvka;
	char SPRDescription[SIZE_OF_EISPP_STRING];
};

// ----- sFZL ----------------------------------------------------------------
class sFZL : public FZL
{
public:
	sFZL();
	static TGroup *Clone();
};

// ----- sNFL ----------------------------------------------------------------
class sNFL : public NFL
{
public:
	sNFL();
	static TGroup *Clone();
};

// ----- PNESPF --------------------------------------------------------------
class PNESPF : public TXMLGroup
{
public:
	PNESPF();

	char opi[SIZE_OF_EISPP_STRING];
};

// ----- sPNE ----------------------------------------------------------------
class sPNE : public PNE
{
public:
	sPNE();
	virtual ~sPNE();
	static TGroup *Clone();

	TXArray<PNESPF> *pnespf;
};

long find_pne_index(TXArray<sPNE> *pnes, long pnesid) { return find_pne_index((TXArray<PNE> *) pnes, pnesid); }
sPNE *find_pne(TXArray<sPNE> *pnes, long pnesid) { return (sPNE *) find_pne((TXArray<PNE> *) pnes, pnesid); }

// ----- sNPRFZLPNE ----------------------------------------------------------
class sNPRFZLPNE : public NPRFZLPNE
{
public:
	sNPRFZLPNE();
	static TGroup *Clone();
};

// ----- sNPRAKTSTS ----------------------------------------------------------
class sNPRAKTSTS : public TNmrGroup
{
public:
	sNPRAKTSTS();

	virtual void ExactCriteria(msql &m, const char *fields = NULL);

	TXArray<sFZL> *fzl;
	TXArray<sNFL> *nfl;
	TXArray<sPNE> *pne;
	TXArray<sNPRFZLPNE> *nprfzlpne;

	long drj;
};

// ----- RZTOPRSPR -----------------------------------------------------------
class RZTOPRSPR : public TXMLRequiredObject
{
public:
	RZTOPRSPR();

	sNPRAKTSTS *npr;
};

// ----- RZT -----------------------------------------------------------------
class RZT : public TXMLRequiredObject
{
public:
	RZT();

	RZTOPRSPR *rztoprspr;
};

// ----- DATA ----------------------------------------------------------------
class DATA : public TXMLRequiredObject
{
public:
	DATA(SBE *sbe);
	DATA(const char *nprnmr);
	DATA();

	VHD *vhd;
	ZQKSPRVHD *zqksprvhd;
	RZT *rzt;
	KST *kst;
};

// ----- Property ------------------------------------------------------------
class Property : public TXMLGroup
{
public:
	Property();
	Property(const char *tName, const char *s);
	Property(const char *tName, long l);

	char name[SIZE_OF_EISPP_STRING];
	char value[SIZE_OF_EISPP_STRING];

protected:
	void Init(const char *name);
};

// ----- Properties ----------------------------------------------------------
class Properties : public TXMLRequiredObject
{
public:
	Properties(long pkt_tip);

	TXArray<Property> *property;
};

// ----- EISPAKET ------------------------------------------------------------
class EISPAKET : public TXMLRequiredObject
{
public:
	EISPAKET(SBE *sbe);

	virtual void Write(TEisFile &x) { TXMLRequiredObject::Write(x); }
	static SBE *ReadEF(const char *name, TRCDKeyContainer *container, bool sbezvk);
	static SBE *ReadEF(TRCDKeyContainer *container, long index, bool sbezvk);
	bool Check(TRCDKeyContainer *container);
	void Write(TRCDKeyContainer *container);
	void Insert(TRCDKeyContainer *container);
	void Update(TRCDKeyContainer *container, SBE *zvk, long resSid);
	static void Delete(TRCDKeyContainer *container, SBE *zvk);
	static void Delete(TRCDKeyContainer *container, long index);

	Properties *properties;
	DATA *data;
};

// ----- TEisQuery -----------------------------------------------------------
class TEisQuery : public TXMLRequiredObject
{
public:
	TEisQuery(TWindow *tParent, const char *tNprnmr, Properties *tProperties, DATA *tData, TOriginContainer *tContainer);

	TWindow *parent;
	Properties *properties;
	DATA *data;
	TOriginContainer *container;

	sign Execute();
	
protected:
	virtual bool Check() { return true; }
	virtual void Reset() { }

	char base[SIZE_OF_TWICE_BASE];
	char outName[SIZE_OF_PATH];
	char xmlName[SIZE_OF_PATH];
	char errName[SIZE_OF_PATH];
	const char *nprnmr;
};

// ----- EISTSAKT ------------------------------------------------------------
class EISTSAKT : public TEisQuery
{
public:
	EISTSAKT(TWindow *parent, TOriginContainer *container, const char *nprnmr);

	sign Execute();
	bool Import();
	void Remove() { remove_file(xmlName, "XML"); }

protected:
	virtual void Write(TEisFile &x);
};
