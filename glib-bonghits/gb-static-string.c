#include "config.h"

#include "gb-static-string.h"
#include "gb-ref-ptr.h"

#include <string.h>

G_LOCK_DEFINE_STATIC (interned_strings);
static GHashTable *interned_strings = NULL;

static void
release_interned_string (gpointer data)
{
  G_LOCK (interned_strings);

  g_hash_table_remove (interned_strings, data);

  G_UNLOCK (interned_strings);
}

char *
gb_static_string_new (const char *str)
{
  gsize len;
  char *res;

  g_return_val_if_fail (str != NULL || *str != '\0', NULL);

  G_LOCK (interned_strings);

  if (interned_strings == NULL)
    interned_strings = g_hash_table_new (g_str_hash, g_str_equal);

  len = strlen (str);

  res = g_hash_table_lookup (interned_strings, str);
  if (res == NULL)
    {
      res = gb_ref_ptr_alloc (len + 1, release_interned_string);
      memcpy (res, str, len);
      res[len] = '\0';

      g_hash_table_add (interned_strings, res);
    }
  else
    gb_static_string_ref (res);

  G_UNLOCK (interned_strings);

  return res;
}

void
gb_static_string_ref (char *str)
{
  gb_ref_ptr_acquire (str);
}

void
gb_static_string_unref (char *str)
{
  gb_ref_ptr_release (str);
}
