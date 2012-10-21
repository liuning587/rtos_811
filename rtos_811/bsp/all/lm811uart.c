/****************************************************************************
 *
 * Filename: lm811uart.c
 * Version: V1.0
 * Description:.
 *
******************************************************************************/

/*-----------------------------------------------------------------------------
 Section: Includes
 ----------------------------------------------------------------------------*/
#include <LM3Sxxx.h>
#include "lm811uart.h"
/*-----------------------------------------------------------------------------
 Section: Constant Definitions
 ----------------------------------------------------------------------------*/
/* None */

/*-----------------------------------------------------------------------------
 Section: Type Definitions
 ----------------------------------------------------------------------------*/
/* None */

/*-----------------------------------------------------------------------------
 Section: static Variable
 ----------------------------------------------------------------------------*/
/* None */

/*-----------------------------------------------------------------------------
 Section: static Function Prototypes
 ----------------------------------------------------------------------------*/
    
static int32_t  lm811UartTxStartup (SIO_CHAN * pSioChan);
static int32_t  lm811UartIoctl  (SIO_CHAN *pSioChan, uint32_t request, void *arg );
static uint32_t lm811UartOptSet    (LM811_CHAN * pChan,    uint32_t	newOpts);
static status_t lm811UartOpen(LM811_CHAN * pChan );
static status_t lm811UartHup(LM811_CHAN * pChan);
static status_t lm811UartSetNewBaudRate( LM811_CHAN * pChan,  uint32_t baud );
static bool_e lm811UartTxIsNull(LM811_CHAN * pChan);

static SIO_DRV_FUNCS lm811UartDrvFuncs =
{
    lm811UartIoctl,
    lm811UartTxStartup,
    NULL,
    NULL
};

/*------------------------------------------------------------------------------
Section: Function Definitions
------------------------------------------------------------------------------*/

void lm811UartDevInit(LM811_CHAN * pChan) {
    pChan->sio.pDrvFuncs = &lm811UartDrvFuncs;
    UARTIntEnable(pChan->baseregs, UART_INT_RX | UART_INT_RT);
    UARTFIFOLevelSet(pChan->baseregs, UART_FIFO_TX1_8, UART_FIFO_RX2_8);
	
}

/**
 ******************************************************************************
 * @brief      uart中断服务程序
 * @param[in]  None
 * @param[out] None
 * @retval     None
 *
 * @details
 *
 * @note
 ******************************************************************************
 */
void lm811UartInt
    (
    LM811_CHAN *	pChan		/* channel generating the interrupt */
    )
{
    uint8_t outchar;
	uint32_t    status_t;

    UARTRxErrorClear(pChan->baseregs);
    status_t = UARTIntStatus(pChan->baseregs, true);

    if( (status_t & UART_INT_RX) || (status_t & UART_INT_RT))
	{
		(pChan->sio.pDrvFuncs->putRcvChar) ( pChan->ttyno, HWREG(pChan ->baseregs  + UART_O_DR));
		UARTIntClear(pChan->baseregs, UART_INT_RT | UART_INT_RX);
		//if(status_t & (UART_INT_RX ))   HWREG(pChan ->baseregs + UART_O_ICR) &= ~UART_INT_RX;
		//if(status_t & (UART_INT_RT))  HWREG(pChan ->baseregs + UART_O_ICR) &= ~UART_INT_RT;
	}
    if( status_t & (UART_INT_TX) )
 	{
        while((HWREG(pChan ->baseregs + UART_O_FR) & UART_FR_TXFF))
        {	//等待uart缓存空闲
        }
        while(!(HWREG(pChan ->baseregs + UART_O_FR) & UART_FR_TXFF))
        {
            if((pChan->sio.pDrvFuncs->getTxChar) (pChan->ttyno, &outchar) != ERROR){
                HWREG(pChan ->baseregs + UART_O_DR) = outchar;
            }
            else{
                HWREG(pChan->baseregs	+ UART_O_IM) &= ~UART_INT_TX;
                break;
            }
        }
        HWREG(pChan ->baseregs + UART_O_ICR) &= ~UART_INT_TX;
    }
}



static int32_t lm811UartTxStartup
    (
    SIO_CHAN * pSioChan                 /* channel to start */
    )
{
#if 0

    //uint32_t status_t ;
	UINT8 outchar;
    LM811_CHAN * pChan = (LM811_CHAN *)pSioChan;
    
  
    //HWREG(pChan->baseregs  + UART_O_IM) |= UART_INT_TX;
    while( (pChan->sio.pDrvFuncs->getTxChar) (pChan->ttyno, &outchar) != ERROR )
	{
        while(HWREG(pChan ->baseregs  + UART_O_FR) & UART_FR_TXFF)
        {
        }

        HWREG(pChan ->baseregs + UART_O_DR) = outchar;
	}
#else

    uint8_t outchar = 0 ;
    LM811_CHAN * pChan = (LM811_CHAN *)pSioChan;

    while((HWREG(pChan ->baseregs + UART_O_FR) & UART_FR_TXFF))
    {}
    while(!(HWREG(pChan ->baseregs + UART_O_FR) & UART_FR_TXFF))
    {
       if((pChan->sio.pDrvFuncs->getTxChar) (pChan->ttyno, &outchar) != ERROR){
           HWREG(pChan ->baseregs + UART_O_DR) = outchar;
       }
       else{
           break;
       }
    }
    HWREG(pChan->baseregs	+ UART_O_IM) |= UART_INT_TX;

#endif
    return OK;

}


