/*
 * Copyright (c) 2013 Patrick Kelsey. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */


#ifndef	_UINET_API_TYPES_H_
#define	_UINET_API_TYPES_H_

#define UINET_IF_NAMESIZE	16

struct uinet_socket;

typedef void * uinet_api_synfilter_cookie_t;

typedef int (*uinet_api_synfilter_callback_t)(struct uinet_socket *, void *, uinet_api_synfilter_cookie_t);

struct uinet_api_synfilter_ctx {
	uinet_api_synfilter_callback_t callback;
	void *arg;
};

typedef uint8_t uinet_sa_family_t;

struct uinet_sockaddr {
	unsigned char		sa_len;		/* total length */
	uinet_sa_family_t	sa_family;	/* address family */
	char			sa_data[14];	/* actually longer; address value */
};

typedef	uint16_t uinet_in_port_t;

typedef	uint32_t uinet_in_addr_t;

struct uinet_in_addr {
	uinet_in_addr_t s_addr;
};

struct uinet_in6_addr {
	union {
		uint8_t		__u6_addr8[16];
		uint16_t	__u6_addr16[8];
		uint32_t	__u6_addr32[4];
	} __u6_addr;			/* 128-bit IP6 address */
};

#define uinet_s6_addr   __u6_addr.__u6_addr8

struct uinet_sockaddr_in {
	uint8_t	sin_len;
	uinet_sa_family_t	sin_family;
	uinet_in_port_t		sin_port;
	struct	uinet_in_addr	sin_addr;
	char	sin_zero[8];
};

struct uinet_in_addr_4in6 {
	uint32_t		ia46_pad32[3];
	struct	uinet_in_addr	ia46_addr4;
};

/*
 * NOTE: ipv6 addrs should be 64-bit aligned, per RFC 2553.  in_conninfo has
 * some extra padding to accomplish this.
 */
struct uinet_in_endpoints {
	uint16_t	ie_fport;		/* foreign port */
	uint16_t	ie_lport;		/* local port */
	/* protocol dependent part, local and foreign addr */
	union {
		/* foreign host table entry */
		struct	uinet_in_addr_4in6 ie46_foreign;
		struct	uinet_in6_addr ie6_foreign;
	} ie_dependfaddr;
	union {
		/* local host table entry */
		struct	uinet_in_addr_4in6 ie46_local;
		struct	uinet_in6_addr ie6_local;
	} ie_dependladdr;
};
#define	ie_faddr	ie_dependfaddr.ie46_foreign.ia46_addr4
#define	ie_laddr	ie_dependladdr.ie46_local.ia46_addr4
#define	ie6_faddr	ie_dependfaddr.ie6_foreign
#define	ie6_laddr	ie_dependladdr.ie6_local

struct uinet_in_conninfo {
	uint8_t	inc_flags;
	uint8_t	inc_len;
	uint16_t	inc_fibnum;	/* XXX was pad, 16 bits is plenty */
	/* protocol dependent part */
	struct	uinet_in_endpoints inc_ie;
};


#define UINET_IOV_MAX	1024

struct uinet_iovec {
	void	*iov_base;	/* Base address. */
	uint64_t iov_len;	/* Length. */
};

struct uinet_uio {
	struct uinet_iovec *uio_iov;	/* scatter/gather list */
	int	uio_iovcnt;		/* length of scatter/gather list */
	int64_t	uio_offset;		/* offset in target object */
	int64_t	uio_resid;		/* remaining bytes to process */
};


#define	UINET_SS_ISCONNECTED		0x0002	/* socket connected to a peer */
#define	UINET_SS_ISCONNECTING		0x0004	/* in process of connecting to peer */
#define	UINET_SS_ISDISCONNECTING	0x0008	/* in process of disconnecting */
#define	UINET_SS_NBIO			0x0100	/* non-blocking ops */
#define	UINET_SS_ISCONFIRMING		0x0400	/* deciding to accept connection req */
#define	UINET_SS_ISDISCONNECTED		0x2000	/* socket disconnected from peer */


