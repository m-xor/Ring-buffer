/*
 * rbc.h
 *
 *  Created on: 29.03.2019
 *      Author: slawek
 *
 *  Ring buffer (FIFO queue) with static memory allocation and definable type of
 *  element. Intended for use on µc.
 *
 *  Known drawbacks:
 *  - can't define different data types for different instances
 *
 */

#ifndef RBC_H_
#define RBC_H_

#include <stdint.h>
#include <stddef.h>
#include <assert.h>

#define RBC_OK				0
#define RBC_NO_DATA		0x0100
#define RBC_BUF_OVF		0x0200

/*
 * Type of elements the ring buffer is to operates on.
 * Define particular type thru -D switch, for example
 * -DDATA_TYPE=double
 */
#ifndef DATA_TYPE
#define DATA_TYPE uint8_t
#endif

#define T RingBuf_T
typedef struct T *T;

/*
 * Placeholder
 */
struct rbc_ph{uint16_t a_;uint16_t b_;uint16_t c_;DATA_TYPE d_[];};
/*
 * Computes size of memory needed for ring buffer object of given capacity
 * Params:
 * size - capacity of ring buffer
 */
#define RBC_MEM_SIZE(size) ((sizeof(DATA_TYPE [size>0?size+1:-1])+sizeof(struct rbc_ph)))
/*
 * Statically define memory for ring buffer object. Variable is aligned appropriately
 * Params:
 * name - name of allocated array
 * size - ring buffer capacity
 */
#define RBC_ALLOC_MEM(name,size) _Alignas(struct rbc_ph) char name[RBC_MEM_SIZE(size)]

/*
 * Initialize ring buffer object
 *
 * Parms:
 * mempool - name of array allocated by RBC_ALLOC_MEM macro
 * size - size of this array, given by RBC_MEM_SIZE macro or sizeof operator
 * Returns:
 * pointer to initialized ring buffer object
 */
extern T rbcInit(char *mempool, size_t poolsize);
/*
 * Size of ring buffer.
 *
 * Parms:
 * rb - pointer to ring buffer object
 * Returns:
 * Ring buffer capacity
 */
extern uint16_t rbcBufLen(T rb);
/*
 * Fetches one element from ring buffer. Returns RBC_NO_DATA if there hasn't been
 *  any, RBC_OK otherwise
 */
extern uint16_t rbcGetc(T rb, DATA_TYPE *data);
/*
 * Saves one element in ring buffer. Returns RBC_BUF_OVF if ring buffer has been
 * full and operation has failed or RBC_OK otherwise.
 */
extern uint16_t rbcPutc(T rb, DATA_TYPE  data);
/*
 * Fetches data block of given size.
 * Params:
 * data - pointer to buffer for data
 * len - pointer to len of data block to be fetched. Variable is overwritten by
 * actual number of fetched elements
 * Return:
 * RBC_OK if successfull, RBC_NO_DATA if less data than requested has been fetched
 *
 */
extern uint16_t rbcRead(T restrict rb, DATA_TYPE * restrict data, size_t * restrict len);
/*
 * Saves data block into ring buffer.
 * Params:
 * data - pointer to data block to be saved
 * len - pointer to len of data block (in elements). Variable is overwritten by
 * actual number of saved elements.
 * Return:
 * RBC_OK if successfull, RBC_BUF_OVF if less data than requested has been saved
 */
extern uint16_t rbcWrite(T restrict rb, DATA_TYPE const * restrict data, size_t *len);

#undef T

#endif /* RBC_H_ */
