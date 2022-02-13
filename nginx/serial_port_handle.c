#define QRNGS_CMD_MAX_LEN    (128U)

#define QRNG_STX              0xA2      /*start transfer*/
#define QRNG_FRAME_RX         0x00      /*宿主机 -> lzzay*/
#define QRNG_FRAME_TX         0x00      /*lzzay -> 宿主机*/
#define QRNG_ETX              0xA3      /*end transfer*/



typedef enum
{
   QRNGS_BIT_STX = 0;
   QRNGS_BIT_FRAME,
   QRNGS_BIT_CMD,
   QRNGS_BIT_LEN,
   QRNGS_BIT_DAT,
   QRNGS_BIT_RESLEN,
   QRNGS_BIT_RESERVE,
   QRNGS_BIT_CRCH,
   QRNGS_BIT_CRCL,
   QRNGS_BIT_EXT,
   QRNGS_BIT_MAX
}QRNG_BIT_T;


typedef struct
{
	u8     stx;
	u8     frame;
	u8     cmd;
	u16    len;
	u8     data[128];
	u8     res_len;
	u8     reserve;
	u16    crc;
	u8     etx;
}cli_packet_t;

struct ringbuf_t
{
	u16   max;
	char* buffer;
	u16   count;
	u16   read;
	u16   write;
}



struct qrngs_uart_t
{
	u8 dev;
	u8 len;
	cli_packet_t pack;
	u8 data[QRNGS_CMD_MAX_LEN];
	u8 (*empty)(u8 id);
	u8 (*rxChar)(u8 id);
	void (*rxData)(u8 id,char* data,u32 len);
	
	u8 (*txChar)(u8 id,char c);
	void (*txData)(u8 id,char* data,u32 len);
	
	u8 (*txString)(u8 id,char* string);
	u16 (*rxLength)(u8 id);
};

void ringbuf_init(struct ringbuf_t* ring,u16 max,char *buf)
{
	ring->max = max;
	ring->buffer = buf;
	ring->count = 0;
	ring->read = 0;
	ring->write = 0;
}


void ringbuf_putchar(struct ringbuf_t* ring,char c)
{
	if(ring->count > ring->max)
	{
		return;
	}
	*(ring->buffer + ring->write) = c;
	ring->write++;
	
	if(ring->write >= ring->max)
	{
		ring->write = 0;
	}
	
	ring->count++;
}

void ringbuf_putdata(struct ringbuf_t* ring,char* data,u16 size)
{
	if(ring->count + size >= max)
	{
		 return;
	}
	if(ring->write + size >= max)
	{
		u16 len = ring->max - ring->write;
		memcpy(ring->buffer + ring->write,data,len);
		memcpy(ring->buffer,data + len,ring->write + size - ring->max);
		ring->write = ring->write + size - ring->max;
	}
	else
	{
		memcpy(ring->buffer + ring->write,data,size);
		ring->write += size;
	}
	ring->count += size;
}

char ringbuf_getchar(struct ringbuf_t *ring)
{
   char c;
   c = *(ring->buffer + ring->read);
   ring->read++;
   
   if(ring->read >= ring->max)
   {
	   ring->read = 0;
   }
   
   if(ring->count > 0)
   {
	   ring->count--;
   }

   return c; 
}

char ringbuf_getdata(struct ringbuf_t *ring,char* data,u16 len)
{
	if(ring->count < size)
	{
		return ;
	}
	if(ring->read + size > ring->max)
	{
		u16 len = ring->max - ring->read;
		memcpy(data,ring->buffer + ring->read,len);
		memcpy(data + len,ring->buffer,ring->read + size - ring->max );
		ring->read = ring->read + size - ring->max;
	}
	else
	{
		memcpy(data,ring->buffer + ring->read,size);
		ring->read += size;
	}
	
	ring->count -=size;
}


u16 ring_size(struct ringbuf_t *ring)
{
	return ring->count;
}


u16 ring_empty(void)
{
	return (ring->count == 0) ? 1: 0;
}


static char g_rxbuf[CoMn][USART_RX_LEN];
static ringbuf_t g_rx_queue[CoMn];
static struct qrngs_uart_t g_com_qrng;


void usart_init_queue(u8 com_id)
{
	
	ringbuf_init(&g_rx_queue[com_id],USART_RX_LEN,g_rxbuf[com_id]);
	return ;
}


u8 usart_rxchar(u8 com_id)
{
	ringbuf_getchar(&g_rx_queue[com_id]);
}

u8 usart_rxdata(u8 com_id,char* data,u32 len)
{
   ringbuf_getdata(&g_rx_queue[com_id], data,len);
}

u8 usart_rxempty(u8 com_id)
{
	return ringbuf_empty(&g_rx_queue[com_id]);
}


u8 usart_txchar(u8 com_id,char c);
u8 usart_txdata(u8 com_id,char* data,u32 len);

void init_qrng_uart(int com_id)
{
	memset(&g_com_qrng,0x00,sizeof(struct qrngs_uart_t));
	g_com_qrng.dev = com_id;
	g_com_qrng.len = 0;
	g_com_qrng.empty = usart_rxempty;
	g_com_qrng.rxChar = usart_rxchar;
	g_com_qrng.rxData = usart_rxdata;
	g_com_qrng.txChar = usart_txchar;
}


void qrng_uart_control(void)
{
	char c;
	int ret = 0;
	struct qrng_uart_t *cli = &g_com_qrng;
	while(!cli->empty(cli->dev))
	{
		c = cli->rxChar(cli->dev) 
		c_push(cli,c);
		
		if(cli->len >= QRNGS_CMD_MAX_LEN)
		{
			memset(cli->data,0,QRNGS_CMD_MAX_LEN);
			cli->len = 0;
		}
		if(cli->len >= QRNGS_BIT_MAX)
		{
			/*读取最后一个整数帧，如果有数据丢shi ，舍弃前面*/
			u8 *p = cli->data + (cli->len - QRNGS_BIT_MAX);
		    ret = parser_check(p,QRNGS_BIT_MAX);
			if( 1== ret)
			{
				process_cmd();
				memset(cli->data,0x00,QRNGS_BIT_MAX);
				cli->len = 0;
			}
		}
	}
	
}

void send_frame(struct qrng_uart_t *cli,u8 cmd,u8 data)
{
	u16  crc16 = 0;
	char frame[QRNGS_BIT_MAX] = {QRNG_STX,QRNG_FRAME_TX,0,0,0,0,0,0,0,QRNG_ETX};
	
	frame[QRNGS_BIT_CMD] = cmd;
	frame[QRNGS_BIT_LEN] = 0x03;
	frame[QRNGS_BIT_DAT] = data;
	frame[QRNGS_BIT_RESLEN] = 0x01;
	frame[QRNGS_BIT_RESERVE] = 0x00;
	crc16 = crc16_table_calc(&frame[1],6,0xFFFF);
	frame[QRNGS_BIT_CRCH] = (crc16 >> 8 ) & 0xff;
	frame[QRNGS_BIT_CRCL] = crc16 & 0xff;
	cli->txData(cli->dev,frame,QRNGS_BIT_MAX);
}
