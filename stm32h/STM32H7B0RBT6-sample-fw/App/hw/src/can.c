#include "can.h"
#include "qbuffer.h"
// #include "fdcan.h"



#ifdef _USE_HW_CAN

typedef struct
{
  uint32_t prescaler;
  uint32_t sjw;
  uint32_t tseg1;
  uint32_t tseg2;
} can_baud_cfg_t;

const can_baud_cfg_t can_baud_cfg_80m_normal[] =
{
    {50, 8, 13, 2}, // 100K, 87.5%
    {40, 8, 13, 2}, // 125K, 87.5%
    {20, 8, 13, 2}, // 250K, 87.5%
    {10, 8, 13, 2}, // 500K, 87.5%
    {5,  8, 13, 2}, // 1M,   87.5%
};

const can_baud_cfg_t can_baud_cfg_80m_data[] =
{
    {40, 8, 11, 8}, // 100K, 60%
    {32, 8, 11, 8}, // 125K, 60%
    {16, 8, 11, 8}, // 250K, 60%
    {8,  8, 11, 8}, // 500K, 60%
    {4,  8, 11, 8}, // 1M,   60%
    {2,  8, 11, 8}, // 2M    60%
    {1,  8, 11, 8}, // 4M    60%
    {1,  8,  9, 6}, // 5M    62.5%
};



const uint32_t frame_tbl[] =
{
    FDCAN_FRAME_CLASSIC,
    FDCAN_FRAME_FD_NO_BRS,
    FDCAN_FRAME_FD_BRS
};

const uint32_t mode_tbl[] =
{
    FDCAN_MODE_NORMAL,
    FDCAN_MODE_BUS_MONITORING,
    FDCAN_MODE_INTERNAL_LOOPBACK
};

const uint32_t dlc_len_tbl[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 12, 16, 20, 24, 32, 48, 64};

const uint32_t dlc_tbl[] =
{
    FDCAN_DLC_BYTES_0,
    FDCAN_DLC_BYTES_1,
    FDCAN_DLC_BYTES_2,
    FDCAN_DLC_BYTES_3,
    FDCAN_DLC_BYTES_4,
    FDCAN_DLC_BYTES_5,
    FDCAN_DLC_BYTES_6,
    FDCAN_DLC_BYTES_7,
    FDCAN_DLC_BYTES_8,
    FDCAN_DLC_BYTES_12,
    FDCAN_DLC_BYTES_16,
    FDCAN_DLC_BYTES_20,
    FDCAN_DLC_BYTES_24,
    FDCAN_DLC_BYTES_32,
    FDCAN_DLC_BYTES_48,
    FDCAN_DLC_BYTES_64
};


typedef struct
{
  bool is_init;
  bool is_open;

  uint32_t err_code;
  uint8_t  state;
  uint32_t recovery_cnt;

  uint32_t q_rx_full_cnt;
  uint32_t q_tx_full_cnt;
  uint32_t fifo_full_cnt;
  uint32_t fifo_lost_cnt;

  uint32_t fifo_idx;
  uint32_t enable_int;
  CanMode_t  mode;
  CanFrame_t frame;
  CanBaud_t  baud;
  CanBaud_t  baud_data;

  uint32_t rx_cnt;
  uint32_t tx_cnt;

  FDCAN_HandleTypeDef  hfdcan;
  bool (*handler)(uint8_t ch, CanEvent_t evt, can_msg_t *arg);

  qbuffer_t q_msg;
  can_msg_t can_msg[CAN_MSG_RX_BUF_MAX];
} can_tbl_t;

const can_baud_cfg_t *p_baud_normal = can_baud_cfg_80m_normal;
const can_baud_cfg_t *p_baud_data   = can_baud_cfg_80m_data;

static can_tbl_t can_tbl[CAN_MAX_CH];
static CanFilterType_t can_filter_type = CAN_ID_MASK;
static volatile uint32_t err_int_cnt = 0;









bool canMXInit(FDCAN_HandleTypeDef hfdcan, uint8_t ch)
{
  bool ret = true;

  canInit();

  can_tbl[ch].hfdcan = hfdcan;
  can_tbl[ch].mode                  = hfdcan.Init.Mode;
  can_tbl[ch].frame                 = hfdcan.Init.FrameFormat;
  can_tbl[ch].baud                  = canChangeEnumBaud(canGetBaudrate(hfdcan));
  can_tbl[ch].baud_data             = canChangeEnumBaud(canGetBaudrateData(hfdcan));
  can_tbl[ch].fifo_idx              = FDCAN_RX_FIFO0;
  can_tbl[ch].enable_int            = FDCAN_IT_RX_FIFO0_MESSAGE_LOST |
                                      FDCAN_IT_RX_FIFO0_FULL |
                                      FDCAN_IT_RX_FIFO0_NEW_MESSAGE  |
                                      FDCAN_IT_BUS_OFF |
                                      FDCAN_IT_ERROR_WARNING |
                                      FDCAN_IT_ERROR_PASSIVE;

  can_tbl[ch].err_code              = CAN_ERR_NONE;

  return ret;
}


bool canStart(uint8_t ch)
{
  bool ret = true;
  FDCAN_HandleTypeDef  *p_can;

  if (ch >= CAN_MAX_CH) return false;

  p_can = &can_tbl[ch].hfdcan;

  if(HAL_FDCAN_Start(p_can) != HAL_OK)
  {
    return false;
  }


  return ret;
}




