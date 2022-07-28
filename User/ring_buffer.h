/********************************** (C) COPYRIGHT *******************************
* File Name          : ring_buffer.h
* Author             : YuHR
* Version            : V1.0
* Date               : 2021/7/18
* Description        : This file contains all the functions prototypes for the 
*                      ring buffer library.
********************************************************************************/
#ifndef __RING_BUFFER_H_
#define __RING_BUFFER_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
 
#include <string.h>
//#include <stdint.h>
#include "macro.h"

/*********************************************************************
 * DEFINES
 */

#define RING_BUFFER_SIZE	2048

#ifndef SUCCESS
#define SUCCESS         0
#define FAILED          1
#endif


/*********************************************************************
 * TYPEDEFS
 */

// /* define variable types */
// typedef unsigned char   uint8_t;
// typedef unsigned short  uint16_t;


/* define structure types */
typedef struct RING_BUFFER
{
    uint8_t *buffer_ptr;      //Ring buffer address
    uint16_t buffer_size;     //Ring buffer size
    uint16_t wr_index;        //Offset address of write data
    uint16_t rd_index;        //Offset address of read data
    uint16_t dat_len;         //Length of saved data
}RING_BUFFER_t;


/*********************************************************************
 * API FUNCTIONS
 */

void RingBuffer_Init(RING_BUFFER_t *prb, uint8_t *pool, uint16_t size);
uint8_t RingBuffer_WriteByte(RING_BUFFER_t *prb, uint8_t byte);
uint8_t RingBuffer_WriteData(RING_BUFFER_t *prb, uint8_t *psrc, uint16_t wr_len);
uint8_t RingBuffer_WriteByteOnly(RING_BUFFER_t *prb, uint8_t byte);
uint8_t RingBuffer_WriteDataOnly(RING_BUFFER_t *prb, uint8_t *psrc, uint16_t wr_len);
uint8_t RingBuffer_WriteParaUpdate(RING_BUFFER_t *prb, uint16_t wr_len);
uint8_t RingBuffer_ReadData(RING_BUFFER_t *prb, uint8_t *pdest, uint16_t rd_len);
uint8_t RingBuffer_ReadDataOnly(RING_BUFFER_t *prb, uint8_t *pdest, uint16_t rd_len);
uint8_t RingBuffer_ReadParaUpdate(RING_BUFFER_t *prb, uint16_t rd_len);
 

#ifdef __cplusplus
}
#endif

#endif /*__RING_BUFFER_H_*/
