inline long get_layout() { return (long) GetKeyboardLayout(0); }
void set_layout_ll(long layout, bool reorder);
void set_layout(long layout);
bool get_login(TWindow *parent);
