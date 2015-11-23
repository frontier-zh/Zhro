#ifndef PUBLIC_DEFINE_H
#define PUBLIC_DEFINE_H

extern const char* FONT_NAME;

#define SAFE_DELETE(a) if (a) { delete (a); (a) = NULL;}

#endif // PUBLIC_DEFINE_H
