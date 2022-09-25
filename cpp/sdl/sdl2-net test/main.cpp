#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <cstdio>
#include <cstring>
#include <iostream>
using namespace std;

int main(int argc, char **argv) {
  if (SDL_Init(0) == -1) {
    printf("SDL_Init: %s\n", SDL_GetError());
    exit(1);
  }
  if (SDLNet_Init() == -1) {
    printf("SDLNet_Init: %s\n", SDLNet_GetError());
    exit(2);
  }

  bool done = false;

  if (argc == 2 && strcmp(argv[1], "server") == 0) {
    printf("Starting server...\n");
    TCPsocket server, client;
    IPaddress ip;
    if (SDLNet_ResolveHost(&ip, NULL, 9999) == -1) {
      printf("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
      exit(1);
    }
    server = SDLNet_TCP_Open(&ip);
    if (!server) {
      printf("SDLNet_TCP_Open: %s\n", SDLNet_GetError());
      exit(2);
    }
    while (!done) {
      /* try to accept a connection */
      client = SDLNet_TCP_Accept(server);
      if (!client) { /* no connection accepted */
        /*printf("SDLNet_TCP_Accept: %s\n",SDLNet_GetError()); */
        SDL_Delay(100); /*sleep 1/10th of a second */
        continue;
      }

      /* get the clients IP and port number */
      IPaddress *remoteip;
      remoteip = SDLNet_TCP_GetPeerAddress(client);
      if (!remoteip) {
        printf("SDLNet_TCP_GetPeerAddress: %s\n", SDLNet_GetError());
        continue;
      }

      /* print out the clients IP and port number */
      Uint32 ipaddr;
      ipaddr = SDL_SwapBE32(remoteip->host);
      printf("Accepted a connection from %d.%d.%d.%d port %hu\n", ipaddr >> 24,
             (ipaddr >> 16) & 0xff, (ipaddr >> 8) & 0xff, ipaddr & 0xff,
             remoteip->port);

      while (1) {
        /* read the buffer from client */
        char message[1024];
        int len = SDLNet_TCP_Recv(client, message, 1024);
        if (!len) {
          printf("SDLNet_TCP_Recv: %s\n", SDLNet_GetError());
          break;
        }
        /* print out the message */
        printf("Received: %.*s\n", len, message);
        if (message[0] == 'q') {
          printf("Disconecting on a q\n");
          break;
        }
        if (message[0] == 'Q') {
          printf("Closing server on a Q.\n");
          done = true;
          break;
        }
      }
      SDLNet_TCP_Close(client);
    }
  } else if (argc == 2 && strcmp(argv[1], "client") == 0) {
    printf("Starting client...\n");
    IPaddress ip;
    TCPsocket tcpsock;

    if (SDLNet_ResolveHost(&ip, "192.168.1.2", 9999) == -1) {
      printf("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
      exit(1);
    }

    tcpsock = SDLNet_TCP_Open(&ip);
    if (!tcpsock) {
      printf("SDLNet_TCP_Open: %s\n", SDLNet_GetError());
      exit(2);
    }

    while (1) {
      printf("message: ");
      char message[1024];
      fgets(message, 1024, stdin);
      int len = strlen(message);

      /* strip the newline */
      message[len - 1] = '\0';

      if (len) {
        int result;

        /* print out the message */
        printf("Sending: %.*s\n", len, message);

        result =
            SDLNet_TCP_Send(tcpsock, message, len); /* add 1 for the NULL */
        if (result < len)
          printf("SDLNet_TCP_Send: %s\n", SDLNet_GetError());
      }

      if (len == 2 && tolower(message[0]) == 'q') {
        break;
      }
    }

    SDLNet_TCP_Close(tcpsock);
  } else {
    printf("Choose server or client\n");
  }

  SDLNet_Quit();
  SDL_Quit();
}