/* 'which' values for socket upcalls. */
#define	UINET_SO_RCV		1
#define	UINET_SO_SND		2

/* Return values for socket upcalls. */
#define	UINET_SU_OK		0
#define	UINET_SU_ISCONNECTED	1


#define	UINET_SOCK_STREAM	1	/* stream socket */
#define	UINET_SOCK_DGRAM	2	/* datagram socket */


#define	UINET_SO_DEBUG		0x00000001	/* turn on debugging info recording */
#define	UINET_SO_ACCEPTCONN	0x00000002	/* socket has had listen() */
#define	UINET_SO_REUSEADDR	0x00000004	/* allow local address reuse */
#define	UINET_SO_KEEPALIVE	0x00000008	/* keep connections alive */
#define	UINET_SO_DONTROUTE	0x00000010	/* just use interface addresses */
#define	UINET_SO_BROADCAST	0x00000020	/* permit sending of broadcast msgs */
#define	UINET_SO_LINGER		0x00000080	/* linger on close if data present */
#define	UINET_SO_OOBINLINE	0x00000100	/* leave received OOB data in line */
#define	UINET_SO_REUSEPORT	0x00000200	/* allow local address & port reuse */
#define	UINET_SO_TIMESTAMP	0x00000400	/* timestamp received dgram traffic */
#define	UINET_SO_NOSIGPIPE	0x00000800	/* no SIGPIPE from EPIPE */
#define	UINET_SO_ACCEPTFILTER	0x00001000	/* there is an accept filter */
#define	UINET_SO_BINTIME	0x00002000	/* timestamp received dgram traffic */
#define	UINET_SO_NO_OFFLOAD	0x00004000	/* socket cannot be offloaded */
#define	UINET_SO_NO_DDP		0x00008000	/* disable direct data placement */
#define	UINET_SO_PROMISC	0x00010000	/* socket will be used for promiscuous listen */


#define	UINET_SO_SNDBUF		0x1001		/* send buffer size */
#define	UINET_SO_RCVBUF		0x1002		/* receive buffer size */
#define	UINET_SO_SNDLOWAT	0x1003		/* send low-water mark */
#define	UINET_SO_RCVLOWAT	0x1004		/* receive low-water mark */
#define	UINET_SO_SNDTIMEO	0x1005		/* send timeout */
#define	UINET_SO_RCVTIMEO	0x1006		/* receive timeout */
#define	UINET_SO_ERROR		0x1007		/* get error status and clear */
#define	UINET_SO_TYPE		0x1008		/* get socket type */
#define	UINET_SO_LABEL		0x1009		/* socket's MAC label */
#define	UINET_SO_PEERLABEL	0x1010		/* socket's peer's MAC label */
#define	UINET_SO_LISTENQLIMIT	0x1011		/* socket's backlog limit */
#define	UINET_SO_LISTENQLEN	0x1012		/* socket's complete queue length */
#define	UINET_SO_LISTENINCQLEN	0x1013		/* socket's incomplete queue length */
#define	UINET_SO_SETFIB		0x1014		/* use this FIB to route */
#define	UINET_SO_USER_COOKIE	0x1015		/* user cookie (dummynet etc.) */
#define	UINET_SO_PROTOCOL	0x1016		/* get socket protocol (Linux name) */
#define	UINET_SO_PROTOTYPE	UINET_SO_PROTOCOL	/* alias for UINET_SO_PROTOCOL (SunOS name) */
#define UINET_SO_L2INFO		0x1017		/* PROMISCUOUS_INET MAC addrs and tags */


#define	UINET_SOL_SOCKET	0xffff	/* options for socket level */

#define	UINET_AF_UNSPEC		0		/* unspecified */
#define	UINET_AF_INET		2		/* internetwork: UDP, TCP, etc. */
#define	UINET_AF_INET6		28		/* IPv6 */

#define	UINET_PF_UNSPEC		UINET_AF_UNSPEC
#define	UINET_PF_INET		UINET_AF_INET
#define	UINET_PF_INET6		UINET_AF_INET6


