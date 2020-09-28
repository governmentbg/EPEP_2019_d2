//{{TListWindow = TPaymentListWindow}}
class TPaymentListWindow : public TDoubleListWindow
{
public:
	TPaymentListWindow(TWindow* parent, int resourceId, TPayment *tSearch);
	virtual ~TPaymentListWindow() { delete array; }

	virtual void Refresh();

	TPayment *Payment() { return (TPayment *) Group(); }
	TPayment *PaymentAt(int index) { return (TPayment *) GroupAt(index); }

	void FindText(const char *text, int step) { QuickSearch(step, text); }
	void AddRepeat();
	void Edit();
	void Delete();
	void PayPending();

protected:
	TPayment *search;

	void LoadData();
	virtual void DrawSecond(TListWindItem &lwi, TGroup *group);

	virtual TGroup *NewGroup() const;
	virtual bool EditGroup(TGroup *group);

//{{TPaymentListWindowVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TPaymentListWindowVIRTUAL_END}}
};    //{{TPaymentListWindow}}
