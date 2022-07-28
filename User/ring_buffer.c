/********************************** (C) COPYRIGHT *******************************
* File Name          : ring_buffer.h
* Author             : YuHR
* Version            : V1.0
* Date               : 2021/7/18
* Description        : This file provides all the ring buffer functions. 
********************************************************************************/

/*********************************************************************
 * INCLUDES
 */

#include "ring_buffer.h"


/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/*********************************************************************
 * @fn      RingBuffer_Init
 * @brief   Initialization of ring buffer structure parameters.
 * @param   prb: The address of the ring buffer structure.
 * @param   pool: Ring buffer address.
 * @param   size: The length of the ring buffer.
 * @return  None
 */
void RingBuffer_Init(RING_BUFFER_t *prb, uint8_t *pool, uint16_t size)
{
    /* initialize read, write index, and data length */
    prb->wr_index = 0;
    prb->rd_index = 0;
    prb->dat_len = 0;

    /* set buffer pool and size */
    prb->buffer_ptr = pool;
    prb->buffer_size = size;
}


/*********************************************************************
 * @fn      RingBuffer_WriteByte
 * @brief   Write byte to the ring buffer.
 * @param   prb: The address of the ring buffer structure.
 * @param   byte: The byte to be written.
 * @return  SUCCESS or FAILED.
 */
uint8_t RingBuffer_WriteByte(RING_BUFFER_t *prb, uint8_t byte)
{
    if (prb->dat_len == prb->buffer_size)//data overflow
    {
        return FAILED;
    }

    prb->buffer_ptr[prb->wr_index] = byte;

    prb->wr_index = (prb->wr_index + 1) % prb->buffer_size;//Parameter update 
    ++prb->dat_len;

    return SUCCESS;
}


/*********************************************************************
 * @fn      RingBuffer_WriteData
 * @brief   Write data to the ring buffer.
 * @param   prb: The address of the ring buffer structure.
 * @param   psrc: The address of the data to be written.
 * @param   wr_len: The length of the data to be written.
 * @return  SUCCESS or FAILED.
 */
uint8_t RingBuffer_WriteData(RING_BUFFER_t *prb, uint8_t *psrc, uint16_t wr_len)
{
    uint16_t overflow_cnt = 0;

    if ((prb->dat_len + wr_len) > prb->buffer_size)//data overflow
    {
        return FAILED;
    }

    if ((prb->wr_index + wr_len) > prb->buffer_size)//data exceeds the end of the buffer
    {
        overflow_cnt = prb->wr_index + wr_len - prb->buffer_size;
        memcpy(&prb->buffer_ptr[prb->wr_index], psrc, wr_len- overflow_cnt);         
        memcpy(prb->buffer_ptr, &psrc[wr_len- overflow_cnt], overflow_cnt);
    }
    else
    {
        memcpy(&prb->buffer_ptr[prb->wr_index], psrc, wr_len);
    }

    prb->wr_index = (prb->wr_index + wr_len) % prb->buffer_size;//Parameter update 
    prb->dat_len += wr_len;

    return SUCCESS;
}


/*********************************************************************
 * @fn      RingBuffer_WriteByteOnly
 * @brief   Write byte to the ring buffer without updating parameter.
 * @param   prb: The address of the ring buffer structure.
 * @param   byte: The byte to be written.
 * @return  SUCCESS or FAILED.
 */
uint8_t RingBuffer_WriteByteOnly(RING_BUFFER_t *prb, uint8_t byte)
{
    if (prb->dat_len == prb->buffer_size)//data overflow
    {
        return FAILED;
    }

    prb->buffer_ptr[prb->wr_index] = byte;

    prb->wr_index = (prb->wr_index + 1) % prb->buffer_size;//Parameter update 

    return SUCCESS;
}


/*********************************************************************
 * @fn      RingBuffer_WriteDataOnly
 * @brief   Write data to the ring buffer without updating parameter.
 * @param   prb: The address of the ring buffer structure.
 * @param   psrc: The address of the data to be written.
 * @param   wr_len: The length of the data to be written.
 * @return  SUCCESS or FAILED.
 */
