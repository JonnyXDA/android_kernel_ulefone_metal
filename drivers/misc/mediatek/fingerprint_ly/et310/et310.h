#ifndef _FP_LINUX_DIRVER_H_
#define _FP_LINUX_DIRVER_H_

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/poll.h>
#include <linux/sched.h>
#include <linux/irq.h>
#include <asm/irq.h>
#include <linux/io.h>
#include <linux/interrupt.h>
#include <linux/uaccess.h>
//#include <mach/hardware.h>
#include <linux/platform_device.h>
#include <linux/cdev.h>
#include <linux/miscdevice.h>
#include <linux/gpio.h>

//#include <linux/gpio.h>
//#include <mach/gpio.h>
//#include <plat/gpio-cfg.h>
//#include <linux/module.h>
#include <linux/spi/spi.h>
#include <../../../../../drivers/spi/mediatek/mt6735/mt_spi.h>

//#include <mach/mt_gpio.h>
//#include <mach/mt_pm_ldo.h>
//#include <mach/mt_spi.h>

//#include <mach/eint.h>
//#include <cust_eint.h>

//#include <mach/mt_spi.h>
//#include <mach/eint.h>
//#include "cust_gpio_usage.h"
//#include <cust_eint.h>
//#include <mach/irqs.h>


///////

//#include <mach/mt_gpio.h>
//#include <linux/xlog.h>
//#include <mach/mt_pm_ldo.h>


#define FP_MAJOR					150//153 /* assigned */
#define N_SPI_MINORS				32  /* ... up to 256 */

#define FP_ADDRESS_0				0x00
#define FP_WRITE_ADDRESS			0xAC
#define FP_READ_DATA				0xAF
#define FP_WRITE_DATA				0xAE

#define FP_EEPROM_WREN_OP			0x06
#define FP_EEPROM_WRDI_OP			0x04
#define FP_EEPROM_RDSR_OP			0x05
#define FP_EEPROM_WRSR_OP			0x01
#define FP_EEPROM_READ_OP			0x03
#define FP_EEPROM_WRITE_OP			0x02

/* ------------------------- Register Definition ------------------------*/
/*
 * Sensor Registers
 */

#define FDATA_FP_ADDR				0x00
#define FSTATUS_FP_ADDR				0x01
/*
 * Detect Define
 */
#define FRAME_READY_MASK			0x01

/* ------------------------- Opcode -------------------------------------*/
#define FP_REGISTER_READ			0x01
#define FP_REGISTER_WRITE			0x02
#define FP_GET_ONE_IMG				0x03
#define FP_SENSOR_RESET				0x04
#define FP_POWER_ONOFF				0x05
#define FP_SET_SPI_CLOCK			0x06
#define FP_RESET_SET				0x07

//added by xiehaifei for factory test
#define FSFACTORY_FP_ADDR                              0x04 
#define FP_REGISTER_FACTORY_TEST_READ          0x09 
#define FP_REGISTER_TEST_READ_DATA            0x50
//end by xiehaifei for factory

#define FP_EEPROM_WREN				0x90
#define FP_EEPROM_WRDI				0x91
#define FP_EEPROM_RDSR				0x92
#define FP_EEPROM_WRSR				0x93
#define FP_EEPROM_READ				0x94
#define FP_EEPROM_WRITE				0x95

/* trigger signal initial routine*/
#define INT_TRIGGER_INIT			0xa4
/* trigger signal close routine*/
#define INT_TRIGGER_CLOSE			0xa5
/* read trigger status*/
#define INT_TRIGGER_READ			0xa6
/* polling trigger status*/
#define INT_TRIGGER_POLLING			0xa7
/* polling abort*/
#define INT_TRIGGER_ABORT			0xa8


/* ------------------------- Interrupt ------------------------------*/
/* interrupt init */
int Interrupt_Init(int int_num, int detect_period, int detect_threshold);

/* interrupt free */
int Interrupt_Free(void);

void fps_interrupt_abort(void);

/* interrupt polling */
unsigned int fps_interrupt_poll(
	struct file *file,
	struct poll_table_struct *wait
);
struct interrupt_desc {
	int gpio;
	int number;
	char *name;
	int int_count;
	struct timer_list timer;
	bool finger_on;
	int detect_period;
	int detect_threshold;
};

/* ------------------------- Structure ------------------------------*/
struct egis_ioc_transfer {
	u8 *tx_buf;
	u8 *rx_buf;

	__u32 len;
	__u32 speed_hz;

	__u16 delay_usecs;
	__u8 bits_per_word;
	__u8 cs_change;
	__u8 opcode;
	__u8 pad[3];

};

#define EGIS_IOC_MAGIC			'k'
#define EGIS_MSGSIZE(N) \
	((((N)*(sizeof(struct egis_ioc_transfer))) < (1 << _IOC_SIZEBITS)) \
		? ((N)*(sizeof(struct egis_ioc_transfer))) : 0)
#define EGIS_IOC_MESSAGE(N) _IOW(EGIS_IOC_MAGIC, 0, char[EGIS_MSGSIZE(N)])

struct fp_data {
	dev_t devt;
	spinlock_t spi_lock;
	struct spi_device *spi;
	struct list_head device_entry;

	/* buffer is NULL unless this device is open (users > 0) */
	struct mutex buf_lock;
	unsigned users;
	u8 *buffer;
};

/* ------------------------- Data Transfer --------------------------*/
int fp_io_read_register(struct fp_data *fp, u8 *addr, u8 *buf);
int fp_io_write_register(struct fp_data *fp, u8 *buf);
int fp_io_get_one_image(struct fp_data *fp, u8 *buf, u8 *image_buf);
int fp_read_register(struct fp_data *fp, u8 addr, u8 *buf);
int fp_mass_read(struct fp_data *fp, u8 addr, u8 *buf, int read_len);
#endif
