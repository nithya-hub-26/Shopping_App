#include "Server.h";

int main() {
   int tcpPort = 8080;
   int udpPort = 8081;
   string productFile = "products.csv";
   Server& server = Server::getInstance(tcpPort, udpPort, productFile);
   LOG_INFO("Shopping app server started");
   server.start();
   return 0;
}