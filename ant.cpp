#include <lwip/sockets.h>

#define SENDER_PORT_NUM 6000
#define SENDER_IP_ADDR "192.136.23.20"

#define SERVER_PORT_NUM 6001
#define SERVER_IP_ADDRESS "192.136.23.21"


void main(void)
{

    int socket_fd;
    struct sockaddr_in sa,ra;

    int recv_data; char data_buffer[80]; /* Creates an TCP socket (SOCK_STREAM) with Internet Protocol Family (PF_INET).
     * Protocol family and Address family related. For example PF_INET Protocol Family and AF_INET family are coupled.
    */

    socket_fd = socket(PF_INET, SOCK_STREAM, 0);

    if ( socket_fd < 0 )
    {

        printf("socket call failed");
        exit(0);

    }

    memset(&sa, 0, sizeof(struct sockaddr_in));
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = inet_addr(SENDER_IP_ADDR);
    sa.sin_port = htons(SENDER_PORT_NUM);


    /* Bind the TCP socket to the port SENDER_PORT_NUM and to the current
    * machines IP address (Its defined by SENDER_IP_ADDR).
    * Once bind is successful for UDP sockets application can operate
    * on the socket descriptor for sending or receiving data.
    */
    if (bind(socket_fd, (struct sockaddr *)&sa, sizeof(struct sockaddr_in)) == -1)
    {
    printf("Bind to Port Number %d ,IP address %s failed\n",SENDER_PORT_NUM,SENDER_IP_ADDR);
    close(socket_fd);
    exit(1);
    }
    /* Receiver connects to server ip-address. */

    memset(&ra, 0, sizeof(struct sockaddr_in));
    ra.sin_family = AF_INET;
    ra.sin_addr.s_addr = inet_addr(SERVER_IP_ADDRESS);
    ra.sin_port = htons(SERVER_PORT_NUM);


    if(connect(socket_fd,(struct sockaddr_in*)&ra,sizeof(struct sockaddr_in)) < 0)
    {

        printf("connect failed \n");
        close(socket_fd);
        exit(2);

    }
    recv_data = recv(socket_fd,data_buffer,sizeof(data_buffer),0);
    if(recv_data < 0)
    {

        printf("recv failed \n");
        close(socket_fd);
        exit(2);

    }
    data_buffer[recv_data] = '\0';
    printf("received data: %s\n",data_buffer);

    close(socket_fd); 

} 
