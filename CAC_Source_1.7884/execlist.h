// ----- TExecLink -----------------------------------------------------------
class TExecLink : public TLinkWind, public TTryAddress
{
public:
	TExecLink();
	TExecLink(TSubpoena *subpoena);
	static TGroup *Clone();

	void TryAddress() { TTryAddress::TryAddress(ucn, ucnType); }

	virtual void Print(mstr &m);	// link - address
	virtual void Print(char* s, const size_t sSz) const { TLinkWind::Print(s, sSz); }
};

// ----- TFloatExecListTotal -------------------------------------------------
class TFloatExecListTotal : public TCurrency
{
public:
	TFloatExecListTotal();

	bool operator<=(const TFloatExecListTotal &that) { return order <= that.order; }

	double sum;
};

// ----- TFloatExecListTotalArray --------------------------------------------
class TFloatExecListTotalArray : TGSArray<TFloatExecListTotal>
{
public:
	void AddSum(const TExecListSumBase *sum);
	void Print(mstr &m, const char *separator) const;
};

// ---- functions ------------------------------------------------------------
void load_exec_list_data(TExecList *execList);
bool process_exec_list(TWindow *parent, TExecList *execList, TGArray<TLinkWind> &xLinks, TGArray<TSideWind> &sideWinds,
	const char *categories, char category, bool edit);
void emit_exec_list_additional_text(mstr &m, TExecList *execList, TGArray<TLinkWind> &xLinks);
void exec_list_book(TWindow *parent);
void float_exec_list(TWindow *parent);
