#ifndef __IPCOMMUNICATION_H__
#define __IPCOMMUNICATION_H__

#include <msq.win32.h>
#include <ypparam.h>

int ipcmd_send_msq( struct ipcmd_t *ipcmd, YPSpur_msg *data );
int ipcmd_recv_msq( struct ipcmd_t *ipcmd, YPSpur_msg *data );
void ipcmd_flush_msq( struct ipcmd_t *ipcmd );
int ipcmd_send_tcp( struct ipcmd_t *ipcmd, YPSpur_msg *data );
int ipcmd_recv_tcp( struct ipcmd_t *ipcmd, YPSpur_msg *data );
void ipcmd_flush_tcp( struct ipcmd_t *ipcmd );
int ipcmd_send( struct ipcmd_t *ipcmd, YPSpur_msg *data );
int ipcmd_recv( struct ipcmd_t *ipcmd, YPSpur_msg *data );
void ipcmd_flush( struct ipcmd_t *ipcmd );

int ipcmd_open_msq( struct ipcmd_t *ipcmd, int key, int creat );
int ipcmd_open_tcp( struct ipcmd_t *ipcmd, char *host, int port );
void ipcmd_close( struct ipcmd_t *ipcmd );

#endif

