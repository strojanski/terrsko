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
#include <math.h>
#include <stdbool.h>

#include "main.h"
#include "adc.h"
#include "crc.h"
#include "dac.h"
#include "dma.h"
#include "fdcan.h"
#include "i2c.h"
#include "quadspi.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"
#include "fmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usbd_cdc_if.h"
#include "lcd.h"
#include "lcd_demo.h"
#include "ugui.h"
#include "lcd_ugui.h"
#include "XPT2046_touch.h"
#include "joystick_level_control.h"
#include <terrlib.h>

#include "guysko.h"
#include "action.h"
#include "velocity.h"
#include "move.h"
#include "position.h"
#include "scene.h"
#include "interactions.h"

#include "movable.h"
#include "rand_range.h"
#include "rand_range.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define WIDTH 320
#define HEIGHT 240
#define BLOCK_WIDTH 4

// int SCENF[60][80];
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
TIM_HandleTypeDef htim2;
bool cycle = false;
uint8_t FPS = FPS_100;
/* USER CODE END 0 */
/**
 * @brief  The application entry point.
 * @retval int
 */

int main(void)
{
	/* USER CODE BEGIN 1 */
	coord_t joystick_raw = {0, 0};
	coord_t joystick_new = {0, 0};
	joystick_t joystick;
	// char MSG[100]={0};

	// char str[50];
	// float bitrate;

	// int x = floor->center->x;
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
	MX_ADC1_Init();
	MX_ADC2_Init();
	MX_FMC_Init();
	MX_I2C2_Init();
	MX_UART4_Init();
	MX_UART5_Init();
	MX_USART1_UART_Init();
	MX_USART2_UART_Init();
	MX_QUADSPI1_Init();
	MX_SPI1_Init();
	MX_TIM5_Init();
	MX_TIM8_Init();
	MX_TIM20_Init();
	MX_ADC3_Init();
	MX_DAC1_Init();
	MX_DAC2_Init();
	MX_FDCAN2_Init();
	MX_I2C1_Init();
	MX_TIM15_Init();
	MX_USART3_UART_Init();
	MX_USB_Device_Init();
	MX_DMA_Init();
	MX_CRC_Init();

	/* USER CODE BEGIN 2 */

	MX_ADC4_Init(); // bug workaround: https://community.st.com/s/question/0D50X0000BVnBhASQV/bug-report-dma-and-adc-initialization-order-changed-in-stm32f4-hal-v1241-causing-incorrect-adc-operation

	for (uint8_t i = 0; i < 3; i++)
	{
		HAL_Delay(100);
		HAL_GPIO_WritePin(GPIOF, LED0_Pin | LED1_Pin | LED2_Pin | LED3_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, LED4_Pin | LED5_Pin | LED6_Pin | LED7_Pin, GPIO_PIN_SET);
		HAL_Delay(100);
		HAL_GPIO_WritePin(GPIOF, LED0_Pin | LED1_Pin | LED2_Pin | LED3_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, LED4_Pin | LED5_Pin | LED6_Pin | LED7_Pin, GPIO_PIN_RESET);
	}

	LCD_Init();
	LCD_UG_init();

	UG_DriverRegister(DRIVER_FILL_FRAME, (void *)_HW_FillFrame_);

	// LCD_Intro_LogoSlide(140,200);
	// bitrate = DrawColors(0,0,80);

	// UG_SetForecolor(C_WHITE);
	// UG_FontSelect(&FONT_8X12);
	// sprintf(str,"%.2f MB/s, %.0f fps",bitrate/(1024*1024), 100.0);// bitrate/(100.0*180*2));
	// UG_PutString(5,105,str);

	// to mi deli
	// UG_SetForecolor(C_WHITE);
	// UG_FontSelect(&FONT_16X26);
	// UG_PutString(5,205,"To mi deli, TANK!");

	joystick_init(&joystick);
	HAL_ADC_Start_DMA(&hadc4, &joystick_raw, 2);

	HAL_NVIC_SetPriority(EXTI0_IRQn, 1, 2);
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);

	HAL_NVIC_SetPriority(EXTI1_IRQn, 1, 2);
	HAL_NVIC_EnableIRQ(EXTI1_IRQn);

	HAL_NVIC_SetPriority(EXTI4_IRQn, 1, 2);
	HAL_NVIC_EnableIRQ(EXTI4_IRQn);

	HAL_NVIC_SetPriority(EXTI9_5_IRQn, 1, 2);
	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 1, 2);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

	/* USER CODE END 2 */
	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	init_world();
	/*	TIM2 IS READY FOR ACTIVITY IN FREQUENCY: 1HZ
	 * TIMER FREQ: 168000000 (168 MHz)
	 * Prescaler 10000 - 1
	 * TIM4 intervals: 1 / 16800
	 * Meaning it "ticks" 16800 times every 1 second
	 * */
	//	__HAL_RCC_TIM2_CLK_ENABLE();
	//	htim2.Instance = TIM2;
	//	htim2.Init.Prescaler = 10000 - 1;
	//	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	//	htim2.Init.Period = 16800 - 1;
	//	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	//	htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	//	HAL_TIM_Base_Init(&htim2);
	//
	//	__HAL_TIM_ENABLE_IT(&htim2, TIM_IT_UPDATE);
	//	HAL_TIM_Base_Start(&htim2);
	//
	//	HAL_NVIC_SetPriority(TIM2_IRQn, 1, 2);
	//	HAL_NVIC_EnableIRQ(TIM2_IRQn);


	/*
	 * Procedure for movable objects:
	 *
	 * 1.) The input of player will be read and updated all the time
	 * 2.) Just before the calculation of movement of guysko, each movement and action will be declared
	 * 3.) Calculate acc, vel, movement, action and demage of guysko and every other movable object
	 * 	FOR EVERY MOVABLE OBJECT:
	 *	3.1) calculate/add acceleration in y direction
	 *	3.2) calculate velocity in every direction.
	 *	Depends on : 	a)blocks around moveable object,
	 *								b)acceleration,
	 *								c)player input
	 *	3.2.1) Calculate damage done to movable object based on difference of velocity of guysko
	 *	3.3) change postion of movable object based on velocity
	 *	3.4) update move of guysko based on velocity
	 *	3.5) calculate postion of action based on orientation of move object
	 *	3.6) action()
	 *		3.6.1) (POINT DAMAGE CALCULATOR) calculate damage done to another movables
	 * 4.) Calculate health points based on POINT DAMAGE CALCULATOR
	 * 5.) Draw all the movable objects in scene
	 */

	/*	TIM2 IS READY FOR ACTIVITY IN FREQUENCY: 1HZ
	 * TIMER FREQ: 168000000 (168 MHz)
	 * Prescaler 10000 - 1
	 * TIM4 intervals: 1 / 16800
	 * Meaning it "ticks" 16800 times every 1 second
	 * */
	//	__HAL_RCC_TIM2_CLK_ENABLE();
	//	htim2.Instance = TIM2;
	//	htim2.Init.Prescaler = 10000 - 1;
	//	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	//	htim2.Init.Period = 16800 - 1;
	//	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	//	htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	//	HAL_TIM_Base_Init(&htim2);
	//
	//	__HAL_TIM_ENABLE_IT(&htim2, TIM_IT_UPDATE);
	//	HAL_TIM_Base_Start(&htim2);
	//
	//	HAL_NVIC_SetPriority(TIM2_IRQn, 1, 2);
	//	HAL_NVIC_EnableIRQ(TIM2_IRQn);

	// initialize guysko
	/*
	 * Procedure for movable objects:
	 *
	 * 1.) The input of player will be read and updated all the time
	 * 2.) Just before the calculation of movement of guysko, each movement and action will be declared
	 * 3.) Calculate acc, vel, movement, action and demage of guysko and every other movable object
	 * 	FOR EVERY MOVABLE OBJECT:
	 *	3.1) calculate/add acceleration in y direction
	 *	3.2) calculate velocity in every direction.
	 *	Depends on : 	a)blocks around moveable object,
	 *								b)acceleration,
	 *								c)player input
	 *	3.2.1) Calculate damage done to movable object based on difference of velocity of guysko
	 *	3.3) change postion of movable object based on velocity
	 *	3.4) update move of guysko based on velocity
	 *	3.5) calculate postion of action based on orientation of move object
	 *	3.6) action()
	 *		3.6.1) (POINT DAMAGE CALCULATOR) calculate damage done to another movables
	 * 4.) Calculate health points based on POINT DAMAGE CALCULATOR
	 * 5.) Draw all the movable objects in scene
	 */
	// initialize guysko

	guysko* player = new_guysko();
	movable* beings = new_movables();

	block_c new_camera_x = camera_x_block;
	block_c new_camera_y = camera_y_block;

	// Init guysko to spawn in camera coordinates
	player->pos->x = block_to_pixel(camera_x_block);
	player->pos->y = block_to_pixel(camera_y_block);

	update_camera_center(new_camera_x, new_camera_y);

	old_camera_x = camera_x_block;
	old_camera_y = camera_y_block;
	draw_scene(true);


	while (1) {
		cycle = false;
		draw_scene(false);

		old_camera_x = camera_x_block;
		old_camera_y = camera_y_block;

		if (!building_mode) {
			refresh_guysko(player, FPS);
		}
//		new_camera_x = player->pos->x / BLOCK_WIDTH;
//		new_camera_y = player->pos->y / BLOCK_WIDTH;

		// When guysko is at postition x=0, it will be drawn nearly fully off the screen, becouse of this two if sentences. Fix it to
		// roll the camera smoothly
		if (camera_x_block - player->pos->x / BLOCK_WIDTH > GUYSKO_WINDOW_SPAN_PIXEL_X / BLOCK_WIDTH) {
			new_camera_x = camera_x_block - abs(camera_x_block - GUYSKO_WINDOW_SPAN_PIXEL_X / BLOCK_WIDTH - player->pos->x / BLOCK_WIDTH);
		} else if (camera_x_block - player->pos->x / BLOCK_WIDTH < - GUYSKO_WINDOW_SPAN_PIXEL_X / BLOCK_WIDTH) {
			new_camera_x = camera_x_block + abs(-camera_x_block - GUYSKO_WINDOW_SPAN_PIXEL_X / BLOCK_WIDTH + player->pos->x / BLOCK_WIDTH);
		}
		if (camera_y_block * BLOCK_WIDTH - player->pos->y > GUYSKO_WINDOW_SPAN_PIXEL_Y) {
			new_camera_y = (camera_y_block * BLOCK_WIDTH - abs(camera_y_block * BLOCK_WIDTH - GUYSKO_WINDOW_SPAN_PIXEL_Y - player->pos->y)) / BLOCK_WIDTH;
		} else if (camera_y_block * BLOCK_WIDTH - player->pos->y < (-1) * GUYSKO_WINDOW_SPAN_PIXEL_Y) {
			new_camera_y = (camera_y_block * BLOCK_WIDTH + abs((-1) * camera_y_block * BLOCK_WIDTH - GUYSKO_WINDOW_SPAN_PIXEL_Y + player->pos->y)) / BLOCK_WIDTH;
		}

		// Set camera to block position, allow moving it with actions
		if (building_mode) {
			// Changed in place_block function
			new_camera_x = camera_x_block;
			new_camera_y = camera_y_block;

			// Move with actions
			if (act_down) {
				new_camera_y += BUILD_CAMERA_SPEED;
				action_reset(ACT_DOWN_INDEX);
			}
			if (act_left) {
				new_camera_x -= BUILD_CAMERA_SPEED;
				action_reset(ACT_LEFT_INDEX);
			}
			if (act_up) {
				new_camera_y -= BUILD_CAMERA_SPEED;
				action_reset(ACT_UP_INDEX);
			}
			if (act_right) {
				new_camera_x += BUILD_CAMERA_SPEED;
				action_reset(ACT_RIGHT_INDEX);
			}

			// Actual building - build with OK
			if (ok) {
				pixel_position pos = {
						x: block_to_pixel(new_camera_x),
						y: block_to_pixel(new_camera_y)
				};

				place_block(&pos, _dirt, 0, 0);

				action_reset(OK_INDEX);
			}


		}

		update_camera_center(new_camera_x, new_camera_y);

		if (!building_mode) {
			// Buttons for digging
			if (act_down) {
				dig_down(player->pos);
				action_reset(ACT_DOWN_INDEX);
			}

			if (act_left) {
				dig_left(player->pos);
				action_reset(ACT_LEFT_INDEX);

				// Compensate for guysko movement
				set_position(player->pos, player->pos->x+5, player->pos->y);
			}

			if (act_right) {
				dig_right(player->pos);
				action_reset(ACT_RIGHT_INDEX);
			}
		}

		// Trigger building
		if (ok) {
			building_mode = true;
			draw_scene(true);
			action_reset(OK_INDEX);
		}

		// Cancel building
		if (esc) {
			building_mode = false;
			draw_scene(true);
			action_reset(ESC_INDEX);
		}


		action_set(&joystick_raw);
		while (!cycle)
		{
			/*
			 * if joystick_new will ever be needed:
			 * joystick_get(&joystick_raw, &joystick_new, &joystick);
			 */
			action_set(&joystick_raw);
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
	HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48 | RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
	RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
	RCC_OscInitStruct.PLL.PLLN = 42;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV6;
	RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_10) != HAL_OK)
	{
		Error_Handler();
	}
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
	volatile int a = 0;
	while (1)
	{
		a++;
	}
	/* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
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
