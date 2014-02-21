#ifndef PID_H
#define PID_H

class pid
{
public:
	pid(float sp = 0, float p = 1, float i = 0, float d = 0);
	virtual ~pid();
	float input(float x, float setpoint);
	void params(float sp = 0, float p = 1, float i = 0, float d = 0);
	void reset();
private:
	float m_sp, m_p, m_i, m_d;
	float m_last;
	float m_int;
	double m_tlast;
	int m_it;
};

#endif
