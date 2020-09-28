
class TfzlNkzDialog : public TEisDialog
{
public:
	TfzlNkzDialog(TWindow *parent, FZL *fzl, bool edit, int resId = IDD_FZL_NKZ);
        
        TStatic *nprnmrLabel;
        TStringFace *nprnmr;
        TStatic *fzlFaceLabel;
        TStatic *fzlegnLabel;
        TStringFace *fzlegn;
        TStatic *fzllncLabel;
        TStringFace *fzllnc;
        TStatic *fzlimecyrLabel;
        TStringFace *fzlimecyr;
        TStatic *fzlsLabel;
        TGroupListWindow *fzls;
        TButton *addFzl, *editFzl, *delFzl;
        TStatic *nkzsLabel;
        TGroupListWindow *nkzs;
        TButton *addNkz, *editNkz, *delNkz;
};