#include <kore/kore.h>
#include <kore/http.h>
#include <passgen/version.h>
#include <passgen/pattern.h>
#include <passgen/random.h>
#include <jansson.h>
#define bad_request(req, str) \
  http_response(req, 400, str, strlen(str)); \
  return KORE_RESULT_OK

int	preset(struct http_request *);
int	format(struct http_request *);

int format(struct http_request *req) {
  static random_t *random = NULL;

  // create only one random source.
  if(!random) {
    random = random_new();
  }

  // parse the query string.
  http_populate_qs(req);

  uint32_t amount;
  char *format;

  if(http_argument_get_uint32(req, "amount", &amount) == KORE_RESULT_OK) {
    // make sure it's not too many.
  } else {
    amount = 1;
  }

  if(http_argument_get_string(req, "format", &format) != KORE_RESULT_OK) {
    bad_request(req, "Need to specify a format string as a GET parameter.");
  }

  printf("got format '%s'\n", format);

  const char *pos = format;
  pattern_t *pattern = pattern_parse(&pos);

  if(!pattern) {
    bad_request(req, "Error parsing format string.");
  }

  const char *pass = pattern_random(pattern, random);

  if(pass) {
    http_response(req, 200, pass, strlen(pass));
  } else {
    http_response(req, 501, "Server error", 12);
  }

  return (KORE_RESULT_OK);
}
