#include "cac.h"

#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL

#include "EditArticleDialog.h"
#include "EditArticleDialog.cpp"

static bool edit_article(TWindow *parent, TArticle *article, bool edit)
{
	return TEditArticleDialog(parent, article, edit).Execute() == IDOK;
}

// 2007:212 copied from subject_corrupt()
static void subject_article(TWindow *parent, TArticle *article)
{
	TGArray<TArticleSubject> articleSubjects;
	TGArray<TSubject> subjects, tSubjects;
	TSubject *tSubject;
	msql m;

	m.Begin(ArticleSubject->Name);
	m.AddLong("F_ARTICLE", article->article);
	ArticleSubject->LoadArray(articleSubjects, str(m));

	// 2007:149 this isn't a FloatCriteria(), so add order
	m.Begin(Subject->Name);
	m.AddChars("F_KIND_I", KIND_ARTICLE_LAWS);
	TQuery q(str(m));
	while (q.Read())
	{
		*Subject << q;

		for (int i = 0; i < articleSubjects.Count(); i++)
		{
			if (articleSubjects[i]->subject == Subject->subject)
			{
				tSubject = new TSubject;
				*tSubject << Subject;
				subjects.Add(tSubject);

				tSubject = new TSubject;
				*tSubject << Subject;
				tSubjects.Add(tSubject);
				break;
			}
		}
	}

	if (select_subjects(parent, &tSubjects, KIND_ARTICLE_LAWS, false, true))
	{
		int i = 0;

		while (i < subjects.Count())
		{
			int j;

			for (j = 0; j < tSubjects.Count(); j++)
				if (subjects[i]->subject == tSubjects[j]->subject)
					break;

			if (j < tSubjects.Count())
			{
				subjects.Remove(i);
				tSubjects.Remove(j);
			}
			else
				i++;
		}

		ArticleSubject->article = article->article;

		for (int i = 0; i < subjects.Count(); i++)
		{
			ArticleSubject->subject = subjects[i]->subject;
			ArticleSubject->Delete(true);
		}

		for (int j = 0; j < tSubjects.Count(); j++)
		{
			ArticleSubject->subject = tSubjects[j]->subject;
			ArticleSubject->Insert();
		}
	}
}

#include "ArticleListWindow.h"
#include "ArticleListWindow.cpp"

#include "ArticleListDialog.h"
#include "ArticleListDialog.cpp"

void edit_articles(TWindow *parent)
{
	log(LOG_NOM, CM_ADMIN_ARTICLES, "основания - представители");
	TArticleListDialog(parent).Execute();
	Article->ReloadCache();
	ArticleSubject->ReloadCache();
}
#endif  // AREAL || MILITARY || REGIONAL
