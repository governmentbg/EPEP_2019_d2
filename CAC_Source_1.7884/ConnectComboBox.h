//{{TComboBox = TConnectComboBox}}
class TConnectComboBox : public TGroupComboBox
{
public:
	TConnectComboBox(TWindow* parent, int resourceId, SBE *sbe, TRCDKeyContainer *origin);

	TConnect *Connect() { return (TConnect *) Group(); }

	const char *Label();
	long Eispp();

protected:
	char connectType;

	virtual TGroup *NewGroup() const;
	virtual void Refresh();

//{{ConnectComboBoxRSP_TBL_BEGIN}}
	long EvFaceValid(TParam1, TParam2);
//{{ConnectComboBoxRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TConnectComboBox);
};    //{{TConnectComboBox}}
