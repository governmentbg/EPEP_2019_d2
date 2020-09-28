class TNTOpenSaveDialog : public TOpenSaveDialog
{
public:
	TNTOpenSaveDialog(TWindow* parent, TData& data, bool tOpen, int templateId = 0, const char far* title = 0,
		TModule* module = 0);

protected:
	bool open;

	virtual int DoExecute();
};
