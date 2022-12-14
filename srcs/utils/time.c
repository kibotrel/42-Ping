#include "core.h"
#include "utils.h"

suseconds_t get_timestamp(void) {
	struct timeval time;

	if (gettimeofday(&time, NULL) < 0)	{
		warn_with_code(FAILED_GET_TIME);
	}

	return (time.tv_sec * 1000000.0 + time.tv_usec);
}

suseconds_t compute_rtt(struct timeval *time) {
	suseconds_t now = get_timestamp();

	return (now - time->tv_sec * 1000000.0 - time->tv_usec);
}
