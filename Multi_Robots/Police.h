#pragma once
#include<cmath>
#include"Thief.h"

class Police
{
public:
	// ������ٶ�
	static constexpr double v_police = 0.8;
	// �������İ뾶
	static constexpr double r = 1.0;
	// ����ļ��뾶
	static constexpr double monitor_threshold = 3.0;
	// �����λ��
	double pos[2];
	// �����״̬ 0-���� 1-׷��
	int status;

	Police(const double x, const double z)
	{
		this->pos[0] = x;
		this->pos[1] = z;
		status = 0;
	}

	void walkAround()
	{

	}

	double getDistance(const Thief& theft)
	{
		return sqrt(pow(theft.pos[0] - this->pos[0], 2)
			+ pow(theft.pos[1] - this->pos[1], 2));
	}

	bool checkBarrier()
	{
		return true;
	}


	void move(const Thief& theft)
	{
		const double dis = getDistance(theft);
		if (dis <= monitor_threshold)
		{
			// ��ʼΧ���������ƶ�����


			// ����Ƿ���Ҫ����
			if (checkBarrier())
			{

			}
			return;
		}
		else
		{
			// ���ֲ���
			return;
		}
	}

};

