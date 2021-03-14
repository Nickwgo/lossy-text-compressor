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
#include "decompress.h"

int decompress(FILE *input, FILE *output) {
	unsigned char minibuf[8];
	size_t tmp;
	srand(time(NULL)); /* normally I use clock_gettime() and seed rand
			      with the nanosecond component, but apparently
			      not all systems support that */
	for (;;) {
		tmp = fread(minibuf, 1, 7, input);
		if (tmp != 7)
			if (ferror(input)) {
				fprintf(stderr, "decompress: Error"
						" reading\n");
				exit(EXIT_FAILURE);
			} else if (fwrite(minibuf, 1, tmp, output) != tmp) {
				fprintf(stderr, "decompress: Error"
						" writing\n");
				exit(EXIT_FAILURE);
			} else {
				return 0;
			}
		for (int i = 6; i >= 0; i--) {
			minibuf[i+1] = (minibuf[i+1] >> (i+1))
				| (minibuf[i] << (7-i));
			minibuf[i+1] ^= rand()%2;
		}
		minibuf[0] ^= rand()%2;
		if (fwrite(minibuf, 1, 8, output) != 8) {
			fprintf(stderr, "decompress: Error writing\n");
			exit(EXIT_FAILURE);
		}
	}
	exit(EXIT_FAILURE); /* THIS SHOULD NEVER BE RUN */
}
