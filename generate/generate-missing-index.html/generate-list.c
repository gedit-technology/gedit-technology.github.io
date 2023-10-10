#include <locale.h>
#include <gio/gio.h>

/* Restrict what is supported as filenames, otherwise it's more complicated to
 * encode the links in HTML (what if the filename contains a '"' char for
 * instance).
 */
static gboolean
filename_char_is_supported (gchar ch)
{
	return (g_ascii_isalnum (ch) ||
		ch == '-' ||
		ch == '_' ||
		ch == '.');
}

static gboolean
filename_is_supported (const gchar *filename)
{
	gint i;

	g_assert (filename != NULL);

	for (i = 0; filename[i] != '\0'; i++)
	{
		if (!filename_char_is_supported (filename[i]))
		{
			return FALSE;
		}
	}

	return TRUE;
}

int
main (int    argc,
      char **argv)
{
	GFile *dir;
	GFileEnumerator *enumerator;
	GError *error = NULL;
	GList *list_of_files = NULL;
	GList *list_of_dirs = NULL;
	GList *l;
	int return_value = EXIT_SUCCESS;

	/* To print correctly all Unicode characters (e.g. accents, etc). */
	setlocale (LC_ALL, "");

	if (argc != 2)
	{
		g_printerr ("Usage: %s <dir>\n", argv[0]);
		return EXIT_FAILURE;
	}

	dir = g_file_new_for_commandline_arg (argv[1]);
	enumerator = g_file_enumerate_children (dir,
						G_FILE_ATTRIBUTE_STANDARD_NAME ","
						G_FILE_ATTRIBUTE_STANDARD_DISPLAY_NAME ","
						G_FILE_ATTRIBUTE_STANDARD_TYPE,
						G_FILE_QUERY_INFO_NONE,
						NULL,
						&error);

	if (error != NULL)
	{
		g_printerr ("Failed to enumerate the directory: %s\n", error->message);
		return_value = EXIT_FAILURE;
		goto end;
	}

	while (TRUE)
	{
		GFileInfo *info;
		GFileType type;
		const gchar *display_name;

		info = g_file_enumerator_next_file (enumerator, NULL, &error);

		if (error != NULL)
		{
			g_printerr ("Error while enumerating the directory: %s\n", error->message);
			return_value = EXIT_FAILURE;
			goto end;
		}

		if (info == NULL)
		{
			break;
		}

		display_name = g_file_info_get_display_name (info);

		if (!filename_is_supported (display_name))
		{
			g_printerr ("File with name '%s' contains an unsupported character.\n",
				    display_name);
			return_value = EXIT_FAILURE;
			g_object_unref (info);
			goto end;
		}

		type = g_file_info_get_file_type (info);

		if (type == G_FILE_TYPE_REGULAR)
		{
			list_of_files = g_list_prepend (list_of_files, g_strdup (display_name));
		}
		else if (type == G_FILE_TYPE_DIRECTORY)
		{
			list_of_dirs = g_list_prepend (list_of_dirs, g_strdup (display_name));
		}
		else
		{
			g_printerr ("File with name '%s' is neither a regular file nor a directory.\n",
				    display_name);
			return_value = EXIT_FAILURE;
			g_object_unref (info);
			goto end;
		}

		g_object_unref (info);
	}

	list_of_dirs = g_list_sort (list_of_dirs, (GCompareFunc) g_utf8_collate);
	list_of_files = g_list_sort (list_of_files, (GCompareFunc) g_utf8_collate);

	g_print ("<ul>\n");
	for (l = list_of_dirs; l != NULL; l = l->next)
	{
		g_print ("  <li><a href=\"%s/\">%s/</a></li>\n",
			 l->data,
			 l->data);
	}
	for (l = list_of_files; l != NULL; l = l->next)
	{
		/* Skip index.html because it is the file showing the list. */
		if (g_str_equal (l->data, "index.html"))
		{
			continue;
		}

		g_print ("  <li><a href=\"%s\">%s</a></li>\n",
			 l->data,
			 l->data);
	}
	g_print ("</ul>\n");

end:
	g_object_unref (dir);
	g_clear_object (&enumerator);
	g_clear_error (&error);
	g_list_free_full (list_of_files, g_free);
	g_list_free_full (list_of_dirs, g_free);
	return return_value;
}
