#ifndef __SH7040S__
#define __SH7040S__

/************************************************************************/
/* SH7040 Series Include File Ver 2.0 */

/************************************************************************/
struct st_sci
{												/* struct SCI */
	union
	{											/* SMR */
		unsigned char BYTE;						/* Byte Access */
		struct
		{										/* Bit Access */
			unsigned char CA:1;					/* C/A */
			unsigned char CHR:1;				/* CHR */
			unsigned char _PE:1;				/* PE */
			unsigned char OE:1;					/* O/E */
			unsigned char STOP:1;				/* STOP */
			unsigned char MP:1;					/* MP */
			unsigned char CKS:2;				/* CKS */
		} BIT;									/* */
	} SMR;										/* */
	unsigned char BRR;							/* BRR */
	union
	{											/* SCR */
		unsigned char BYTE;						/* Byte Access */
		struct
		{										/* Bit Access */
			unsigned char TIE:1;				/* TIE */
			unsigned char RIE:1;				/* RIE */
			unsigned char TE:1;					/* TE */
			unsigned char RE:1;					/* RE */
			unsigned char MPIE:1;				/* MPIE */
			unsigned char TEIE:1;				/* TEIE */
			unsigned char CKE:2;				/* CKE */
		} BIT;									/* */
	} SCR;										/* */
	unsigned char TDR;							/* TDR */
	union
	{											/* SSR */
		unsigned char BYTE;						/* Byte Access */
		struct
		{										/* Bit Access */
			unsigned char TDRE:1;				/* TDRE */
			unsigned char RDRF:1;				/* RDRF */
			unsigned char ORER:1;				/* ORER */
			unsigned char FER:1;				/* FER */
			unsigned char PER:1;				/* PER */
			unsigned char TEND:1;				/* TEND */
			unsigned char MPB:1;				/* MPB */
			unsigned char MPBT:1;				/* MPBT */
		} BIT;									/* */
	} SSR;										/* */
	unsigned char RDR;							/* RDR */
};												/* */
struct st_mtu
{												/* struct MTU */
	union
	{											/* TOER */
		unsigned char BYTE;						/* Byte Access */
		struct
		{										/* Bit Access */
			unsigned char:2;					/* */
			unsigned char OE4D:1;				/* OE4D */
			unsigned char OE4C:1;				/* OE4C */
			unsigned char OE3D:1;				/* OE3D */
			unsigned char OE4B:1;				/* OE4B */
			unsigned char OE4A:1;				/* OE4A */
			unsigned char OE3B:1;				/* OE3B */
		} BIT;									/* */
	} TOER;										/* */
	union
	{											/* TOCR */
		unsigned char BYTE;						/* Byte Access */
		struct
		{										/* Bit Access */
			unsigned char:1;					/* */
			unsigned char PSYE:1;				/* PSYE */
			unsigned char:4;					/* */
			unsigned char OLSN:1;				/* OLSN */
			unsigned char OLSP:1;				/* OLSP */
		} BIT;									/* */
	} TOCR;										/* */
	char wk1;									/* */
	union
	{											/* TGCR */
		unsigned char BYTE;						/* Byte Access */
		struct
		{										/* Bit Access */
			unsigned char:1;					/* */
			unsigned char BDC:1;				/* BDC */
			unsigned char N:1;					/* N */
			unsigned char P:1;					/* P */
			unsigned char FB:1;					/* FB */
			unsigned char WF:1;					/* WF */
			unsigned char VF:1;					/* VF */
			unsigned char UF:1;					/* UF */
		} BIT;									/* */
	} TGCR;										/* */
	char wk2[6];								/* */
	unsigned short TCDR;						/* TCDR */
	unsigned short TDDR;						/* TDDR */
	char wk3[8];								/* */
	unsigned short TCNTS;						/* TCNTS */
	unsigned short TCBR;						/* TCBR */
	char wk4[28];								/* */
	union
	{											/* TSTR */
		unsigned char BYTE;						/* Byte Access */
		struct
		{										/* Bit Access */
			unsigned char CST4:1;				/* CST4 */
			unsigned char CST3:1;				/* CST3 */
			unsigned char:3;					/* */
			unsigned char CST2:1;				/* CST2 */
			unsigned char CST1:1;				/* CST1 */
			unsigned char CST0:1;				/* CST0 */
		} BIT;									/* */
	} TSTR;										/* */
	union
	{											/* TSYR */
		unsigned char BYTE;						/* Byte Access */
		struct
		{										/* Bit Access */
			unsigned char SYNC4:1;				/* SYNC4 */
			unsigned char SYNC3:1;				/* SYNC3 */
			unsigned char:3;					/* */
			unsigned char SYNC2:1;				/* SYNC2 */
			unsigned char SYNC1:1;				/* SYNC1 */
			unsigned char SYNC0:1;				/* SYNC0 */
		} BIT;									/* */
	} TSYR;										/* */
};												/* */
struct st_mtu0
{												/* struct MTU0 */
	union
	{											/* TCR */
		unsigned char BYTE;						/* Byte Access */
		struct
		{										/* Bit Access */
			unsigned char CCLR:3;				/* CCLR */
			unsigned char CKEG:2;				/* CKEG */
			unsigned char TPSC:3;				/* TPSC */
		} BIT;									/* */
	} TCR;										/* */
	union
	{											/* TMDR */
		unsigned char BYTE;						/* Byte Access */
		struct
		{										/* Bit Access */
			unsigned char:2;					/* */
			unsigned char BFB:1;				/* BFB */
			unsigned char BFA:1;				/* BFA */
			unsigned char MD:4;					/* MD */
		} BIT;									/* */
	} TMDR;										/* */
	union
	{											/* TIOR */
		unsigned short WORD;					/* Word Access */
		struct
		{										/* Byte Access */
			unsigned char H;					/* TIORH */
			unsigned char L;					/* TIORL */
		} BYTE;									/* */
		struct
		{										/* Bit Access */
			unsigned char IOB:4;				/* IOB */
			unsigned char IOA:4;				/* IOA */
			unsigned char IOD:4;				/* IOD */
			unsigned char IOC:4;				/* IOC */
		} BIT;									/* */
	} TIOR;										/* */
	union
	{											/* TIER */
		unsigned char BYTE;						/* Byte Access */
		struct
		{										/* Bit Access */
			unsigned char TTGE:1;				/* TTGE */
			unsigned char:2;					/* */
			unsigned char TCIEV:1;				/* TCIEV */
			unsigned char TGIED:1;				/* TGIED */
			unsigned char TGIEC:1;				/* TGIEC */
			unsigned char TGIEB:1;				/* TGIEB */
			unsigned char TGIEA:1;				/* TGIEA */
		} BIT;									/* */
	} TIER;										/* */
	union
	{											/* TSR */
		unsigned char BYTE;						/* Byte Access */
		struct
		{										/* Bit Access */
			unsigned char:3;					/* */
			unsigned char TCFV:1;				/* TCFV */
			unsigned char TGFD:1;				/* TGFD */
			unsigned char TGFC:1;				/* TGFC */
			unsigned char TGFB:1;				/* TGFB */
			unsigned char TGFA:1;				/* TGFA */
		} BIT;									/* */
	} TSR;										/* */
	unsigned short TCNT;						/* TCNT */
	unsigned short TGRA;						/* TGRA */
	unsigned short TGRB;						/* TGRB */
	unsigned short TGRC;						/* TGRC */
	unsigned short TGRD;						/* TGRD */
};												/* */
struct st_mtu1
{												/* struct MTU1 */
	union
	{											/* TCR */
		unsigned char BYTE;						/* Byte Access */
		struct
		{										/* Bit Access */
			unsigned char:1;					/* */
			unsigned char CCLR:2;				/* CCLR */
			unsigned char CKEG:2;				/* CKEG */
			unsigned char TPSC:3;				/* TPSC */
		} BIT;									/* */
	} TCR;										/* */
	union
	{											/* TMDR */
		unsigned char BYTE;						/* Byte Access */
		struct
		{										/* Bit Access */
			unsigned char:4;					/* */
			unsigned char MD:4;					/* MD */
		} BIT;									/* */
	} TMDR;										/* */
	union
	{											/* TIOR */
		unsigned char BYTE;						/* Byte Access */
		struct
		{										/* Bit Access */
			unsigned char IOB:4;				/* IOB */
			unsigned char IOA:4;				/* IOA */
		} BIT;									/* */
	} TIOR;										/* */
	char wk;									/* */
	union
	{											/* TIER */
		unsigned char BYTE;						/* Byte Access */
		struct
		{										/* Bit Access */
			unsigned char TTGE:1;				/* TTGE */
			unsigned char:1;					/* */
			unsigned char TCIEU:1;				/* TCIEU */
			unsigned char TCIEV:1;				/* TCIEV */
			unsigned char:2;					/* */
			unsigned char TGIEB:1;				/* TGIEB */
			unsigned char TGIEA:1;				/* TGIEA */
		} BIT;									/* */
	} TIER;										/* */
	union
	{											/* TSR */
		unsigned char BYTE;						/* Byte Access */
		struct
		{										/* Bit Access */
			unsigned char TCFD:1;				/* TCFD */
			unsigned char:1;					/* */
			unsigned char TCFU:1;				/* TCFU */
			unsigned char TCFV:1;				/* TCFV */
			unsigned char:2;					/* */
			unsigned char TGFB:1;				/* TGFB */
			unsigned char TGFA:1;				/* TGFA */
		} BIT;									/* */
	} TSR;										/* */
	unsigned short TCNT;						/* TCNT */
	unsigned short TGRA;						/* TGRA */
	unsigned short TGRB;						/* TGRB */
};												/* */
struct st_mtu3
{												/* struct MTU3 */
	union
	{											/* TCR */
		unsigned char BYTE;						/* Byte Access */
		struct
		{										/* Bit Access */
			unsigned char CCLR:3;				/* CCLR */
			unsigned char CKEG:2;				/* CKEG */
			unsigned char TPSC:3;				/* TPSC */
		} BIT;									/* */
	} TCR;										/* */
	char wk1;									/* */
	union
	{											/* TMDR */
		unsigned char BYTE;						/* Byte Access */
		struct
		{										/* Bit Access */
			unsigned char:2;					/* */
			unsigned char BFB:1;				/* BFB */
			unsigned char BFA:1;				/* BFA */
			unsigned char MD:4;					/* MD */
		} BIT;									/* */
	} TMDR;										/* */
	char wk2;									/* */
	union
	{											/* TIOR */
		unsigned short WORD;					/* Word Access */
		struct
		{										/* Byte Access */
			unsigned char H;					/* TIORH */
			unsigned char L;					/* TIORL */
		} BYTE;									/* */
		struct
		{										/* Bit Access */
			unsigned char IOB:4;				/* IOB */
			unsigned char IOA:4;				/* IOA */
			unsigned char IOD:4;				/* IOD */
			unsigned char IOC:4;				/* IOC */
		} BIT;									/* */
	} TIOR;										/* */
	char wk3[2];								/* */
	union
	{											/* TIER */
		unsigned char BYTE;						/* Byte Access */
		struct
		{										/* Bit Access */
			unsigned char TTGE:1;				/* TTGE */
			unsigned char:2;					/* */
			unsigned char TCIEV:1;				/* TCIEV */
			unsigned char TGIED:1;				/* TGIED */
			unsigned char TGIEC:1;				/* TGIEC */
			unsigned char TGIEB:1;				/* TGIEB */
			unsigned char TGIEA:1;				/* TGIEA */
		} BIT;									/* */
	} TIER;										/* */
	char wk4[7];								/* */
	unsigned short TCNT;						/* TCNT */
	char wk5[6];								/* */
	unsigned short TGRA;						/* TGRA */
	unsigned short TGRB;						/* TGRB */
	char wk6[8];								/* */
	unsigned short TGRC;						/* TGRC */
	unsigned short TGRD;						/* TGRD */
	char wk7[4];								/* */
	union
	{											/* TSR */
		unsigned char BYTE;						/* Byte Access */
		struct
		{										/* Bit Access */
			unsigned char TCFD:1;				/* TCFD */
			unsigned char:2;					/* */
			unsigned char TCFV:1;				/* TCFV */
			unsigned char TGFD:1;				/* TGFD */
			unsigned char TGFC:1;				/* TGFC */
			unsigned char TGFB:1;				/* TGFB */
			unsigned char TGFA:1;				/* TGFA */
		} BIT;									/* */
	} TSR;										/* */
};												/* */
struct st_mtu4
{												/* struct MTU4 */
	char wk1;									/* */
	union
	{											/* TCR */
		unsigned char BYTE;						/* Byte Access */
		struct
		{										/* Bit Access */
			unsigned char CCLR:3;				/* CCLR */
			unsigned char CKEG:2;				/* CKEG */
			unsigned char TPSC:3;				/* TPSC */
		} BIT;									/* */
	} TCR;										/* */
	char wk2;									/* */
	union
	{											/* TMDR */
		unsigned char BYTE;						/* Byte Access */
		struct
		{										/* Bit Access */
			unsigned char:2;					/* */
			unsigned char BFB:1;				/* BFB */
			unsigned char BFA:1;				/* BFA */
			unsigned char MD:4;					/* MD */
		} BIT;									/* */
	} TMDR;										/* */
	char wk3[2];								/* */
	union
	{											/* TIOR */
		unsigned short WORD;					/* Word Access */
		struct
		{										/* Byte Access */
			unsigned char H;					/* TIORH */
			unsigned char L;					/* TIORL */
		} BYTE;									/* */
		struct
		{										/* Bit Access */
			unsigned char IOB:4;				/* IOB */
			unsigned char IOA:4;				/* IOA */
			unsigned char IOD:4;				/* IOD */
			unsigned char IOC:4;				/* IOC */
		} BIT;									/* */
	} TIOR;										/* */
	char wk4;									/* */
	union
	{											/* TIER */
		unsigned char BYTE;						/* Byte Access */
		struct
		{										/* Bit Access */
			unsigned char TTGE:1;				/* TTGE */
			unsigned char:2;					/* */
			unsigned char TCIEV:1;				/* TCIEV */
			unsigned char TGIED:1;				/* TGIED */
			unsigned char TGIEC:1;				/* TGIEC */
			unsigned char TGIEB:1;				/* TGIEB */
			unsigned char TGIEA:1;				/* TGIEA */
		} BIT;									/* */
	} TIER;										/* */
	char wk5[8];								/* */
	unsigned short TCNT;						/* TCNT */
	char wk6[8];								/* */
	unsigned short TGRA;						/* TGRA */
	unsigned short TGRB;						/* TGRB */
	char wk7[8];								/* */
	unsigned short TGRC;						/* TGRC */
	unsigned short TGRD;						/* TGRD */
	char wk8;									/* */
	union
	{											/* TSR */
		unsigned char BYTE;						/* Byte Access */
		struct
		{										/* Bit Access */
			unsigned char TCFD:1;				/* TCFD */
			unsigned char:2;					/* */
			unsigned char TCFV:1;				/* TCFV */
			unsigned char TGFD:1;				/* TGFD */
			unsigned char TGFC:1;				/* TGFC */
			unsigned char TGFB:1;				/* TGFB */
			unsigned char TGFA:1;				/* TGFA */
		} BIT;									/* */
	} TSR;										/* */
};												/* */
struct st_poe
{												/* struct POE */
	union
	{											/* ICSR */
		unsigned short WORD;					/* Word Access */
		struct
		{										/* Bit Access */
			unsigned char POE3F:1;				/* POE3F */
			unsigned char POE2F:1;				/* POE2F */
			unsigned char POE1F:1;				/* POE1F */
			unsigned char POE0F:1;				/* POE0F */
			unsigned char:3;					/* */
			unsigned char PIE:1;				/* PIE */
			unsigned char POE3M:2;				/* POE3M */
			unsigned char POE2M:2;				/* POE2M */
			unsigned char POE1M:2;				/* POE1M */
			unsigned char POE0M:2;				/* POE0M */
		} BIT;									/* */
	} ICSR;										/* */
	union
	{											/* OCSR */
		unsigned char BYTE;						/* Byte Access */
		struct
		{										/* Bit Access */
			unsigned char OSF:1;				/* OSF */
			unsigned char:5;					/* */
			unsigned char OCE:1;				/* OCE */
			unsigned char OIE:1;				/* OIE */
		} BIT;									/* */
	} OCSR;										/* */
};												/* */
struct st_intc
{												/* struct INTC */
	union
	{											/* IPRA */
		unsigned short WORD;					/* Word Access */
		struct
		{										/* Bit Access */
			unsigned char _IRQ0:4;				/* IRQ0 */
			unsigned char _IRQ1:4;				/* IRQ1 */
			unsigned char _IRQ2:4;				/* IRQ2 */
			unsigned char _IRQ3:4;				/* IRQ3 */
		} BIT;									/* */
	} IPRA;										/* */
	union
	{											/* IPRB */
		unsigned short WORD;					/* Word Access */
		struct
		{										/* Bit Access */
			unsigned char _IRQ4:4;				/* IRQ4 */
			unsigned char _IRQ5:4;				/* IRQ5 */
			unsigned char _IRQ6:4;				/* IRQ6 */
			unsigned char _IRQ7:4;				/* IRQ7 */
		} BIT;									/* */
	} IPRB;										/* */
	union
	{											/* IPRC */
		unsigned short WORD;					/* Word Access */
		struct
		{										/* Bit Access */
			unsigned char _DMAC0:4;				/* DMAC0 */
			unsigned char _DMAC1:4;				/* DMAC1 */
			unsigned char _DMAC2:4;				/* DMAC2 */
			unsigned char _DMAC3:4;				/* DMAC3 */
		} BIT;									/* */
	} IPRC;										/* */
	union
	{											/* IPRD */
		unsigned short WORD;					/* Word Access */
		struct
		{										/* Bit Access */
			unsigned char _MTU0G:4;				/* MTU0 TGI */
			unsigned char _MTU0C:4;				/* MTU0 TCI */
			unsigned char _MTU1G:4;				/* MTU1 TGI */
			unsigned char _MTU1C:4;				/* MTU1 TCI */
		} BIT;									/* */
	} IPRD;										/* */
	union
	{											/* IPRE */
		unsigned short WORD;					/* Word Access */
		struct
		{										/* Bit Access */
			unsigned char _MTU2G:4;				/* MTU2 TGI */
			unsigned char _MTU2C:4;				/* MTU2 TCI */
			unsigned char _MTU3G:4;				/* MTU3 TGI */
			unsigned char _MTU3C:4;				/* MTU3 TCI */
		} BIT;									/* */
	} IPRE;										/* */
	union
	{											/* IPRF */
		unsigned short WORD;					/* Word Access */
		struct
		{										/* Bit Access */
			unsigned char _MTU4G:4;				/* MTU4 TGI */
			unsigned char _MTU4C:4;				/* MTU4 TCI */
			unsigned char _SCI0:4;				/* SCI0 */
			unsigned char _SCI1:4;				/* SCI1 */
		} BIT;									/* */
	} IPRF;										/* */
	union
	{											/* IPRG */
		unsigned short WORD;					/* Word Access */
		struct
		{										/* Bit Access */
			unsigned char _AD:4;				/* A/D */
			unsigned char _DTC:4;				/* DTC */
			unsigned char _CMT0:4;				/* CMT0 */
			unsigned char _CMT1:4;				/* CMT1 */
		} BIT;									/* */
	} IPRG;										/* */
	union
	{											/* IPRH */
		unsigned short WORD;					/* Word Access */
		struct
		{										/* Bit Access */
			unsigned char _WDT:4;				/* WDT,BSC */
			unsigned char _IO:4;				/* I/O */
		} BIT;									/* */
	} IPRH;										/* */
	union
	{											/* ICR */
		unsigned short WORD;					/* Word Access */
		struct
		{										/* Bit Access */
			unsigned char NMIL:1;				/* NMIL */
			unsigned char:6;					/* */
			unsigned char NMIE:1;				/* NMIE */
			unsigned char IRQ0S:1;				/* IRQ0S */
			unsigned char IRQ1S:1;				/* IRQ1S */
			unsigned char IRQ2S:1;				/* IRQ2S */
			unsigned char IRQ3S:1;				/* IRQ3S */
			unsigned char IRQ4S:1;				/* IRQ4S */
			unsigned char IRQ5S:1;				/* IRQ5S */
			unsigned char IRQ6S:1;				/* IRQ6S */
			unsigned char IRQ7S:1;				/* IRQ7S */
		} BIT;									/* */
	} ICR;										/* */
	union
	{											/* ISR */
		unsigned short WORD;					/* Word Access */
		struct
		{										/* Bit Access */
			unsigned char:8;					/* */
			unsigned char IRQ0F:1;				/* IRQ0F */
			unsigned char IRQ1F:1;				/* IRQ1F */
			unsigned char IRQ2F:1;				/* IRQ2F */
			unsigned char IRQ3F:1;				/* IRQ3F */
			unsigned char IRQ4F:1;				/* IRQ4F */
			unsigned char IRQ5F:1;				/* IRQ5F */
			unsigned char IRQ6F:1;				/* IRQ6F */
			unsigned char IRQ7F:1;				/* IRQ7F */
		} BIT;									/* */
	} ISR;										/* */
};												/* */
struct st_pa
{												/* struct PA */
	union
	{											/* PADR */
		unsigned int LONG;						/* Long Access */
		struct
		{										/* Word Access */
			unsigned short H;					/* High */
			unsigned short L;					/* Low */
		} WORD;									/* */
		struct
		{										/* Byte Access */
			char wk;							/* */
			unsigned char HL;					/* High,Low */
			unsigned char LH;					/* Low ,High */
			unsigned char LL;					/* Low ,Low */
		} BYTE;									/* */
		struct
		{										/* Bit Access */
			unsigned char:8;					/* */
			unsigned char B23:1;				/* Bit 23 */
			unsigned char B22:1;				/* Bit 22 */
			unsigned char B21:1;				/* Bit 21 */
			unsigned char B20:1;				/* Bit 20 */
			unsigned char B19:1;				/* Bit 19 */
			unsigned char B18:1;				/* Bit 18 */
			unsigned char B17:1;				/* Bit 17 */
			unsigned char B16:1;				/* Bit 16 */
			unsigned char B15:1;				/* Bit 15 */
			unsigned char B14:1;				/* Bit 14 */
			unsigned char B13:1;				/* Bit 13 */
			unsigned char B12:1;				/* Bit 12 */
			unsigned char B11:1;				/* Bit 11 */
			unsigned char B10:1;				/* Bit 10 */
			unsigned char B9:1;					/* Bit 9 */
			unsigned char B8:1;					/* Bit 8 */
			unsigned char B7:1;					/* Bit 7 */
			unsigned char B6:1;					/* Bit 6 */
			unsigned char B5:1;					/* Bit 5 */
			unsigned char B4:1;					/* Bit 4 */
			unsigned char B3:1;					/* Bit 3 */
			unsigned char B2:1;					/* Bit 2 */
			unsigned char B1:1;					/* Bit 1 */
			unsigned char B0:1;					/* Bit 0 */
		} BIT;									/* */
	} DR;										/* */
};												/* */
struct st_pb
{												/* struct PB */
	union
	{											/* PBDR */
		unsigned short WORD;					/* Word Access */
		struct
		{										/* Byte Access */
			unsigned char H;					/* High */
			unsigned char L;					/* Low */
		} BYTE;									/* */
		struct
		{										/* Bit Access */
			unsigned char:6;					/* */
			unsigned char B9:1;					/* Bit 9 */
			unsigned char B8:1;					/* Bit 8 */
			unsigned char B7:1;					/* Bit 7 */
			unsigned char B6:1;					/* Bit 6 */
			unsigned char B5:1;					/* Bit 5 */
			unsigned char B4:1;					/* Bit 4 */
			unsigned char B3:1;					/* Bit 3 */
			unsigned char B2:1;					/* Bit 2 */
			unsigned char B1:1;					/* Bit 1 */
			unsigned char B0:1;					/* Bit 0 */
		} BIT;									/* */
	} DR;										/* */
};												/* */
struct st_pc
{												/* struct PC */
	union
	{											/* PCDR */
		unsigned short WORD;					/* Word Access */
		struct
		{										/* Byte Access */
			unsigned char H;					/* High */
			unsigned char L;					/* Low */
		} BYTE;									/* */
		struct
		{										/* Bit Access */
			unsigned char B15:1;				/* Bit 15 */
			unsigned char B14:1;				/* Bit 14 */
			unsigned char B13:1;				/* Bit 13 */
			unsigned char B12:1;				/* Bit 12 */
			unsigned char B11:1;				/* Bit 11 */
			unsigned char B10:1;				/* Bit 10 */
			unsigned char B9:1;					/* Bit 9 */
			unsigned char B8:1;					/* Bit 8 */
			unsigned char B7:1;					/* Bit 7 */
			unsigned char B6:1;					/* Bit 6 */
			unsigned char B5:1;					/* Bit 5 */
			unsigned char B4:1;					/* Bit 4 */
			unsigned char B3:1;					/* Bit 3 */
			unsigned char B2:1;					/* Bit 2 */
			unsigned char B1:1;					/* Bit 1 */
			unsigned char B0:1;					/* Bit 0 */
		} BIT;									/* */
	} DR;										/* */
};												/* */
struct st_pd
{												/* struct PD */
	union
	{											/* PDDR */
		unsigned int LONG;						/* Long Access */
		struct
		{										/* Word Access */
			unsigned short H;					/* High */
			unsigned short L;					/* Low */
		} WORD;									/* */
		struct
		{										/* Byte Access */
			unsigned char HH;					/* High,High */
			unsigned char HL;					/* High,Low */
			unsigned char LH;					/* Low ,High */
			unsigned char LL;					/* Low ,Low */
		} BYTE;									/* */
		struct
		{										/* Bit Access */
			unsigned char B31:1;				/* Bit 31 */
			unsigned char B30:1;				/* Bit 30 */
			unsigned char B29:1;				/* Bit 29 */
			unsigned char B28:1;				/* Bit 28 */
			unsigned char B27:1;				/* Bit 27 */
			unsigned char B26:1;				/* Bit 26 */
			unsigned char B25:1;				/* Bit 25 */
			unsigned char B24:1;				/* Bit 24 */
			unsigned char B23:1;				/* Bit 23 */
			unsigned char B22:1;				/* Bit 22 */
			unsigned char B21:1;				/* Bit 21 */
			unsigned char B20:1;				/* Bit 20 */
			unsigned char B19:1;				/* Bit 19 */
			unsigned char B18:1;				/* Bit 18 */
			unsigned char B17:1;				/* Bit 17 */
			unsigned char B16:1;				/* Bit 16 */
			unsigned char B15:1;				/* Bit 15 */
			unsigned char B14:1;				/* Bit 14 */
			unsigned char B13:1;				/* Bit 13 */
			unsigned char B12:1;				/* Bit 12 */
			unsigned char B11:1;				/* Bit 11 */
			unsigned char B10:1;				/* Bit 10 */
			unsigned char B9:1;					/* Bit 9 */
			unsigned char B8:1;					/* Bit 8 */
			unsigned char B7:1;					/* Bit 7 */
			unsigned char B6:1;					/* Bit 6 */
			unsigned char B5:1;					/* Bit 5 */
			unsigned char B4:1;					/* Bit 4 */
			unsigned char B3:1;					/* Bit 3 */
			unsigned char B2:1;					/* Bit 2 */
			unsigned char B1:1;					/* Bit 1 */
			unsigned char B0:1;					/* Bit 0 */
		} BIT;									/* */
	} DR;										/* */
};												/* */
struct st_pe
{												/* struct PE */
	union
	{											/* PEDR */
		unsigned short WORD;					/* Word Access */
		struct
		{										/* Byte Access */
			unsigned char H;					/* High */
			unsigned char L;					/* Low */
		} BYTE;									/* */
		struct
		{										/* Bit Access */
			unsigned char B15:1;				/* Bit 15 */
			unsigned char B14:1;				/* Bit 14 */
			unsigned char B13:1;				/* Bit 13 */
			unsigned char B12:1;				/* Bit 12 */
			unsigned char B11:1;				/* Bit 11 */
			unsigned char B10:1;				/* Bit 10 */
			unsigned char B9:1;					/* Bit 9 */
			unsigned char B8:1;					/* Bit 8 */
			unsigned char B7:1;					/* Bit 7 */
			unsigned char B6:1;					/* Bit 6 */
			unsigned char B5:1;					/* Bit 5 */
			unsigned char B4:1;					/* Bit 4 */
			unsigned char B3:1;					/* Bit 3 */
			unsigned char B2:1;					/* Bit 2 */
			unsigned char B1:1;					/* Bit 1 */
			unsigned char B0:1;					/* Bit 0 */
		} BIT;									/* */
	} DR;										/* */
};												/* */
struct st_pf
{												/* struct PF */
	union
	{											/* PFDR */
		unsigned char BYTE;						/* Byte Access */
		struct
		{										/* Bit Access */
			unsigned char B7:1;					/* Bit 7 */
			unsigned char B6:1;					/* Bit 6 */
			unsigned char B5:1;					/* Bit 5 */
			unsigned char B4:1;					/* Bit 4 */
			unsigned char B3:1;					/* Bit 3 */
			unsigned char B2:1;					/* Bit 2 */
			unsigned char B1:1;					/* Bit 1 */
			unsigned char B0:1;					/* Bit 0 */
		} BIT;									/* */
	} DR;										/* */
};												/* */
struct st_pfc
{												/* struct PFC */
	union
	{											/* PAIORH */
		unsigned short WORD;					/* Word Access */
		struct
		{										/* Byte Access */
			unsigned char H;					/* High */
			unsigned char L;					/* Low */
		} BYTE;									/* */
		struct
		{										/* Bit Access */
			unsigned char:8;					/* */
			unsigned char B23:1;				/* Bit 23 */
			unsigned char B22:1;				/* Bit 22 */
			unsigned char B21:1;				/* Bit 21 */
			unsigned char B20:1;				/* Bit 20 */
			unsigned char B19:1;				/* Bit 19 */
			unsigned char B18:1;				/* Bit 18 */
			unsigned char B17:1;				/* Bit 17 */
			unsigned char B16:1;				/* Bit 16 */
		} BIT;									/* */
	} PAIORH;									/* */
	union
	{											/* PAIORL */
		unsigned short WORD;					/* Word Access */
		struct
		{										/* Byte Access */
			unsigned char H;					/* High */
			unsigned char L;					/* Low */
		} BYTE;									/* */
		struct
		{										/* Bit Access */
			unsigned char B15:1;				/* Bit 15 */
			unsigned char B14:1;				/* Bit 14 */
			unsigned char B13:1;				/* Bit 13 */
			unsigned char B12:1;				/* Bit 12 */
			unsigned char B11:1;				/* Bit 11 */
			unsigned char B10:1;				/* Bit 10 */
			unsigned char B9:1;					/* Bit 9 */
			unsigned char B8:1;					/* Bit 8 */
			unsigned char B7:1;					/* Bit 7 */
			unsigned char B6:1;					/* Bit 6 */
			unsigned char B5:1;					/* Bit 5 */
			unsigned char B4:1;					/* Bit 4 */
			unsigned char B3:1;					/* Bit 3 */
			unsigned char B2:1;					/* Bit 2 */
			unsigned char B1:1;					/* Bit 1 */
			unsigned char B0:1;					/* Bit 0 */
		} BIT;									/* */
	} PAIORL;									/* */
	union
	{											/* PACRH */
		unsigned short WORD;					/* Word Access */
		struct
		{										/* Byte Access */
			unsigned char H;					/* High */
			unsigned char L;					/* Low */
		} BYTE;									/* */
		struct
		{										/* Bit Access */
			unsigned char:1;					/* */
			unsigned char PA23MD:1;				/* PA23MD */
			unsigned char:1;					/* */
			unsigned char PA22MD:1;				/* PA22MD */
			unsigned char:1;					/* */
			unsigned char PA21MD:1;				/* PA21MD */
			unsigned char:1;					/* */
			unsigned char PA20MD:1;				/* PA20MD */
			unsigned char PA19MD:2;				/* PA19MD */
			unsigned char PA18MD:2;				/* PA18MD */
			unsigned char:1;					/* */
			unsigned char PA17MD:1;				/* PA17MD */
			unsigned char:1;					/* */
			unsigned char PA16MD:1;				/* PA16MD */
		} BIT;									/* */
	} PACRH;									/* */
	char wk1[2];								/* */
	union
	{											/* PACRL1 */
		unsigned short WORD;					/* Word Access */
		struct
		{										/* Byte Access */
			unsigned char H;					/* High */
			unsigned char L;					/* Low */
		} BYTE;									/* */
		struct
		{										/* Bit Access */
			unsigned char:1;					/* */
			unsigned char PA15MD:1;				/* PA15MD */
			unsigned char:1;					/* */
			unsigned char PA14MD:1;				/* PA14MD */
			unsigned char:1;					/* */
			unsigned char PA13MD:1;				/* PA13MD */
			unsigned char:1;					/* */
			unsigned char PA12MD:1;				/* PA12MD */
			unsigned char:1;					/* */
			unsigned char PA11MD:1;				/* PA11MD */
			unsigned char:1;					/* */
			unsigned char PA10MD:1;				/* PA10MD */
			unsigned char PA9MD:2;				/* PA9MD */
			unsigned char PA8MD:2;				/* PA8MD */
		} BIT;									/* */
	} PACRL1;									/* */
	union
	{											/* PACRL2 */
		unsigned short WORD;					/* Word Access */
		struct
		{										/* Byte Access */
			unsigned char H;					/* High */
			unsigned char L;					/* Low */
		} BYTE;									/* */
		struct
		{										/* Bit Access */
			unsigned char PA7MD:2;				/* PA7MD */
			unsigned char PA6MD:2;				/* PA6MD */
			unsigned char PA5MD:2;				/* PA5MD */
			unsigned char:1;					/* */
			unsigned char PA4MD:1;				/* PA4MD */
			unsigned char:1;					/* */
			unsigned char PA3MD:1;				/* PA3MD */
			unsigned char PA2MD:2;				/* PA2MD */
			unsigned char:1;					/* */
			unsigned char PA1MD:1;				/* PA1MD */
			unsigned char:1;					/* */
			unsigned char PA0MD:1;				/* PA0MD */
		} BIT;									/* */
	} PACRL2;									/* */
	char wk2[4];								/* */
	union
	{											/* PBIOR */
		unsigned short WORD;					/* Word Access */
		struct
		{										/* Byte Access */
			unsigned char H;					/* High */
			unsigned char L;					/* Low */
		} BYTE;									/* */
		struct
		{										/* Bit Access */
			unsigned char:6;					/* */
			unsigned char B9:1;					/* Bit 9 */
			unsigned char B8:1;					/* Bit 8 */
			unsigned char B7:1;					/* Bit 7 */
			unsigned char B6:1;					/* Bit 6 */
			unsigned char B5:1;					/* Bit 5 */
			unsigned char B4:1;					/* Bit 4 */
			unsigned char B3:1;					/* Bit 3 */
			unsigned char B2:1;					/* Bit 2 */
			unsigned char B1:1;					/* Bit 1 */
			unsigned char B0:1;					/* Bit 0 */
		} BIT;									/* */
	} PBIOR;									/* */
	union
	{											/* PCIOR */
		unsigned short WORD;					/* Word Access */
		struct
		{										/* Byte Access */
			unsigned char H;					/* High */
			unsigned char L;					/* Low */
		} BYTE;									/* */
		struct
		{										/* Bit Access */
			unsigned char B15:1;				/* Bit 15 */
			unsigned char B14:1;				/* Bit 14 */
			unsigned char B13:1;				/* Bit 13 */
			unsigned char B12:1;				/* Bit 12 */
			unsigned char B11:1;				/* Bit 11 */
			unsigned char B10:1;				/* Bit 10 */
			unsigned char B9:1;					/* Bit 9 */
			unsigned char B8:1;					/* Bit 8 */
			unsigned char B7:1;					/* Bit 7 */
			unsigned char B6:1;					/* Bit 6 */
			unsigned char B5:1;					/* Bit 5 */
			unsigned char B4:1;					/* Bit 4 */
			unsigned char B3:1;					/* Bit 3 */
			unsigned char B2:1;					/* Bit 2 */
			unsigned char B1:1;					/* Bit 1 */
			unsigned char B0:1;					/* Bit 0 */
		} BIT;									/* */
	} PCIOR;									/* */
	union
	{											/* PBCR1 */
		unsigned short WORD;					/* Word Access */
		struct
		{										/* Byte Access */
			unsigned char H;					/* High */
			unsigned char L;					/* Low */
		} BYTE;									/* */
		struct
		{										/* Bit Access */
			unsigned char:8;					/* */
			unsigned char:4;					/* */
			unsigned char PB9MD:2;				/* PB9MD */
			unsigned char PB8MD:2;				/* PB8MD */
		} BIT;									/* */
	} PBCR1;									/* */
	union
	{											/* PBCR2 */
		unsigned short WORD;					/* Word Access */
		struct
		{										/* Byte Access */
			unsigned char H;					/* High */
			unsigned char L;					/* Low */
		} BYTE;									/* */
		struct
		{										/* Bit Access */
			unsigned char PB7MD:2;				/* PB7MD */
			unsigned char PB6MD:2;				/* PB6MD */
			unsigned char PB5MD:2;				/* PB5MD */
			unsigned char PB4MD:2;				/* PB4MD */
			unsigned char PB3MD:2;				/* PB3MD */
			unsigned char PB2MD:2;				/* PB2MD */
			unsigned char:1;					/* */
			unsigned char PB1MD:1;				/* PB1MD */
			unsigned char:1;					/* */
			unsigned char PB0MD:1;				/* PB0MD */
		} BIT;									/* */
	} PBCR2;									/* */
	union
	{											/* PCCR */
		unsigned short WORD;					/* Word Access */
		struct
		{										/* Byte Access */
			unsigned char H;					/* High */
			unsigned char L;					/* Low */
		} BYTE;									/* */
		struct
		{										/* Bit Access */
			unsigned char PC15MD:1;				/* PC15MD */
			unsigned char PC14MD:1;				/* PC14MD */
			unsigned char PC13MD:1;				/* PC13MD */
			unsigned char PC12MD:1;				/* PC12MD */
			unsigned char PC11MD:1;				/* PC11MD */
			unsigned char PC10MD:1;				/* PC10MD */
			unsigned char PC9MD:1;				/* PC9MD */
			unsigned char PC8MD:1;				/* PC8MD */
			unsigned char PC7MD:1;				/* PC7MD */
			unsigned char PC6MD:1;				/* PC6MD */
			unsigned char PC5MD:1;				/* PC5MD */
			unsigned char PC4MD:1;				/* PC4MD */
			unsigned char PC3MD:1;				/* PC3MD */
			unsigned char PC2MD:1;				/* PC2MD */
			unsigned char PC1MD:1;				/* PC1MD */
			unsigned char PC0MD:1;				/* PC0MD */
		} BIT;									/* */
	} PCCR;										/* */
	char wk3[6];								/* */
	union
	{											/* PDIORH */
		unsigned short WORD;					/* Word Access */
		struct
		{										/* Byte Access */
			unsigned char H;					/* High */
			unsigned char L;					/* Low */
		} BYTE;									/* */
		struct
		{										/* Bit Access */
			unsigned char B31:1;				/* Bit 31 */
			unsigned char B30:1;				/* Bit 30 */
			unsigned char B29:1;				/* Bit 29 */
			unsigned char B28:1;				/* Bit 28 */
			unsigned char B27:1;				/* Bit 27 */
			unsigned char B26:1;				/* Bit 26 */
			unsigned char B25:1;				/* Bit 25 */
			unsigned char B24:1;				/* Bit 24 */
			unsigned char B23:1;				/* Bit 23 */
			unsigned char B22:1;				/* Bit 22 */
			unsigned char B21:1;				/* Bit 21 */
			unsigned char B20:1;				/* Bit 20 */
			unsigned char B19:1;				/* Bit 19 */
			unsigned char B18:1;				/* Bit 18 */
			unsigned char B17:1;				/* Bit 17 */
			unsigned char B16:1;				/* Bit 16 */
		} BIT;									/* */
	} PDIORH;									/* */
	union
	{											/* PDIORL */
		unsigned short WORD;					/* Word Access */
		struct
		{										/* Byte Access */
			unsigned char H;					/* High */
			unsigned char L;					/* Low */
		} BYTE;									/* */
		struct
		{										/* Bit Access */
			unsigned char B15:1;				/* Bit 15 */
			unsigned char B14:1;				/* Bit 14 */
			unsigned char B13:1;				/* Bit 13 */
			unsigned char B12:1;				/* Bit 12 */
			unsigned char B11:1;				/* Bit 11 */
			unsigned char B10:1;				/* Bit 10 */
			unsigned char B9:1;					/* Bit 9 */
			unsigned char B8:1;					/* Bit 8 */
			unsigned char B7:1;					/* Bit 7 */
			unsigned char B6:1;					/* Bit 6 */
			unsigned char B5:1;					/* Bit 5 */
			unsigned char B4:1;					/* Bit 4 */
			unsigned char B3:1;					/* Bit 3 */
			unsigned char B2:1;					/* Bit 2 */
			unsigned char B1:1;					/* Bit 1 */
			unsigned char B0:1;					/* Bit 0 */
		} BIT;									/* */
	} PDIORL;									/* */
	union
	{											/* PDCRH1 */
		unsigned short WORD;					/* Word Access */
		struct
		{										/* Byte Access */
			unsigned char H;					/* High */
			unsigned char L;					/* Low */
		} BYTE;									/* */
		struct
		{										/* Bit Access */
			unsigned char PD31MD:2;				/* PD31MD */
			unsigned char PD30MD:2;				/* PD30MD */
			unsigned char PD29MD:2;				/* PD29MD */
			unsigned char PD28MD:2;				/* PD28MD */
			unsigned char PD27MD:2;				/* PD27MD */
			unsigned char PD26MD:2;				/* PD26MD */
			unsigned char PD25MD:2;				/* PD25MD */
			unsigned char PD24MD:2;				/* PD24MD */
		} BIT;									/* */
	} PDCRH1;									/* */
	union
	{											/* PDCRH2 */
		unsigned short WORD;					/* Word Access */
		struct
		{										/* Byte Access */
			unsigned char H;					/* High */
			unsigned char L;					/* Low */
		} BYTE;									/* */
		struct
		{										/* Bit Access */
			unsigned char PD23MD:2;				/* PD23MD */
			unsigned char PD22MD:2;				/* PD22MD */
			unsigned char PD21MD:2;				/* PD21MD */
			unsigned char PD20MD:2;				/* PD20MD */
			unsigned char PD19MD:2;				/* PD19MD */
			unsigned char PD18MD:2;				/* PD18MD */
			unsigned char PD17MD:2;				/* PD17MD */
			unsigned char PD16MD:2;				/* PD16MD */
		} BIT;									/* */
	} PDCRH2;									/* */
	union
	{											/* PDCRL */
		unsigned short WORD;					/* Word Access */
		struct
		{										/* Byte Access */
			unsigned char H;					/* High */
			unsigned char L;					/* Low */
		} BYTE;									/* */
		struct
		{										/* Bit Access */
			unsigned char PD15MD:1;				/* PD15MD */
			unsigned char PD14MD:1;				/* PD14MD */
			unsigned char PD13MD:1;				/* PD13MD */
			unsigned char PD12MD:1;				/* PD12MD */
			unsigned char PD11MD:1;				/* PD11MD */
			unsigned char PD10MD:1;				/* PD10MD */
			unsigned char PD9MD:1;				/* PD9MD */
			unsigned char PD8MD:1;				/* PD8MD */
			unsigned char PD7MD:1;				/* PD7MD */
			unsigned char PD6MD:1;				/* PD6MD */
			unsigned char PD5MD:1;				/* PD5MD */
			unsigned char PD4MD:1;				/* PD4MD */
			unsigned char PD3MD:1;				/* PD3MD */
			unsigned char PD2MD:1;				/* PD2MD */
			unsigned char PD1MD:1;				/* PD1MD */
			unsigned char PD0MD:1;				/* PD0MD */
		} BIT;									/* */
	} PDCRL;									/* */
	char wk4[6];								/* */
	union
	{											/* PEIOR */
		unsigned short WORD;					/* Word Access */
		struct
		{										/* Byte Access */
			unsigned char H;					/* High */
			unsigned char L;					/* Low */
		} BYTE;									/* */
		struct
		{										/* Bit Access */
			unsigned char B15:1;				/* Bit 15 */
			unsigned char B14:1;				/* Bit 14 */
			unsigned char B13:1;				/* Bit 13 */
			unsigned char B12:1;				/* Bit 12 */
			unsigned char B11:1;				/* Bit 11 */
			unsigned char B10:1;				/* Bit 10 */
			unsigned char B9:1;					/* Bit 9 */
			unsigned char B8:1;					/* Bit 8 */
			unsigned char B7:1;					/* Bit 7 */
			unsigned char B6:1;					/* Bit 6 */
			unsigned char B5:1;					/* Bit 5 */
			unsigned char B4:1;					/* Bit 4 */
			unsigned char B3:1;					/* Bit 3 */
			unsigned char B2:1;					/* Bit 2 */
			unsigned char B1:1;					/* Bit 1 */
			unsigned char B0:1;					/* Bit 0 */
		} BIT;									/* */
	} PEIOR;									/* */
	char wk5[2];								/* */
	union
	{											/* PECR1 */
		unsigned short WORD;					/* Word Access */
		struct
		{										/* Byte Access */
			unsigned char H;					/* High */
			unsigned char L;					/* Low */
		} BYTE;									/* */
		struct
		{										/* Bit Access */
			unsigned char PE15MD:2;				/* PE15MD */
			unsigned char PE14MD:2;				/* PE14MD */
			unsigned char PE13MD:2;				/* PE13MD */
			unsigned char:1;					/* */
			unsigned char PE12MD:1;				/* PE12MD */
			unsigned char:1;					/* */
			unsigned char PE11MD:1;				/* PE11MD */
			unsigned char:1;					/* */
			unsigned char PE10MD:1;				/* PE10MD */
			unsigned char:1;					/* */
			unsigned char PE9MD:1;				/* PE9MD */
			unsigned char:1;					/* */
			unsigned char PE8MD:1;				/* PE8MD */
		} BIT;									/* */
	} PECR1;									/* */
	union
	{											/* PECR2 */
		unsigned short WORD;					/* Word Access */
		struct
		{										/* Byte Access */
			unsigned char H;					/* High */
			unsigned char L;					/* Low */
		} BYTE;									/* */
		struct
		{										/* Bit Access */
			unsigned char:1;					/* */
			unsigned char PE7MD:1;				/* PE7MD */
			unsigned char:1;					/* */
			unsigned char PE6MD:1;				/* PE6MD */
			unsigned char:1;					/* */
			unsigned char PE5MD:1;				/* PE5MD */
			unsigned char:1;					/* */
			unsigned char PE4MD:1;				/* PE4MD */
			unsigned char PE3MD:2;				/* PE3MD */
			unsigned char PE2MD:2;				/* PE2MD */
			unsigned char PE1MD:2;				/* PE1MD */
			unsigned char PE0MD:2;				/* PE0MD */
		} BIT;									/* */
	} PECR2;									/* */
	char wk6[12];								/* */
	union
	{											/* IFCR */
		unsigned short WORD;					/* Word Access */
		struct
		{										/* Byte Access */
			unsigned char H;					/* High */
			unsigned char L;					/* Low */
		} BYTE;									/* */
		struct
		{										/* Bit Access */
			unsigned char:8;					/* */
			unsigned char:4;					/* */
			unsigned char IRQMD32:2;			/* IRQMD3,2 */
			unsigned char IRQMD10:2;			/* IRQMD1,0 */
		} BIT;									/* */
	} IFCR;										/* */
};												/* */
struct st_cmt
{												/* struct CMT */
	union
	{											/* CMSTR */
		unsigned short WORD;					/* Word Access */
		struct
		{										/* Bit Access */
			unsigned char:8;					/* */
			unsigned char:6;					/* */
			unsigned char STR1:1;				/* STR1 */
			unsigned char STR0:1;				/* STR0 */
		} BIT;									/* */
	} CMSTR;									/* */
};												/* */
struct st_cmt0
{												/* struct CMT0 */
	union
	{											/* CMCSR */
		unsigned short WORD;					/* Word Access */
		struct
		{										/* Byte Access */
			unsigned char:8;					/* */
			unsigned char CMF:1;				/* CMF */
			unsigned char CMIE:1;				/* CMIE */
			unsigned char:4;					/* */
			unsigned char CKS:2;				/* CKS */
		} BIT;									/* */
	} CMCSR;									/* */
	unsigned short CMCNT;						/* CMCNT */
	unsigned short CMCOR;						/* CMCOR */
};												/* */
struct st_ad0
{												/* struct A/D0 */
	union
	{											/* ADDRA */
		unsigned short WORD;					/* Word Access */
		unsigned char BYTE;						/* Byte Access */
	} ADDRA;									/* */
	union
	{											/* ADDRB */
		unsigned short WORD;					/* Word Access */
		unsigned char BYTE;						/* Byte Access */
	} ADDRB;									/* */
	union
	{											/* ADDRC */
		unsigned short WORD;					/* Word Access */
		unsigned char BYTE;						/* Byte Access */
	} ADDRC;									/* */
	union
	{											/* ADDRD */
		unsigned short WORD;					/* Word Access */
		unsigned char BYTE;						/* Byte Access */
	} ADDRD;									/* */
	char wk1[8];								/* */
	union
	{											/* ADCSR */
		unsigned char BYTE;						/* Byte Access */
		struct
		{										/* Bit Access */
			unsigned char ADF:1;				/* ADF */
			unsigned char ADIE:1;				/* ADIE */
			unsigned char ADST:1;				/* ADST */
			unsigned char SCAN:1;				/* SCAN */
			unsigned char CKS:1;				/* CKS */
			unsigned char CH:3;					/* CH */
		} BIT;									/* */
	} ADCSR;									/* */
	char wk2;									/* */
	union
	{											/* ADCR */
		unsigned char BYTE;						/* Byte Access */
		struct
		{										/* Bit Access */
			unsigned char TRGE:1;				/* TRGE */
		} BIT;									/* */
	} ADCR;										/* */
};												/* */
struct st_ad1
{												/* struct A/D1 */
	union
	{											/* ADDRA */
		unsigned short WORD;					/* Word Access */
		unsigned char BYTE;						/* Byte Access */
	} ADDRA;									/* */
	union
	{											/* ADDRB */
		unsigned short WORD;					/* Word Access */
		unsigned char BYTE;						/* Byte Access */
	} ADDRB;									/* */
	union
	{											/* ADDRC */
		unsigned short WORD;					/* Word Access */
		unsigned char BYTE;						/* Byte Access */
	} ADDRC;									/* */
	union
	{											/* ADDRD */
		unsigned short WORD;					/* Word Access */
		unsigned char BYTE;						/* Byte Access */
	} ADDRD;									/* */
	char wk1;									/* */
	union
	{											/* ADCSR */
		unsigned char BYTE;						/* Byte Access */
		struct
		{										/* Bit Access */
			unsigned char ADF:1;				/* ADF */
			unsigned char ADIE:1;				/* ADIE */
			unsigned char ADST:1;				/* ADST */
			unsigned char SCAN:1;				/* SCAN */
			unsigned char CKS:1;				/* CKS */
			unsigned char CH:3;					/* CH */
		} BIT;									/* */
	} ADCSR;									/* */
	char wk2;									/* */
	union
	{											/* ADCR */
		unsigned char BYTE;						/* Byte Access */
		struct
		{										/* Bit Access */
			unsigned char TRGE:1;				/* TRGE */
		} BIT;									/* */
	} ADCR;										/* */
};												/* */
struct st_flash
{												/* struct FLASH */
	union
	{											/* FLMCR1 */
		unsigned char BYTE;						/* Byte Access */
		struct
		{										/* Bit Access */
			unsigned char FWE:1;				/* FWE */
			unsigned char SWE:1;				/* SWE */
			unsigned char ESU1:1;				/* ESU1 */
			unsigned char PSU1:1;				/* PSU1 */
			unsigned char EV1:1;				/* EV1 */
			unsigned char PV1:1;				/* PV1 */
			unsigned char E1:1;					/* E1 */
			unsigned char P1:1;					/* P1 */
		} BIT;									/* */
	} FLMCR1;									/* */
	union
	{											/* FLMCR2 */
		unsigned char BYTE;						/* Byte Access */
		struct
		{										/* Bit Access */
			unsigned char FLER:1;				/* FLER */
			unsigned char:1;					/* */
			unsigned char ESU2:1;				/* ESU2 */
			unsigned char PSU2:1;				/* PSU2 */
			unsigned char EV2:1;				/* EV2 */
			unsigned char PV2:1;				/* PV2 */
			unsigned char E2:1;					/* E2 */
			unsigned char P2:1;					/* P2 */
		} BIT;									/* */
	} FLMCR2;									/* */
	union
	{											/* EBR1 */
		unsigned char BYTE;						/* Byte Access */
		struct
		{										/* Bit Access */
			unsigned char:4;					/* */
			unsigned char EB3:1;				/* EB3 */
			unsigned char EB2:1;				/* EB2 */
			unsigned char EB1:1;				/* EB1 */
			unsigned char EB0:1;				/* EB0 */
		} BIT;									/* */
	} EBR1;										/* */
	union
	{											/* EBR2 */
		unsigned char BYTE;						/* Byte Access */
		struct
		{										/* Bit Access */
			unsigned char EB11:1;				/* EB11 */
			unsigned char EB10:1;				/* EB10 */
			unsigned char EB9:1;				/* EB9 */
			unsigned char EB8:1;				/* EB8 */
			unsigned char EB7:1;				/* EB7 */
			unsigned char EB6:1;				/* EB6 */
			unsigned char EB5:1;				/* EB5 */
			unsigned char EB4:1;				/* EB4 */
		} BIT;									/* */
	} EBR2;										/* */
	char wk[165];								/* */
	union
	{											/* RAMER */
		unsigned char BYTE;						/* Byte Access */
		struct
		{										/* Bit Access */
			unsigned char:5;					/* */
			unsigned char RAMS:1;				/* RAMS */
			unsigned char RAM:2;				/* RAM */
		} BIT;									/* */
	} RAMER;									/* */
};												/* */
struct st_ubc
{												/* struct UBC */
	void *UBAR;									/* UBAR */
	unsigned int UBAMR;							/* UBAMR */
	union
	{											/* UBBR */
		unsigned short WORD;					/* Word Access */
		struct
		{										/* Bit Access */
			unsigned char:8;					/* */
			unsigned char CP:2;					/* CP */
			unsigned char ID:2;					/* ID */
			unsigned char RW:2;					/* RW */
			unsigned char SZ:2;					/* SZ */
		} BIT;									/* */
	} UBBR;										/* */
};												/* */
union un_wdt
{												/* union WDT */
	struct
	{											/* Read Access */
		union
		{										/* TCSR */
			unsigned char BYTE;					/* Byte Access */
			struct
			{									/* Bit Access */
				unsigned char OVF:1;			/* OVF */
				unsigned char WTIT:1;			/* WT/IT */
				unsigned char TME:1;			/* TME */
				unsigned char:2;				/* */
				unsigned char CKS:3;			/* CKS */
			} BIT;								/* */
		} TCSR;									/* */
		unsigned char TCNT;						/* TCNT */
		char wk;								/* */
		union
		{										/* RSTCSR */
			unsigned char BYTE;					/* Byte Access */
			struct
			{									/* */
				unsigned char WRST:1;			/* WSRT */
				unsigned char RSTE:1;			/* RSTE */
				unsigned char RSTS:1;			/* RSTS */
			} BIT;								/* */
		} RSTCSR;								/* */
	} READ;										/* */
	struct
	{											/* Write Access */
		unsigned short TCSR;					/* TCSR/TCNT */
		unsigned short RSTCSR;					/* RSTCSR */
	} WRITE;									/* */
};												/* */
union un_sbycr
{												/* union SBYCR */
	unsigned char BYTE;							/* Byte Access */
	struct
	{											/* Bit Access */
		unsigned char SBY:1;					/* SBY */
		unsigned char HIZ:1;					/* HIZ */
	} BIT;										/* */
};												/* */
struct st_bsc
{												/* struct BSC */
	union
	{											/* BCR1 */
		unsigned short WORD;					/* Word Access */
		struct
		{										/* Bit Access */
			unsigned char:2;					/* */
			unsigned char MTURWE:1;				/* MTURWE */
			unsigned char:4;					/* */
			unsigned char IOE:1;				/* IOE */
			unsigned char A3LG:1;				/* A3LG */
			unsigned char A2LG:1;				/* A2LG */
			unsigned char A1LG:1;				/* A1LG */
			unsigned char A0LG:1;				/* A0LG */
			unsigned char A3SZ:1;				/* A3SZ */
			unsigned char A2SZ:1;				/* A2SZ */
			unsigned char A1SZ:1;				/* A1SZ */
			unsigned char A0SZ:1;				/* A0SZ */
		} BIT;									/* */
	} BCR1;										/* */
	union
	{											/* BCR2 */
		unsigned short WORD;					/* Word Access */
		struct
		{										/* Bit Access */
			unsigned char IW3:2;				/* IW3 */
			unsigned char IW2:2;				/* IW2 */
			unsigned char IW1:2;				/* IW1 */
			unsigned char IW0:2;				/* IW0 */
			unsigned char CW3:1;				/* CW3 */
			unsigned char CW2:1;				/* CW2 */
			unsigned char CW1:1;				/* CW1 */
			unsigned char CW0:1;				/* CW0 */
			unsigned char SW3:1;				/* SW3 */
			unsigned char SW2:1;				/* SW2 */
			unsigned char SW1:1;				/* SW1 */
			unsigned char SW0:1;				/* SW0 */
		} BIT;									/* */
	} BCR2;										/* */
	union
	{											/* WCR1 */
		unsigned short WORD;					/* Word Access */
		struct
		{										/* Bit Access */
			unsigned char W3:4;					/* W3 */
			unsigned char W2:4;					/* W2 */
			unsigned char W1:4;					/* W1 */
			unsigned char W0:4;					/* W0 */
		} BIT;									/* */
	} WCR1;										/* */
	union
	{											/* WCR2 */
		unsigned short WORD;					/* Word Access */
		struct
		{										/* Bit Access */
			unsigned char:8;					/* */
			unsigned char:2;					/* */
			unsigned char DDW:2;				/* DDW */
			unsigned char DSW:4;				/* DSW */
		} BIT;									/* */
	} WCR2;										/* */
	char wk[2];									/* */
	union
	{											/* DCR */
		unsigned short WORD;					/* Word Access */
		struct
		{										/* Bit Access */
			unsigned char TPC:1;				/* TPC */
			unsigned char RCD:1;				/* RCD */
			unsigned char TRAS:2;				/* TRAS */
			unsigned char DWW:2;				/* DWW */
			unsigned char DWR:2;				/* DWR */
			unsigned char DIW:1;				/* DIW */
			unsigned char:1;					/* */
			unsigned char BE:1;					/* BE */
			unsigned char RASD:1;				/* RASD */
			unsigned char SZ:2;					/* SZ */
			unsigned char AMX:2;				/* AMX */
		} BIT;									/* */
	} DCR;										/* */
	union
	{											/* RTCSR */
		unsigned short WORD;					/* Word Access */
		struct
		{										/* Bit Access */
			unsigned char:8;					/* */
			unsigned char:1;					/* */
			unsigned char CMF:1;				/* CMF */
			unsigned char CMIE:1;				/* CMIE */
			unsigned char CKS:3;				/* CKS */
			unsigned char RFSH:1;				/* RFSH */
			unsigned char RMD:1;				/* RMD */
		} BIT;									/* */
	} RTCSR;									/* */
	unsigned short RTCNT;						/* RTCNT */
	unsigned short RTCOR;						/* RTCOR */
};												/* */
struct st_dmac
{												/* struct DMAC */
	union
	{											/* DMAOR */
		unsigned short WORD;					/* Word Access */
		struct
		{										/* Bit Access */
			unsigned short:6;					/* */
			unsigned short PR:2;				/* PR */
			unsigned short:5;					/* */
			unsigned short AE:1;				/* AE */
			unsigned short NMIF:1;				/* NMIF */
			unsigned short DME:1;				/* DME */
		} BIT;									/* */
	} DMAOR;									/* */
};												/* */
struct st_dmac0
{												/* struct DMAC0 */
	void *SAR;									/* SAR */
	void *DAR;									/* DAR */
	unsigned int DMATCR;						/* DMATCR */
	union
	{											/* CHCR */
		unsigned int LONG;						/* Long Access */
		struct
		{										/* Word Access */
			unsigned short H;					/* High Word */
			unsigned short L;					/* Low Word */
		} WORD;									/* */
		struct
		{										/* Bit Access */
			unsigned short:13;					/* */
			unsigned short RL:1;				/* RL */
			unsigned short AM:1;				/* AM */
			unsigned short AL:1;				/* AL */
			unsigned short DM:2;				/* DM */
			unsigned short SM:2;				/* SM */
			unsigned short RS:4;				/* RS */
			unsigned short:1;					/* */
			unsigned short DS:1;				/* DS */
			unsigned short TM:1;				/* TM */
			unsigned short TS:2;				/* TS */
			unsigned short IE:1;				/* IE */
			unsigned short TE:1;				/* TE */
			unsigned short DE:1;				/* DE */
		} BIT;									/* */
	} CHCR;										/* */
};												/* */
struct st_dmac2
{												/* struct DMAC2 */
	void *SAR;									/* SAR */
	void *DAR;									/* DAR */
	unsigned int DMATCR;						/* DMATCR */
	union
	{											/* CHCR */
		unsigned int LONG;						/* Long Access */
		struct
		{										/* Word Access */
			unsigned short H;					/* High Word */
			unsigned short L;					/* Low Word */
		} WORD;									/* */
		struct
		{										/* Bit Access */
			unsigned short:12;					/* */
			unsigned short RO:1;				/* RO */
			unsigned short:3;					/* */
			unsigned short DM:2;				/* DM */
			unsigned short SM:2;				/* SM */
			unsigned short RS:4;				/* RS */
			unsigned short:2;					/* */
			unsigned short TM:1;				/* TM */
			unsigned short TS:2;				/* TS */
			unsigned short IE:1;				/* IE */
			unsigned short TE:1;				/* TE */
			unsigned short DE:1;				/* DE */
		} BIT;									/* */
	} CHCR;										/* */
};												/* */
struct st_dmac3
{												/* struct DMAC3 */
	void *SAR;									/* SAR */
	void *DAR;									/* DAR */
	unsigned int DMATCR;						/* DMATCR */
	union
	{											/* CHCR */
		unsigned int LONG;						/* Long Access */
		struct
		{										/* Word Access */
			unsigned short H;					/* High Word */
			unsigned short L;					/* Low Word */
		} WORD;									/* */
		struct
		{										/* Bit Access */
			unsigned short:11;					/* */
			unsigned short DI:1;				/* DI */
			unsigned short:4;					/* */
			unsigned short DM:2;				/* DM */
			unsigned short SM:2;				/* SM */
			unsigned short RS:4;				/* RS */
			unsigned short:2;					/* */
			unsigned short TM:1;				/* TM */
			unsigned short TS:2;				/* TS */
			unsigned short IE:1;				/* IE */
			unsigned short TE:1;				/* TE */
			unsigned short DE:1;				/* DE */
		} BIT;									/* */
	} CHCR;										/* */
};												/* */
struct st_dtc
{												/* struct DTC */
	union
	{											/* DTEA */
		unsigned char BYTE;						/* Byte Access */
		struct
		{										/* Bit Access */
			unsigned char TGI4A:1;				/* TGI4A */
			unsigned char TGI4B:1;				/* TGI4B */
			unsigned char TGI4C:1;				/* TGI4C */
			unsigned char TGI4D:1;				/* TGI4D */
			unsigned char TGI4V:1;				/* TGI4V */
			unsigned char TGI3A:1;				/* TGI3A */
			unsigned char TGI3B:1;				/* TGI3B */
			unsigned char TGI3C:1;				/* TGI3C */
		} BIT;									/* */
	} DTEA;										/* */
	union
	{											/* DTEB */
		unsigned char BYTE;						/* Byte Access */
		struct
		{										/* Bit Access */
			unsigned char TGI3D:1;				/* TGI3D */
			unsigned char TGI2A:1;				/* TGI2A */
			unsigned char TGI2B:1;				/* TGI2B */
			unsigned char TGI1A:1;				/* TGI1A */
			unsigned char TGI1B:1;				/* TGI1B */
			unsigned char TGI0A:1;				/* TGI0A */
			unsigned char TGI0B:1;				/* TGI0B */
			unsigned char TGI0C:1;				/* TGI0C */
		} BIT;									/* */
	} DTEB;										/* */
	union
	{											/* DTEC */
		unsigned char BYTE;						/* Byte Access */
		struct
		{										/* Bit Access */
			unsigned char TGI0D:1;				/* TGI0D */
			unsigned char ADI:1;				/* ADI */
			unsigned char IRQ0:1;				/* IRQ0 */
			unsigned char IRQ1:1;				/* IRQ1 */
			unsigned char IRQ2:1;				/* IRQ2 */
			unsigned char IRQ3:1;				/* IRQ3 */
			unsigned char IRQ4:1;				/* IRQ4 */
			unsigned char IRQ5:1;				/* IRQ5 */
		} BIT;									/* */
	} DTEC;										/* */
	union
	{											/* DTED */
		unsigned char BYTE;						/* Byte Access */
		struct
		{										/* Bit Access */
			unsigned char IRQ6:1;				/* IRQ6 */
			unsigned char IRQ7:1;				/* IRQ7 */
			unsigned char CMI0:1;				/* CMI0 */
			unsigned char CMI1:1;				/* CMI1 */
			unsigned char RXI0:1;				/* RXI0 */
			unsigned char TXI0:1;				/* TXI0 */
			unsigned char RXI1:1;				/* RXI1 */
			unsigned char TXI1:1;				/* TXI1 */
		} BIT;									/* */
	} DTED;										/* */
	union
	{											/* DTEE */
		unsigned char BYTE;						/* Byte Access */
		struct
		{										/* Bit Access */
			unsigned char CMI:1;				/* CMI */
		} BIT;									/* */
	} DTEE;										/* */
	char wk;									/* */
	union
	{											/* DTCSR */
		unsigned short WORD;					/* Word Access */
		struct
		{										/* Bit Access */
			unsigned char:5;					/* */
			unsigned char NMIF:1;				/* NMIF */
			unsigned char AE:1;					/* AE */
			unsigned char SWDTE:1;				/* SWDTE */
			unsigned char DTVEC:8;				/* DTVEC */
		} BIT;									/* */
	} DTCSR;									/* */
	unsigned short DTBR;						/* DTBR */
};												/* */
union un_ccr
{												/* union CCR */
	unsigned short WORD;						/* Word Access */
	struct
	{											/* Bit Access */
		unsigned char:8;						/* */
		unsigned char:3;						/* */
		unsigned char CEDRAM:1;					/* CEDRAM */
		unsigned char CECS3:1;					/* CECS3 */
		unsigned char CECS2:1;					/* CECS2 */
		unsigned char CECS1:1;					/* CECS1 */
		unsigned char CECS0:1;					/* CECS0 */
	} BIT;										/* */
};												/* */
#define SCI0  (*(volatile struct st_sci   *)0xFFFF81A0)	/* SCI0 Address */
#define SCI1  (*(volatile struct st_sci   *)0xFFFF81B0)	/* SCI1 Address */
#define MTU   (*(volatile struct st_mtu   *)0xFFFF820A)	/* MTU Address */
#define MTU0  (*(volatile struct st_mtu0  *)0xFFFF8260)	/* MTU0 Address */
#define MTU1  (*(volatile struct st_mtu1  *)0xFFFF8280)	/* MTU1 Address */
#define MTU2  (*(volatile struct st_mtu1  *)0xFFFF82A0)	/* MTU2 Address */
#define MTU3  (*(volatile struct st_mtu3  *)0xFFFF8200)	/* MTU3 Address */
#define MTU4  (*(volatile struct st_mtu4  *)0xFFFF8200)	/* MTU4 Address */
#define POE   (*(volatile struct st_poe   *)0xFFFF83C0)	/* POE Address */
#define INTC  (*(volatile struct st_intc  *)0xFFFF8348)	/* INTC Address */
#define PA    (*(volatile struct st_pa    *)0xFFFF8380)	/* PA Address */
#define PB    (*(volatile struct st_pb    *)0xFFFF8390)	/* PB Address */
#define PC    (*(volatile struct st_pc    *)0xFFFF8392)	/* PC Address */
#define PD    (*(volatile struct st_pd    *)0xFFFF83A0)	/* PD Address */
#define PE    (*(volatile struct st_pe    *)0xFFFF83B0)	/* PE Address */
#define PF    (*(volatile struct st_pf    *)0xFFFF83B3)	/* PF Address */
#define PFC   (*(volatile struct st_pfc   *)0xFFFF8384)	/* PFC Address */
#define CMT   (*(volatile struct st_cmt   *)0xFFFF83D0)	/* CMT Address */
#define CMT0  (*(volatile struct st_cmt0  *)0xFFFF83D2)	/* CMT0 Address */
#define CMT1  (*(volatile struct st_cmt0  *)0xFFFF83D8)	/* CMT1 Address */
#define AD0   (*(volatile struct st_ad0   *)0xFFFF8400)	/* A/D0 Address */
#define AD1   (*(volatile struct st_ad1   *)0xFFFF8408)	/* A/D1 Address */
#define FLASH (*(volatile struct st_flash *)0xFFFF8580)	/* FLASH Address */
#define UBC   (*(volatile struct st_ubc   *)0xFFFF8600)	/* UBC Address */
#define WDT   (*(volatile union  un_wdt   *)0xFFFF8610)	/* WDT Address */
#define SBYCR (*(volatile union  un_sbycr *)0xFFFF8614)	/* SBYCR Address */
#define BSC   (*(volatile struct st_bsc   *)0xFFFF8620)	/* BSC Address */
#define DMAC  (*(volatile struct st_dmac  *)0xFFFF86B0)	/* DMAC Address */
#define DMAC0 (*(volatile struct st_dmac0 *)0xFFFF86C0)	/* DMAC0 Address */
#define DMAC1 (*(volatile struct st_dmac0 *)0xFFFF86D0)	/* DMAC1 Address */
#define DMAC2 (*(volatile struct st_dmac2 *)0xFFFF86E0)	/* DMAC2 Address */
#define DMAC3 (*(volatile struct st_dmac3 *)0xFFFF86F0)	/* DMAC3 Address */
#define DTC   (*(volatile struct st_dtc   *)0xFFFF8700)	/* DTC Address */
#define CCR   (*(volatile union  un_ccr   *)0xFFFF8740)	/* CCR Address */

#endif