#define	UINET_MSG_DONTWAIT	0x80		/* this message should be nonblocking */
#define	UINET_MSG_NBIO		0x4000		/* FIONBIO mode, used by fifofs */
#define	UINET_MSG_HOLE_BREAK	0x40000		/* break at and indicate hole boundary */

#define	UINET_SHUT_RD		0		/* shut down the reading side */
#define	UINET_SHUT_WR		1		/* shut down the writing side */
#define	UINET_SHUT_RDWR		2		/* shut down both sides */


#define	UINET_IPPROTO_ICMP	1	/* control message protocol */
#define	UINET_IPPROTO_TCP	6	/* tcp */
#define	UINET_IPPROTO_UDP	17	/* user datagram protocol */

#define	UINET_INADDR_ANY		(uint32_t)0x00000000
#define UINET_IN_PROMISC_PORT_ANY	0

/*
 * Options for use with [gs]etsockopt at the IP level.
 * First word of comment is data type; bool is stored in int.
 */
#define	UINET_IP_BINDANY	24   /* bool: allow bind to any address */



#define	UINET_TCP_NODELAY	0x01	/* don't delay send to coalesce packets */
#define	UINET_TCP_MAXSEG	0x02	/* set maximum segment size */
#define UINET_TCP_NOPUSH	0x04	/* don't push last block of write */
#define UINET_TCP_NOOPT		0x08	/* don't use TCP options */
#define UINET_TCP_MD5SIG	0x10	/* use MD5 digests (RFC2385) */
#define	UINET_TCP_INFO		0x20	/* retrieve tcp_info structure */
#define	UINET_TCP_CONGESTION	0x40	/* get/set congestion control algorithm */
#define	UINET_TCP_KEEPINIT	0x80	/* N, time to establish connection */
#define	UINET_TCP_KEEPIDLE	0x100	/* L,N,X start keepalives after this period */
#define	UINET_TCP_KEEPINTVL	0x200	/* L,N interval between keepalives */
#define	UINET_TCP_KEEPCNT	0x400	/* L,N number of keepalives before close */
#define UINET_TCP_REASSDL	0x800	/* wait this long for missing segments */

struct uinet_tcp_info {
	uint8_t		tcpi_state;		/* TCP FSM state. */
	uint8_t		__tcpi_ca_state;
	uint8_t		__tcpi_retransmits;
	uint8_t		__tcpi_probes;
	uint8_t		__tcpi_backoff;
	uint8_t		tcpi_options;		/* Options enabled on conn. */
	uint8_t		tcpi_snd_wscale:4,	/* RFC1323 send shift value. */
			tcpi_rcv_wscale:4;	/* RFC1323 recv shift value. */

	uint32_t	tcpi_rto;		/* Retransmission timeout (usec). */
	uint32_t	__tcpi_ato;
	uint32_t	tcpi_snd_mss;		/* Max segment size for send. */
	uint32_t	tcpi_rcv_mss;		/* Max segment size for receive. */

	uint32_t	__tcpi_unacked;
	uint32_t	__tcpi_sacked;
	uint32_t	__tcpi_lost;
	uint32_t	__tcpi_retrans;
	uint32_t	__tcpi_fackets;

	/* Times; measurements in usecs. */
	uint32_t	__tcpi_last_data_sent;
	uint32_t	__tcpi_last_ack_sent;	/* Also unimpl. on Linux? */
	uint32_t	tcpi_last_data_recv;	/* Time since last recv data. */
	uint32_t	__tcpi_last_ack_recv;

	/* Metrics; variable units. */
	uint32_t	__tcpi_pmtu;
	uint32_t	__tcpi_rcv_ssthresh;
	uint32_t	tcpi_rtt;		/* Smoothed RTT in usecs. */
	uint32_t	tcpi_rttvar;		/* RTT variance in usecs. */
	uint32_t	tcpi_snd_ssthresh;	/* Slow start threshold. */
	uint32_t	tcpi_snd_cwnd;		/* Send congestion window. */
	uint32_t	__tcpi_advmss;
	uint32_t	__tcpi_reordering;

