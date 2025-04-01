// #include "ap.h"

// #include "main.h"

// // bool is_connected = false;

// // void HAL_PCD_ConnectCallback(PCD_HandleTypeDef *hpcd)
// // {
// //   is_connected = true;
// // }

// // void HAL_PCD_DisconnectCallback(PCD_HandleTypeDef *hpcd)
// // {
// //   is_connected = false;
// // }

// // int can_index = 0;

// // void apInit(void)
// // {
// //   canStart(_DEF_CAN1);
// // }




// // extern FDCAN_HandleTypeDef hfdcan1;

// bool canHeartBeat(void)
// { 
//   can_msg_t state_msg;
//   char can_buffer[128];
//   // int can_buffer_add;

//   state_msg.frame   = CAN_FD_NO_BRS;
//   state_msg.id_type = CAN_STD;
//   // state_msg.id_type = CAN_EXT;
//   state_msg.dlc     = CAN_DLC_8;
//   state_msg.id      = 0x123;
//   state_msg.length  = 8;

//   for (int i = 0; i < state_msg.length; i++)
//   {
//     state_msg.data[i] = i + can_index;
//   }

//   if(canMsgWrite(_DEF_CAN1, &state_msg, 10) > 0)
//   {
//     // can_index %= 10000;
//     sprintf(can_buffer, "0x%03d", (int)state_msg.id);
//     CDC_Transmit_HS((uint8_t*)can_buffer, strlen(can_buffer));
//     // can_buffer_add = sprintf(can_buffer, "%03d(T) -> id ", can_index++);
//     // if (state_msg.id_type == CAN_STD)
//     // {
//     //   can_buffer_add += sprintf(can_buffer + can_buffer_add, "std ");
//     // }
//     // else
//     // {
//     //   can_buffer_add += sprintf(can_buffer + can_buffer_add, "ext ");
//     // }
//     // can_buffer_add += sprintf(can_buffer + can_buffer_add, ": 0x%08X, L:%02d, ", state_msg.id, state_msg.length);
//     // for (int i=0; i<state_msg.length; i++)
//     // {
//     //   can_buffer_add += sprintf(can_buffer + can_buffer_add, "0x%02X ", state_msg.data[i]);
//     // }
//     // can_buffer_add += sprintf(can_buffer + can_buffer_add, "\r\n");
//   }
  
//   return true;
// }



// void apMain(void)
// {
//   uint32_t pre_time;
//   pre_time = millis();
//   char buffer[64];
//   int count = 0;
  
//   // FDCAN_TxHeaderTypeDef TxHeader;
//   // // FDCAN_RxHeaderTypeDef RxHeader;

//   // // uint8_t RxCANData[8];
//   // uint8_t TxCANData[8];

//   // TxHeader.Identifier = 0x125;
//   // TxHeader.IdType = FDCAN_STANDARD_ID;
//   // TxHeader.TxFrameType = FDCAN_DATA_FRAME;
//   // TxHeader.DataLength = FDCAN_DLC_BYTES_8;  // 데이터 길이
//   // TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
//   // TxHeader.BitRateSwitch = FDCAN_BRS_OFF;
//   // TxHeader.FDFormat = FDCAN_FD_CAN;
//   // TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
//   // TxHeader.MessageMarker = 0x00;

//   while(1)
//   {  
//     if (millis() - pre_time >= 500)
//     {
//       pre_time = millis();
//       count++;
//       for (int i = 0; i < 8; i++)
//       {
//         // TxCANData[i] = i + count;
//       }

//       // if(HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader, TxCANData) == HAL_OK)
//       // {
//       //   while(HAL_FDCAN_GetTxFifoFreeLevel(&hfdcan1) == 0)
//       //   {
//       //     if (millis()-pre_time >= 10)
//       //     {
//       //       break;
//       //     }
//       //   } 
//       // }
//       // // canHeartBeat();
//       // sprintf(buffer, "[%d]:%d=>\r\n", count++, strlen(buffer));
//       // CDC_Transmit_HS((uint8_t*)buffer, strlen(buffer));
//       HAL_GPIO_TogglePin(USER_BUTTON_GPIO_Port, USER_BUTTON_Pin);
//     }

//     // if(HAL_GPIO_ReadPin(USER_BUTTON_GPIO_Port, USER_BUTTON_Pin) == GPIO_PIN_SET)
//     // {
//     //   HAL_GPIO_WritePin(USER_LED_GPIO_Port, USER_LED_Pin, GPIO_PIN_SET);
//     // }
//     // else
//     // {
//     //   HAL_GPIO_WritePin(USER_LED_GPIO_Port, USER_LED_Pin, GPIO_PIN_RESET);
//     // }

    

//   }
// } 