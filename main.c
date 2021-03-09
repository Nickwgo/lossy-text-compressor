/* Copyright © 2021 Nicholas Godfrey
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Public License Version 3 as published by the
 * Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	if (argc < 3) {
		fprintf(stderr, "Please enter 2 files\n");
		exit(EXIT_FAILURE);
	} 

	FILE *input = fopen(argv[1], "r");
	FILE *output = fopen(argv[2], "w");
	char ibuf[BUFSIZ];
	char obuf[BUFSIZ];
	setvbuf(input, ibuf, _IOFBF, BUFSIZ);
	setvbuf(output, obuf, _IOFBF, BUFSIZ);
	
	char minibuf[8];
	size_t tmp;
	for (;;) {
		tmp = fread(minibuf, 1, 8, input);
		if (tmp != 8) {
			if (ferror(input)) {
				fprintf(stderr, "Error reading\n");
				exit(EXIT_FAILURE);
			} else {
				fwrite(minibuf, 1, tmp, output);
			}
			exit(EXIT_SUCCESS);
		}
		for (int i = 0; i < 7; i++) {
			minibuf[i] |= minibuf[i+1]>>(7-i);
			minibuf[i+1] = minibuf[i+1]<<(i+1);
		}
		fwrite(minibuf, 1, 7, output);
	}
	exit(EXIT_FAILURE); // THIS SHOULD NEVER BE RUN
}
