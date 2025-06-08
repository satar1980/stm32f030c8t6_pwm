TIM_HandleTypeDef htim1;
TIM_OC_InitTypeDef sConfigOC = { 0 };
int32_t CH1_DC = 0;

void setup() {
  // put your setup code here, to run once:

  __HAL_RCC_TIM1_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  GPIO_InitTypeDef GPIOA_InitStruct = { 0 };
  GPIOA_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9;
  GPIOA_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIOA_InitStruct.Pull = GPIO_NOPULL;
  GPIOA_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIOA_InitStruct.Alternate = GPIO_AF2_TIM1;
  HAL_GPIO_Init(GPIOA, &GPIOA_InitStruct);

  MX_TIM1_Init();

  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  // HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
}

void loop() {

  while (CH1_DC < 500) {
    TIM1->CCR1 = CH1_DC;
    CH1_DC += 1;
    HAL_Delay(1);
  }
  while (CH1_DC > 0) {
    TIM1->CCR1 = CH1_DC;
    CH1_DC -= 1;
    HAL_Delay(1);
  }

}

static void MX_TIM1_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig = { 0 };
  TIM_MasterConfigTypeDef sMasterConfig = { 0 };

  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 96 - 1;  //1 mhz
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 500 - 1;  //2 khz
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  HAL_TIM_Base_Init(&htim1);

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig);
  HAL_TIM_PWM_Init(&htim1);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig);

  sConfigOC.OCMode = TIM_OCMODE_PWM1;

  sConfigOC.Pulse = 250;  // periode/2 = 50% dutycycle
  HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1);
  HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_2);

}