uint8_t RingBuffer_WriteDataOnly(RING_BUFFER_t *prb, uint8_t *psrc, uint16_t wr_len)
{
    uint16_t overflow_cnt = 0;

    if ((prb->dat_len + wr_len) > prb->buffer_size)//data overflow
    {
        return FAILED;
    }

    if ((prb->wr_index + wr_len) > prb->buffer_size)//data exceeds the end of the buffer
    {
        overflow_cnt = prb->wr_index + wr_len - prb->buffer_size;
        memcpy(&prb->buffer_ptr[prb->wr_index], psrc, wr_len- overflow_cnt);         
        memcpy(prb->buffer_ptr, &psrc[wr_len- overflow_cnt], overflow_cnt);
    }
    else
    {
        memcpy(&prb->buffer_ptr[prb->wr_index], psrc, wr_len);
    }

    prb->wr_index = (prb->wr_index + wr_len) % prb->buffer_size;//Parameter update 

    return SUCCESS;
}


/*********************************************************************
 * @fn      RingBuffer_WriteParaUpdate
 * @brief   Update the ring buffer parameter.
 * @param   prb: The address of the ring buffer structure.
 * @param   wr_len: The length of the data to be written.
 * @return  SUCCESS or FAILED.
 */
uint8_t RingBuffer_WriteParaUpdate(RING_BUFFER_t *prb, uint16_t wr_len)
{
    prb->dat_len += wr_len;//Parameter update 
    
    return SUCCESS;
}


/*********************************************************************
 * @fn      RingBuffer_ReadData
 * @brief   Read data from the ring buffer.
 * @param   prb: The address of the ring buffer structure.
 * @param   pdest: The storage address of the read data.
 * @param   rd_len: The length of the data to be read.
 * @return  SUCCESS or FAILED.
 */
uint8_t RingBuffer_ReadData(RING_BUFFER_t *prb, uint8_t *pdest, uint16_t rd_len)
{
    uint16_t overflow_cnt = 0;

    if (rd_len > prb->dat_len)
    {
        return FAILED;
    }

    if ((prb->rd_index + rd_len) > prb->buffer_size)//data exceeds the end of the buffer
    {
        overflow_cnt = prb->rd_index + rd_len - prb->buffer_size;
        memcpy(pdest, &prb->buffer_ptr[prb->rd_index], rd_len - overflow_cnt);
        memcpy(&pdest[rd_len - overflow_cnt], prb->buffer_ptr, overflow_cnt);
    }
    else
    {
        memcpy(pdest, &prb->buffer_ptr[prb->rd_index], rd_len);
    }

    prb->rd_index = (prb->rd_index + rd_len) % prb->buffer_size;//Parameter update 
    prb->dat_len -= rd_len;

    return SUCCESS;
}


/*********************************************************************
 * @fn      RingBuffer_ReadDataOnly
 * @brief   Read data from the ring buffer without updating parameter.
 * @param   prb: The address of the ring buffer structure.
 * @param   pdest: The storage address of the read data.
 * @param   rd_len: The length of the data to be read.
 * @return  SUCCESS or FAILED.
 */
uint8_t RingBuffer_ReadDataOnly(RING_BUFFER_t *prb, uint8_t *pdest, uint16_t rd_len)
{
    uint16_t overflow_cnt = 0;

    if (rd_len > prb->dat_len)
    {
        return FAILED;
    }

    if ((prb->rd_index + rd_len) > prb->buffer_size)//data exceeds the end of the buffer
    {
        overflow_cnt = prb->rd_index + rd_len - prb->buffer_size;
        memcpy(pdest, &prb->buffer_ptr[prb->rd_index], rd_len - overflow_cnt);
        memcpy(&pdest[rd_len - overflow_cnt], prb->buffer_ptr, overflow_cnt);
    }
    else
    {
        memcpy(pdest, &prb->buffer_ptr[prb->rd_index], rd_len);
    }

    prb->rd_index = (prb->rd_index + rd_len) % prb->buffer_size;//Parameter update 

    return SUCCESS;
}


/*********************************************************************
 * @fn      RingBuffer_ReadParaUpdate
 * @brief   Update the ring buffer parameter.
 * @param   prb: The address of the ring buffer structure.
 * @param   rd_len: The length of the data to be read.
 * @return  SUCCESS or FAILED.
 */
uint8_t RingBuffer_ReadParaUpdate(RING_BUFFER_t *prb, uint16_t rd_len)
{
    prb->dat_len -= rd_len;//Parameter update 

    return SUCCESS;
}
