#include "cac.h"

#include <initguid.h>
#include "ocidl.h"
#include "docobj.h"
#include "servprov.h"

#include "exdisp.h"
#include "exdispid.h"
#include "mshtml.h"
#include "WebBrowser.h"
#include "ClientSite.h"

#include "ClientSite.cpp"
#include "Dispatch.cpp"
#include "InPlaceSite.cpp"
#include "InPlaceFrame.cpp"
#include "Storage.cpp"
#include "mshtml.cpp"
#include "WebBrowser.cpp"

DEFINE_GUID(DIID_DWebBrowserEvents2,	0x34A715A0, 0x6587, 0x11D0, 0x92, 0x4A, 0x00, 0x20, 0xAF, 0xC7, 0xAC, 0x4D);
DEFINE_GUID(CLSID_WebBrowser,			0x8856F961, 0x340A, 0x11D0, 0xA9, 0x6B, 0x00, 0xC0, 0x4F, 0xD7, 0x05, 0xA2);
DEFINE_GUID(IID_IWebBrowser2,			0xD30C1661, 0xCDAF, 0x11D0, 0x8A, 0x3E, 0x00, 0xC0, 0x4F, 0xC9, 0xE2, 0x6E);
DEFINE_GUID(IID_IShellBrowser,		0x000214E2, 0x0000, 0x0000, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46);
DEFINE_GUID(IID_IServiceProvider,		0x6D5140C1, 0x7436, 0x11CE, 0x80, 0x34, 0x00, 0xAA, 0x00, 0x60, 0x09, 0xFA);
DEFINE_GUID(IID_IHTMLDocument2,		0x626FC520, 0xA41E, 0x11CF, 0xA7, 0x31, 0x00, 0xA0, 0xC9, 0x08, 0x26, 0x37);

static constant HTML_BLANK_PAGE = "about:blank";

#include "HtmlBaseDialog.cpp"

#include "ShowHtmlDialog.h"
#include "ShowHtmlDialog.cpp"

bool show_html(TWindow *parent, const char *name, bool remove)
{
#if RANGECHECK
	if (!html_name(name))
		error("non-html_name = %s", name);
#endif  // RANGECHECK
	return show_dlg(new TShowHtmlDialog(parent, name, remove), parent);
}

bool show_html(TWindow *parent, char *text)
{
	return show_dlg(new TShowHtmlDialog(parent, text), parent);
}
