#include "a10dof.h"

const float l3gd20::s_dps[3] = {0.00875f, 0.0175f, 0.070f};
const int l3gd20::s_addr = 0xD6;
const int l3gd20::s_id = 0xD4;

l3gd20::l3gd20(const int& module) : i2csensor(module, s_addr), m_range(DPS250), m_state(0)
{
}

int l3gd20::start(const range_t& range)
{
	int ret = read8(l3gd20::WHO_AM_I);
	if (ret != s_id) {
		m_state = 0;
		return -1;
	}
	ret = write8(CTRL_REG1, 0x0F);
	if (ret < 0) {
		m_state = 0;
		return -1;
	}
	ret = setrange(range);
	if (ret < 0) {
		m_state = 0;
		return -1;
	}
	m_state = 1;
	return 0;
}

int l3gd20::setrange(const range_t& range)
{
	int ret;
	switch(m_range)
	{
	case DPS250:
		ret = write8(CTRL_REG4, 0x00);
		break;
	case DPS500:
		ret = write8(CTRL_REG4, 0x10);
		break;
	case DPS2000:
		ret = write8(CTRL_REG4, 0x20);
		break;
	}
	if (ret < 0)
		return ret;
	m_range = range;
	return ret;
}

int l3gd20::get(float &x, float &y, float &z)
{
	int16_t data[3];
	int ret;
	if (!m_state)
		return -1;

	ret = read(OUT_X_L | 0x80, data, sizeof(data));

	if (ret < 0)
		return ret;

	data[0] = ((data[0] & 0xFF00) >> 8) | ((data[0] & 0x00FF) << 8);
	data[1] = ((data[1] & 0xFF00) >> 8) | ((data[1] & 0x00FF) << 8);
	data[2] = ((data[2] & 0xFF00) >> 8) | ((data[2] & 0x00FF) << 8);
	
	x = data[0] * s_dps[m_range];
	y = data[1] * s_dps[m_range];
	z = data[2] * s_dps[m_range];

	return ret;
}

float l3gd20::getangle()
{
	int16_t data;
	int ret;
	float y;
	
	if (!m_state)
		return 0.f;

	ret = read(OUT_Z_L | 0x80, &data, sizeof(data));
	
	if (ret < 0)
		return 0.f;

	data = ((data & 0xFF00) >> 8) | ((data & 0x00FF) << 8);
	
	y = data * s_dps[m_range];

	return y;
}
