/*
 * Copyright © 2021 Nicholas Godfrey
 *
 * This file is part of Lossy Text Compressor.
 *
 * Lossy Text Compressor is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Public License Version 3 as published
 * by the Free Software Foundation.
 *
 * Lossy Text Compressor is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
 * Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "compress.h"
#include "decompress.h"
#include "help.h"
#include <errno.h>
#include <string.h>
#include <stdlib.h>

enum instructions {
	COMPRESS,
	DECOMPRESS,
};
static int do_function(enum instructions *, int, FILE *, FILE *);
static int fmov(FILE *, FILE *); /* copies the contents of one file to
				    another. */
static void rmtmp(void); /* removes the temporary file */

static char *output_file;
static char *input_file;
static char *tmp_file;

int main(int argc, char *argv[]) {
	enum instructions *queue = malloc(8 * sizeof(enum instructions));
	/*
	 * This is where the list of commands given by the user will be
	 * stored.
	 */
	if (!queue) {
		fprintf(stderr, "malloc: An error occurred\n");
		exit(EXIT_FAILURE);
	}
	int queueno = 0; /* number of instructions in the queue */
	for (int i = 1; i < argc; i++) { /* iterate over, and process, every
					    argument given by the user */
		if (*argv[i] != '-') {
			if (output_file) {
				fprintf(stderr, "Error: output file defined"
						" more than once\n");
				exit(EXIT_FAILURE);
			}
			output_file = argv[i];
			continue;
		}
		goto label_1;
label_0: /* Please don't shoot me */
		if (!(queueno % 8)) {
			queue = realloc(queue, sizeof(enum instructions)
					* (queueno + 8));
			/* Whenever the number of commands exceeds the current
			 * size of the queue, I increase it by 8. I haven't
			 * done any testing, but 8 seems like a nice balance
			 * between not being so small that it's inefficient,
			 * and not being so large that it wastes lots of
			 * space.
			 */
			if (!queue) {
				fprintf(stderr, "realloc: An error occurred\n");
				exit(EXIT_FAILURE);
			}
		}
label_1:
		switch (*++argv[i]) {
		case '-':
			/*
			 * If I add lots of options, I might turn this into a
			 * hash table. Until then, a few if statements can't
			 * hurt.
			 */
			++argv[i];
			if (!strcmp("input", argv[i]))
				goto case_i;
			if (!strcmp("decompress", argv[i]))
				goto case_d;
			if (!strcmp("compress", argv[i]))
				goto case_c;
			if (!strcmp("help", argv[i]))
				goto case_h;
			fprintf(stderr, "Unrecognised option:  --%s\nTry '%s"
					" --help' for more information.\n",
					argv[i], argv[0]);
			exit(EXIT_FAILURE);
		case 'i':
		case_i:
			if (input_file) {
				fprintf(stderr, "Error: Input file defined"
						" more than once\n");
				exit(EXIT_FAILURE);
			}
			input_file = argv[++i];
			break;
		case 'c':
		case_c:
			queue[queueno++] = COMPRESS;
			goto label_0;
		case 'd':
		case_d:
			queue[queueno++] = DECOMPRESS;
			goto label_0;
		case 'h':
		case_h:
			help();
			exit(EXIT_SUCCESS);
		case '\0':
			break;
		default:
			fprintf(stderr, "Unrecognised option:  -%c\nTry '%s"
					" --help' for more information.\n",
					*argv[i], argv[0]);
			exit(EXIT_FAILURE);
		}
	}
	if (!queueno) {
		fprintf(stderr, "Nothing to do. Exiting...\n");
		exit(EXIT_SUCCESS);
	}
	if (!input_file) {
		fprintf(stderr, "Error: No input file defined\n");
		exit(EXIT_FAILURE);
	}
	if (!output_file) {
		fprintf(stderr, "Error: No output file defined\n");
		exit(EXIT_FAILURE);
	}
	FILE *ifile = fopen(input_file, "rb");
	if (!ifile) {
		perror("fopen (ifile)");
		exit(EXIT_FAILURE);
	}
	FILE *ofile = fopen(output_file, "rb"); /* check if the output file
						   already exists */
	if (ofile) {
		if (fclose(ofile) == EOF) {
			perror("fclose (ofile)");
			exit(EXIT_FAILURE);
		}
label_2: /* I can't really think of a better way of naming these */
		fprintf(stderr,"This file already exists. Continue? (y/n)  ");
		switch (getchar()) {
		case 'y':
		case 'Y':
			break;
		case 'n':
		case 'N':
			exit(EXIT_SUCCESS);
		default:
			while (getchar()!='\n')
				;
			/* fall through */
		case '\n':
			goto label_2;
		}
	}
	else if (errno != ENOENT) {
		perror("fopen (ofile)");
		exit(EXIT_FAILURE);
	}
	ofile = fopen(output_file, "wb");
	if (!ofile) {
		perror("fopen (ofile)");
		exit(EXIT_FAILURE);
	}
	char ibuf[BUFSIZ];
	char obuf[BUFSIZ];
	/*
	 * I am using block buffering, as opposed to line buffering, because
	 * it is far better when dealing with binary data, and is also a lot
	 * more efficient.
	 */
	if (setvbuf(ifile, ibuf, _IOFBF, BUFSIZ)) {
		if (errno)
			perror("setvbuf (ifile)");
		else
			fprintf(stderr, "setvbuf (ifile):"
					" An error occurred\n");
		exit(EXIT_FAILURE);
	}
	if (setvbuf(ofile, obuf, _IOFBF, BUFSIZ)) {
		if (errno)
			perror("setvbuf (ofile)");
		else
			fprintf(stderr, "setvbuf (ofile):"
					" An error occurred\n");
		exit(EXIT_FAILURE);
	}

	if (queueno == 1) { /* if there is only one command, then there is no
			       need for a temporary file */
		do_function(queue, 0, ifile, ofile);
		exit(EXIT_SUCCESS);
	}

	tmp_file = malloc(strlen(output_file)+5);
	if (!tmp_file) {
		fprintf(stderr, "malloc: An error occurred\n");
		exit(EXIT_FAILURE);
	}
	strcpy(tmp_file, output_file);
	strcat(tmp_file, ".tmp");
	FILE *tmpfile = fopen(tmp_file, "rb");
	if (tmpfile) {
		/*
		 * If the temporary file already exists, it could mean that
		 * there is a process already running.
		 */
label_3:
		fprintf(stderr, "Error: %s already exists. Abort? (y/n)", tmp_file);
		switch (getchar()) {
		case 'y':
		case 'Y':
			exit(EXIT_SUCCESS);
		case 'n':
		case 'N':
			break;
		default:
			while (getchar()!='\n')
				;
			/* fall through */
		case '\n':
			goto label_3;
		}
	} else if (errno != ENOENT) {
		perror("fopen (tmpfile)");
		exit(EXIT_FAILURE);
	}
	tmpfile = fopen(tmp_file, "wb");
	if (!tmpfile) {
		perror("fopen (tmpfile)");
		exit(EXIT_FAILURE);
	}
	if (atexit(rmtmp)) { /* from now on, the temporary file will be
				automatically deleted if the program should
				terminate */
		fprintf(stderr, "atexit: An error occurred\n");
		exit(EXIT_FAILURE);
	}
	char tmpbuf[BUFSIZ];
	if (setvbuf(tmpfile, tmpbuf, _IOFBF, BUFSIZ)) {
		if (errno)
			perror("setvbuf (tmpfile)");
		else
			fprintf(stderr, "setvbuf (tmpfile):"
					" An error occurred\n");
		exit(EXIT_FAILURE);
	}

	int i;
	for (i = 0; i < queueno; i++) /* iterate over every command */
		/*
		 * The first command uses the original input file as an input,
		 * and the temporary file as an output (seeing as it's already
		 * open for writing). Subsequent odd-numbered commands
		 * (commands start at 0), use the temporary file as an input,
		 * and the output file as an output. Subsequent even-numbered
		 * commands use the output file as an input, and the temporary
		 * file as an output. Be thankful for caching.
		 */
		if (!i)
			do_function(queue, 0, ifile, tmpfile);
		else if (i%2) {
			if (!freopen(tmp_file, "rb", tmpfile)) {
				perror("freopen (tmpfile)");
				exit(EXIT_FAILURE);
			}
			if (i-1 && !freopen(output_file, "wb", ofile)) {
				perror("freopen (ofile)");
				exit(EXIT_FAILURE);
			}
			do_function(queue, i, tmpfile, ofile);
		} else {
			if (!freopen(tmp_file, "wb", tmpfile)) {
				perror("freopen (tmpfile)");
				exit(EXIT_FAILURE);
			}
			if (!freopen(output_file, "rb", ofile)) {
				perror("freopen (ofile)");
				exit(EXIT_FAILURE);
			}
			do_function(queue, i, ofile, tmpfile);
		}
	if (i%2) { /* if the last command was an even-numbered command, then
		      the contents of the temporary file must be copied to
		      the output file. */
		if (!freopen(tmp_file, "rb", tmpfile)) {
			perror("freopen (tmpfile)");
			exit(EXIT_FAILURE);
		}
		if (!freopen(output_file, "wb", ofile)) {
			perror("freopen (ofile)");
			exit(EXIT_FAILURE);
		}
		fmov(tmpfile, ofile);
	}
	exit(EXIT_SUCCESS);
}