static status_t lm811UartSetNewBaudRate( LM811_CHAN * pChan,  uint32_t baud )
{

    uint32_t ulUARTClk = SysCtlClockGet();
    uint32_t ulDiv = 0;
    //
    // Is the required baud rate greater than the maximum rate supported
    // without the use of high speed mode?
    //
    if((baud * 16) > ulUARTClk)
    {
      //
      // Enable high speed mode.
      //
      HWREG(pChan->baseregs+ UART_O_CTL) |= UART_CTL_HSE;

      //
      // Half the supplied baud rate to compensate for enabling high speed
      // mode.  This allows the following code to be common to both cases.
      //
      baud /= 2;
    }
    else
    {
      //
      // Disable high speed mode.
      //
      HWREG(pChan->baseregs + UART_O_CTL) &= ~(UART_CTL_HSE);
    }

    //
    // Compute the fractional baud rate divider.
    //
    ulDiv = (((ulUARTClk * 8) / baud) + 1) / 2;

    //
    // Set the baud rate.
    //
    HWREG(pChan->baseregs + UART_O_IBRD) = ulDiv / 64;
    HWREG(pChan->baseregs + UART_O_FBRD) = ulDiv % 64;
    //
    // Clear the flags register.
    //
    HWREG(pChan->baseregs  + UART_O_FR) = 0;

    //
    // Start the UART.
    //

    lm811UartOpen(pChan);
    return OK;

}


static int32_t lm811UartIoctl
    (
    SIO_CHAN *	pSioChan,		/* device to control */
    uint32_t	request,		/* request code */
    void *	someArg			/* some argument */
    )
{
    LM811_CHAN *pChan = ( LM811_CHAN * )pSioChan;
    /* int32_t     oldlevel;		*//* current interrupt level mask */
    int32_t     arg = (int)someArg;
    switch (request)
    {
    case SIO_BAUD_SET:
        if (arg == 0)
    		return lm811UartHup (pChan);
    	/* Calculate the baud rate constant for the new baud rate */
    	switch(arg)
    	{
        case 300:
        case 600:
    	case 1200:
    	case 2400:
    	case 4800:
    	case 9600:
    	case 19200:
    	case 38400:
    	case 57600:
    	case 115200:
    	case 230400:
    	case 460800:
    		return ( lm811UartSetNewBaudRate( pChan, arg ) );
    	default:
    		return -5; //(EIO);
    	}	
    	
    	//return(OK);
    case SIO_HW_OPTS_SET:
        return ( lm811UartOptSet (pChan, arg) );
    case SIO_HUP:
        return ( lm811UartHup (pChan) );
    case SIO_OPEN:
    	return ( lm811UartOpen (pChan) ); /* always open */
    case SIO_TXISE:{
        *((bool_e *)someArg) = lm811UartTxIsNull(pChan);
        return OK;
    	}
    default:
        return -71; //(ENOSYS);
    }
    //return (ENOSYS);
}

static status_t lm811UartHup
    (
    LM811_CHAN * pChan 	/* pointer to channel */
    )
{
    //
    // Wait for end of TX.
    //
    while(HWREG(pChan->baseregs + UART_O_FR) & UART_FR_BUSY)
    {
    }

    //
    // Disable the FIFO.
    //
    //HWREG(HWREG(pChan->baseregs + UART_O_LCRH) &= ~(UART_LCRH_FEN);

    //
    // Disable the UART.
    //
    HWREG(pChan->baseregs + UART_O_CTL) &= ~(UART_CTL_UARTEN | UART_CTL_TXE|
                                    UART_CTL_RXE);
    return OK;
    
}    

static status_t lm811UartOpen
    (
    LM811_CHAN * pChan 	/* pointer to channel */
    )
{


    // Enable the FIFO.
    //
    //HWREG(pChan->baseregs + UART_O_LCRH) |= UART_LCRH_FEN;

    //
    // Enable RX, TX, and the UART.
    //
    HWREG((pChan->baseregs)  + UART_O_IFLS) = (1 << 3) | (0 << 0) ;
	HWREG((pChan->baseregs) + UART_O_LCRH) |= 1 << 4;
    HWREG(pChan->baseregs + UART_O_CTL) |= (UART_CTL_UARTEN |UART_CTL_TXE | UART_CTL_RXE);
    return OK;
}



static uint32_t lm811UartOptSet
    (
    LM811_CHAN * pChan,		/* channel */
    uint32_t	newOpts     /* new options */
    )
{

    lm811UartHup(pChan);
    //
    // Set parity, data length, and number of stop bits.
    //
    HWREG(pChan->baseregs + UART_O_LCRH) = newOpts;

    //
    // Clear the flags register.
    //
    HWREG(pChan->baseregs  + UART_O_FR) = 0;

    //
    // Start the UART.
    //
    lm811UartOpen(pChan);

    return (OK);
}
          

static bool_e lm811UartTxIsNull
    (
    LM811_CHAN * pChan 	/* pointer to channel */
    )
{
    if(HWREG(pChan ->baseregs + UART_O_FR) & UART_FR_TXFE) 
        return TRUE;
    else 
        return FALSE;
}

/*****************************End of lm811uart.c *****************************/
