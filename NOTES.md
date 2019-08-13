# Notes

- libuev https://github.com/troglobit/libuev/blob/master/API.md#overview
- posix message queue (or nanomsg?)
- shitty bloom filter to rate-limit with timer
- shitty timer to close slow connections
- capnproto for serialisation/deserialisation

## Serialisation

capnproto?
msgpack?
protobuf?

## IPC

POSIX message queues?
zmq?
nanomsg?
shared memory?

## HTTP

- https://facil.io?

  source library, not yet tested.
- libhttp?

  seems to be basically equivalent to mongoose, but based off an older version.
- https://cesanta.com/docs/http/server-example.html (mongoose)?
  
  works, but is a bit slow (60k req/sec)
- https://github.com/lpereira/lwan

  doesn't compile on macOS, tried compiling by hand but crashes.

  they do fuzzing, which should be good.
- https://kore.io

  works quite nicely, needs to be recompiled to turn off TLS?

  has a cli and a custom build system. I kinda prefer a meson based build
  system for portability.
- http://nxweb.org

      configure: error: darwin18.6.0 is not currently supported
- http://duda.io

  not really maintained (last commit three years ago?)
- http://libevent.org (https://github.com/criticalstack/libevhtp)

## TLS Support

https://hitch-tls.org

## Make it fast

libuev?
libevent?
manual polling?
