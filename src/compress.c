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

int compress(FILE *input, FILE *output) {
	unsigned char minibuf[8];
	size_t tmp;
	for (;;) {
		tmp = fread(minibuf, 1, 8, input);
		if (tmp != 8) {
			if (ferror(input)) {
				fprintf(stderr, "compress: Error reading\n");
				exit(EXIT_FAILURE);
			} else if (fwrite(minibuf, 1, tmp, output) != tmp) {
				fprintf(stderr, "compress: Error writing\n");
				exit(EXIT_FAILURE);
			}
			return 0;
		}

		for (int i = 0; i < 7; i++) {
			minibuf[i] &= 0xff<<(i+1);
			minibuf[i] |= minibuf[i+1]>>(7-i);
			minibuf[i+1] = minibuf[i+1]<<(i+1);
		}
		if (fwrite(minibuf, 1, 7, output) != 7) {
			fprintf(stderr, "compress: Error writing\n");
			exit(EXIT_FAILURE);
		}
	}
	exit(EXIT_FAILURE); /* THIS SHOULD NEVER BE RUN */
}
