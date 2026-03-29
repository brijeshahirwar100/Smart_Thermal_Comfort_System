#ifndef NETWORK_H
#define NETWORK_H

#include <PubSubClient.h>

extern PubSubClient client;

void network_begin();
void network_loop();

#endif