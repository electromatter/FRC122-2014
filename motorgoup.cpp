#include "motorgroup.h"

motorgroup::motorgroup(int pwma, int pwmb, int dir) : m_sp(1), m_p(1), m_i(0), m_d(0), m_last(0), m_int(0), m_tlast(0), m_it(0), m_a(NULL), m_b(NULL), m_enc(NULL), m_dir(dir)
{
	m_a = new Talon(pwma);
	m_b = new Talon(pwmb);
}

motorgroup::motorgroup(int pwma, int pwmb, int enca, int encb) : m_sp(1), m_p(1), m_i(0), m_d(0), m_last(0), m_int(0), m_tlast(0), m_it(0), m_a(NULL), m_b(NULL), m_enc(NULL), m_dir(0)
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
	float e = get() * m_sp - sp;
	float pwm;
	float dt = deltat(m_tlast);
	float de = e - m_last;
	m_last = e;
	if (m_it == 0) {
		pwm = m_p * e;
	} else {
		m_int += e * dt;
		pwm = m_p * e + m_i * m_int + m_d * de;
	}
	m_it++;
	set(pwm);
}

void motorgroup::params(float sp, float p, float i, float d)
{
	m_sp = sp;
	m_p = p;
	m_i = i;
	m_d = d;
	m_it = 0;
	m_last = 0;
	m_int = 0;
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
	if (m_enc)
		m_enc->Reset();
}
