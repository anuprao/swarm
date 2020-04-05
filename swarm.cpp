extern "C" 
{ 
	#include <lwip/sys.h>
	#include <lwip/timeouts.h>
	#include <lwip/debug.h>
	#include <lwip/stats.h>
	#include <lwip/init.h>
	#include <lwip/tcpip.h>
	#include <lwip/netif.h>
	#include <lwip/ip_addr.h>
	#include <lwip/api.h>
	#include <lwip/sockets.h>
	#include <lwip/tcpip.h>
	#include <lwip/etharp.h>
	#include <netif/ethernet.h>
	#include <netif/tapif.h>
	
	#include <httpserver-netconn.h>	
}

#ifndef LWIP_EXAMPLE_APP_ABORT
#define LWIP_EXAMPLE_APP_ABORT() 0
#endif

// Define this to 1 to enable a port-specific ethernet interface as default interface. 
#ifndef USE_DEFAULT_ETH_NETIF
#define USE_DEFAULT_ETH_NETIF 1
#endif

// Use an ethernet adapter? Default to enabled if port-specific ethernet netif or PPPoE are used. 
#ifndef USE_ETHERNET
#define USE_ETHERNET  (USE_DEFAULT_ETH_NETIF || PPPOE_SUPPORT)
#endif

// Use an ethernet adapter for TCP/IP? By default only if port-specific ethernet netif is used. 
#ifndef USE_ETHERNET_TCPIP
#define USE_ETHERNET_TCPIP  (USE_DEFAULT_ETH_NETIF)
#endif

#define NETIF_MAX 64

typedef struct _stTapIf
{
	struct eth_addr *ethaddr;
	
	int fd;
	
	char *name;
	
	ip_addr_t ipaddr;
	ip_addr_t netmask;
	ip_addr_t gw;
}stTapIf;

stTapIf arrTapIfs[NETIF_MAX];
struct netif arrNetIfs[NETIF_MAX];
int n = 0;

void default_netif_poll(void)
{
  tapif_poll(&arrNetIfs[n]);
}

// This function initializes this lwIP test. When NO_SYS=1, this is done in
// the main_loop context (there is no other one), when NO_SYS=0, this is done
// in the tcpip_thread context 
static void swarm_init(void * arg)
{
	// remove compiler warning 
#if NO_SYS
	LWIP_UNUSED_ARG(arg);
#else // NO_SYS 
	sys_sem_t *init_sem;
	LWIP_ASSERT("arg != NULL", arg != NULL);
	init_sem = (sys_sem_t*)arg;
#endif // NO_SYS 
	
	memset(arrTapIfs,0,sizeof(arrTapIfs));
	memset(arrNetIfs,0,sizeof(arrNetIfs));
	
	//
	
	IP4_ADDR(&arrTapIfs[n].ipaddr,	172,	16,		0,		2);
	IP4_ADDR(&arrTapIfs[n].netmask,	255,	255,	255,	0);
	IP4_ADDR(&arrTapIfs[n].gw,		172,	16,		0,		1);

	//
		
	// init randomizer again (seed per thread) 
	//srand((unsigned int)time(0));

	// init network interfaces 
	//test_netif_init();
	
	netif_add(&arrNetIfs[n],
		&arrTapIfs[n].ipaddr,
		&arrTapIfs[n].netmask,
		&arrTapIfs[n].gw,
		&arrTapIfs[n],
		tapif_init,
		tcpip_input);

	// init apps 
	//apps_init();
	
	if (n == 0)
	{
		netif_set_default(&arrNetIfs[n]);
	}
	
	netif_set_up(&arrNetIfs[n]);
	
	/*
	//
	//if (IP4_OR_NULL(arrTapIfs[n].ipaddr) == 0 &&
	//	IP4_OR_NULL(arrTapIfs[n].netmask) == 0 &&
	//	IP4_OR_NULL(arrTapIfs[n].gw) == 0)
	//{
	//	dhcp_start(&arrNetIfs[n]);
	//}
	//
	*/
	http_server_netconn_init();
	

#if !NO_SYS
	sys_sem_signal(init_sem);
#endif // !NO_SYS 
}

int main(void)
{
#if !NO_SYS
	err_t err;
	sys_sem_t init_sem;
#endif // NO_SYS 

	// initialize lwIP stack, network interfaces and applications 
	#if NO_SYS
		lwip_init();
		swarm_init(NULL);
		
	#else // NO_SYS 
		err = sys_sem_new(&init_sem, 0);
		
		LWIP_ASSERT("failed to create init_sem", err == ERR_OK);
		LWIP_UNUSED_ARG(err);
		
		tcpip_init(swarm_init, &init_sem);
		
		// we have to wait for initialization to finish before calling update_adapter()
		sys_sem_wait(&init_sem);
		sys_sem_free(&init_sem);
		
	#endif // NO_SYS 

	#if (LWIP_SOCKET || LWIP_NETCONN) && LWIP_NETCONN_SEM_PER_THREAD
		netconn_thread_init();
	#endif

  /* MAIN LOOP for driver update (and timers if NO_SYS) */
  while (!LWIP_EXAMPLE_APP_ABORT()) 
  {
	#if NO_SYS
		/* handle timers (already done in tcpip.c when NO_SYS=0) */
		sys_check_timeouts();
	#endif /* NO_SYS */

	#if USE_ETHERNET
		default_netif_poll();
	#endif

	#if ENABLE_LOOPBACK && !LWIP_NETIF_LOOPBACK_MULTITHREADING
		// check for loopback packets on all netifs
		netif_poll_all();
	#endif 

  }
  
#if (LWIP_SOCKET || LWIP_NETCONN) && LWIP_NETCONN_SEM_PER_THREAD
	netconn_thread_cleanup();
#endif
 
	return 0;
} 
