class TLinkWind : public TBaseWind
{
public:  // Construction / destruction
	TLinkWind();
	TLinkWind(const char *name, TGroup *(*const tClone)());

	static TGroup *Clone();

public:  // Methods
	virtual void ExactCriteria(msql &m, const char *fields = NULL);

	void FloatCriteriaByOrdinary(msql &m, const char *fields = NULL);
	void SetOrdinary(const TSideWind& side);
	void SetOrdinary(const TSideWind *sideWind) { SetOrdinary(*sideWind); }
	void SetProxy(const TSideWind& side);
	void SetProxy(const TSideWind *side) { SetProxy(*side); }
  void SetProxyKind(const bool useOrdinarySideAddress, const int which_any = 0); //20070730

	void SetRedirect(const TSideWind& side);
	void SetRedirectKind(const bool useOrdinarySideAddress);
  void ClearRedirection();

  virtual bool GetAddressSide(TSideWind& side) const;
	virtual void GetOrdinary(TSideWind& side) const;
	virtual void GetProxy(TSideWind& side) const;
	virtual void GetRedirect(TSideWind& side) const;
  virtual void Print(mstr &m);	// emulation
  virtual void Print(char* s, const size_t sSz) const;

  virtual bool FilledOk() const;
  bool Proxied() const;
  bool Redirected() const;

  bool operator==(const TLinkWind &that) const;
  bool operator<(const TLinkWind &that) const;
  bool operator<=(const TLinkWind &that) const { return operator<(that) || operator==(that); }

  bool SameLink(const TLinkWind *that) const;	// SameWind() + proxyUcn, proxyUcnType, proxyInvolvement
  bool Contains(const TSideWind *sideWind) const;	// 2017:048

public:  // Data
	char proxyUcnType;
	char proxyUcn[SIZE_OF_UCN];
	char proxyInvolvement;
	char proxyKind;

	char redirectUcnType;
	char redirectUcn[SIZE_OF_UCN];
	char redirectInvolvement;
	char redirectKind;

  char state;
  unsigned ordinaryKey;
  unsigned proxyKey;

private:
	void Init();
};  // class TLinkWind

// ----- TLinkWindArray ------------------------------------------------------
// 2014:269 LPR: replacement for TOrderedLinkWind with better ordering
class TLinkWindArray : public TGArray<TLinkWind>
{
public:
	TLinkWindArray(TSideWindArray *tSideWinds);

	int Add(TLinkWind *linkWind);
	void Insert(TLinkWind *linkWind, int index);
	TSideWindArray *Sides() { return sideWinds; }
	TLinkWind *FindLink(const TLinkWind *linkWind) const;

protected:
	static bool LessEqlT(const long t1, const long t2);

	bool Contains(const TLinkWind *linkWind) const { return TGArray<TLinkWind>::Contains(linkWind); }
	int Find(const TLinkWind *linkWind) const { return TGArray<TLinkWind>::Find(linkWind); }
	TSideWind *FindSide(const char *ucn, char ucnType, char involvement) const;
	void FixKeys(TLinkWind *linkWind);

	TSideWindArray *sideWinds;
};
