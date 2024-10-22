#include <gtest/gtest.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "server.cpp"


struct SocketPair {
    int client;
    int server;
};

SocketPair createSocketPair() {
    int sockets[2];
    socketpair(AF_UNIX, SOCK_STREAM, 0, sockets);
    return {sockets[0], sockets[1]};
}

TEST(HandleClientTest, ReturnsCorrectResponseSize) {
SocketPair socketPair = createSocketPair();

// Fork the process to simulate the client-server interaction
pid_t pid = fork();

if (pid == 0) {
// Child process (client)
close(socketPair.server);
int responseSize = handleClient(socketPair.client);
close(socketPair.client);

// Check if the response size is correct
EXPECT_EQ(responseSize, sizeof(response));
exit(0);
} else {
// Parent process (server)
close(socketPair.client);

// Wait for the child process to exit
int status;
waitpid(pid, &status, 0);

// Check if the child process exited successfully
EXPECT_TRUE(WIFEXITED(status));
EXPECT_EQ(WEXITSTATUS(status), 0);
}
}
