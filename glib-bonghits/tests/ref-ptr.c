#define GLIB_ENABLE_BONGHITS
#include <glib-bonghits/glib-bonghits.h>

typedef struct _Point 	Point;
typedef struct _Size	Size;
typedef struct _Rect	Rect;

struct _Point { float x, y; };
struct _Size { float width, height; };
struct _Rect { Point origin; Size size; };

static void
ref_ptr_base (void)
{
  Rect *r;

  r = gb_ref_ptr_alloc (sizeof (Rect));
  r->origin.x = 100.f;
  r->origin.y = 100.f;
  r->size.width = 50.f;
  r->size.height = 50.f;

  gb_ref_ptr_release (r);
}

static gboolean notify_was_called = FALSE;

static void
rect_notify (gpointer data)
{
  Rect *r = data;

  g_assert_cmpfloat (r->origin.x, ==, 100.f);
  g_assert_cmpfloat (r->origin.y, ==, 100.f);
  g_assert_cmpfloat (r->size.width, ==, 50.f);
  g_assert_cmpfloat (r->size.height, ==, 50.f);

  notify_was_called = TRUE;
}

static void
ref_ptr_notify (void)
{
  Rect *r;

  r = gb_ref_ptr_alloc_with_notify (sizeof (Rect), rect_notify);
  r->origin.x = 100.f;
  r->origin.y = 100.f;
  r->size.width = 50.f;
  r->size.height = 50.f;

  gb_ref_ptr_release (r);

  g_assert (notify_was_called);
}

int
main (int argc, char *argv[])
{
  g_test_init (&argc, &argv, NULL);

  g_test_add_func ("/ref-ptr/base", ref_ptr_base);
  g_test_add_func ("/ref-ptr/notify", ref_ptr_notify);

  return g_test_run ();
}
