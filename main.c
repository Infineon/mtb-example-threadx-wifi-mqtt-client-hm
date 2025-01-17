/*******************************************************************************
* File Name:   main.c
*
* Description: This is the source code for the MQTT client.
*
* Related Document: See README.md
*
*
*********************************************************************************
* Copyright 2021-2023, Cypress Semiconductor Corporation (an Infineon company) or
* an affiliate of Cypress Semiconductor Corporation.  All rights reserved.
*
* This software, including source code, documentation and related
* materials ("Software") is owned by Cypress Semiconductor Corporation
* or one of its affiliates ("Cypress") and is protected by and subject to
* worldwide patent protection (United States and foreign),
* United States copyright laws and international treaty provisions.
* Therefore, you may use this Software only as provided in the license
* agreement accompanying the software package from which you
* obtained this Software ("EULA").
* If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
* non-transferable license to copy, modify, and compile the Software
* source code solely for use in connection with Cypress's
* integrated circuit products.  Any reproduction, modification, translation,
* compilation, or representation of this Software except as specified
* above is prohibited without the express written permission of Cypress.
*
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
* reserves the right to make changes to the Software without notice. Cypress
* does not assume any liability arising out of the application or use of the
* Software or any product or circuit described in the Software. Cypress does
* not authorize its products for use in any products where a malfunction or
* failure of the Cypress product may reasonably be expected to result in
* significant property damage, injury or death ("High Risk Product"). By
* including Cypress's product in a High Risk Product, the manufacturer
* of such system or application assumes all risk of such use and in doing
* so agrees to indemnify Cypress against all liability.
*******************************************************************************/

/*******************************************************************************
* Header Files
*******************************************************************************/
#include "cyhal.h"
#include "cybsp.h"
#include "cy_retarget_io.h"
#include "mqtt_task.h"
#include "cyabs_rtos.h"
/*******************************************************************************
* Macros
*******************************************************************************/

static uint64_t mqtt_task_stack[MQTT_CLIENT_TASK_STACK_SIZE / 8];

/*******************************************************************************
* Global Variables
*******************************************************************************/
cy_thread_t mqtt_task_handle;

/*******************************************************************************
* Function Prototypes
*******************************************************************************/


/*******************************************************************************
* Function Definitions
*******************************************************************************/

/*******************************************************************************
* Function Name: main()
********************************************************************************
* Summary:
* This is the main function for CPU. It...
*    1. Initializes the BSP
*    2. Enables Global interrupt
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
int main(void)
{
    cy_rslt_t result;
    cyhal_clock_t obj;

    /* Initialize the device and board peripherals */
    result = cybsp_init();

    /* Board init failed. Stop program execution */
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }
    
    /* Update CPU clock frequency to CYW55913_CLOCK_FREQ HZ */
    if (CY_RSLT_SUCCESS == cyhal_clock_get(&obj, &CYHAL_CLOCK_RSC_CPU))
    {
        result = cyhal_clock_set_frequency(&obj, CYW55913_CLOCK_FREQ, NULL);
    }
    CY_ASSERT(result == CY_RSLT_SUCCESS);

    /* Enable global interrupts */
     __enable_irq();

    /* Initialize retarget-io to use the debug UART port */
    result = cy_retarget_io_init(CYBSP_DEBUG_UART_TX, CYBSP_DEBUG_UART_RX,
                                 CY_RETARGET_IO_BAUDRATE);
    /* retarget-io init failed. Stop program execution */
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    printf("\x1b[2J\x1b[;H");
    printf("============================================================\n");
    printf("Connectivity Example: MQTT Client with Hosted Mode support\n");
    printf("============================================================\n\n");

    result = cy_rtos_thread_create(&mqtt_task_handle,mqtt_client_task,"mqtt_client_task",&mqtt_task_stack,MQTT_CLIENT_TASK_STACK_SIZE,MQTT_CLIENT_TASK_PRIORITY,0);

    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

}

/* [] END OF FILE */
