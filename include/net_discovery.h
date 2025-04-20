#pragma once

#include <linux/rtnetlink.h>
#include <net/if.h>

// Structure to hold interface information
typedef struct
{
    char name[32];
    int index;
    int type;   // ARPHRD_ETHER for Ethernet, ARPHRD_IEEE80211 for WiFi
    int is_up;
    char mac_address[18];
} interface_info_t;

// Function to discover all network interfaces
int discover_interfaces(interface_info_t **interfaces, int *count);

// Function to free the interface list
void free_interface_list(interface_info_t *interfaces);
