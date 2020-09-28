//{{TComboBox = TReceiverAliasFace}}
class TReceiverAliasFace : public TStringAliasFace
{
public:
	TReceiverAliasFace(TWindow* parent, int resourceId, const char *name, TReceiver *receiver, const char *xCrit,
		const char *tWhat = NULL);
	virtual ~TReceiverAliasFace();

	const char *SNow();
	char UCNType();
	void SetUCN(const char *ucn);

protected:
	const char *what;
	
	virtual bool IsValid();

	virtual void Criteria(msql &m, TGroup *group);
	virtual int GetVisualIndex();

//{{TReceiverAliasFaceRSP_TBL_BEGIN}}
	void CBNDropDown();
//{{TReceiverAliasFaceRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TReceiverAliasFace);
};    //{{TReceiverAliasFace}}
