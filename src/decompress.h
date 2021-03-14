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
#ifndef DECOMPRESS_H
#define DECOMPRESS_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int decompress(FILE *, FILE *);

#endif
