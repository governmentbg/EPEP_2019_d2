//{{TArticleAliasFace Implementation}}

TArticleAliasFace::TArticleAliasFace(TWindow* parent, int resourceId, const char *name, long tSubject, long flags)
:
	TLongAliasFace(parent, resourceId, name, ArticleSubject, NULL, flags), subject(tSubject)
{
	ArticleSubject->subject = subject;
}

TGroup *TArticleAliasFace::NewGroup() const
{
	TArticleSubject *articleSubject = new TArticleSubject;
	articleSubject->subject = subject;
	return articleSubject;
}
