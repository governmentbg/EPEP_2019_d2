class TDateFaceUpDown : public TUpDown
{  // 14.aug.2002; 2010:069 LPR: facelift
  public:
    TDateFaceUpDown(TWindow* parent, int resourceId, TDateFace *tDateFace);
    virtual void SetupWindow();
  protected:  // Methods
    void EvLButtonDown(uint modKeys, TPoint& point);
  protected:  // Data
    TDateFace *dateFace;
    int oldPos;
DECLARE_RESPONSE_TABLE(TDateFaceUpDown);
};  // class DateFaceUpDown