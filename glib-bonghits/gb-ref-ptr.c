#include "config.h"

#include "gb-ref-ptr.h"

#include "valgrind.h"

#include <string.h>

typedef struct _GbRefPtr        GbRefPtr;

#define STRUCT_ALIGNMENT        (2 * sizeof (gsize))
#define ALIGN_STRUCT(offset)    ((offset + (STRUCT_ALIGNMENT - 1)) & -STRUCT_ALIGNMENT)

struct _GbRefPtr
{
  volatile int ref_count;

  gsize alloc_size;

  GDestroyNotify notify;
};

/**
 * gb_ref_ptr_alloc:
 * @alloc_size: the size of the memory allocation
 *
 * Allocates (and clears) a reference counted region of memory, and returns
 * a pointer to it.
 *
 * References can be acquired using gb_ref_ptr_acquire(), and released using
 * gb_ref_ptr_release().
 *
 * Once the last reference has been released, the memory region is
 * deallocated. If the memory region contains allocated memory, you should
 * use gb_ref_ptr_alloc_with_notify() and provide a function to be called
 * when the memory region is deallocated.
 *
 * Return value: (transfer full): the newly allocated, reference counted,
 *   memory region.
 */
gpointer
gb_ref_ptr_alloc (gsize alloc_size)
{
  return gb_ref_ptr_alloc_with_notify (alloc_size, NULL);
}

/**
 * gb_ref_ptr_dup:
 * @data: data to copy
 * @alloc_size: size of @data
 *
 * Allocates a reference counted region of memory, copies @alloc_size bytes of
 * the passed @data, and returns a pointer to it.
 *
 * References can be acquired using gb_ref_ptr_acquire(), and released using
 * gb_ref_ptr_release().
 *
 * See also: gb_ref_ptr_alloc().
 *
 * Return value: (transfer full): the newly allocated, reference counted,
 *   memory region.
 */
gpointer
gb_ref_ptr_dup (gconstpointer data,
                gsize         alloc_size)
{
  gpointer res;

  g_return_val_if_fail (data != NULL && alloc_size > 0, NULL);

  res = gb_ref_ptr_alloc (alloc_size);
  memcpy (res, data, alloc_size);

  return res;
}

/**
 * gb_ref_ptr_alloc_with_notify:
 * @alloc_size: the size of the memory allocation
 * @notify: a function to be called when the reference count reaches zero
 *
 * Allocates (and clears) a reference counted region of memory, and returns
 * a pointer to it.
 *
 * References can be acquired using gb_ref_ptr_acquire(), and released using
 * gb_ref_ptr_release().
 *
 * Once the last reference has been released, the memory region is
 * deallocated; if a @notify function is provided, it will be called prior
 * to the memory being freed.
 *
 * Return value: (transfer full): the newly allocated, reference counted,
 *   memory region.
 */
gpointer
gb_ref_ptr_alloc_with_notify (gsize          alloc_size,
                              GDestroyNotify notify)
{
  GbRefPtr *ref_pointer;
  gsize private_size;
  gpointer res;

  g_return_val_if_fail (alloc_size > 0, NULL);

  private_size = sizeof (GbRefPtr);

  /* if we're running under valgrind, we grow the allocation by a single pointer,
   * and we use it to point to the beginning of the allocation, so that valgrind
   * doesn't get confused, and thinks that we're leaking the preamble that we use
   * to store the reference count
   *
   * this nifty trick is taken from gtype.c in GObject, and it's what GType uses
   * to overallocate the instance structure to make space for the instance private
   * data structure in a valgrind-safe manner.
   */
  if (RUNNING_ON_VALGRIND)
    {
      private_size += ALIGN_STRUCT (1);

      res = g_slice_alloc0 (private_size + alloc_size + sizeof (gpointer));
      *(gpointer *) (res + private_size + alloc_size) = res + ALIGN_STRUCT (1);

      /* we also tell valgrind to treat this whole function as an allocator */
      VALGRIND_MALLOCLIKE_BLOCK (res + private_size, alloc_size + sizeof (gpointer), 0, TRUE);
      VALGRIND_MALLOCLIKE_BLOCK (res + ALIGN_STRUCT (1), private_size - ALIGN_STRUCT (1), 0, TRUE);
    }
  else
    res = g_slice_alloc0 (private_size + alloc_size);

  ref_pointer = res;
  ref_pointer->ref_count = 1;
  ref_pointer->notify = notify;
  ref_pointer->alloc_size = alloc_size;

  return res + private_size;
}

/*< private >
 * gb_ref_ptr_free:
 * @ref_pointer: reference counted memory area to be freed
 *
 * Releases the resources associated with @data.
 *
 * This function calls GbRefPtr.notify, if one is present.
 */
void
gb_ref_ptr_free (gpointer ref_pointer,
                 gboolean run_notify)
{
  gsize private_size = sizeof (GbRefPtr);
  GbRefPtr *ref;
  gchar *allocated;
  gsize alloc_size;

  ref = (GbRefPtr *) (((gchar *) ref_pointer) - private_size);
  alloc_size = ref->alloc_size;

  if (run_notify && ref->notify != NULL)
    ref->notify (ref_pointer);

  allocated = ((gchar *) ref_pointer) - private_size;

  /* see gb_ref_ptr_alloc() */
  if (RUNNING_ON_VALGRIND)
    {
      private_size += ALIGN_STRUCT (1);
      allocated -= ALIGN_STRUCT (1);

      *(gpointer *) (allocated + private_size + alloc_size) = NULL;
      g_slice_free1 (private_size + alloc_size + sizeof (gpointer), allocated);

      VALGRIND_FREELIKE_BLOCK (allocated + ALIGN_STRUCT (1), 0);
      VALGRIND_FREELIKE_BLOCK (ref_pointer, 0);
    }
  else
    g_slice_free1 (private_size + ref->alloc_size, allocated);
}

/**
 * gb_ref_ptr_acquire:
 * @ref_pointer: a pointer to a reference counted memory region
 *
 * Acquires a reference to the memory region pointed by @ref_pointer.
 *
 * Return value: the memory region, with its reference count increased
 *   by one
 */
gpointer
gb_ref_ptr_acquire (gpointer ref_pointer)
{
  GbRefPtr *ref;

  g_return_val_if_fail (ref_pointer != NULL, NULL);

  ref = (GbRefPtr *) (((gchar *) ref_pointer) - sizeof (GbRefPtr));

  g_atomic_int_add (&ref->ref_count, 1);

  return ref_pointer;
}

/**
 * gb_ref_ptr_release:
 * @ref_pointer: a pointer to a reference counted memory region
 *
 * Releases a reference to the memory region pointed by @ref_pointer.
 *
 * If the last reference is released, the memory region will be deallocated.
 */
void
gb_ref_ptr_release (gpointer ref_pointer)
{
  GbRefPtr *ref;

  g_return_val_if_fail (ref_pointer != NULL, NULL);

  ref = (GbRefPtr *) (((gchar *) ref_pointer) - sizeof (GbRefPtr));

  if (g_atomic_int_dec_and_test (&ref->ref_count))
    gb_ref_ptr_free (ref_pointer, TRUE);
}
