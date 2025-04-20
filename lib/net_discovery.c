#include "net_discovery.h"

#include <arpa/inet.h>
#include <libmnl/libmnl.h>
#include <linux/if_arp.h>
#include <linux/if_link.h>
#include <net/if.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "log.h"

#define __FILENAME__ "net_discovery.c"

// Callback data structure
struct netlink_data
{
    interface_info_t *interfaces;
    int count;
    int max_count;
};

// Callback function for parsing link information
static int data_cb(const struct nlmsghdr *nlh, void *data)
{
    struct ifinfomsg *ifm = mnl_nlmsg_get_payload(nlh);
    struct nlattr *attr;
    struct netlink_data *net_data = (struct netlink_data *) data;

    if (net_data->count >= net_data->max_count)
        return MNL_CB_OK;

    interface_info_t *iface = &net_data->interfaces[net_data->count];

    // Get interface index
    iface->index = ifm->ifi_index;

    // Get interface type
    iface->type = ifm->ifi_type;

    // Check if interface is up
    iface->is_up = (ifm->ifi_flags & IFF_UP) ? 1 : 0;

    // Parse attributes
    mnl_attr_for_each(attr, nlh, sizeof(*ifm))
    {
        int type = mnl_attr_get_type(attr);

        if (mnl_attr_type_valid(attr, IFLA_MAX) < 0)
            continue;

        switch (type)
        {
        case IFLA_IFNAME:
            if (mnl_attr_validate(attr, MNL_TYPE_STRING) < 0)
            {
                ERROR("Invalid IFLA_IFNAME attribute");
                break;
            }
            strncpy(iface->name, mnl_attr_get_str(attr), 31);
            iface->name[31] = '\0';
            break;

        case IFLA_ADDRESS:
            if (mnl_attr_validate(attr, MNL_TYPE_BINARY) < 0)
            {
                ERROR("Invalid IFLA_ADDRESS attribute");
                break;
            }
            uint8_t *hwaddr = mnl_attr_get_payload(attr);
            int hwaddr_len = mnl_attr_get_payload_len(attr);

            // Convert MAC address to string format
            if (hwaddr_len == 6)
            {   // Standard Ethernet MAC address
                snprintf(iface->mac_address, sizeof(iface->mac_address),
                         "%02x:%02x:%02x:%02x:%02x:%02x",
                         hwaddr[0], hwaddr[1], hwaddr[2],
                         hwaddr[3], hwaddr[4], hwaddr[5]);
            }
            else
            {
                strcpy(iface->mac_address, "N/A");
            }
            break;
        }
    }

    net_data->count++;
    return MNL_CB_OK;
}

int discover_interfaces(interface_info_t **interfaces, int *count)
{
    char buf[MNL_SOCKET_BUFFER_SIZE];
    struct nlmsghdr *nlh;
    struct rtgenmsg *rt;
    struct mnl_socket *nl;
    int ret;
    unsigned int seq, portid;

    DEBUG(INTF_DSC, "Starting interface discovery...");

    nl = mnl_socket_open(NETLINK_ROUTE);
    if (nl == NULL)
    {
        ERROR("Could not open netlink socket");
        return -1;
    }

    if (mnl_socket_bind(nl, 0, MNL_SOCKET_AUTOPID) < 0)
    {
        ERROR("Could not bind netlink socket");
        mnl_socket_close(nl);
        return -1;
    }

    portid = mnl_socket_get_portid(nl);

    // Create netlink message to request interface list
    nlh = mnl_nlmsg_put_header(buf);
    nlh->nlmsg_type = RTM_GETLINK;
    nlh->nlmsg_flags = NLM_F_REQUEST | NLM_F_DUMP;
    nlh->nlmsg_seq = seq = time(NULL);
    rt = mnl_nlmsg_put_extra_header(nlh, sizeof(struct rtgenmsg));
    rt->rtgen_family = AF_PACKET;

    if (mnl_socket_sendto(nl, nlh, nlh->nlmsg_len) < 0)
    {
        ERROR("Could not send netlink request");
        mnl_socket_close(nl);
        return -1;
    }

    // Allocate temporary space for interfaces
    interface_info_t temp_interfaces[128];   // Adjust size as needed
    struct netlink_data net_data = {
        .interfaces = temp_interfaces,
        .count = 0,
        .max_count = 128};

    // Receive and parse the response
    ret = mnl_socket_recvfrom(nl, buf, sizeof(buf));
    while (ret > 0)
    {
        ret = mnl_cb_run(buf, ret, seq, portid, data_cb, &net_data);
        if (ret <= MNL_CB_STOP)
            break;
        ret = mnl_socket_recvfrom(nl, buf, sizeof(buf));
    }

    if (ret == -1)
    {
        ERROR("Error receiving netlink data");
        mnl_socket_close(nl);
        return -1;
    }

    mnl_socket_close(nl);

    // Allocate and copy the results
    *count = net_data.count;
    if (*count > 0)
    {
        *interfaces = (interface_info_t *) malloc(sizeof(interface_info_t) * (*count));
        if (!*interfaces)
        {
            ERROR("Failed to allocate memory for interfaces");
            return -1;
        }
        memcpy(*interfaces, temp_interfaces, sizeof(interface_info_t) * (*count));
    }
    else
    {
        *interfaces = NULL;
    }

    DEBUG(INTF_DSC, "Found %d interfaces.", *count);

    return 0;
}

void free_interface_list(interface_info_t *interfaces)
{
    free(interfaces);
}
