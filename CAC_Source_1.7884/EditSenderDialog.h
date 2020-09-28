//{{TDialog = TEditSenderDialog}}
class TEditSenderDialog : public TEditNamedDialog
{
public:
	TEditSenderDialog(TWindow* parent, TSender *sender, bool edit, bool full, int resId);

	DEFINE_GROUP_TYPE(TSender, Sender)	
};    //{{TEditSenderDialog}}
