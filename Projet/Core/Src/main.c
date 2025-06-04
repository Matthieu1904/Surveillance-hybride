/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
ADC_HandleTypeDef hadc1;

I2C_HandleTypeDef hi2c1;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim5;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM2_Init(void);
static void MX_ADC1_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM5_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

// DATE: 22/04/2021
/*
 * Afin de pouvoir debugger facilement des informations, on pourra re-définir l’implémentation
d’une fonction printf redirigeant les caractères vers l’UART et non la sortie standard console
en C
 */

#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small
printf
 set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/*
 * Ajouter ensuite le code lié à l’implémentation de cette fonction
 */

PUTCHAR_PROTOTYPE
{
/* Place your implementation of fputc here */
/* e.g. write a character to the USART2 and Loop until the end of transmission */
HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 0xFFFF);
return ch;
}

void getTempI2C(void){
	/*
	   * Modifier votre programme principal pour permettre de lire la température (registre à l’adresse 0x05).
	   * Pour cela, vous privilégierez les fonctions suivantes :
	   * 	HAL_I2C_Master_Transmit() ; //Ecriture de données
	   * 	HAL_I2C_Mem_Read() ; //Lecture de données
	   */

	  // Transmettre, en mode Maître, une quantité de donnée en mode block
	  /*
	   * HAL_I2C_Master_Transmit(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout)
	   */

	/*
	 * L'adresse du Capteur de Temperature MCP9808 est '0011,A2,A1,A0' en binaire
	 * On met A2, A1 et A0 à '0'; ce qui donne 0011000
	 *
	 * En Hexadecimal, la valeur 0011000 est égale à 18
	 */
	 uint8_t DevAddress = 0x18<<1;	// Adresse de l'appareil (du capteur de température) avec declage vers la gauche pour le placer en 8 bits
	 uint8_t RegistreAdress = 0x05;	// Adresse du Registre contenant la Température
	 // uint8_t RegistreAdress = 0x07;
	 uint8_t pData[2];				// Ce que je transmet
	pData[0] = RegistreAdress;		// Stockage de l'Adresse du Registre dans la 1ère case de pData

	/*
	 * Pour hi2c1		I2C_HandleTypeDef qui contient les informations de configuration pour le I2C spécifié.
	 * Pour 1			On n'envoit que la première case de pData
	 */
	HAL_StatusTypeDef HALstage;
	// HALstage = HAL_I2C_Master_Transmit(&hi2c1, DevAddress, pData, 1, HAL_MAX_DELAY);

	 // Si la Transmission s'est bien passée
	// if(HALstage==HAL_OK){
		  /* Lecture du registre à l'adresse 0x05
		   *
		   * HAL_StatusTypeDef HAL_I2C_Mem_Read(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout)
		   *
		   * Pour "I2C_MEMADD_SIZE_8BIT"
		   *	On donne la Taille sur 8 bits de l'I2C
		   * Pour "Size"
		   * 	Dans pData, on récupére 2 éléments;
		   * 		1) MSB
		   * 		2) LSB
		   */

		/*
			 * L'adresse du Capteur de Temperature MCP9808 est '0011,A2,A1,A0' en binaire
			 * On met A2, A1 et A0 à '0'; ce qui donne 0011000
			 *
			 * En Hexadecimal, la valeur 0011000 est égale à 18
		*/

		/*
			uint8_t DevAddress = 0x18<<1;	// Adresse de l'appareil (du capteur de température) avec declage vers la gauche pour le placer en 8 bits
			uint8_t RegistreAdress = 0x05;	// Adresse du Registre contenant la Température
			// uint8_t RegistreAdress = 0x07;
			uint8_t pData[2];				// Tableau qui contiendra les données récupérées
			pData[0] = RegistreAdress;		// Stockage de l'Adresse du Registre dans la 1ère case de pData
		*/

		/*
			* Pour hi2c1		I2C_HandleTypeDef qui contient les informations de configuration pour le I2C spécifié.
			* Pour 2			On cherche à récupérer 2 élément (dans les 2 cases de pData)
		*/
		 HALstage=HAL_I2C_Mem_Read(&hi2c1, DevAddress, RegistreAdress, I2C_MEMADD_SIZE_8BIT, pData, 2, HAL_MAX_DELAY);

		 // Si la Reception s'est bien passée
		 if(HALstage==HAL_OK){
			/*
			* A la fin de HAL_I2C_Mem_Read
			* Dans	pData[0], on aura MSB
			*		pData[1], on aura LSB
			*/

			/*
			printf("\npData[0]=%02x\n",pData[0]);
			printf("pData[1]=%02x\n",pData[1]);
			*/

			// Convertir pData[0] (MSB) qui est en Hexadecimal vers la version binaire (puis decimale)

			// Combiner les Bits
			int16_t tmp = ((int16_t)pData[0] << 4) | (pData[1] >> 4);

			// Convert to 2's complement, since temperature can be negative
			if(tmp < 0x7FF){
				tmp |= 0xF000;
			}

			// Convert to float temperature value (CELSIUS)
			int16_t temp_val = tmp * 0.0625;

			// Convert temperature to decimal format
			temp_val *= 100;

			// store to databuffer
			sprintf((char*)pData,
				"%u.%u C\r\n",
				((unsigned int)temp_val / 1000),
				((unsigned int)fmod(temp_val,100))
			);

			printf("Temperature : %s\n",pData);
			// }
		}
		else{
			printf("ERREUR RECEPTION\n");
		}
	 /*
	}
	else{
		printf("ERREUR TRANSMISSION\n");
	}
	*/
 }

