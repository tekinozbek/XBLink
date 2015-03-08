/* 
 * tun_alloc was modified from the interface example that can be found at
 * <https://www.kernel.org/doc/Documentation/networking/tuntap.txt>
 * 
 * tun_alloc_old() apparently does not exist (or no one can find it), so it was
 * replaced by an error value.
 */

#include <linux/if_tun.h>
#include <net/if.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <cstring>
#include <unistd.h>

#include <tunnel.h>

int tun_alloc(char *dev) {
    
    struct ifreq ifr;
    int fd, err;

    if((fd = open("/dev/net/tun", O_RDWR)) < 0)
     return -1;

    memset(&ifr, 0, sizeof(ifr));
    
    // set as TUN device, no packet information
    ifr.ifr_flags = IFF_TUN | IFF_NO_PI; 
    
    if (*dev)
        strncpy(ifr.ifr_name, dev, IFNAMSIZ);

    if ((err = ioctl(fd, TUNSETIFF, (void *) &ifr)) < 0) {
        
        close(fd);
        return -2;
    }

    return fd;
}  
