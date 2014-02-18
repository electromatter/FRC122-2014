#include "a10dof.h"

const int lsm303a::s_addr = 0x32;

lsm303a::lsm303a(const int& module) : i2csensor(module, s_addr)
{
}

int lsm303a::start()
{
	return write8(CTRL_REG1_A, 0x27);
}

int lsm303a::get(float &x, float &y, float &z)
{
	int16_t data[3];
	int ret;
	ret = read(OUT_X_L_A | 0x80, data, sizeof(data));
	x = 0;
	y = 0;
	z = 0;
	if (ret < 0)
		return ret;
	data[0] = ((data[0] & 0xFF00) >> 8) | ((data[0] & 0x00FF) << 8);
	data[1] = ((data[1] & 0xFF00) >> 8) | ((data[1] & 0x00FF) << 8);
	data[2] = ((data[2] & 0xFF00) >> 8) | ((data[2] & 0x00FF) << 8);
	data[0] >>= 4;
	data[1] >>= 4;
	data[2] >>= 4;
	x = data[0] * 0.001f * 9.80665f;
	y = data[1] * 0.001f * 9.80665f;
	z = data[2] * 0.001f * 9.80665f;
	return ret;
}

const int lsm303m::s_addr = 0x3C;

lsm303m::lsm303m(const int& module) : i2csensor(module, s_addr)
{
}

int lsm303m::start(const gain_t gain)
{
	int ret;
	ret = write8(MR_REG_M, 0x00);
	if (ret < 0)
		return ret;
	ret = setgain(gain);
	if (ret < 0)
		return ret;
	return ret;
}

int lsm303m::setgain(const gain_t gain)
{
	int ret;
	ret = write8(CRB_REG_M, gain);
	if (ret < 0)
		return ret;
	m_gain = gain;
	switch(gain)
	{
	case GAIN_1_3:
		m_xy = 1100;
		m_z  = 980;
		break;
	case GAIN_1_9:
		m_xy = 855;
		m_z  = 760;
		break;
	case GAIN_2_5:
		m_xy = 670;
		m_z  = 600;
		break;
	case GAIN_4_0:
		m_xy = 450;
		m_z  = 400;
		break;
	case GAIN_4_7:
		m_xy = 400;
		m_z  = 255;
		break;
	case GAIN_5_6:
		m_xy = 330;
		m_z  = 295;
		break;
	case GAIN_8_1:
		m_xy = 230;
		m_z  = 205;
		break;
	}
	return ret;
}

int lsm303m::get(float &x, float &y, float &z)
{
	int16_t data[3];
	int ret;
	ret = read(OUT_X_H_M, data, sizeof(data));
	x = 0;
	y = 0;
	z = 0;
	if (ret < 0)
		return ret;
	x = data[0] / m_xy * 100;
	y = data[1] / m_xy * 100;
	z = data[2] / m_z * 100;
	return ret;
}
