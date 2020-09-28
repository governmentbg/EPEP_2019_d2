//{{TDialog = TEditArticleDialog}}
class TEditArticleDialog : public TEditDialog
{
public:
	TEditArticleDialog(TWindow* parent, TArticle *tArticle, bool edit, int resId = IDD_EDIT_ARTICLE);

protected:
	TLongFace *article;

//{{TEditArticleDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TEditArticleDialogVIRTUAL_END}}
};    //{{TEditArticleDialog}}
