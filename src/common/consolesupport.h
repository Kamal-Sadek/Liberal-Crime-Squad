void set_color(short f, short b, bool bright, bool blink = false);
void translategetch(int &c);
void translategetch_cap(int &c);
/* Refreshes the screen, empties the keyboard buffer, waits for a new key to be pressed, and returns the key pressed */
int getkey();
/* Variant of getkey() that doesn't make all letters lowercase */
int getkey_cap();
/* Empties the keyboard buffer, and returns most recent key pressed, if any */
int checkkey();
/* Variant of checkkey() that doesn't make all letters lowercase */
int checkkey_cap();
#ifdef CH_USE_UNICODE
bool setup_unicode();
#endif
void set_title(char *c);
void init_console();
#ifdef WIN32
void begin_cleartype_fix();
void end_cleartype_fix();
#endif