bool canMsgWrite(uint8_t ch, can_msg_t *p_msg, uint32_t timeout)
{
  FDCAN_HandleTypeDef  *p_can;
  FDCAN_TxHeaderTypeDef tx_header;
  uint32_t pre_time;
  bool ret = true;


  if(ch > CAN_MAX_CH) return false;

  if (can_tbl[ch].err_code & CAN_ERR_BUS_OFF) return false;
  if (can_tbl[ch].err_code & CAN_ERR_PASSIVE) return false;


  p_can = &can_tbl[ch].hfdcan;

  switch(p_msg->id_type)
  {
    case CAN_STD :
      tx_header.IdType = FDCAN_STANDARD_ID;
      break;

    case CAN_EXT :
      tx_header.IdType = FDCAN_EXTENDED_ID;
      break;
  }

  switch(p_msg->frame)
  {
    case CAN_CLASSIC:
      tx_header.FDFormat      = FDCAN_CLASSIC_CAN;
      tx_header.BitRateSwitch = FDCAN_BRS_OFF;
      break;

    case CAN_FD_NO_BRS:
      tx_header.FDFormat      = FDCAN_FD_CAN;
      tx_header.BitRateSwitch = FDCAN_BRS_OFF;
      break;

    case CAN_FD_BRS:
      tx_header.FDFormat      = FDCAN_FD_CAN;
      tx_header.BitRateSwitch = FDCAN_BRS_ON;
      break;
  }

  tx_header.Identifier          = p_msg->id;
  tx_header.MessageMarker       = 0;
  tx_header.TxEventFifoControl  = FDCAN_NO_TX_EVENTS;
  tx_header.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
  tx_header.TxFrameType         = FDCAN_DATA_FRAME;
  tx_header.DataLength          = dlc_tbl[p_msg->dlc];


  if (HAL_FDCAN_GetTxFifoFreeLevel(p_can) == 0)
  {
    return false;
  }


  pre_time = millis();
  if(HAL_FDCAN_AddMessageToTxFifoQ(p_can, &tx_header, p_msg->data) == HAL_OK)
  {
    /* Wait transmission complete */
    while(HAL_FDCAN_GetTxFifoFreeLevel(p_can) == 0)
    {
      if (millis()-pre_time >= timeout)
      {
        ret = false;
        break;
      }
    }
  }
  else
  {
    ret = false;
  }

  return ret;
}






uint32_t canGetBaudrate(FDCAN_HandleTypeDef hfdcan)
{
  uint32_t fdcan_clk = HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_FDCAN);  // 
  uint32_t prescaler = hfdcan.Init.NominalPrescaler;  // 1
  uint32_t ts1 = hfdcan.Init.NominalTimeSeg1;         // 13
  uint32_t ts2 = hfdcan.Init.NominalTimeSeg2;         // 2
  uint32_t sync_seg = 1;
  uint32_t tq = prescaler * (sync_seg + ts1 + ts2);
  uint32_t baudrate = fdcan_clk / tq;

  return baudrate;
}


uint32_t canGetBaudrateData(FDCAN_HandleTypeDef hfdcan)
{
  uint32_t fdcan_clk = HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_FDCAN);  // 
  uint32_t prescaler = hfdcan.Init.DataPrescaler;  // 1
  uint32_t ts1 = hfdcan.Init.DataTimeSeg1;         // 13
  uint32_t ts2 = hfdcan.Init.DataTimeSeg2;         // 2
  uint32_t sync_seg = 1;
  uint32_t tq = prescaler * (sync_seg + ts1 + ts2);
  uint32_t baudrate = fdcan_clk / tq;

  return baudrate;
}


uint32_t canChangeEnumBaud(uint32_t val)
{
  uint32_t ret = 0;

  switch (val)
  {
  case 100000:
    ret = CAN_100K;
    break;
  case 125000:
    ret = CAN_125K;
    break;
  case 250000:
    ret = CAN_250K;
    break;
  case 500000:
    ret = CAN_500K;
    break;
  case 1000000:
    ret = CAN_1M;
    break;
  case 2000000:
    ret = CAN_2M;
    break;
  case 4000000:
    ret = CAN_4M;
    break;
  case 5000000:
    ret = CAN_5M;
    break;
  case CAN_100K:
    ret = 100000;
    break;
  case CAN_125K:
    ret = 125000;
    break;
  case CAN_250K:
    ret = 250000;
    break;
  case CAN_500K:
    ret = 500000;
    break;
  case CAN_1M:
    ret = 1000000;
    break;
  case CAN_2M:
    ret = 2000000;
    break;
  case CAN_4M:
    ret = 4000000;
    break;
  case CAN_5M:
    ret = 5000000;
    break;  
  default:
    break;
  }

  return ret;
}

bool canInit(void)
{
  bool ret = true;

  uint8_t i;


  for(i = 0; i < CAN_MAX_CH; i++)
  {
    can_tbl[i].is_init  = true;
    can_tbl[i].is_open  = false;
    can_tbl[i].err_code = CAN_ERR_NONE;
    can_tbl[i].state    = 0;
    can_tbl[i].recovery_cnt = 0;

    can_tbl[i].q_rx_full_cnt = 0;
    can_tbl[i].q_tx_full_cnt = 0;
    can_tbl[i].fifo_full_cnt = 0;
    can_tbl[i].fifo_lost_cnt = 0;

    qbufferCreateBySize(&can_tbl[i].q_msg, (uint8_t *)&can_tbl[i].can_msg[0], sizeof(can_msg_t), CAN_MSG_RX_BUF_MAX);
  }

//  logPrintf("[OK] canInit()\n");

#ifdef _USE_HW_CLI
  cliAdd("can", cliCan);
#endif
  return ret;
}




#endif




