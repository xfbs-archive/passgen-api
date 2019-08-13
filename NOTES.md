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

https://facil.io?
libhttp?
https://cesanta.com/docs/http/server-example.html (mongoose)?
https://github.com/lpereira/lwan
https://kore.io
http://nxweb.org
http://duda.io
http://libevent.org (https://github.com/criticalstack/libevhtp)

## Make it fast

libuev?
libevent?
manual polling?
