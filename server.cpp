#include <iostream>

#include "wrappedsocket.h"

// test server-side socket of wrappedsocket
int main() {
  Socket server;

  server.bind_(10010);

  server.listen_(10);

  auto client = server.accept_();

  client->send("hello wolrd");

  client->close_();

  server.close_();

  return 0;
}
