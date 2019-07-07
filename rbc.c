/*
 * rbc.c
 *
 *  Created on: 29.03.2019
 *      Author: slawek
 */

#include "rbc.h"
#include <assert.h>

#ifndef DATA_TYPE
#define DATA_TYPE uint8_t
#endif

/*
 * struct RingBuf_T* typedefed to RingBuf_T in rbc.h
 */
struct RingBuf_T {
	volatile uint16_t head;
	volatile uint16_t tail;
	uint16_t bufSize;
	volatile DATA_TYPE buf[];
};

/*
 * Test whether placeholder matches struct RingBuf_T
 */
__attribute__((unused)) static char rbc_tst[
					(
					sizeof(struct rbc_ph)==(sizeof(struct RingBuf_T)) &&
					_Alignof(struct rbc_ph)==_Alignof(struct RingBuf_T)
					)
					? 0 : -1
				];


RingBuf_T rbcInit(char *mempool, size_t poolsize)
{
	RingBuf_T mem = (RingBuf_T)mempool;

	mem->bufSize =  (poolsize - sizeof(struct RingBuf_T))/sizeof(DATA_TYPE);
	mem->head = 0;
	mem->tail = 0;

	return mem;
}

uint16_t rbcBufLen(RingBuf_T rbc)
{
	assert(rbc);
	return rbc->bufSize - 1;
}

uint16_t rbcRead(RingBuf_T restrict rbc, DATA_TYPE * restrict data, size_t * restrict len)
{
	size_t cnt;
	uint16_t res = RBC_OK;
	DATA_TYPE *p;

	assert(rbc && data && len);
	for(p = data, cnt = *len; cnt && (res=rbcGetc(rbc, p))!=RBC_NO_DATA; p++, cnt-- )
		;
	*len = p - data;

	return res;
}


uint16_t rbcWrite(RingBuf_T restrict rbc, DATA_TYPE const * restrict data, size_t *len)
{
	size_t cnt;
	uint16_t res = RBC_OK;
	DATA_TYPE const *p;

	assert(rbc && data && len);
	for( p = data, cnt = *len; cnt && (res=rbcPutc(rbc, *p))!=RBC_BUF_OVF; p++, cnt--)
		;
	*len = p - data;

	return res;
}


uint16_t rbcGetc(RingBuf_T rbc, DATA_TYPE * data)
{
	size_t tmptail;

	assert(rbc && data);
	if(rbc->head == rbc->tail)
		return RBC_NO_DATA;

	tmptail = ((rbc->tail + 1) < rbc->bufSize) ? (rbc->tail + 1) : 0;
	*data = rbc->buf[tmptail];
	rbc->tail = tmptail;

	return  RBC_OK;
}

uint16_t rbcPutc(RingBuf_T rb, DATA_TYPE data)
{
	size_t tmphead;

	assert(rb && data);
	tmphead = (( rb->head + 1) < rb->bufSize) ? ( rb->head + 1) : 0;

	if ( tmphead == rb->tail ) {
			return RBC_BUF_OVF;
	}else{
			rb->buf[tmphead] = data;
			rb->head = tmphead;
	}

    return RBC_OK;
}
