//{{TDoubleListHeader = TWindow}}
class TDoubleListHeader : public TWindow
{
public:
	TDoubleListHeader(THandle handle);

protected:
	HCURSOR cursor;

//{{TDoubleListHeaderRSP_TBL_BEGIN}}
	bool EvSetCursor(THandle, uint, uint);
//{{TDoubleListHeaderRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TDoubleListHeader);
};    //{{TDoubleListHeader}}
