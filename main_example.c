/*
 * main.c
 *
 *  Created on: 25.03.2019
 *      Author: slawek
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "rbc.h"


#define BUFSIZE			16
#define BUFSIZE_1		20

static const char alpha[] = "ABCDEFGHIJKLMNOPRSTUWYZ";

//allocate ring buffer object in BSS section under name "memory"
static RBC_ALLOC_MEM(memory,BUFSIZE);

int main()
{

	DATA_TYPE buf_out[100];
	size_t len;

	//initialize ring buffer object
	//RingBuf_T rb = rbcInit(memory,RBC_MEM_SIZE(BUFSIZE));
	RingBuf_T rb = rbcInit(memory,sizeof memory);
	printf(	"\n"
					"Capacity of the buffer: %u\n"
					"Size of Ring Buffer object: %u, %u\n"
					"Size of data type: %d\n"
					"\n"
					,rbcBufLen(rb)
					,RBC_MEM_SIZE(BUFSIZE), sizeof( memory)
					,sizeof(DATA_TYPE)
					);

	//save some data
	printf("Saving string: %s\n",alpha);
	for(uint8_t i=0; i<strlen(alpha); i++)
	{
		DATA_TYPE tmp = (DATA_TYPE)alpha[i];
		len = 1;
		if(rbcWrite(rb, &tmp, &len))
			{
				printf("ERR:Buffer is full\n");
				break;
			}
	}
	//get data back
	len=strlen(alpha);
	rbcRead((RingBuf_T)memory, buf_out, &len);
	printf("Read string: ");
	for(unsigned int i=0; i<len; i++)
	{
			printf("%c",(char)buf_out[i]);
	}
	printf("\n%d elems read\n\n", len);


	return EXIT_SUCCESS;
}

