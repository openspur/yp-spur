/* ------------------------------------------------------- */
/* SH2/7045でアナログ値を取得するための関数群 　　　 */
/* ------------------------------------------------------- */
#include<sh7040s.h>
#include<adc.h>
#include<sh-globals.h>

/*-----------------ADの関数-------------------*/
// //////////////////////// init AD //////////////////////////////////

void initAD(  )
{
	AD0.ADCSR.BIT.SCAN = 1;
	AD1.ADCSR.BIT.SCAN = 1;
	AD0.ADCSR.BIT.CH = 3;
	AD1.ADCSR.BIT.CH = 3;
	AD0.ADCSR.BIT.ADST = 1;
	AD1.ADCSR.BIT.ADST = 1;
}
