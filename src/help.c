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
#include "help.h"

void help(void) {
	printf(
"Usage: ltc [options] file\n\n"
"Options:\n"
"\t-c\n"
"\t--compress\n"
"\t\tCompress the input file.\n"
"\t-d\n"
"\t--decompress\n"
"\t\tDecompress the input file.\n"
"\t-h\n"
"\t--help\n"
"\t\tDisplay this help dialogue.\n"
"\t-i\n"
"\t--input\n"
"\t\tSet the input file.\n\n"
"Short options (one '-') that take no arguments may be proceeded immediately\n"
"(without a space) by another short option. For example:\n\n"
"\tltc -cdi ifile ofile\n\n"
"compresses then decompresses 'ifile', outputting the result to 'ofile'.\n\n"
"The output file should be specified on its own, that is, without an\n"
"accompanying option (as above).\n");
	return;
}
