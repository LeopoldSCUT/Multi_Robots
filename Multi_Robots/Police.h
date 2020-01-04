#pragma once
#include<cmath>
#include"Thief.h"

class Police
{
public:
	// 警察的速度
	static constexpr double v_police = 0.8;
	// 警察对象的半径
	static constexpr double r = 1.0;
	// 警察的检测半径
	static constexpr double monitor_threshold = 3.0;
	// 警察的位置
	double pos[2];
	// 警察的状态 0-监视 1-追捕
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
			// 开始围捕，计算移动方向


			// 检测是否需要避障
			if (checkBarrier())
			{

			}
			return;
		}
		else
		{
			// 保持不动
			return;
		}
	}

};

