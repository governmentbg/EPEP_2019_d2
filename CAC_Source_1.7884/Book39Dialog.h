//{{TDialog = TBook39Dialog}}
class TBook39Dialog : public TFloatDialog
{
public:
	TBook39Dialog(TWindow* parent, TBook39Group *group, int resId = IDD_BOOK_39);

protected:
	TCharListFace *types;
	TLongFace *minNo;
	TLongFace *maxNo;
	TYearFace *minYear;
	TYearFace *maxYear;
	TUserListBox *judges;
	TCharListFace *compositions;
	TSenderListBox *receivers;
	TSLCReasonListBox *slcReasons;
	TDateFace *minOutRegDate;
	TDateFace *maxOutRegDate;
	TCheckFace *eventText;

	virtual bool IsValid();
	virtual void Action(bool);

//{{TBook39DialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TBook39DialogVIRTUAL_END}}
};    //{{TBook39Dialog}}
