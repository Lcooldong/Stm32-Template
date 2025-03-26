#include "usb.h"

USBD_HandleTypeDef hUsbDeviceHS;
extern PCD_HandleTypeDef hpcd_USB_OTG_HS;

static bool is_init = false;
static UsbMode_t is_usb_mode = USB_NON_MODE;

bool usbInit(void)
{
#ifdef _USE_HW_CLI 
  cliAdd("usb", cliCmd);
#endif
  return true;
}

bool usbBegin(UsbMode_t usb_mode)
{
  is_init = true;

if (usb_mode == USB_CDC_MODE)
  {
    #if HW_USE_CDC == 1
    /* Init Device Library */
    USBD_Init(&hUsbDeviceHS, &HS_Desc, DEVICE_HS);

    /* Add Supported Class */
    USBD_RegisterClass(&hUsbDeviceHS, USBD_CDC_CLASS);

    /* Add CDC Interface Class */
    USBD_CDC_RegisterInterface(&hUsbDeviceHS, &USBD_Interface_fops_HS);

    /* Start Device Process */
    USBD_Start(&hUsbDeviceHS);

    HAL_PWREx_EnableUSBVoltageDetector();

    is_usb_mode = USB_CDC_MODE;
    
    logPrintf("[OK] usbBegin()\n");
    logPrintf("     USB_CDC\r\n");
    #endif
  }
  else if (usb_mode == USB_MSC_MODE)
  {
    #if HW_USE_MSC == 1
    /* Init Device Library */
    USBD_Init(&hUsbDeviceHS, &MSC_Desc, DEVICE_HS);

    /* Add Supported Class */
    USBD_RegisterClass(&hUsbDeviceHS, USBD_MSC_CLASS);

    /* Add Storage callbacks for MSC Class */
    USBD_MSC_RegisterStorage(&hUsbDeviceHS, &USBD_DISK_fops);

    /* Start Device Process */
    USBD_Start(&hUsbDeviceHS);

    HAL_PWREx_EnableUSBVoltageDetector();

    is_usb_mode = USB_MSC_MODE;

    logPrintf("[OK] usbBegin()\n");
    logPrintf("     USB_MSC\r\n");
    #endif
  }
  else
  {
    is_init = false;

    logPrintf("[NG] usbBegin()\n");
  }

  return is_init;
}

// void HAL_PCD_SuspendCallback(PCD_HandleTypeDef *hpcd)
// {
//   /* Inform USB library that core enters in suspend Mode. */
//   USBD_LL_Suspend((USBD_HandleTypeDef*)hpcd->pData);
//   __HAL_PCD_GATE_PHYCLOCK(hpcd);
//   /* Enter in STOP mode. */
//   /* USER CODE BEGIN 2 */
//   if (hpcd->Init.low_power_enable)
//   {
//     /* Set SLEEPDEEP bit and SleepOnExit of Cortex System Control Register. */
//     SCB->SCR |= (uint32_t)((uint32_t)(SCB_SCR_SLEEPDEEP_Msk | SCB_SCR_SLEEPONEXIT_Msk));
//   }

//   is_connected = false;

//   /* USER CODE END 2 */
// }

// USBD_StatusTypeDef USBD_LL_SetUSBAddress(USBD_HandleTypeDef *pdev, uint8_t dev_addr)
// {
//   HAL_StatusTypeDef hal_status = HAL_OK;
//   USBD_StatusTypeDef usb_status = USBD_OK;

//   hal_status = HAL_PCD_SetAddress(pdev->pData, dev_addr);

//   usb_status =  USBD_Get_USB_Status(hal_status);

//   is_connected = true;

//   return usb_status;
// }


bool usbIsConnect(void)
{
  if(hUsbDeviceHS.pClassData == NULL)
  {
    return false;
  }
  if (hUsbDeviceHS.dev_state != USBD_STATE_CONFIGURED)
  {
    return false;
  }
  if (hUsbDeviceHS.dev_config == 0)
  {
    return false;
  }
  if (USBD_is_connected() == false)
  {
    return false;
  }
  

  return true;
}