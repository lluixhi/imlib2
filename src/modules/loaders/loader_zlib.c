#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <zlib.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>

#include "common.h"
#include "image.h"

#define OUTBUF_SIZE 16484
#define INBUF_SIZE 1024

static int handle_buffer (DATA8 *src, unsigned long src_len,
                          DATA8 **dest, unsigned long *dest_len)
{
	static DATA8 outbuf[OUTBUF_SIZE];
	uLongf outbuf_len = OUTBUF_SIZE;
	int res;

	assert (src);
	assert (src_len);
	assert (dest);
	assert (dest_len);

	res = uncompress (outbuf, &outbuf_len, src, src_len);

	switch (res) {
		case Z_OK:
			*dest = outbuf;
			*dest_len = (unsigned long) outbuf_len;
			return 1;
		case Z_BUF_ERROR:
			return 0;
		default:
			*dest = NULL;
			*dest_len = 0;
			return 0;
	}
}

static void uncompress_file (int src, int dest, off_t size)
{
	DATA8 inbuf[INBUF_SIZE], *outbuf;
	off_t left;
	ssize_t inlen;
	unsigned long outlen = 0;

	for (left = size; left; left -= inlen) {
		inlen = read (src, inbuf, MIN (left, INBUF_SIZE));
		
		if (inlen <= 0)
			break;

		if (handle_buffer (inbuf, inlen, &outbuf, &outlen))
			write (dest, outbuf, outlen);
	}
}

char load (ImlibImage *im, ImlibProgressFunction progress,
           char progress_granularity, char immediate_load)
{
	ImlibLoader *loader;
	int src, dest;
	char *file, tmp[] = "/tmp/imlib2_loader_zlib-XXXXXX";
	struct stat st;

	assert (im);

	/* we'll need a copy of it later */
	file = im->real_file;

	if (stat (im->real_file, &st) < 0)
		return 0;

	if ((dest = mkstemp (tmp)) < 0)
		return 0;

	if ((src = open (im->real_file, O_RDONLY)) < 0) {
		unlink (tmp);
		return 0;
	}

	uncompress_file (src, dest, st.st_size);

	close (src);
	close (dest);

	if (!(loader = __imlib_FindBestLoaderForFile (tmp, 0))) {
		unlink (tmp);
		return 0;
	}

	free (im->real_file);
	im->real_file = strdup (tmp);
	loader->load (im, progress, progress_granularity, immediate_load);

	free (im->real_file);
	im->real_file = strdup (file);
	unlink (tmp);

	return 1;
}

void formats (ImlibLoader *l)
{
	/* this is the only bit you have to change... */
	char *list_formats[] = {"gz"};
	int i;

   /* don't bother changing any of this - it just reads this in
	* and sets the struct values and makes copies
	*/
	l->num_formats = sizeof (list_formats) / sizeof (char *);
	l->formats = malloc (sizeof (char *) * l->num_formats);

	for (i = 0; i < l->num_formats; i++)
		l->formats[i] = strdup (list_formats[i]);
}
