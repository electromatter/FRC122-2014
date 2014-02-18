#include "a10dof.h"

const int bmp085::s_addr = 0xEE;
const int bmp085::s_id = 0x55;

bmp085::bmp085(const int& module) : i2csensor(module, s_addr), m_state(0)
{
}

int bmp085::start()
{
	int ret;
	int16_t data;
	
	if (read8(CHIPID) != s_id)
		return -1;
	
	ret = read(CAL_AC1, &data, sizeof(data));
	if (ret < 0)
		return ret;
	//data = ((data & 0x00FF) << 8) | ((data & 0xFF00) >> 8);
	m_calib.ac1 = data;

	ret = read(CAL_AC2, &data, sizeof(data));
	if (ret < 0)
		return ret;
	//data = ((data & 0x00FF) << 8) | ((data & 0xFF00) >> 8);
	m_calib.ac2 = data;

	ret = read(CAL_AC3, &data, sizeof(data));
	if (ret < 0)
		return ret;
	//data = ((data & 0x00FF) << 8) | ((data & 0xFF00) >> 8);
	m_calib.ac3 = data;

	ret = read(CAL_AC4, &data, sizeof(data));
	if (ret < 0)
		return ret;
	//data = ((data & 0x00FF) << 8) | ((data & 0xFF00) >> 8);
	m_calib.ac4 = data;

	ret = read(CAL_AC5, &data, sizeof(data));
	if (ret < 0)
		return ret;
	//data = ((data & 0x00FF) << 8) | ((data & 0xFF00) >> 8);
	m_calib.ac5 = data;

	ret = read(CAL_AC6, &data, sizeof(data));
	if (ret < 0)
		return ret;
	//data = ((data & 0x00FF) << 8) | ((data & 0xFF00) >> 8);
	m_calib.ac6 = data;
	
	ret = read(CAL_MB, &data, sizeof(data));
	if (ret < 0)
		return ret;
	//data = ((data & 0x00FF) << 8) | ((data & 0xFF00) >> 8);
	m_calib.mb = data;
	
	ret = read(CAL_MC, &data, sizeof(data));
	if (ret < 0)
		return ret;
	//data = ((data & 0x00FF) << 8) | ((data & 0xFF00) >> 8);
	m_calib.mc = data;
	
	ret = read(CAL_MD, &data, sizeof(data));
	if (ret < 0)
		return ret;
	//data = ((data & 0x00FF) << 8) | ((data & 0xFF00) >> 8);
	m_calib.md = data;
	
	ret = read(CAL_B1, &data, sizeof(data));
	if (ret < 0)
		return ret;
	//data = ((data & 0x00FF) << 8) | ((data & 0xFF00) >> 8);
	m_calib.b1 = data;
	
	ret = read(CAL_B2, &data, sizeof(data));
	if (ret < 0)
		return ret;
	//data = ((data & 0x00FF) << 8) | ((data & 0xFF00) >> 8);
	m_calib.b2 = data;
	
	m_state = 1;
	
	return 123;
}
/*
void bmp085::setmode(const mode_t &mode)
{
	m_mode = mode;
}

int bmp085::get(float &temp, float &pressure)
{
	if (!m_state)
		return -1;
}*/

int bmp085::get(float &temp)
{
	int ret;
	int16_t data;
	temp = 0;
	if (!m_state)
		return -1;
	ret = write8(CONTROL, READTEMPCMD);
	if (ret < 0)
		return ret;
	Wait(5.f/1000.f);
	ret = read(TEMPDATA, &data, sizeof(data));
	if (ret < 0)
		return ret;
	//data = ((data & 0x00FF) << 8) | ((data & 0xFF00) >> 8);
	float X1 = (((int)data - m_calib.ac6) * m_calib.ac5) / 32768.;
	float X2 = (m_calib.mc << 11)  / (X1 + (float)m_calib.md);
	temp = (X1 + X2 + 8.) / 16.;
	return ret;
}

float bmp085::readtemp()
{
	float t;
	if (get(t) < 0)
		return 0;
	return t;
}
