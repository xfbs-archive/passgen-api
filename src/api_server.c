#include <mongoose.h>
#include <passgen/version.h>
#include <jansson.h>

static const char *s_http_port = "8000";
static int s_sig_num = 0;

typedef void handle_method(struct mg_connection *conn, const struct http_message *msg);
struct route {
  struct mg_str path;
  struct mg_str method;
  handle_method *handler;
};

static void signal_handler(int sig_num) {
  signal(sig_num, signal_handler);
  s_sig_num = sig_num;
}

static int has_prefix(const struct mg_str *uri, const struct mg_str *prefix) {
  return uri->len >= prefix->len && memcmp(uri->p, prefix->p, prefix->len) == 0;
}

static int is_equal(const struct mg_str *s1, const struct mg_str *s2) {
  return s1->len == s2->len && memcmp(s1->p, s2->p, s2->len) == 0;
}

void handle_version(struct mg_connection *nc, const struct http_message *hm);
void handle_passgen(struct mg_connection *nc, const struct http_message *hm);
void handle_version_json(struct mg_connection *nc, const struct http_message *hm);

static void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
  static const struct route routes[] = {
    {MG_MK_STR("/api/v1/version.json"), MG_MK_STR("GET"), &handle_version_json},
    {MG_MK_STR("/api/v1/version"), MG_MK_STR("GET"), &handle_version},
    {MG_MK_STR("/api/v1/passgen"), MG_MK_STR("GET"), &handle_passgen},
  };
  struct http_message *hm = (struct http_message *) ev_data;
  bool found = false;

  switch (ev) {
    case MG_EV_HTTP_REQUEST:
      for(size_t i = 0; i < sizeof(routes); i++) {
        if(is_equal(&hm->method, &routes[i].method) &&
            has_prefix(&hm->uri, &routes[i].path)) {
          routes[i].handler(nc, hm);
          found = true;
          break;
        }
      }

      if(!found) {
        mg_printf(nc,
          "HTTP/1.0 404 Not Found\r\n"
          "Content-Length: 0\r\n\r\n");
      }
      break;
    default:
      break;
  }
}

void handle_version(struct mg_connection *nc, const struct http_message *hm) {
  const char *data = passgen_version_str();

  mg_printf(nc,
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/plain\r\n"
    "Content-Length: %d\r\n\r\n%s",
    (int) strlen(data), data);
}

void handle_passgen(struct mg_connection *nc, const struct http_message *hm) {
  char format[256];
  int ret = mg_get_http_var(&hm->query_string, "format", format, sizeof(format));

  if(ret > 0) {
    mg_printf(nc,
      "HTTP/1.1 200 OK\r\n"
      "Content-Type: text/plain\r\n"
      "Content-Length: %d\r\n\r\n%s",
      ret, format);
  } else {
    mg_printf(nc,
      "HTTP/1.0 413 Request Entity Too Large\r\n"
      "Content-Length: 0\r\n\r\n");
  }
}

void handle_version_json(struct mg_connection *nc, const struct http_message *hm) {
  const char *version_str = passgen_version_str();
  passgen_version_t version = passgen_version();

  json_t *body = json_object();
  json_object_set_new(body, "version", json_string_nocheck(version_str));
  json_object_set_new(body, "major", json_integer(version.maj));
  json_object_set_new(body, "minor", json_integer(version.min));
  json_object_set_new(body, "revision", json_integer(version.rev));

  char *output = json_dumps(body, JSON_COMPACT);
  size_t output_len = strlen(output);

  mg_printf(nc,
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/plain\r\n"
    "Content-Length: %zu\r\n\r\n%s",
    output_len, output);

  free(output);
  json_decref(body);
}

int main(int argc, char *argv[]) {
  struct mg_mgr mgr;
  struct mg_connection *nc;

  /* Open listening socket */
  mg_mgr_init(&mgr, NULL);
  nc = mg_bind(&mgr, s_http_port, ev_handler);
  mg_set_protocol_http_websocket(nc);

  signal(SIGINT, signal_handler);
  signal(SIGTERM, signal_handler);

  /* Run event loop until signal is received */
  printf("Starting RESTful server on port %s\n", s_http_port);
  while (s_sig_num == 0) {
    mg_mgr_poll(&mgr, 1000);
  }

  /* Cleanup */
  mg_mgr_free(&mgr);

  printf("Exiting on signal %d\n", s_sig_num);

  return 0;
}
