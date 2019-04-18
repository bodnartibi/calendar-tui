#ifndef TEXT_H
#define TEXT_H

typedef struct _Text Text;

struct _Text
{
  int color;
  wchar_t *str;
  struct _Text *next;
};

#endif
