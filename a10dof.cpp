#include "a10dof.h"

i2csensor::i2csensor(const int& module, const int& addr)
{
	//that r b an exception.
	DigitalModule* m = DigitalModule::GetInstance(module);
	if (m == NULL)
		;//WTF;
	m_i2c = m->GetI2C(addr);
	if (m_i2c == NULL)
		;//WTF;
}

i2csensor::~i2csensor()
{
	delete m_i2c;
}

int i2csensor::write8(int addr, int x)
{
	return m_i2c->Write(addr, x) ? 1 : 0;
}

int i2csensor::read8(int addr)
{
	unsigned char d;
	int ret;
	ret = read(addr, &d, 1);
	if (ret < 0)
		return ret;
	return d;
}

int i2csensor::read(int addr, void* x, size_t len)
{
	if (m_i2c->Read(addr, len, (uint8_t*)x))
		return -1;
	else
		return len;
}
