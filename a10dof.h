#ifndef A10DOF_H
#define A10DOF_H

#include <WPILib.h>

class i2csensor
{
public:
	i2csensor(const int& module, const int& addr);
	virtual ~i2csensor();
	int write8(int addr, int x);
	int read8(int addr);
	//int write(int addr, const void* x, size_t len);
	int read(int addr, void* x, size_t len);
private:
	I2C *m_i2c;
};

class bmp085 : public i2csensor
{
public:
	static const int s_addr;// = 0xEE;
	static const int s_id;// = 0x55;
	enum reg_t {
		CAL_AC1			= 0xAA,// R Calibration data (16 bits)
		CAL_AC2			= 0xAC,// R Calibration data (16 bits)
		CAL_AC3			= 0xAE,// R Calibration data (16 bits)
		CAL_AC4			= 0xB0,// R Calibration data (16 bits)
		CAL_AC5			= 0xB2,// R Calibration data (16 bits)
		CAL_AC6			= 0xB4,// R Calibration data (16 bits)
		CAL_B1			= 0xB6,// R Calibration data (16 bits)
		CAL_B2			= 0xB8,// R Calibration data (16 bits)
		CAL_MB			= 0xBA,// R Calibration data (16 bits)
		CAL_MC			= 0xBC,// R Calibration data (16 bits)
		CAL_MD			= 0xBE,// R Calibration data (16 bits)
		CHIPID			= 0xD0,
		VERSION			= 0xD1,
		SOFTRESET		= 0xE0,
		CONTROL			= 0xF4,
		TEMPDATA		= 0xF6,
		PRESSUREDATA		= 0xF6,
		READTEMPCMD		= 0x2E,
		READPRESSURECMD		= 0x34,
	};
	/*enum mode_t {
		ULTRALOWPOER	= 0,
		STANDARD	= 1,
		HIGHRES		= 2,
		ULTRAHIGHRES	= 3,
	};*/
	struct calib {
		int ac1;
		int ac2;
		int ac3;
		int ac4;
		int ac5;
		int ac6;
		int b1;
		int b2;
		int mb;
		int mc;
		int md;
	};
	bmp085(const int& module = 1);
	int start();//(const mode_t &mode = ULTRAHIGHRES);
	//void recalib();
	//void setmode(const mode_t &mode = ULTRAHIGHRES);
	//int get(float &temp, float &pressure);
	int get(float &temp);
	float readtemp();
private:
	int m_state;
	//int m_mode;
	struct calib m_calib;
};

class l3gd20 : public i2csensor
{
public:
	static const int s_addr;// = 0xD6;
	static const int s_id;// = 0xD4;
	static const float s_dps[3];// = {0.00875f, 0.0175f, 0.070f};
	enum reg_t {
		WHO_AM_I		= 0x0F,
		CTRL_REG1		= 0x20, // 00000111 rw
		CTRL_REG2		= 0x21, // 00000000 rw
		CTRL_REG3		= 0x22, // 00000000 rw
		CTRL_REG4		= 0x23, // 00000000 rw
		CTRL_REG5		= 0x24, // 00000000 rw
		REFERENCE		= 0x25, // 00000000 rw
		OUT_TEMP		= 0x26, //r
		STATUS_REG		= 0x27, //r
		OUT_X_L			= 0x28, //r
		OUT_X_H			= 0x29, //r
		OUT_Y_L			= 0x2A, //r
		OUT_Y_H			= 0x2B, //r
		OUT_Z_L			= 0x2C, //r
		OUT_Z_H			= 0x2D, //r
		FIFO_CTRL_REG		= 0x2E, // 00000000 rw
		FIFO_SRC_REG		= 0x2F, //r
		INT1_CFG		= 0x30, // 00000000 rw
		INT1_SRC		= 0x31, //r
		TSH_XH			= 0x32, // 00000000 rw
		TSH_XL			= 0x33, // 00000000 rw
		TSH_YH			= 0x34, // 00000000 rw
		TSH_YL			= 0x35, // 00000000 rw
		TSH_ZH			= 0x36, // 00000000 rw
		TSH_ZL			= 0x37, // 00000000 rw
		INT1_DURATION		= 0x38, // 00000000 rw
	};
	enum range_t {
		DPS250 = 0,
		DPS500 = 1,
		DPS2000 = 2,
	};
	l3gd20(const int& module = 1);
	int start(const range_t& range = DPS250);
	int setrange(const range_t& range);
	int get(float &x, float &y, float &z);
	float getangle();
private:
	range_t m_range;
	int m_state;
};

