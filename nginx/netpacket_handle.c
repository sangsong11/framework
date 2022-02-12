#include "osal_def.h"

#define QSW_NETPACKET_MAX_LENGTH            4096
#define QSW_DATA_HEADER_LENGTH               10

#define qsw_strcmp(m,c)                            \
   m[0] == c &&  m[1] == c &&  m[2] == c &&  m[3] == c
   
/******************************************
|头|4  0xef oxef oxef oxef
|  |2  数据长度
|部|4  CRC
******************************************/

#pragma pack(1)
//包头结构
typedef struct _COMM_PKG_HEADER
{
	char head[4];
	unsigned short pkglen;
	int crc;
}COMM_PKG_HEADER,*LPCOMM_PKG_HEADER;
#pragma pack()

void TcpDriver::tcp_packet_handleprocess(lpqsw_connection_t conn)
{
	
	ssize_t n;
	int packe_len = 0, sumdata_len = 0,crc_value = 0, nremainsize = 0;
	char recv[QSW_NETPACKET_MAX_LENGTH] = {0};
	char* packe_tail = 0;
	char* oneframedata = 0;
	
	n = recv(conn->fd,recv,QSW_NETPACKET_MAX_LENGTH,0);
	if( n > 0 )
	{
		osal_memcpy(conn->databuf + conn->lastpos,recv,n);
		conn->lastpos += n;
		while( conn->lastpos > QSW_NETPACKET_MAX_LENGTH )
		{
			if(qsw_strcmp(conn->databuf,0xef)) 
			{
				LPCOMM_PKG_HEADER pPkgHeader = (LPCOMM_PKG_HEADER)(conn->databuf);
				packe_len = ntohs(pPkgHeader->pkglen);
				sumdata_len = conn->databuf + packe_len + QSW_DATA_HEADER_LENGTH + 4;
				crc_value = ntohs(pPkgHeader->crc);
				
				if(conn->lastpos >= sumdata_len)
				{
					int calccrc = CCR32::GetInstance()->Get_CRC((unsigned char*)conn->databuf + QSW_DATA_HEADER_LENGTH ,sumdata_len);
					packe_tail = conn->databuf + packe_len + QSW_DATA_HEADER_LENGTH;
					
					nremainsize = conn->lastpos - sumdata_len;
					if( calccrc == crc_value && qsw_strcmp(packe_tail,0xfe))
					{
						oneframedata = (char*)osal_malloc(sumdata_len);
						osal_memcpy(oneframedata,conn->databuf,sumdata_len);
						
						//处理数据 协议
						//deal_data();
					}
					if(nremainsize > 0)
					{
						osal_memcpy(conn->databuf,conn->databuf + sumdata_len,nremainsize);
					}
					conn->lastpos = nremainsize;
				}
				else
			    {
				  break;
			    }
			}
			else
			{
				bool isfind = false;
				int nfindstart = 0;
				for(int k = 1; k < conn->lastpos;k++)
				{
					if(qsw_strcmp(conn->databuf[k],oxef))
					{
						nfindstart = k;
						isfind = true;
						break;
					}
				}
				if(isfind == true)
				{
					osal_memcpy(conn->databuf,conn->databuf + k,conn->lastpos - nfindstart); 
					conn->lastpos -= nfindstart;
				}
				else
				{
					osal_memset(conn->databuf,0x00,QSW_NETPACKET_MAX_LENGTH);
					conn->lastpos = 0;
				}
			}	
		}
	}
	return ;
}