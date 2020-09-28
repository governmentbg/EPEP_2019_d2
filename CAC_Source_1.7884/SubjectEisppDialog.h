//{{TDialog = TSubjectEisppDialog}}
class TSubjectEisppDialog : public TGroupDialog
{
public:
	TSubjectEisppDialog(TWindow* parent, TSubject *subject);

protected:
	TLongFace *vid;

#if TESTVER
	DEFINE_GROUP_TYPE(TSubject, Subject)
	virtual bool IsValid();
#endif  // TESTVER
};    //{{TSubjectEisppDialog}}
