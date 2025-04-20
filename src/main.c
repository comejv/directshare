#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "net_discovery.h"

#define __FILENAME__ "main.c"

int main()
{
    interface_info_t *interfaces = NULL;
    int count = 0;
    int i;

    printf("Discovering network interfaces...\n");

    if (discover_interfaces(&interfaces, &count) != 0)
    {
        ERROR("Failed to discover interfaces");
        return 1;
    }

    printf("Found %d network interfaces:\n", count);

    for (i = 0; i < count; i++)
    {
        printf("Interface #%d:\n", i + 1);
        printf("  Name: %s\n", interfaces[i].name);
        printf("  Index: %d\n", interfaces[i].index);
        printf("  Type: %d ", interfaces[i].type);

        // Print human-readable type
        if (interfaces[i].type == 1)
        {   // ARPHRD_ETHER
            printf("(Ethernet)\n");
        }
        else if (interfaces[i].type == 801)
        {   // ARPHRD_IEEE80211
            printf("(WiFi)\n");
        }
        else
        {
            printf("(Other)\n");
        }

        printf("  Status: %s\n", interfaces[i].is_up ? "UP" : "DOWN");
        printf("  MAC: %s\n", interfaces[i].mac_address);
        printf("\n");
    }

    free_interface_list(interfaces);
    return EXIT_SUCCESS;
}
