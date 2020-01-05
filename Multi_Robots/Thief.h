#pragma once
#include <cmath>
#include <cstdio>
#include <iostream>
#include "GL/glut.h"
using namespace std;

class Thief
{
private:
	// 地图
	bool grid_flag_[20][20];
	// 速度
	static constexpr double v_thief= 0.008;
	// 被捕半径
	static constexpr double hunt_threshold = 1.1;
	// 被发现后需要逃往的位置
	double escape_pos_[2];
	// 警察的位置
	double police_pos_[4][2];
	// 警察距离小偷的距离
	double police_dis_[4];
	// 记录上下左右哪个方向被警察堵住，+Z,+X,-Z,-X
	bool lock_[4] = {};
	// 记录目标点与当前点的相对位置
	int tar_state_[3] = {};

	static constexpr double pi = 3.1415926;

	double get_dis(const double x, const double z)
	{
		return sqrt(pow(x - this->pos[0], 2) + pow(z - this->pos[1], 2));
	}
	
	bool check_barrier(const double dx, const double dz)
	{
		double x = pos[0] + dx, z = pos[1] + dz;
		if (grid_flag_[int(20 - floor(x + r))][int(floor(z + r))] || grid_flag_[int(20 - floor(x - r))][int(floor(z + r))]
			|| grid_flag_[int(20 - floor(x + r))][int(floor(z - r))] || grid_flag_[int(20 - floor(x - r))][int(floor(z - r))])
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool check_lock(const double dx, const double dz)
	{
		if (abs(dx) < dz && dz >= 0)
		{
			// +Z
			return lock_[0];
		}
		else if (abs(dx) < -dz && dz < 0)
		{
			// -Z
			return lock_[2];
		}
		else if (dx > abs(dz) && dx >= 0)
		{
			// +X
			return lock_[1];
		}
		else if (-dx > abs(dz) && dx < 0)
		{
			// -X
			return lock_[3];
		}
	}

	bool update_tar_state(const double dx, const double dz)
	{
		if (tar_state_[0] == tar_state_[2] && tar_state_[0] != tar_state_[1] && tar_state_[0] != 0)
		{
			tar_state_[0] = tar_state_[1] = tar_state_[2] = 0;
			return false;
		}
		else
		{
			tar_state_[0] = tar_state_[1];
			tar_state_[1] = tar_state_[2];
			if (dx == 0 && dz > 0)
			{
				tar_state_[2] = 1;
			}
			else if (dx > 0 && dz > 0)
			{
				tar_state_[2] = 2;
			}
			else if (dx > 0 && dz == 0)
			{
				tar_state_[2] = 3;
			}
			else if (dx > 0 && dz < 0)
			{
				tar_state_[2] = 4;
			}
			else if (dx == 0 && dz < 0)
			{
				tar_state_[2] = 5;
			}
			else if (dx < 0 && dz < 0)
			{
				tar_state_[2] = 6;
			}
			else if (dx < 0 && dz == 0)
			{
				tar_state_[2] = 7;
			}
			else if (dx < 0 && dz > 0)
			{
				tar_state_[2] = 8;
			}
			return true;
		}
	}
	
public:
	// 渲染对象的半径
	static constexpr double r = 0.45;
	// 小偷位置
	double pos[2];
	// 状态 0-自由移动 1-跑路 2-被捕
	int status;

	Thief(const double x, const double z, const bool flag[20][20])
	{
		this->pos[0] = x;
		this->pos[1] = z;
		for (auto i = 0; i < 20; i++)
		{
			for (auto j = 0; j< 20; j++)
			{
				this->grid_flag_[i][j] = flag[i][j];
			}
		}
		status = 0;
	}

	// 操作执行
	void action()
	{
		static double dis, dx, dz, tar_pos[2];
		static auto at_new_pos = true, is_new_esc = true;
		// 执行操作
		switch (status)
		{
		case 0:
			// 自由移动
			if (at_new_pos)
			{
				do
				{
					tar_pos[0] = double(rand() % 20) + 0.5;
					tar_pos[1] = double(rand() % 20) + 0.5;
					printf("again\n");
				}
				while (grid_flag_[int(20 - floor(tar_pos[0]))][int(floor(tar_pos[1]))]);
				at_new_pos = false;
				printf("NEW pos: %f %f\n", tar_pos[0], tar_pos[1]);
			}
			dis = get_dis(tar_pos[0], tar_pos[1]);
			dx = v_thief * (tar_pos[0] - this->pos[0]) / dis;
			dz = v_thief * (tar_pos[1] - this->pos[1]) / dis;
			
			glColor3d(1.0, 0.0, 0.0);
			glBegin(GL_POLYGON);
			for (int i = 0; i < 1000; i++)
			{
				glVertex3f(tar_pos[0] + 0.1 * cos(2 * pi * i / 1000), 0.1, tar_pos[1] + 0.1 * sin(2 * pi * i / 1000));   //定义顶点
			}
			glEnd();
			
			at_new_pos = dis <= 1;
			break;
		case 1:
			// 跑路
			at_new_pos = true;
			dis = get_dis(escape_pos_[0], escape_pos_[1]);
			dx = v_thief * (escape_pos_[0] - this->pos[0]) / dis;
			dz = v_thief * (escape_pos_[1] - this->pos[1]) / dis;

			break;
		case 2:
			// 被捕
			printf("Success to arrest! ");
			exit(0);
			break;
		default:
			break;
		}
		
		if (check_barrier(dx, dz) || check_lock(dx, dz))
		{
			if (!check_barrier(dx, 0) && !check_lock(dx, 0))
			{
				dx = v_thief * dx / fabs(dx);
				dz = 0;
			}
			else if (!check_barrier(0, dz) && !check_lock(0, dz))
			{
				dx = 0;
				dz = v_thief * dz / fabs(dz);
			}
			else
			{
				dx = 0;
				dz = 0;
				printf("other situation\n");
			}
		}
		pos[0] += dx;
		pos[1] += dz;
		bool result = update_tar_state(dx, dz);
		if (!result)
		{
			at_new_pos = true;
		}
		printf("X: %f Z: %f %d %d %d %d \n", dx, dz, lock_[0], lock_[2], lock_[1], lock_[3]);
	}
	
	void update(double po_pos[4][2], int po_state[4])
	{
		auto cnt = 0;
		double vector[4][2];
		escape_pos_[0] = this->pos[0];
		escape_pos_[1] = this->pos[1];

		for (auto i = 0; i < 4; i++) 
		{
			lock_[i] = false;
		}

		for (auto i = 0; i < 4; i++)
		{
			// 导入警察位置，计算距离
			police_pos_[i][0] = po_pos[i][0];
			police_pos_[i][1] = po_pos[i][1];
			police_dis_[i] = get_dis(police_pos_[i][0], police_pos_[i][1]);

			// 根据警察状态判断小偷状态
			if (po_state[i] == 3)
			{
				// 其中一个警察在追捕，计算逃逸向量
				status = 1;
				vector[i][0] = (this->pos[0] - police_pos_[i][0]) / police_dis_[i];
				vector[i][1] = (this->pos[1] - police_pos_[i][1]) / police_dis_[i];
				escape_pos_[0] += vector[i][0] / police_dis_[i];
				escape_pos_[1] += vector[i][1] / police_dis_[i];

				// 警察抓捕到某一方向，更新lock_[]
				if (police_dis_[i] <= hunt_threshold)
				{
					if (abs(vector[i][0]) < vector[i][1] && vector[i][1] >= 0)
					{
						// +Z
						lock_[0] = true;
					}
					else if (abs(vector[i][0]) < -vector[i][1] && vector[i][1] < 0)
					{
						// -Z
						lock_[2] = true;
					}
					else if (vector[i][0] > abs(vector[i][1]) && vector[i][0] >= 0)
					{
						// +X
						lock_[1] = true;
					}
					else if (-vector[i][0] > abs(vector[i][1]) && vector[i][0] < 0)
					{
						// -X
						lock_[3] = true;
					}
				}
			}
			else
			{
				cnt++;
			}
		}
		
		if (cnt == 4)
		{
			status = 0;
		}

		for (auto i = 0; i < 4; i++)
		{
			if (lock_[i] == false)
			{
				break;
			}
			if (i == 4 || ((lock_[0] || lock_[2]) && (lock_[1] || lock_[3])))
			{
				status = 2;
			}
		}

		action();
	}
};

