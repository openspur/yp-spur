#ifndef __MSQ_WIN32_H__
#define __MSQ_WIN32_H__

#ifdef __MINGW32__
// Windows用

#include <windows.h>

#include <sys/types.h>
#include <unistd.h>

#define IPC_CREAT	01000
#define IPC_EXCL	02000
#define IPC_NOWAIT	04000
#define IPC_RMID	0
#define IPC_SET		1
#define IPC_STAT	2


#ifndef __key_t_defined
typedef int key_t;
# define __key_t_defined
#endif

#ifndef __uid_t_defined
typedef int uid_t;
# define __uid_t_defined
#endif

# ifndef __gid_t_defined
typedef int gid_t;
#  define __gid_t_defined
# endif

typedef unsigned long int msgqnum_t;
typedef unsigned long int msglen_t;

struct ipc_perm
{
	key_t __key;					/* Key.  */
	uid_t uid;						/* Owner's user ID.  */
	gid_t gid;						/* Owner's group ID.  */
	uid_t cuid;						/* Creator's user ID.  */
	gid_t cgid;						/* Creator's group ID.  */
	unsigned short int mode;		/* Read/write permission.  */
	unsigned short int __pad1;
	unsigned short int __seq;		/* Sequence number.  */
	unsigned short int __pad2;
	unsigned long int __unused1;
	unsigned long int __unused2;
};
struct msqid_ds
{
  struct ipc_perm msg_perm;			/* structure describing operation permission */
  time_t msg_stime;					/* time of last msgsnd command */
  unsigned long int __unused1;
  time_t msg_rtime;					/* time of last msgrcv command */
  unsigned long int __unused2;
  time_t msg_ctime;					/* time of last change */
  unsigned long int __unused3;
  unsigned long int __msg_cbytes; 	/* current number of bytes on queue */
  msgqnum_t msg_qnum;				/* number of messages currently on queue */
  msglen_t msg_qbytes;				/* max number of bytes allowed on queue */
  pid_t msg_lspid;					/* pid of last msgsnd() */
  pid_t msg_lrpid;					/* pid of last msgrcv() */
  unsigned long int __unused4;
  unsigned long int __unused5;
};

int msgget( key_t key, int msgflg );
int msgsnd( int msqid, const void *msgp, size_t msgsz, int msgflg );
ssize_t msgrcv( int msqid, void *msgp, size_t msgsz, long msgtyp, int msgflg );
int msgctl( int msqid, int cmd, struct msqid_ds *buf );

#else
// Linux用

#	include <sys/types.h>
#	include <sys/ipc.h>
#	include <sys/msg.h>

#endif



#endif
