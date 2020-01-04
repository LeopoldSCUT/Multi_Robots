#pragma once
#include<cmath>
#include<iostream>
using namespace std;

class Police
{
public:
	// 速度
	static constexpr double v_police = 0.004;
	// 渲染对象的半径
	static constexpr double r = 0.5;
	// 警戒半径
	static constexpr double alert_threshold = 2.5;
	// 视野半径
	static constexpr double view_threshold = 3.5;
	// 最后一次更新的位置
	static double last_found_pos[2];
	// 是否召集其他警察前往last_found_pos
	static bool all_action;
	// 警察编号
	int id;
	// 警察目标偏移
	double offset[2];
	// 位置
	double pos[2];
	// 出生位置
	double origin_pos[2];
	// 状态 0-巡逻 1-赶路 2-归位 3-追捕
	static enum { PATROL, HEADING, RETURN, CAPTURE };
	int curr_status, last_status;

public:
	Police(const double x, const double z, const int id)
	{
		this->pos[0] = x;
		this->pos[1] = z;
		this->origin_pos[0] = x;
		this->origin_pos[1] = z;
		this->id = id;
		switch (this->id)
		{
		case 0:
			offset[0] = 0;
			offset[1] = -2*r;
			break;
		case 1:
			offset[0] = -2*r;
			offset[1] = 0;
			break;
		case 2:
			offset[0] = 0;
			offset[1] = +2*r;
			break;
		case 3:
			offset[0] = +2*r;
			offset[1] = 0;
			break;
		default:
			break;
		}
		this->curr_status = PATROL;
		this->last_status = this->curr_status;
	}

	double getDistance(const double target_pos[])
	{
		return sqrt(pow(target_pos[0] - this->pos[0], 2)
			+ pow(target_pos[1] - this->pos[1], 2));
	}

	bool get_flag(const double x, const double y)
	{
		return 0;
	}

	bool checkBarrier(const double dx, const double dz, const bool grid_flag[20][20])
	{
		double x = this->pos[0] + dx, z = this->pos[1] + dz;
		if (grid_flag[int(20 - floor(x + r))][int(floor(z + r))] || grid_flag[int(20 - floor(x - r))][int(floor(z + r))]
			|| grid_flag[int(20 - floor(x + r))][int(floor(z - r))] || grid_flag[int(20 - floor(x - r))][int(floor(z - r))])
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void patrol()
	{
		return;
	}

	void move(const double thief_pos[], const bool grid_flag[20][20])
	{
		const double dis = getDistance(thief_pos);
		double dx = 0;
		double dz = 0;

		/* 1. 确定当前状态 */
		// 先检测是否受到召唤
		if (this->curr_status != CAPTURE && all_action == true)
		{
			this->curr_status = HEADING;		// 状态设置为赶路
		}
		else if (all_action == false && this->last_status == HEADING)
		{
			this->curr_status = RETURN;			// 状态设置为归位
		}
		// 再检测是否在视野范围之外
		if (dis >= view_threshold)
		{
			if (this->last_status == CAPTURE)
			{
				this->curr_status = HEADING;
			}
			if (this->curr_status == RETURN)
			{
				if (getDistance(this->origin_pos) < 0.1)
				{
					this->curr_status = PATROL;	// 状态设置为巡逻
				}
			}
		}
		else
		{
			// 最后检测是否在警戒范围内
			if (dis <= alert_threshold || this->last_status == CAPTURE)
			{
				this->curr_status = CAPTURE;	// 状态设置为抓捕
				all_action = true;
			}
		}
		this->last_status = this->curr_status;


		/* 2. 执行相应动作 */
		if (this->curr_status == PATROL)		// 巡逻
		{
			patrol();
		}
		else if (this->curr_status == HEADING)	// 赶路
		{
			dx = this->v_police * (last_found_pos[0] - this->pos[0]) / getDistance(last_found_pos);
			dz = this->v_police * (last_found_pos[1] - this->pos[1]) / getDistance(last_found_pos);
			if (getDistance(last_found_pos) < 0.1)
			{
				all_action = false;
			}
		}
		else if (this->curr_status == RETURN)	// 归位
		{
			dx = this->v_police * (this->origin_pos[0] - this->pos[0]) / getDistance(this->origin_pos);
			dz = this->v_police * (this->origin_pos[1] - this->pos[1]) / getDistance(this->origin_pos);
		}
		else if (this->curr_status == CAPTURE)	// 追捕
		{
			// 实时更新最后发现位置
			last_found_pos[0] = thief_pos[0];
			last_found_pos[1] = thief_pos[1];
			dx = this->v_police * (thief_pos[0] - (this->pos[0] + offset[0])) / dis;
			dz = this->v_police * (thief_pos[1] - (this->pos[1] + offset[1])) / dis;
		}

		/* 3. 处理静态障碍物and小偷的碰撞 */
		// 已经计算出期望的dx和dy，检测是否需要避障
		if (checkBarrier(dx, dz, grid_flag))
		{
			if (dx != 0 && !checkBarrier(v_police * dx / fabs(dx), 0, grid_flag))
			{
				dx = v_police * dx / fabs(dx);
				dz = 0;
			}
			else if (dz != 0 && !checkBarrier(0, v_police * dz / fabs(dz), grid_flag))
			{
				dx = 0;
				dz = v_police * dz / fabs(dz);
			}
			else
			{
				dx = 0;
				dz = 0;
				cout << "ERROR" << endl;
			}
		}


		/* 4. 最后更新实际位置 */
		this->pos[0] += dx;
		this->pos[1] += dz;
		// cout << grid_flag[int(20 - floor(this->pos[0]))][int(floor(this->pos[1]))] << endl;
		return;
	}
};

