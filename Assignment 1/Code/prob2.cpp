/*
 * Name - Ayush Agarwal
 * ENo. - 17114017
 */
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/if.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>

int main()
{
  struct ifreq s;
  int fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);

  strcpy(s.ifr_name, "wifi0");
  if (0 == ioctl(fd, SIOCGIFHWADDR, &s))
  {
    int i;
    unsigned char *hwaddr = (unsigned char *)s.ifr_hwaddr.sa_data;
    printf("Mac Address: %02X:%02X:%02X:%02X:%02X:%02X\n",
           hwaddr[0], hwaddr[1], hwaddr[2],
           hwaddr[3], hwaddr[4], hwaddr[5]);
    return 0;
  }

  return 1;
}