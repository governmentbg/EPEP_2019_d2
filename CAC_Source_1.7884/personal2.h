typedef struct TTextSide TTextSide;
typedef struct TTextPart TTextPart;
typedef struct TTextPair TTextPair;

class TPersonal
{
public:
	TPersonal(const TRCDKeyContainer *container, TStringArray *extraUcns = NULL);
	~TPersonal();

	void Filter(char *text, int strenght, IDispatch *iActive);
	void Filter(mstr &text, int strenght) { Filter(ncstr(text), strenght, NULL); }
	void Filter(wchar_t *text, int strenght, IDispatch *iActive);

	TReplacePairs repairs;	// 2014:330 public for filter

private:
	static TTextPart *GetParts(void *data, int index);

	TTextSide *sides;
	TTextPair *pairs;
};

// TPersonal should be used to speedup multiply calls for the same origin
void filter_personal(const TRCDKeyContainer *container, char *text, int strenght, IDispatch *iActive = NULL);
void filter_personal(const TRCDKeyContainer *container, mstr &text, int strenght);
