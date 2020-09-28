static HRESULT STDMETHODCALLTYPE InPlaceFrame_QueryInterface(IOleInPlaceFrame *This, REFIID riid, void **ppvObject)
{
	if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_IOleInPlaceFrame))
		*ppvObject = (IOleInPlaceFrame *) This;
	else
	{
		*ppvObject = NULL;
		return E_NOINTERFACE;
	}

	return S_OK;
}

static ULONG STDMETHODCALLTYPE InPlaceFrame_AddRef(IOleInPlaceFrame *)
{
	return 1;
}

static ULONG STDMETHODCALLTYPE InPlaceFrame_Release(IOleInPlaceFrame *)
{
	return 1;
}

static HRESULT STDMETHODCALLTYPE InPlaceFrame_GetWindow(IOleInPlaceFrame *This, HWND *phwnd)
{
	*phwnd = ClientSitePtr(inPlaceFrame)->hwnd;
	return S_OK;
}

static HRESULT STDMETHODCALLTYPE InPlaceFrame_ContextSensitiveHelp(IOleInPlaceFrame *, BOOL)
{
	return E_NOTIMPL;
}

static HRESULT STDMETHODCALLTYPE InPlaceFrame_GetBorder(IOleInPlaceFrame *, LPRECT)
{
	return INPLACE_E_NOTOOLSPACE;
}

static HRESULT STDMETHODCALLTYPE InPlaceFrame_RequestBorderSpace(IOleInPlaceFrame *, LPCBORDERWIDTHS)
{
	return INPLACE_E_NOTOOLSPACE;
}

static HRESULT STDMETHODCALLTYPE InPlaceFrame_SetBorderSpace(IOleInPlaceFrame *, LPCBORDERWIDTHS)
{
	return OLE_E_INVALIDRECT;
}

static HRESULT STDMETHODCALLTYPE InPlaceFrame_SetActiveObject(IOleInPlaceFrame *This, IOleInPlaceActiveObject *pActiveObject,
	LPCOLESTR)
{
	ClientSitePtr(inPlaceFrame)->pActiveObject = pActiveObject;
	return S_OK;
}

static HRESULT STDMETHODCALLTYPE InPlaceFrame_InsertMenus(IOleInPlaceFrame *, HMENU, LPOLEMENUGROUPWIDTHS)
{
	return E_NOTIMPL;
}

static HRESULT STDMETHODCALLTYPE InPlaceFrame_DoSetMenu(IOleInPlaceFrame *This, HMENU hmenuShared, HOLEMENU, HWND)
{
	if (hmenuShared)
		SetMenu(ClientSitePtr(inPlaceFrame)->hwnd, hmenuShared);

	return S_OK;
}

static HRESULT STDMETHODCALLTYPE InPlaceFrame_RemoveMenus(IOleInPlaceFrame *, HMENU)
{
	return S_OK;
}

static HRESULT STDMETHODCALLTYPE InPlaceFrame_SetStatusText(IOleInPlaceFrame *, LPCOLESTR)
{
	return S_OK;
}

static HRESULT STDMETHODCALLTYPE InPlaceFrame_EnableModeless(IOleInPlaceFrame *, BOOL)
{
	return S_OK;
}

static HRESULT STDMETHODCALLTYPE InPlaceFrame_TranslateAccelerator(IOleInPlaceFrame *, LPMSG, WORD)
{
	return S_OK;
}

IOleInPlaceFrameVtbl InPlaceFrameVtbl =
{
	InPlaceFrame_QueryInterface,
	InPlaceFrame_AddRef,
	InPlaceFrame_Release,
	InPlaceFrame_GetWindow,
	InPlaceFrame_ContextSensitiveHelp,
	InPlaceFrame_GetBorder,
	InPlaceFrame_RequestBorderSpace,
	InPlaceFrame_SetBorderSpace,
	InPlaceFrame_SetActiveObject,
	InPlaceFrame_InsertMenus,
	InPlaceFrame_DoSetMenu,
	InPlaceFrame_RemoveMenus,
	InPlaceFrame_SetStatusText,
	InPlaceFrame_EnableModeless,
	InPlaceFrame_TranslateAccelerator
};
