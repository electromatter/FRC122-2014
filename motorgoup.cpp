#include "motorgroup.h"

motorgroup::motorgroup(int pwma, int pwmb, int dir) : m_a(NULL), m_b(NULL), m_enc(NULL), m_dir(dir)
{
	m_a = new Talon(pwma);
	m_b = new Talon(pwmb);
}

motorgroup::motorgroup(int pwma, int pwmb, int enca, int encb) : m_a(NULL), m_b(NULL), m_enc(NULL), m_dir(0)
{
	m_a = new Talon(pwma);
	m_b = new Talon(pwmb);
	m_enc = new Encoder(enca, encb);
	m_enc->Start();
}

motorgroup::~motorgroup()
{
	if (m_a)
		delete m_a;
	if (m_b)
		delete m_b;
	if (m_enc)
		delete m_enc;
}

void motorgroup::speed(float sp)
{
	set(input(get(), sp));
}

void motorgroup::set(float pwm)
{
	if (m_a)
		m_a->Set(pwm);
	if (m_b)
		m_b->Set(m_dir ? -pwm : pwm);
}

float motorgroup::get()
{
	if (m_enc)
		return m_enc->GetRate();
	else
		return 0;
}

void motorgroup::reset()
{
	pid::reset();
	if (m_enc)
		m_enc->Reset();
}
