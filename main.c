#include "stm32f4xx.h"
#include "main.h"
#include "mqtt.h"
#include "stm32f4x7_eth.h"
#include "stm32f4xx_rcc.h"
#include "netconf.h"
#include "string.h"
#include "mqttApp.h"


#define MESAJ_SEND_PERIOD_MS	3000
#define SYSTEMTICK_PERIOD_MS  	10
__IO uint32_t LocalTime = 0; /* this variable is used to create a time reference incremented by 10ms */
uint32_t timingdelay;
//volatile uint32_t msTime;
volatile uint32_t msTick = MESAJ_SEND_PERIOD_MS;

char tmp = 0;

struct ip_addr serverIp;
mqtt_client_t static_client;
int main(void)
{
	int ethernetok = 0;

	IP4_ADDR(&serverIp, 192,168,1,40);

	SystemInit();

	ethernetok = ETH_BSP_Config();


	if (ethernetok) {
		/* Initilaize the LwIP stack */
		LwIP_Init();
	}

	example_do_connect(&static_client);

    while(1)
    {

    	if (ethernetok) {
    	    // check if any packet received //
    	    if (ETH_CheckFrameReceived()) {
    	      // process received ethernet packet //
    	      LwIP_Pkt_Handle();
    	    }
    	    // handle periodic timers for LwIP //
    	    LwIP_Periodic_Handle(LocalTime);

    	    if(msTick==1) {
    	    	tmp = mqtt_client_is_connected(&static_client);
    	    	msTick = MESAJ_SEND_PERIOD_MS;
    	    	if(tmp)
    	    		example_publish(&static_client,0);

    	    }

    	}
    }
}



/**
  * @brief  Inserts a delay time.
  * @param  nCount: number of 10ms periods to wait for.
  * @retval None
  */
void Delay(uint32_t nCount)
{
  /* Capture the current local time */
  timingdelay = LocalTime + nCount;

  /* wait until the desired delay finish */
  while(timingdelay > LocalTime)
  {
  }
}

/**
  * @brief  Updates the system local time
  * @param  None
  * @retval None
  */
void Time_Update(void)
{
  LocalTime += SYSTEMTICK_PERIOD_MS;
 // msTime++;
  msTick--;

}