class lsm303a : public i2csensor
{
public:
	static const int s_addr;// = 0x32;
	//static const int s_id;
	enum reg_t {
		CTRL_REG1_A		= 0x20, // 00000111 rw
		CTRL_REG2_A		= 0x21, // 00000000 rw
		CTRL_REG3_A		= 0x22, // 00000000 rw
		CTRL_REG4_A		= 0x23, // 00000000 rw
		CTRL_REG5_A		= 0x24, // 00000000 rw
		CTRL_REG6_A		= 0x25, // 00000000 rw
		REFERENCE_A		= 0x26, // 00000000 r
		STATUS_REG_A		= 0x27, // 00000000 r
		OUT_X_L_A		= 0x28,
		OUT_X_H_A		= 0x29,
		OUT_Y_L_A		= 0x2A,
		OUT_Y_H_A		= 0x2B,
		OUT_Z_L_A		= 0x2C,
		OUT_Z_H_A		= 0x2D,
		FIFO_CTRL_REG_A		= 0x2E,
		FIFO_SRC_REG_A		= 0x2F,
		INT1_CFG_A		= 0x30,
		INT1_SOURCE_A		= 0x31,
		INT1_THS_A		= 0x32,
		INT1_DURATION_A		= 0x33,
		INT2_CFG_A		= 0x34,
		INT2_SOURCE_A		= 0x35,
		INT2_THS_A		= 0x36,
		INT2_DURATION_A		= 0x37,
		CLICK_CFG_A		= 0x38,
		CLICK_SRC_A		= 0x39,
		CLICK_THS_A		= 0x3A,
		TIME_LIMIT_A		= 0x3B,
		TIME_LATENCY_A		= 0x3C,
		TIME_WINDOW_A		= 0x3D,
	};
	lsm303a(const int& module = 1);
	int start();
	int get(float &x, float &y, float &z);
};

class lsm303m : public i2csensor
{
public:
	static const int s_addr;// = 0x3C;
	//static const int s_id;// = 0x32;
	enum reg_t {
		CRA_REG_M	= 0x00,
		CRB_REG_M	= 0x01,
		MR_REG_M	= 0x02,
		OUT_X_H_M	= 0x03,
		OUT_X_L_M	= 0x04,
		OUT_Z_H_M	= 0x05,
		OUT_Z_L_M	= 0x06,
		OUT_Y_H_M	= 0x07,
		OUT_Y_L_M	= 0x08,
		SR_REG_M	= 0x09,
		IRA_REG_M	= 0x0A,
		IRB_REG_M	= 0x0B,
		IRC_REG_M	= 0x0C,
		TEMP_OUT_H_M	= 0x31,
		TEMP_OUT_L_M	= 0x32,
	};
	enum gain_t {
		GAIN_1_3	= 0x20,	// +/- 1.3
		GAIN_1_9	= 0x40,	// +/- 1.9
		GAIN_2_5	= 0x60,	// +/- 2.5
		GAIN_4_0	= 0x80,	// +/- 4.0
		GAIN_4_7	= 0xA0,	// +/- 4.7
		GAIN_5_6	= 0xC0,	// +/- 5.6
		GAIN_8_1	= 0xE0,	// +/- 8.1
	};
	lsm303m(const int& module = 1);
	int start(const gain_t gain = GAIN_1_3);
	int setgain(const gain_t gain);
	int get(float &x, float &y, float &z);
private:
	gain_t m_gain;
	int m_xy;
	int m_z;
};

#endif