static int do_function(enum instructions *queue, int i, FILE *ifile, FILE *ofile) {
	switch (queue[i]) {
		case COMPRESS:
			compress(ifile, ofile);
			break;
		case DECOMPRESS:
			decompress(ifile, ofile);
			break;
		default: /* THIS SHOULD NEVER HAPPEN */
			exit(EXIT_FAILURE);
	}
	return 0;
}

static int fmov(FILE *ifile, FILE *ofile) {
	char buf[BUFSIZ];
	size_t ret;
	for (;;) {
		ret = fread(buf, 1, BUFSIZ, ifile);
		if (ret != BUFSIZ)
			if (ferror(ifile)) {
				fprintf(stderr, "An error occurred when reading"
						" tmpfile\n");
				exit(EXIT_FAILURE);
			} else if (ret != fwrite(buf, 1, ret, ofile)) {
				fprintf(stderr, "0An error occurred when writing"
						" to ofile\n");
				exit(EXIT_FAILURE);
			} else {
				exit(EXIT_SUCCESS);
			}
		if (BUFSIZ != fwrite(buf, 1, BUFSIZ, ofile)) {
			fprintf(stderr, "1An error occurred when writing"
					" to ofile\n");
			exit(EXIT_FAILURE);
		}
	}
	exit(EXIT_FAILURE); /* THIS SHOULD NEVER BE RUN */
}

static void rmtmp(void) {
	if (tmp_file && remove(tmp_file)) {
		perror("remove (tmpfile)");
	}
}
