#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>

#define SERV_PORT 9527

void err(const char *str)
{
    perror(str);
    exit(1);
}
int main()
{
    struct sockaddr_in serv_addr, clit_addr;
    socklen_t clit_addr_len;
    char buf[BUFSIZ];

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    int lfd = 0;
    int cfd = 0;

    lfd = socket(AF_INET, SOCK_STREAM, 0);
    if (lfd == -1)
    {
        err("socket失败");
    }

    if (bind(lfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
    {
        err("bind失败");
    }

    if (listen(lfd, 128) == -1)
    {
        err("listen失败");
    }

    clit_addr_len = sizeof(clit_addr);
    cfd = accept(lfd, (struct sockaddr *)&clit_addr, &clit_addr_len);
    if (cfd == -1)
    {
        err("accept失败");
    }
    while (1)
    {
        int ret = read(cfd, buf, sizeof(buf));
        write(STDOUT_FILENO, buf, ret);
        if (buf[0] == 'q')
        {
            break;
        }
        for (int i = 0; i < ret; i++)
        {
            buf[i] = toupper(buf[i]);
        }
        (*buf)++;
        write(cfd, buf, ret);
    }
    close(lfd);
    close(cfd);

    return 0;
}
