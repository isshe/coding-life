

#include <linux/kmod.h>
#include <linux/module.h>
#include <linux/version.h>
#include <linux/skbuff.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <net/netfilter/nf_conntrack.h>

#define DRV_VERSION "1.0.0"
#define DRV_DESC    "Collect host info"

struct dhcphdr {
    __u8    opcode;         // operation code
    __u8    hw_type;        // hardware type
    __u8    hw_addr_len;    // hardware address length
    __u8    hops;           // hop count
    __be32  tid;            // transaction ID
    __be16  secs;           // number of seconds
    __be16  flags;          // flags
    __be32  ciaddr;         // client IP address
    __be32  yiaddr;         // your IP address
    __be32  siaddr;         // server IP address
    __be32  giaddr;         // gateway IP address
    __u8    chw_addr[16];   // client hardware address
    __u8    shname[64];     // server host name
    __u8    bfname[128];    // boot filename
    __u8    opmc[4];        // magic cookie in option
    __u8    options[0];     // dhcp options
};


static unsigned char *skb_application_header(const struct sk_buff *skb)
{
    return skb->head + skb->transport_header + sizeof(struct udphdr);
}

static struct dhcphdr *dhcp_hdr(const struct sk_buff *skb)
{
    return (struct dhcphdr *)skb_application_header(skb);
}

static void print_dhcp_options(const unsigned char *options)
{
    char buf[512];
    const unsigned char *p = options;
    int i = 0;

    //p = options;
    while (p && *p != 255 && i < 20) {
        printk("option %d, length %d:\n", *p, *(p + 1));
        if (*p == 12) {
            memset(buf, 0, sizeof(buf));
            memcpy(buf, p + 2, *(p + 1));
            printk("----%s\n", buf);
        } else if (*p == 53) {
            printk("----%d\n", *(p + 2));
        } else if (*p == 55) {
            for (i = 0; i < *(p + 1); i++) {
                printk("----0x%02x(%d)\n", *(p + 2 + i), *(p + 2 + i));
            }

        } else if (*p == 60) {
            memset(buf, 0, sizeof(buf));
            memcpy(buf, p + 2, *(p + 1));
            printk("----%s\n", buf);
        }

        p += 2 + *(p + 1);
        i += 1;
    }
}

static void print_dhcp_header(const struct dhcphdr *hdr)
{
    printk("opcode:%d\n", hdr->opcode);
    printk("hw_type: %d\n", hdr->hw_type);
    printk("hw_addr_len: %d\n", hdr->hw_addr_len);
    printk("hops: %d\n", hdr->hops);
    printk("tid: %d, to_cpu(tid): %d\n", hdr->tid, be32_to_cpu(hdr->tid));
    printk("secs: %d\n", hdr->secs);
    printk("flags: %d\n", hdr->flags);
    printk("ciaddr: %pI4\n", &hdr->ciaddr);
    printk("yiaddr: %pI4\n", &hdr->yiaddr);
    printk("siaddr: %pI4\n", &hdr->siaddr);
    printk("giaddr: %pI4\n", &hdr->giaddr);
    printk("chw_addr: %02x:%02x:%02x:%02x:%02x:%02x\n", hdr->chw_addr[0], hdr->chw_addr[1], hdr->chw_addr[2], hdr->chw_addr[3], hdr->chw_addr[4], hdr->chw_addr[5]);
    printk("shname: %s\n", hdr->shname);
    printk("bfname: %s\n", hdr->bfname);
    printk("opmc: %02x%02x%02x%02x\n", hdr->opmc[0], hdr->opmc[1], hdr->opmc[2], hdr->opmc[3]);

    print_dhcp_options((const unsigned char*)hdr->options);
}

static unsigned int isshe_hic_nf_hook(void *priv,
        struct sk_buff *skb,
        const struct nf_hook_state *state)
{
    //struct net_device *in = state->in;
    //struct net_device *out = state->out;
    //struct udphdr *udp_header;
    struct dhcphdr *dhcp_header;

    if (0 == skb->len - skb->data_len) {
        //printk("---isshe---: data len == 0, len = %d\n", skb->len);
        return NF_ACCEPT;
    }

    // udp only
    if (ip_hdr(skb)->protocol != IPPROTO_UDP) {
        //printk("---isshe---: not udp\n");
        return NF_ACCEPT;
    }

    //udp_header = udp_hdr(skb);
    // dhcp client->server only
    if (be16_to_cpu(udp_hdr(skb)->source) != 68 && be16_to_cpu(udp_hdr(skb)->dest) != 67
        && be16_to_cpu(udp_hdr(skb)->source) != 67 && be16_to_cpu(udp_hdr(skb)->dest) != 68) {
        //printk("---isshe---: not 68/67, src = %d, dst = %d\n", be16_to_cpu(udp_hdr(skb)->source), be16_to_cpu(udp_hdr(skb)->dest));
        return NF_ACCEPT;
    }

    printk("-----------------isshe-----------------\n");

    dhcp_header = dhcp_hdr(skb);

    if ((((unsigned char *)dhcp_header) + sizeof(struct dhcphdr)) > (skb->data + skb->len - skb->data_len)) {
        printk("---isshe---: too small???\n");
        return NF_ACCEPT;
    }

    print_dhcp_header(dhcp_header);

    return NF_ACCEPT;
}


static struct nf_hook_ops isshe_hic_nf_hook_ops[] =
{
    {
        .hook = isshe_hic_nf_hook,
        .pf = NFPROTO_IPV4,
        .hooknum = NF_INET_PRE_ROUTING,         // ubuntu 自测修改 NF_INET_PRE_ROUTING
        .priority = NF_IP_PRI_FIRST,            // ---优先级
    },

};

/**
 * module init
 */
static int __init isshe_hic_init(void)
{
    int ret = 0;

    //need_conntrack();

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,13,0)
    ret = nf_register_net_hooks(&init_net, isshe_hic_nf_hook_ops, ARRAY_SIZE(isshe_hic_nf_hook_ops));
#else
    ret = nf_register_hooks(isshe_hic_nf_hook_ops, ARRAY_SIZE(isshe_hic_nf_hook_ops));
#endif
    if (ret != 0) {
        printk("nf_register_hook failed: %d\n", ret);
        return ret;
    }
    printk("isshe hic init OK\n");

    return 0;
}

/**
 * module uninit
 */
static void __exit isshe_hic_fini(void)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,13,0)
    nf_unregister_net_hooks(&init_net, isshe_hic_nf_hook_ops, ARRAY_SIZE(isshe_hic_nf_hook_ops));
#else
    nf_unregister_hooks(isshe_hic_nf_hook_ops, ARRAY_SIZE(isshe_hic_nf_hook_ops));
#endif

    printk("isshe hic exit OK\n");
}

module_init(isshe_hic_init);
module_exit(isshe_hic_fini);

MODULE_DESCRIPTION(DRV_DESC);
MODULE_VERSION(DRV_VERSION);
MODULE_AUTHOR("isshe <i.sshe@foxmail.com>");
MODULE_LICENSE("GPL v2");
