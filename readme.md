
todo
	- lwip_init
	- lwip_close


# How to build

# Build Linux/POSIX library lwip.so

Alter Filelists.cmake in /mnt/workspace/community/x86_64/lwip/src

Comment out 

	`    ${LWIP_DIR}/src/netif/slipif.c`
	
It should look like

	`#    ${LWIP_DIR}/src/netif/slipif.c`

Start Build

	$ cd /mnt/workspace/community/x86_64/lwip-contrib/ports/unix/lib

	$ mkdir build

	$ cd build

	$ cmake ..

	$ cmake --build .
	or
	$ make

# Build the app

	$ mkdir build

	$ cd build

	$ cmake ..

	$ cmake --build .
	or
	$ make


# How to setup the TAP interface

## QuickStart

$ sudo ip tuntap add dev tap0 mode tap
$ sudo ip link set dev tap0 up
$ sudo ip addr add 172.16.0.1/24 dev tap0
  
Run 

$ ./swarm

Now open http://172.16.0.2 in the browser !

