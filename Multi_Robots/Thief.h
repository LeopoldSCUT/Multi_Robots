#pragma once
#include <cmath>
#include <cstdio>


class Thief
{
private:
	// 地图
	bool grid_flag_[20][20];
	// 速度
	static constexpr double v_thief= 0.1;
	// 被捕半径
	static constexpr double hunt_threshold = 0.5;
	// 被发现后需要逃往的位置
	double escape_pos_[2];
	// 警察的位置
	double police_pos_[4][2];
	// 警察距离小偷的距离
	double police_dis_[4];
	// 记录上下左右哪个方向被警察堵住，+Z,+X,-Z,-X
	bool lock_[4] = {};
	
	double get_dis(const double x, const double z)
	{
		return sqrt(pow(x - this->pos[0], 2) + pow(z - this->pos[1], 2));
	}
	
public:
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
		static double dis, dx, dz, ready_x, ready_z, tar_pos[2];
		static auto at_new_pos = true, is_new_esc = true;
		// 执行操作
		switch (status)
		{
		case 0:
			// 自由移动
			if (at_new_pos)
			{
				// printf("Update new pos\n");
				// double sss = 18 * double(rand()) / RAND_MAX + 1;
				// tar_pos[0] = 18 * double(rand()) / RAND_MAX + 1;
				// tar_pos[1] = 18 * double(rand()) / RAND_MAX + 1;

				do
				{
					tar_pos[0] = (rand() % 19) + 1;
					tar_pos[1] = (rand() % 19) + 1;	
				}
				while (grid_flag_[int(20 - floor(tar_pos[0]))][int(floor(tar_pos[1]))]);
				at_new_pos = false;
				printf("NEW pos: %f %f\n", tar_pos[0], tar_pos[1]);
			}
			dis = get_dis(tar_pos[0], tar_pos[1]);
			dx = v_thief * (tar_pos[0] - this->pos[0]) / dis;
			dz = v_thief * (tar_pos[1] - this->pos[1]) / dis;
			
			at_new_pos = dis <= 0.5;
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
		
		ready_x = pos[0] + dx;
		ready_z = pos[1] + dz;
		if (grid_flag_[int(20 - floor(ready_x))][int(floor(ready_z))])
		{
			if (!grid_flag_[int(20 - floor(ready_x))][int(floor(ready_z - dz))])
			{
				ready_x = pos[0] + 0.5 * v_thief;
				ready_z -= dz;
			}
			else if (!grid_flag_[int(20 - floor(ready_x - dx))][int(floor(ready_z))])
			{
				ready_x -= dx;
				ready_z = pos[1] + 0.5 * v_thief * dz / abs(dz);
			}
			else
			{
				printf("other situation\n");
			}
		}
		pos[0] = ready_x;
		pos[1] = ready_z;
		// printf("X: %f Z: %f\n", pos[0], pos[1]);
	}
	
	void update(double po_pos[4][2], int po_state[4])
	{
		auto cnt = 0;
		double vector[4][2];
		escape_pos_[0] = this->pos[0];
		escape_pos_[1] = this->pos[1];

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
			if (i == 4)
			{
				status = 2;
			}
		}

		action();
	}
};

