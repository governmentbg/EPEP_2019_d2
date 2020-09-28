static HRESULT STDMETHODCALLTYPE ClientSite_QueryInterface(IOleClientSite *This, REFIID riid, void **ppvObject)
{
	if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_IOleClientSite))
		*ppvObject = (IOleClientSite *) This;
	else if (IsEqualIID(riid, IID_IOleInPlaceSite))
		*ppvObject = (void *) &(((IOleClientSiteImpl *) This)->inPlaceSite);
	else if (IsEqualIID(riid, IID_IDispatch))
		*ppvObject = (void *) &((IOleClientSiteImpl *) This)->dispatch;
	else if (IsEqualIID(riid, IID_IOleInPlaceFrame))
		*ppvObject = (void *) &((IOleClientSiteImpl *) This)->inPlaceFrame;
	else
	{
		*ppvObject = NULL;
		return E_NOINTERFACE;
	}

	return S_OK;
}

static ULONG STDMETHODCALLTYPE ClientSite_AddRef(IOleClientSite *)
{
	return 1;
}

static ULONG STDMETHODCALLTYPE ClientSite_Release(IOleClientSite *)
{
	return 1;
}

static HRESULT STDMETHODCALLTYPE ClientSite_SaveObject(IOleClientSite *)
{
	return E_NOTIMPL;
}

static HRESULT STDMETHODCALLTYPE ClientSite_GetMoniker(IOleClientSite *, DWORD, DWORD, IMoniker **ppmk)
{
	*ppmk = NULL;
	return E_NOTIMPL;
}

static HRESULT STDMETHODCALLTYPE ClientSite_GetContainer(IOleClientSite *, IOleContainer **ppContainer)
{
	*ppContainer = NULL;
	return E_NOTIMPL;
}

static HRESULT STDMETHODCALLTYPE ClientSite_ShowObject(IOleClientSite *)
{
	return S_OK;
}

static HRESULT STDMETHODCALLTYPE ClientSite_OnShowWindow(IOleClientSite *, BOOL)
{
	return S_OK;
}

static HRESULT STDMETHODCALLTYPE ClientSite_RequestNewObjectLayout(IOleClientSite *)
{
	return E_NOTIMPL;
}

IOleClientSiteVtbl ClientSiteVtbl =
{
	ClientSite_QueryInterface,
	ClientSite_AddRef,
	ClientSite_Release,
	ClientSite_SaveObject,
	ClientSite_GetMoniker,
	ClientSite_GetContainer,
	ClientSite_ShowObject,
	ClientSite_OnShowWindow,
	ClientSite_RequestNewObjectLayout
};
