#define GLIB_ENABLE_BONGHITS
#include <glib-bonghits/glib-bonghits.h>

static void
static_string_base (void)
{
  char *str;

  str = gb_static_string_new ("Hello, World!");
  g_assert_cmpstr (str, ==, "Hello, World!");

  gb_static_string_unref (str);
}

static void
static_string_interning (void)
{
  char *str1, *str2, *str3;

  str1 = gb_static_string_new ("Hello");
  str2 = gb_static_string_new ("Hello");
  g_assert (str1 == str2);
  g_assert_cmpstr (str1, ==, str2);

  gb_static_string_unref (str1);
  gb_static_string_unref (str2);

  str3 = gb_static_string_new ("Hello");
  g_assert (str1 != str3);

  gb_static_string_unref (str3);
}

int
main (int argc, char *argv[])
{
  g_test_init (&argc, &argv, NULL);

  g_test_add_func ("/static-string/base", static_string_base);
  g_test_add_func ("/static-string/interning", static_string_interning);

  return g_test_run ();
}