void getLightValue(void){
	// Lancement du hadc
	HAL_ADC_Start(&hadc1);

	// printf("ADC lancé\n");

	// HAL_Delay(1);

	// Traitement de la Valeur dans hadc1
	uint32_t adcValue = HAL_ADC_GetValue(&hadc1);
	printf("HADC1: %d\n", adcValue);

	// Hal_Delay(1);

	// Si la valeur est en dessous de 4000, on considère que "la lumière est ALLUMEE"
	if(adcValue<4000){
		printf("Lumière ALLUMEE\n");
		// La LED LD2 s'allume
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 1);
	}
	// Sinon, on considère que la "lumière est ETEINTE"
	else{
		printf("Lumière ETEINTE\n");
		// LD2 s'éteint
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 0);
	}

	// Arret du hadc
	// HAL_ADC_Stop(&hadc1);
}

void getInfraValue(void){
	// Si on appuie sur le boutton poussoir
	// printf("RECHERCHE D'OBSTACLE\n");

	/*
	if(!HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13)){
		  printf("ARRET DES ALERTES\n");
		  // On arrête les alertes (la Led LD2 reste éteinte)
		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
	  }
	  else{
	  */
	    // Si un signal est recu sur le PA10
		  if(!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10)){
			  printf("OBSTACLE\n");
			  // On allume la led LD2
			  // HAL_Delay(1);
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
		  }
		  else{
			  printf("PAS D'OBSTACLE\n");
			  // HAL_Delay(1);
			  // On eteind la led LD2
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
			  // HAL_Delay(1);

			  // On allume la led LD1 (PC7)
			  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, 1);
			  // HAL_Delay(1);

			  // On eteint la led LD0 (PA9)
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, 0);
		  }
	  // }
}

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
  MX_I2C1_Init();
  MX_TIM2_Init();
  MX_ADC1_Init();
  MX_TIM3_Init();
  MX_TIM5_Init();
  /* USER CODE BEGIN 2 */
  // DATE: 22/04/2021

  // Modifier le fichier main.c afin d’y tester la fonction printf().
  int n=25;
  /*
  printf("BONJOUR");
  printf("%d\n",n);
  */

  // getTempI2C();

 // Initialisation du hadc
 HAL_ADC_Init(&hadc1);

 // Lancement du hadc pour que le Capteur de Luminosité ait le temps de la detecter
 HAL_ADC_Start(&hadc1);

 // Pour Lancer le Timer2 en mode Interruption
 HAL_TIM_Base_Start_IT(&htim2);

 // Pour Lancer le Timer5 en mode Interruption
 HAL_TIM_Base_Start_IT(&htim5);

 // getLightValue();

 // HAL_ADC_Start_IT(&hadc1);

 // printf("HTIM5\n");
//  HAL_TIM_PeriodElapsedCallback(&htim5);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  while (1)
  {
    /* USER CODE END WHILE */


    /* USER CODE BEGIN 3 */
	/*
  	getLightValue();
  	getInfraValue();
  	*/

	  // Si on appuie sur le bouton poussoir
	   if(!HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13)){
		   // Si les alertes sont activées (si la led LD2 est allumée)
		   if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5)){
			  printf("ARRET DES ALERTES\n");
			  // On arrête les alertes (la Led LD2 s'éteind)
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
		   }
		   /*
		   else if(!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5)){
			   // On revérifie la présence d'obstacle
			   getInfraValue();
		   }
		   */
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
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
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
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
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */
  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 8400 - 1;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 3000000 - 1;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 38452 - 1;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 65535;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief TIM5 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM5_Init(void)
{

  /* USER CODE BEGIN TIM5_Init 0 */

  /* USER CODE END TIM5_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM5_Init 1 */

  /* USER CODE END TIM5_Init 1 */
  htim5.Instance = TIM5;
  htim5.Init.Prescaler = 8400;
  htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim5.Init.Period = 600000;
  htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim5) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim5, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim5, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM5_Init 2 */

  /* USER CODE END TIM5_Init 2 */

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

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PA5 */
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PA10 */
  GPIO_InitStruct.Pin = GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

// get timer callback code to toggle pin /
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
    /** Check which version of the timer triggered this callback and toggle LED ***/
    if(htim == &htim2){
    	printf("\n\n");
    	getTempI2C();
    	getLightValue();

    	// Lancement de hadc1 en mode interruption
        HAL_ADC_Start_IT(&hadc1);
    }
    else if(htim == &htim5){
		// printf("RELOAD TIMER5\n");
		printf("\n");
       	getInfraValue();
    }
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
