#ifndef __GB_STATIC_STRING_H__
#define __GB_STATIC_STRING_H__

#include <glib.h>

G_BEGIN_DECLS

char *          gb_static_string_new    (const char *str);
void            gb_static_string_ref    (char *str);
void            gb_static_string_unref  (char *str);

G_END_DECLS

#endif /* __GB_STATIC_STRING_H__ */
