# Lossy Text Compressor

As we all know, there are times when we just don't need all of the details - a
vague outline is all that's required. Furthermore, as anyone who has
experimented with methods of lossless compression will know, lossless
compression has its limits. If you are in such a situation, this is the
software for you - a lossy text compressor. As with all lossy compression,
this allows the user to cut out the largely unnecessary details, in order to
achieve a smaller filesize. This revolutionary algorithm has been shown to
consistently reduce the sizes of files by upwards of 14%, by removing the
least significant bit of every byte. Whilst this software was originally
designed for use on plaintext, it can technically be used on any file format,
including raw binary, just as effectively.

Usage:	ltc <input_file> <output_file>

Please note that the program will overwrite the output file, if it already
exists, without warning.
