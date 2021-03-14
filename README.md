# Lossy Text Compressor

As we all know, there are times when we just don't need all of the details - a
vague outline is all that's required. Furthermore, as anyone who has
experimented with methods of lossless compression will know, lossless
compression has its limits. If you are in such a situation, this is the
software for you - a lossy text compressor. As with all lossy compression,
this allows the user to cut out the largely unnecessary details, in order to
achieve a smaller filesize. This revolutionary algorithm has been shown to
consistently reduce the sizes of files by upwards of 12%, by removing the
least significant bit of every byte. Whilst this software was originally
designed for use on plaintext, it can technically be used on any file format,
including raw binary, just as effectively.

# Usage

Usage: ltc [options] file

Options:
	-c
	--compress
		Compress the input file.
	-d
	--decompress
		Decompress the input file.
	-h
	--help
		Display this help dialogue.
	-i
	--input
		Set the input file.

Short options (one '-') that take no arguments may be proceeded
immediately (without a space) by another short option. For example:

	ltc -cdi ifile ofile

compresses then decompresses 'ifile', outputting the result to
'ofile'.

The output file should be specified on its own, that is, without an
accompanying option (as above).

# Other

Copyright © 2021 Nicholas Godfrey\
Email: nickg_8128@outlook.com
