/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "lcd.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <assert.h>
#include <stdbool.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

uint8_t current[5];
uint8_t rx_buff[5];
uint8_t button_val = 0;


void decrypt(uint8_t *tx_buff, size_t size, uint8_t *destination){
  uint8_t key_1[] = {34, 20, 41, 97, 62};
  uint8_t key_2[] = {27, 25, 18, 48, 49};
  uint8_t key_3[] = {35, 28, 24, 43, 21};

  for (size_t i = 0; i < size; i++) {
	  destination[i] = ~(tx_buff[i]); // NOR operation
  }

  for (size_t i = 0; i < size; i++) {
	  destination[i] = destination[i] ^ key_3[i];
  }
  for (size_t i = 0; i < size; i++) {
	  destination[i] = destination[i] ^ key_2[i];
  }

  for (size_t i = 0; i < size; i++) {
	  destination[i] = destination[i] ^ key_1[i];
  }
}

char* convert_to_char_array(uint8_t *int_buffer, size_t buffer_size) {
    // Dynamically allocate memory for the char array (+1 for null terminator)
    char *char_array = (char *)malloc((buffer_size + 1) * sizeof(char));
    if (char_array == NULL) {
        // If memory allocation fails, return NULL
        printf("Memory allocation failed!\n");
        return NULL;
    }

    // Convert each integer to a character and store in the char array
    for (size_t i = 0; i < buffer_size; i++) {
        char_array[i] = (char)int_buffer[i];  // Convert integer to char
    }

    char_array[buffer_size] = '\0';  // Null-terminate the string

    return char_array;  // Return the dynamically allocated char array
}

// Function to compare two strings (returning 0 for equal, positive for str1 > str2, negative for str1 < str2)
int compare(const char *str1, const char *str2) {
    size_t lenString1 = strlen(str1);
    size_t lenString2 = strlen(str2);
    size_t minString = (lenString1 < lenString2) ? lenString1 : lenString2;

    for (size_t i = 0; i < minString; i++) {
        if (str1[i] > str2[i]) {
            return 1;
        } else if (str1[i] < str2[i]) {
            return -1;
        }
    }

    if (lenString1 > lenString2) {
        return 1;
    } else if (lenString1 == lenString2) {
        return 0;
    } else {
        return -1;
    }
}

// Function to assign a string (copy str2 to str1)
void assign(char *str1, const char *str2) {
    size_t i = 0;
    while (str2[i] != '\0') {
        str1[i] = str2[i];
        i++;
    }
    str1[i] = '\0'; // Null-terminate str1
}

// Function to insert a string into the array if it's not already present
void insert(char ***array, size_t *capacity, char *value) {
    // Check for duplicates
    for (size_t i = 0; i < *capacity; i++) {
        if (compare((*array)[i], value) == 0) {
            return;  // Duplicate found, do not insert
        }
    }

    // Reallocate memory for the new string
    *array = realloc(*array, (*capacity + 1) * sizeof(char *));
    if (*array == NULL) {
        // Memory allocation failed, handle error
        return;
    }

    // Allocate memory for the new string and assign it
    (*array)[*capacity] = malloc(strlen(value) + 1);
    if ((*array)[*capacity] == NULL) {
        // Memory allocation failed, handle error
        return;
    }

    assign((*array)[*capacity], value);  // Copy the string into the array
    (*capacity)++;  // Increase the array capacity
}

char **array = NULL;
size_t capacity = 0;

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

  // Lcd_PortType ports[] = { D4_GPIO_Port, D5_GPIO_Port, D6_GPIO_Port, D7_GPIO_Port };
    Lcd_PortType ports[] = { GPIOA, GPIOA, GPIOB, GPIOB };
    // Lcd_PinType pins[] = {D4_Pin, D5_Pin, D6_Pin, D7_Pin};
    Lcd_PinType pins[] = {GPIO_PIN_6, GPIO_PIN_5, GPIO_PIN_9, GPIO_PIN_8};
    Lcd_HandleTypeDef lcd;
    // Lcd_create(ports, pins, RS_GPIO_Port, RS_Pin, EN_GPIO_Port, EN_Pin, LCD_4_BIT_MODE);
    lcd = Lcd_create(ports, pins, GPIOB, GPIO_PIN_6, GPIOA, GPIO_PIN_7, LCD_4_BIT_MODE);
    /*Lcd_cursor(&lcd, 0,1);
    Lcd_string(&lcd, "Nizar Mohideen");
      for ( int x = 1; x <= 200 ; x++ )
      {
        Lcd_cursor(&lcd, 1,7);
        Lcd_int(&lcd, x);
        HAL_Delay (1000);
      }*/

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
    button_val = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);
    if (button_val == 1) {
	      Lcd_cursor(&lcd, 1, 1);
	    Lcd_string(&lcd, "Students...");
    	for (int i = 0; i < capacity; i++){
    		  	    	Lcd_cursor(&lcd, 0,1);
    		  	    	Lcd_string(&lcd, array[i]);
    		  	    	HAL_Delay (2000);
    		  	      }

    	Lcd_cursor(&lcd, 1,1);
    	Lcd_string(&lcd, "End of List");
    }
    /* USER CODE BEGIN 3 */

	  if(HAL_UART_Receive(&huart1, rx_buff, 5, 1000)==HAL_OK) //if transfer is successful
	  	    {
	  	      __NOP(); //You need to toggle a breakpoint on this line!

	  	      Lcd_cursor(&lcd, 1, 1);
	  	    Lcd_string(&lcd, "Students...");


	  	      decrypt(rx_buff, 5, current);
	  	      char *char_array1 = convert_to_char_array(current, sizeof(current));
	  	      insert(&array, &capacity, char_array1);  // Insert "Hello"
	  	      free(char_array1);
	  	      for (int i = 0; i < capacity; i++){
	  	    	Lcd_cursor(&lcd, 0,1);
	  	    	Lcd_string(&lcd, array[i]);
	  	    	HAL_Delay (2000);
	  	      }

	  	   	Lcd_cursor(&lcd, 1,1);
	  	    	Lcd_string(&lcd, "End of List");

	  	    } else {
	  	      __NOP();
	  	    }
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6|GPIO_PIN_8|GPIO_PIN_9, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PA0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PA5 PA6 PA7 */
  GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PB4 */
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF9_I2C3;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PB6 PB8 PB9 */
  GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
