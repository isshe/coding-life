#ifndef _ISSHE_ISOUT_ENCODE_H_
#define _ISSHE_ISOUT_ENCODE_H_

#include <stdint.h>

//#include <event2/bufferevent.h>
//#include <event2/buffer.h>

#include "iuser.h"
#include "isession.h"

#define ISOUT_HEADER_MIN_LEN 32

int isout_encode_hmac(uint8_t *data, int len, uint8_t *result);

int isout_encode_opts(uint8_t *data, int len);

int isout_encode_data(uint8_t *data, int len);

int isout_encode(isession_s *session);

#endif