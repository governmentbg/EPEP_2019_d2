// General note: on wrong IIDs, OLE may happily return S_OK,
// leaving a NULL object. Thus, we check objects, nor hr.

void IWebBrowser2_Resize(IWebBrowser2 *iBrowser, RECT *rect)
{
	IOleInPlaceObject *pInPlaceObject = NULL;

	IWebBrowser2_QueryInterface(iBrowser, IID_IOleInPlaceObject, (void **) &pInPlaceObject);

	if (pInPlaceObject)
	{
		IOleInPlaceObject_SetObjectRects(pInPlaceObject, rect, rect);
		IOleInPlaceObject_Release(pInPlaceObject);
	}
}

void IWebBrowser2_NavigateURL(IWebBrowser2 *iBrowser, BSTR url)
{
	VARIANT vURL;
	VARIANT ve1, ve2, ve3, ve4;

	vURL.vt = VT_BSTR;
	vURL.bstrVal = url;
	ve1.vt = VT_EMPTY;
	ve2.vt = VT_EMPTY;
	ve3.vt = VT_EMPTY;
	ve4.vt = VT_EMPTY;

	IWebBrowser2_Navigate2(iBrowser, &vURL, &ve1, &ve2, &ve3, &ve4);
	VariantClear(&vURL);
}

void IWebBrowser2_Print(IWebBrowser2 *iBrowser, BOOL direct)
{
	IWebBrowser2_ExecWB(iBrowser, OLECMDID_PRINT, direct ? OLECMDEXECOPT_DONTPROMPTUSER : OLECMDEXECOPT_PROMPTUSER, NULL,
		NULL);
}

void IWebBrowser2_PrintPreview(IWebBrowser2 *iBrowser)
{
	IWebBrowser2_ExecWB(iBrowser, OLECMDID_PRINTPREVIEW, OLECMDEXECOPT_DODEFAULT, NULL, NULL);
}

void IWebBrowser2_PageSetup(IWebBrowser2 *iBrowser)
{
	IWebBrowser2_ExecWB(iBrowser, OLECMDID_PAGESETUP, OLECMDEXECOPT_DODEFAULT, NULL, NULL);
}

void IWebBrowser2_SaveAs(IWebBrowser2 *iBrowser, BSTR url)
{
	VARIANT vURL;

	vURL.vt = VT_BSTR;
	vURL.bstrVal = url;

	IWebBrowser2_ExecWB(iBrowser, OLECMDID_SAVEAS, OLECMDEXECOPT_DONTPROMPTUSER, &vURL, NULL);
	VariantClear(&vURL);
}

void IWebBrowser2_SetFontSize(IWebBrowser2 *iBrowser, long lVal)
{
	VARIANT vSize;

	vSize.vt = VT_I4;
	vSize.lVal = lVal;
	IWebBrowser2_ExecWB(iBrowser, OLECMDID_ZOOM, OLECMDEXECOPT_DONTPROMPTUSER, &vSize, NULL);
}

EXTERN_C const IID IID_IShellBrowser;

HWND IWebBrowser2_GetHWnd(IWebBrowser2 *iBrowser)
{
	IServiceProvider *pServiceProvider = NULL;
	HWND hwndBrowser = NULL;

	IWebBrowser2_QueryInterface(iBrowser, IID_IServiceProvider, (void **) &pServiceProvider);

	if (pServiceProvider)
	{
		IOleWindow *pWindow = NULL;

		IServiceProvider_QueryService(pServiceProvider, IID_IShellBrowser, IID_IOleWindow, (void **) &pWindow);

		if (pWindow)
		{
			IOleWindow_GetWindow(pWindow, &hwndBrowser);
			IOleWindow_Release(pWindow);
		}

		IServiceProvider_Release(pServiceProvider);
	}

	return hwndBrowser;
}

void IWebBrowser2_Activate(IWebBrowser2 *iBrowser)
{
	IOleObject *lpOleObject = NULL;

	IWebBrowser2_QueryInterface(iBrowser, IID_IOleObject, (void **) &lpOleObject);

	if (lpOleObject)
	{
		IOleObject_DoVerb(lpOleObject, OLEIVERB_UIACTIVATE, NULL, NULL, 0, NULL, NULL);
		IOleObject_Release(lpOleObject);
	}
}

