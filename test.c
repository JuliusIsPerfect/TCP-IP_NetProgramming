#include <arpa/inet.h>
#include <stdio.h>

int main() {
    struct in_addr my_addr;
    // 以点分十进制形式分配一个 IPv4 地址
    inet_aton("192.168.1.100", &my_addr);

    // 以数值形式打印地址
    printf("IP 地址以数值形式：%u\n", my_addr.s_addr);

    // 以点分十进制形式打印地址
    printf("IP 地址以点分十进制形式：%s\n", inet_ntoa(my_addr));

    // uint32_t ip_num = ntohl(my_addr.s_addr);  // 转换为主机字节序
    uint32_t ip_num = my_addr.s_addr;  // 大端序
    // 现在可以直接访问字节
    uint8_t byte1 = (ip_num >> 24) & 0xFF;
    uint8_t byte2 = (ip_num >> 16) & 0xFF;
    uint8_t byte3 = (ip_num >> 8) & 0xFF;
    uint8_t byte4 = ip_num & 0xFF;

    printf("IP 数字的字节表示：%u.%u.%u.%u\n", byte1, byte2, byte3, byte4);

    return 0;
}
