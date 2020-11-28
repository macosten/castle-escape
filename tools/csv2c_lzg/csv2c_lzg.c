#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>

#include <lzg.h>
#include <stretchy_buffer.h>

int main(int argc, char * argv[]) {
	// Not going to bother much with security or error checking.
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <uncompressed_csv>\n", argv[0]);
		return -1;
	}

	FILE * infile = fopen(argv[1], "r"); // Input file.

	// Figure out the filename without the extension.
	char * filename = basename(argv[1]);
	
	for (char * ptr = filename; *ptr != '\0'; ++ptr) {
		if (*ptr == '.') { 
			*ptr = '\0'; 
			break;
		}
	}

	if (!infile) {
		fprintf(stderr, "Error reading input file.\n");
		return -1;
	}

	// Read all the csv values. (A trailing comma will be necessary, probably.)
	unsigned char * uncompressed_map = NULL;

	size_t count = 0;
	for (count = 0; ; ++count) {
		unsigned char inchar = 0;

		int rval = fscanf(infile, "%hhu,", &inchar); // Parse values one at a time.
		if (rval != 1) { break; } // Not enough tokens found -- terminate loop.

		sb_push(uncompressed_map, inchar);
	}

	// uncompressed_map now contains our uncompressed map data. 
	// Let's compress it.

	unsigned char * enc_map_buf; 
	// This might have appending garbage at the end, so it's not quite fit to be called enc_map

	lzg_uint32_t enc_size, max_enc_size;

	// Configure...
	lzg_encoder_config_t config;
	LZG_InitEncoderConfig(&config);
    config.fast = LZG_TRUE;
    config.level = LZG_LEVEL_9;

	// Determine the maximum size of the compressed data:
	max_enc_size = LZG_MaxEncodedSize(sb_count(uncompressed_map));

	enc_map_buf = malloc(max_enc_size);

	if (enc_map_buf) {
		// Compress the data into the buffer, if it exists.
		enc_size = LZG_Encode(uncompressed_map, sb_count(uncompressed_map), enc_map_buf, max_enc_size, NULL);
		if (enc_size) {
			// Pretty-print out the map code.
			printf("const unsigned char const %s[] = {\n\t", filename);
			for (unsigned int i = 0; i < enc_size; ++i){
				if (i > 0 && i % 16 == 0) { printf("\n\t"); }
				printf("%u,", enc_map_buf[i]);
			}
			printf("\n};");
			// The user can copy+paste from the terminal to their C file, 
			// or they can pipe the output to another file/program.
		} else {
			fprintf(stderr, "Compression failed...\n");
		}

	} else {
		fprintf(stderr, "Error allocating memory. You might not have enough.\n");
		return -1;
	}


}