	uint32_t	__tcpi_rcv_rtt;
	uint32_t	tcpi_rcv_space;		/* Advertised recv window. */

	/* FreeBSD extensions to tcp_info. */
	uint32_t	tcpi_snd_wnd;		/* Advertised send window. */
	uint32_t	tcpi_snd_bwnd;		/* No longer used. */
	uint32_t	tcpi_snd_nxt;		/* Next egress seqno */
	uint32_t	tcpi_rcv_nxt;		/* Next ingress seqno */
	uint32_t	tcpi_toe_tid;		/* HWTID for TOE endpoints */
	uint32_t	tcpi_snd_rexmitpack;	/* Retransmitted packets */
	uint32_t	tcpi_rcv_ooopack;	/* Out-of-order packets */
	uint32_t	tcpi_snd_zerowin;	/* Zero-sized windows sent */
	
	/* Padding to grow without breaking ABI. */
	uint32_t	__tcpi_pad[26];		/* Padding. */
};


#define UINET_IN_L2INFO_MAX_TAGS	16
#define UINET_IN_L2INFO_ADDR_MAX	6

struct uinet_in_l2tagstack {
	uint16_t inl2t_cnt;
	uint32_t inl2t_masks[UINET_IN_L2INFO_MAX_TAGS];
	uint32_t inl2t_tags[UINET_IN_L2INFO_MAX_TAGS]; /* in network byte order */
};


#define UINET_INL2I_TAG_ANY		0x01

struct uinet_in_l2info {
	uint8_t inl2i_local_addr[UINET_IN_L2INFO_ADDR_MAX];
	uint8_t inl2i_foreign_addr[UINET_IN_L2INFO_ADDR_MAX];
	uint16_t inl2i_flags;
	struct uinet_in_l2tagstack inl2i_tagstack;
};


#define UINET_SYNF_ACCEPT		0	/* Process SYN normally */
#define UINET_SYNF_REJECT_RST		1	/* Discard SYN, send RST */
#define UINET_SYNF_REJECT_SILENT	2	/* Discard SYN silently*/
#define UINET_SYNF_DEFER		3	/* Decision will be returned later via setsockopt() */

typedef void * uinet_synf_deferral_t;




struct uinet_pool;

typedef struct uinet_pool * uinet_pool_t;

typedef int (*uinet_pool_ctor)(void *mem, int size, void *arg, int flags);
typedef void (*uinet_pool_dtor)(void *mem, int size, void *arg);
typedef int (*uinet_pool_init)(void *mem, int size, int flags);
typedef void (*uinet_pool_fini)(void *mem, int size);

/*
 * Definitions for uinet_pool_create flags
 */
#define UINET_POOL_ZINIT	0x0002	/* Initialize with zeros */
#define UINET_POOL_NOFREE	0x0020	/* Do not free slabs of this type! */
#define	UINET_POOL_MAXBUCKET	0x0800	/* Use largest buckets */

/* Definitions for align */
#define UINET_POOL_ALIGN_PTR	(sizeof(void *) - 1)	/* Alignment fit for ptr */
#define UINET_POOL_ALIGN_LONG	(sizeof(long) - 1)	/* "" long */
#define UINET_POOL_ALIGN_INT	(sizeof(int) - 1)	/* "" int */
#define UINET_POOL_ALIGN_SHORT	(sizeof(short) - 1)	/* "" short */
#define UINET_POOL_ALIGN_CHAR	(sizeof(char) - 1)	/* "" char */
#define UINET_POOL_ALIGN_CACHE	(0 - 1)			/* Cache line size align */

#define	UINET_POOL_ALLOC_NOWAIT	0x0001		/* do not block */
#define	UINET_POOL_ALLOC_WAITOK	0x0002		/* ok to block */
#define	UINET_POOL_ALLOC_ZERO	0x0100		/* bzero the allocation */

#endif /* _UINET_API_TYPES_H_ */
