#ifndef __TR111_H_
#define __TR111_H_


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/select.h>


#define MIN(a, b) ((a) > (b) ? (b) : (a))



void test();





/*!
 * \struct sched
 * \brief The scheduler to implement a event drived application as a single thread program
 */
struct sched {
	unsigned char type:2;
	unsigned char need_destroy:1;

	void *pdata; /*!<The private data of the scheduler, created, operated and destroyed by
                      * the scheduler's callback functions - on_readable, on_writable,
                      * on_timeout and on_destroy. It is transparent to scheduling module.
                      */

	int fd; /*!<The file descriptor is waiting for readable or writable*/
	time_t timeout;

	//MUST NOT free the scheduler in these functions, if it really want to destroy
	//the scheduler, it should set the need_destroy flag instead of free it.
	void (*on_readable)(struct sched *); /*!<The callback function to be called if the
                                              * scheduler is SCHED_WAITING_READABLE and the
                                              * file descriptor became readable.
                                              */
	void (*on_writable)(struct sched *); /*!<The callback function to be called if the
                                              * scheduler is SCHED_WAITING_WRITABLE and the
                                              * file descriptor became writable.
                                              */
	void (*on_timeout)(struct sched *); /*!<The callback function to be called if the
                                              * scheduler is SCHED_WAITING_READABLE,
                                              * SCHED_WAITING_WRITABLE or
                                              * SCHED_WAITING_TIMEOUT and the timer expired.
                                              */
	void (*on_destroy)(struct sched *); /*!<The callback function to destroy the pdata*/

	struct sched *next;
};


typedef enum {
	STUN_SEND,
	STUN_RECV,
	STUN_OUT
}stunstau;













#endif
