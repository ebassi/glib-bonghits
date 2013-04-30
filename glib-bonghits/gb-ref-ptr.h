#ifndef __GB_REF_POINTER_H__
#define __GB_REF_POINTER_H__

#if !defined(__GLIB_BONGHITS_H_INSIDE__) && !defined(GLIB_BONGHITS_COMPILATION)
#error "Only <glib-bonghits/glib-bonghits.h> can be included directly."
#endif

#include <glib.h>

G_BEGIN_DECLS

gpointer        gb_ref_ptr_alloc                (gsize           alloc_size);
gpointer        gb_ref_ptr_alloc_with_notify    (gsize           alloc_size,
                                                 GDestroyNotify  notify);
gpointer        gb_ref_ptr_dup                  (gconstpointer   data,
                                                 gsize           alloc_size);
gpointer        gb_ref_ptr_acquire              (gpointer        ref_pointer);
void            gb_ref_ptr_release              (gpointer        ref_pointer);

/* private */
G_GNUC_INTERNAL
void            gb_ref_ptr_free                 (gpointer        ref_pointer,
                                                 gboolean        run_notify);

G_END_DECLS

#endif /* __GB_REF_POINTER_H__ */