IWebBrowser2 *CreateEmbeddedWebControl(THtmlBaseDialog *pHtmlDialog)
{
	HWND hwnd = pHtmlDialog->Handle;
	IOleObject *pWebObject = NULL;
	RECT rect;
	IWebBrowser2 *iBrowser;
	IOleClientSiteImpl *lpClientSite;

	// Allocate and init client site
	lpClientSite = new IOleClientSiteImpl;
	lpClientSite->lpVtbl = &ClientSiteVtbl;
	lpClientSite->inPlaceSite.lpVtbl = &InPlaceSiteVtbl;
	lpClientSite->inPlaceFrame.lpVtbl = &InPlaceFrameVtbl;
	lpClientSite->dispatch.lpVtbl = &DispatchVtbl;
	lpClientSite->storage.lpVtbl = &StorageVtbl;
	lpClientSite->hwnd = hwnd;
	lpClientSite->iBrowser = NULL;
	lpClientSite->pActiveObject = NULL;
	lpClientSite->pHtmlDialog = pHtmlDialog;

	OleCreate(CLSID_WebBrowser, IID_IOleObject, OLERENDER_DRAW, 0, (IOleClientSite *) lpClientSite,
		&lpClientSite->storage, (void **) &pWebObject);

	if (!pWebObject)
	{
		delete lpClientSite;
		return NULL;
	}

	GetClientRect(hwnd, &rect);
	IOleObject_SetHostNames(pWebObject, L"Web Host", L"Web View");
	OleSetContainedObject((IUnknown *) pWebObject, TRUE);
	IOleObject_DoVerb(pWebObject, OLEIVERB_SHOW, NULL, (IOleClientSite *) lpClientSite, -1, hwnd, &rect);
	IOleObject_QueryInterface(pWebObject, IID_IWebBrowser2, (void **) &iBrowser);

	if (iBrowser)
	{
		IConnectionPointContainer *pIConnectionContainer = NULL;

		lpClientSite->iBrowser = iBrowser;
		IWebBrowser2_QueryInterface(iBrowser, IID_IConnectionPointContainer, (void **) &pIConnectionContainer);

		if (pIConnectionContainer)
		{
			IConnectionPoint *pIConnectionPoint = NULL;

			IConnectionPointContainer_FindConnectionPoint(pIConnectionContainer, DIID_DWebBrowserEvents2,
				&pIConnectionPoint);

			if (pIConnectionPoint)
			{
				DWORD dwCookie;

				IConnectionPoint_Advise(pIConnectionPoint, (IUnknown *) &lpClientSite->dispatch, &dwCookie);
				IConnectionPoint_Release(pIConnectionPoint);
			}
		}
	}
	else
		delete lpClientSite;

	return iBrowser;
}

void CloseEmbeddedWebControl(IWebBrowser2 *iBrowser)
{
	IOleObject *lpOleObject = NULL;
	IOleClientSiteImpl *lpClientSite = NULL;

	IWebBrowser2_QueryInterface(iBrowser, IID_IOleObject, (void **) &lpOleObject);

	if (lpOleObject)
	{
		IOleObject_GetClientSite(lpOleObject, (IOleClientSite **) &lpClientSite);
		IOleObject_Release(lpOleObject);
	}

	IWebBrowser2_Quit(iBrowser);
	IWebBrowser2_Release(iBrowser);
	delete lpClientSite;
}

BOOL WriteEmbeddedWebControl(IWebBrowser2 *iBrowser, BSTR bstr)
{
	IHTMLDocument *iHTMLDocument = NULL;
	IHTMLDocument2 *iHTMLDocument2 = NULL;

	IWebBrowser2_get_Document(iBrowser, (IDispatch **) &iHTMLDocument);

	if (iHTMLDocument)
	{

		IWebBrowser2_QueryInterface(iHTMLDocument, IID_IHTMLDocument2, (void **) &iHTMLDocument2);

		if (iHTMLDocument2)
		{
			IHTMLDocument2_Write(iHTMLDocument2, bstr);
			IHTMLDocument2_close(iHTMLDocument2);
			IHTMLDocument2_Release(iHTMLDocument2);
		}

		IHTMLDocument_Release(iHTMLDocument);
	}

	return iHTMLDocument2 != NULL;
}
