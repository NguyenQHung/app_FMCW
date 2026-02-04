/**
 * Copyright (C) 2021 Xilinx, Inc
 *
 * Licensed under the Apache License, Version 2.0 (the "License"). You may
 * not use this file except in compliance with the License. A copy of the
 * License is located at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations
 * under the License.
 */
 /* This header file is shared between the DMA Proxy test application and the DMA Proxy device driver. It defines the
 * shared interface to allow DMA transfers to be done from user space.
 *
 * A set of channel buffers are created by the driver for the transmit and receive channel. The application may choose
 * to use only a subset of the channel buffers to allow prioritization of transmit vs receive.
 *
 * Note: the buffer in the data structure should be 1st in the channel interface so that the buffer is cached aligned,
 * otherwise there may be issues when using cached memory.
 */

#define BUFFER_SIZE (128 * 1024)	 	/* must match driver exactly */
#define BUFFER_COUNT 32					/* driver only */

#define TX_BUFFER_COUNT 	1				/* app only, must be <= to the number in the driver */
#define RX_BUFFER_COUNT 	32				/* app only, must be <= to the number in the driver */
#define BUFFER_INCREMENT	1				/* normally 1, but skipping buffers (2) defeats prefetching in the CPU */

// #define FINISH_XFER 	_IOW('a','a',int32_t*)
// #define START_XFER 		_IOW('a','b',int32_t*)
#define XFER 			_IO('a','c')
#define RECV			_IO('a','d')
#define ALLOC_SG     _IOW('a','f', struct dma_buf_info *)
#define ALLOC_CYCLIC _IOW('a','e', struct dma_buf_info *)
#define FREE_SG		 _IO('a','b')
#define WAIT_CYCLIC	 _IO('a','g')
#define DMA_PROXY_MEM_TO_MEM 0
#define	DMA_PROXY_MEM_TO_DEV 1
#define	DMA_PROXY_DEV_TO_MEM 2
#define	DMA_PROXY_DEV_TO_DEV 3
#define	DMA_PROXY_TRANS_NONE 4
struct dma_buf_info {
	unsigned long size;
	uint32_t numsg;
	u32 direction;
};
struct channel_buffer {
	unsigned int buffer[BUFFER_SIZE / sizeof(unsigned int)];
	enum proxy_status { PROXY_NO_ERROR = 0, PROXY_BUSY = 1, PROXY_TIMEOUT = 2, PROXY_ERROR = 3 } status;
	unsigned int length;
} __attribute__ ((aligned (1024)));		/* 64 byte alignment required for DMA, but 1024 handy for viewing memory */
