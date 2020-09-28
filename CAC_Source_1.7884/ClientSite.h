struct IOleClientSiteImpl
{
	IOleClientSiteVtbl *lpVtbl;
	IOleInPlaceSite inPlaceSite;
	IOleInPlaceFrame inPlaceFrame;
	IDispatch dispatch;
	IStorage storage;
	HWND hwnd;
	IWebBrowser2 *iBrowser;
	IOleInPlaceActiveObject *pActiveObject;
	THtmlBaseDialog *pHtmlDialog;
};

extern IOleClientSiteVtbl ClientSiteVtbl;
extern IDispatchVtbl DispatchVtbl;
extern IOleInPlaceFrameVtbl InPlaceFrameVtbl;
extern IOleInPlaceSiteVtbl InPlaceSiteVtbl;
extern IStorageVtbl StorageVtbl;

#define ClientSitePtr(field) ((IOleClientSiteImpl *) (((char *) (This)) - offsetof(IOleClientSiteImpl, field)))
