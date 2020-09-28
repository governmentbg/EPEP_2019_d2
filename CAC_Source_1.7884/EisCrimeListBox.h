//{{TListBox = TEisCrimeListBox}}
class TEisCrimeListBox : public TFindAliasListBox
{
public:
	TEisCrimeListBox(TWindow* parent, int resourceId, long tCode);

	TEisCrime *Crime() { return (TEisCrime *) Group(); }
	TEisCrime *CrimeAt(int index) const { return (TEisCrime *) GroupAt(index); }

protected:
	long code;

//{{TEisCrimeDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TEisCrimeDialogVIRTUAL_END}}
};    //{{TEisCrimeListBox}}
