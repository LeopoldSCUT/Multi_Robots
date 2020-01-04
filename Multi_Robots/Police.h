#pragma once
#include<cmath>
#include"Thief.h"

class Police
{
public:
	// �ٶ�
	static constexpr double v_police = 0.8;
	// ��Ⱦ����İ뾶
	static constexpr double r = 1.0;
	// ����뾶
	static constexpr double alert_threshold = 3.0;
	// ��Ұ�뾶
	static constexpr double view_threshold = 2.0;
	// ���һ�θ��µ�λ��
	static double last_found_pos[2];
	// λ��
	double pos[2];
	// ״̬ 0-Ѳ�� 1-��· 2-׷��
	int status;

	Police(const double x, const double z)
	{
		this->pos[0] = x;
		this->pos[1] = z;
		status = 0;
	}


	double getDistance(const Thief& theft)
	{
		return sqrt(pow(theft.pos[0] - this->pos[0], 2)
			+ pow(theft.pos[1] - this->pos[1], 2));
	}

	bool checkBarrier(const double dx, const double dz, bool grid_flag[20][20])
	{
		
		return false;
	}

	void patrol()
	{
		return;
	}

	void move(const Thief& thief, bool grid_flag[20][20])
	{
		const double dis = getDistance(thief);
		double dx = 0;
		double dz = 0;
		double direction = 0;

		// ����Ƿ��ھ��䷶Χ��
		if (dis <= alert_threshold)
		{
			this->status = 2;
		}
		if (dis >= view_threshold)
		{

		}



		if (this->status == 0)
		{
			patrol();
		}
		else if (this->status == 1)
		{

		}
		else if (this->status == 2)
		{
			// ��ʼΧ���������ƶ�����


			// ����Ƿ���Ҫ����
			if (checkBarrier(dx, dz, grid_flag))
			{

			}
		}
		return;
	}
};

