#include <kore/kore.h>
#include <kore/http.h>
#include <passgen/version.h>
#include <jansson.h>

int	version(struct http_request *);
int version_json(struct http_request *);

int version(struct http_request *req) {
  static const char *version = NULL;
  static size_t length = 0;

  // initialize once.
  if(!version) {
    version = passgen_version_str();
    length = strlen(version);
  }

  http_response(req, 200, version, length);

  return (KORE_RESULT_OK);
}

int version_json(struct http_request *req) {
  static const char *version = NULL;
  static size_t length = 0;

  // build JSON only once.
  if(!version) {
    passgen_version_t v = passgen_version();

    json_t *obj = json_object();
    json_object_set_new(obj, "major", json_integer(v.maj));
    json_object_set_new(obj, "minor", json_integer(v.min));
    json_object_set_new(obj, "revision", json_integer(v.rev));

    version = json_dumps(obj, JSON_COMPACT);
    length = strlen(version);

    json_decref(obj);
  }

  http_response(req, 200, version, length);

  return KORE_RESULT_OK;
}
