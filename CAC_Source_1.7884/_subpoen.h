class TSubpoena: public TLinkWind, public TSubServeData
{
public:
	TSubpoena();
	static TGroup *Clone();

	virtual void ExactCriteria(msql &m, const char *fields = NULL);
	virtual void FloatCriteria(msql &m, const char *fields = NULL);
	void FloatCriteria(char *s, const char *fields = NULL) { TGroup::FloatCriteria(s, fields); }

	virtual void MultiCriteria(msql &m, const char *fields = NULL) const;	// 2008:178 all w/ this leader
	// 2012:045 delete AnnounceIndex; 2014:029 delete attached files
	virtual bool Delete(bool exact);
	virtual void Generate();	// 2015:219 LPR: public for printsub w/ html extra files
	virtual bool Insert();
	static long GenerateRequestId(TCACDataBase *dataBase);	// 2015:089 also for announces

	void Print(char* const dest, const size_t destSz) const;
	void PrintProxy(char* const dest, const size_t destSz) const;
	void PrintLink(char* const dest, const size_t destSz) const;

	void PrintAddress(char *s, long flags = ADDRESS_REMARK) const;
	void PrintAddress(mstr &m, long flags = ADDRESS_REMARK) const;
	void PrintSentDescr(mstr &m) const;
	// 2010:333; 2011:021 LPR: invlsep, default = sidesep
	void PrintNotifieds(mstr &m, TGArray<TLinkWind> *pLinks, const char *sidesep, const char *invlsep = NULL);
	void PrintNotifieds(mstr &m, const char *sidesep, const char *invlsep = NULL);
	void PrintDescription(mstr &m);		// 2009:316
	bool AskDelete() const;				// 2010:011
	bool FindOrigin(TRCDKey &origin) const;

	bool operator<=(const TSubpoena& that) const;
	bool ProxyPresent() const;
	bool Received() const { return strchr(SRS_DELIVEREDS, receivedStatus); }	// 2011:166 FIX: rewritten
	bool RedirectPresent() const;
	// 2018-03-28; 2018-04-02: LPR: mandatory stickers
	void RegardCriteria(msql &m, const char *fields, bool stickers);
	bool HasRegards();	// 2018-04-02: not stickers
	bool OfficiallyNotified() { return officiallyNotified == SUBPOENA_OFFICIALLY_NOTIFIED; }
	char InitialStatus() const;	// 2018-01-19

public:
	long subpoenaNo;

	char subpoenaKind;
	char eventStatus;
	CDate subpoenaDate;
	char officiallyNotified;
	long sideNo;
	long expense;

	long uclp;
	long street;
	char adrNo[SIZE_OF_ADR_NO];
	char subNo[SIZE_OF_SUB_NO];
	char entrance[SIZE_OF_ENTRANCE];
	char floor[SIZE_OF_FLOOR];
	char apartment[SIZE_OF_APARTMENT];
	char blockNo[SIZE_OF_BLOCK_NO];
	char phone[SIZE_OF_PHONE];
	char telex[SIZE_OF_TELEX];
	char fax[SIZE_OF_FAX];
	char email[SIZE_OF_EMAIL];
	char remark[SIZE_OF_ADDRESS_REMARK];

	char delivererUcn[SIZE_OF_UCN];
	char userUcn[SIZE_OF_UCN];
	long byPost;
	long byPostX;
	long regard;

	// 20070718
	char lReceiverUcnType;
	char lReceiverUcn[SIZE_OF_UCN];
	long outRegNo;
	long outRegYear;

	char multi;			// 2008:170
	char country[SIZE_OF_COUNTRY];
	char foreign[SIZE_OF_FOREIGN_ADDRESS];
	long announceNo;		// 2012:039
	long requestId;		// 2013:340
	long execListNo;		// 2014:248
	long execListKind;	// 2014:248
	double coordN;
	double coordE;
	CDate validUntil;		// 2017:214 IRQ: +related

protected:

	// 2009:316 LPR: for PrintDescription()
	void PrintUserUcnDescr(mstr &m, const char *prefix);
	void PrintWllWConverted(mstr &m);
	// 2009:328 LPR: from TConvertedSubpoena
	char converted;

private:
	virtual void Print(mstr &m);	// emulation
};
