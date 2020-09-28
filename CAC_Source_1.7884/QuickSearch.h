enum { SIZE_OF_QUICK_SEARCH_TEXT = 21 };

enum
{
	QUICK_SEARCH_PREV		= 0,
	QUICK_SEARCH_EDIT		= 1,
	QUICK_SEARCH_NEXT		= 2,
	QUICK_SEARCH_CONTROLS	= 3
};

enum
{
	WM_QUICK_SEARCH	= 0x125A,
	MW_SEARCH_QUICK	= 0x2A51
};

const char *qwfindi(const char *text, const char *patt, bool start);

// ----- TQuickSearch --------------------------------------------------------
struct TQuickSearch
{
	TQuickSearchEdit *controls[QUICK_SEARCH_CONTROLS];
	char text[SIZE_OF_QUICK_SEARCH_TEXT];
	TQuickSearchEdit *edit;	// == controls[1]

	void Enable();
	void ReShow() { Show(SW_SHOW); }
	void Disable() { Show(SW_HIDE); }
	bool Focused();
	void SetText();
	void BackChar() { text[strlen(text) - 1] = '\0'; }

protected:
	void Show(int nCmdShow);
};

typedef enum
{
	QUICK_SEARCH_NONE,
	QUICK_SEARCH_ABLE,
	QUICK_SEARCH_AUTO
} QUICK_SEARCH_MODE;
