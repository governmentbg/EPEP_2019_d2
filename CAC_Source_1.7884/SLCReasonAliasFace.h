//{{TComboBox = TSLCReasonAliasFace}}
class TSLCReasonAliasFace : public TLongAliasFace
{
public:
	TSLCReasonAliasFace(TWindow* parent, int resourceId, const char *name, const char tLawKind, long flags = FLAG_NULL);

protected:
	char lawKind;

	virtual TGroup *NewGroup() const;
	virtual void Criteria(msql &m, TGroup *group);
	virtual void GetGroupText(mstr &m, TGroup *group, int);

//{{TSLCReasonAliasFaceVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TSLCReasonAliasFaceVIRTUAL_END}}
};    //{{TSLCReasonAliasFace}}
