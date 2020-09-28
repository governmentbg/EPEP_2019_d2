//{{TListWindow = TTryAddressSideListWindow}}
class TTryAddressSideListWindow : public TDoubleListWindow
{
public:
	TTryAddressSideListWindow(TWindow* parent, int resourceId, TGroupArray *array, int width);

	void Address();
	bool IsValid();

protected:
//{{TTryAddressSideListWindowVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TTryAddressSideListWindowVIRTUAL_END}}
};    //{{TTryAddressSideListWindow}}
