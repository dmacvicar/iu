#pragma once

#include <cstdio>

namespace iu {

int jpeg_scan_head(FILE *infile, int *width, int *height);

}
