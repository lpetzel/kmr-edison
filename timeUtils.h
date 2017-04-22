// Copied from https://gist.github.com/diabloneo/9619917
// Author of the cpp file is Neo Liu (diabloneo)
#pragma once
#include <ctime>

void timespec_diff(struct timespec *start, struct timespec *stop,
                   struct timespec *result);
