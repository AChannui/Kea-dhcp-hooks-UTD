#include <hooks/hooks.h>
#include <dhcpsrv/lease.h>
#include <dhcpsrv/subnet.h>
#include <dhcp/pkt6.h>
#include "mac2ip.h"
#include "hook.h"
#include "logger.h"

using namespace isc::dhcp;
using namespace isc::hooks;
using namespace isc::asiolink;

extern "C" {

int lease6_select(CalloutHandle& handle)
{
    Pkt6Ptr query;
    Lease6Ptr lease;
    Subnet6Ptr subnet;
    bool fake_alloc;

    handle.getArgument("query6", query);

    // check client classes
    if (!hook_mac2ipv6_client_classes.empty())
    {
        bool is_in = false;

        for (auto it = hook_mac2ipv6_client_classes.begin(); it != hook_mac2ipv6_client_classes.end(); ++it)
        {
            is_in |= query->inClass(*it);
        }

        if (is_in == false)
        {
            // hook was configured for another class -> skip it
            return 0;
        }
    }


    handle.getArgument("lease6", lease);

    // set ip address from mac address
    lease->addr_=IOAddress(mac2ipv6(lease->hwaddr_->hwaddr_, query->getRelay6LinkAddress(0).toBytes()));

#ifndef NO_FAKE_ALLOCATION
    handle.setArgument("fake_allocation", true);
#endif

    LOG_DEBUG(hook_mac2ipv6::logger, 1, "Selected address '%1' from mac address '%2' and link address '%3'").arg(lease->addr_).arg(lease->hwaddr_->toText()).arg(query->getRelay6LinkAddress(0));

    return (0);
};

}
