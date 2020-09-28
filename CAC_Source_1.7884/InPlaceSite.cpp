static HRESULT STDMETHODCALLTYPE InPlaceSite_QueryInterface(IOleInPlaceSite *This, REFIID riid, void **ppvObject)
{
	if (IsEqualIID(riid, IID_IOleInPlaceSite))
		*ppvObject = (IOleInPlaceSite *) &This;
	else
	{
		*ppvObject = NULL;
		return E_NOINTERFACE;
	}

	return S_OK;
}

static ULONG STDMETHODCALLTYPE InPlaceSite_AddRef(IOleInPlaceSite *)
{
	return 1;
}

static ULONG STDMETHODCALLTYPE InPlaceSite_Release(IOleInPlaceSite *)
{
	return 1;
}

static HRESULT STDMETHODCALLTYPE InPlaceSite_GetWindow(IOleInPlaceSite *This, HWND *phwnd)
{
	*phwnd = ClientSitePtr(inPlaceSite)->hwnd;
	return S_OK;
}

static HRESULT STDMETHODCALLTYPE InPlaceSite_ContextSensitiveHelp(IOleInPlaceSite *, BOOL)
{
	return E_NOTIMPL;
}

static HRESULT STDMETHODCALLTYPE InPlaceSite_CanInPlaceActivate(IOleInPlaceSite *)
{
	return S_OK;
}

static HRESULT STDMETHODCALLTYPE InPlaceSite_OnInPlaceActivate(IOleInPlaceSite *)
{
	return S_OK;
}

static HRESULT STDMETHODCALLTYPE InPlaceSite_OnUIActivate(IOleInPlaceSite *)
{
	return S_OK;
}

static HRESULT STDMETHODCALLTYPE InPlaceSite_GetWindowContext(IOleInPlaceSite *This, IOleInPlaceFrame **ppFrame,
	IOleInPlaceUIWindow **ppDoc, LPRECT lprcPosRect, LPRECT lprcClipRect, LPOLEINPLACEFRAMEINFO lpFrameInfo)
{
	*ppFrame = &ClientSitePtr(inPlaceSite)->inPlaceFrame;
	*ppDoc = NULL;
	GetClientRect(ClientSitePtr(inPlaceSite)->hwnd, lprcPosRect);
	GetClientRect(ClientSitePtr(inPlaceSite)->hwnd, lprcClipRect);

	lpFrameInfo->fMDIApp = FALSE;
	lpFrameInfo->hwndFrame = ClientSitePtr(inPlaceSite)->hwnd;
	lpFrameInfo->haccel = NULL;
	lpFrameInfo->cAccelEntries = 0;

	return S_OK;
}

static HRESULT STDMETHODCALLTYPE InPlaceSite_Scroll(IOleInPlaceSite *, SIZE)
{
	return E_NOTIMPL;
}

static HRESULT STDMETHODCALLTYPE InPlaceSite_OnUIDeactivate(IOleInPlaceSite *, BOOL)
{
	return S_OK;
}

static HRESULT STDMETHODCALLTYPE InPlaceSite_OnInPlaceDeactivate(IOleInPlaceSite *)
{
	return S_OK;
}

static HRESULT STDMETHODCALLTYPE InPlaceSite_DiscardUndoState(IOleInPlaceSite *)
{
	return E_NOTIMPL;
}

static HRESULT STDMETHODCALLTYPE InPlaceSite_DeactivateAndUndo(IOleInPlaceSite *)
{
	return E_NOTIMPL;
}

static HRESULT STDMETHODCALLTYPE InPlaceSite_OnPosRectChange(IOleInPlaceSite *, LPCRECT)
{
	return S_OK;
}

IOleInPlaceSiteVtbl InPlaceSiteVtbl =
{
	InPlaceSite_QueryInterface,
	InPlaceSite_AddRef,
	InPlaceSite_Release,
	InPlaceSite_GetWindow,
	InPlaceSite_ContextSensitiveHelp,
	InPlaceSite_CanInPlaceActivate,
	InPlaceSite_OnInPlaceActivate,
	InPlaceSite_OnUIActivate,
	InPlaceSite_GetWindowContext,
	InPlaceSite_Scroll,
	InPlaceSite_OnUIDeactivate,
	InPlaceSite_OnInPlaceDeactivate,
	InPlaceSite_DiscardUndoState,
	InPlaceSite_DeactivateAndUndo,
	InPlaceSite_OnPosRectChange
};
