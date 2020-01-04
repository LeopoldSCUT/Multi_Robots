#pragma once
#include <cmath>
#include <cstdio>


class Thief
{
private:
	// ��ͼ
	bool grid_flag_[20][20];
	// �ٶ�
	static constexpr double v_thief= 0.1;
	// �����뾶
	static constexpr double hunt_threshold = 0.5;
	// �����ֺ���Ҫ������λ��
	double escape_pos_[2];
	// �����λ��
	double police_pos_[4][2];
	// �������С͵�ľ���
	double police_dis_[4];
	// ��¼���������ĸ����򱻾����ס��+Z,+X,-Z,-X
	bool lock_[4] = {};
	
	double get_dis(const double x, const double z)
	{
		return sqrt(pow(x - this->pos[0], 2) + pow(z - this->pos[1], 2));
	}
	
public:
	// С͵λ��
	double pos[2];
	// ״̬ 0-�����ƶ� 1-��· 2-����
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

	// ����ִ��
	void action()
	{
		static double dis, dx, dz, ready_x, ready_z, tar_pos[2];
		static auto at_new_pos = true, is_new_esc = true;
		// ִ�в���
		switch (status)
		{
		case 0:
			// �����ƶ�
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
			// ��·
			at_new_pos = true;
			dis = get_dis(escape_pos_[0], escape_pos_[1]);
			dx = v_thief * (escape_pos_[0] - this->pos[0]) / dis;
			dz = v_thief * (escape_pos_[1] - this->pos[1]) / dis;
			break;
		case 2:
			// ����
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
			// ���뾯��λ�ã��������
			police_pos_[i][0] = po_pos[i][0];
			police_pos_[i][1] = po_pos[i][1];
			police_dis_[i] = get_dis(police_pos_[i][0], police_pos_[i][1]);

			// ���ݾ���״̬�ж�С͵״̬
			if (po_state[i] == 3)
			{
				// ����һ��������׷����������������
				status = 1;
				vector[i][0] = (this->pos[0] - police_pos_[i][0]) / police_dis_[i];
				vector[i][1] = (this->pos[1] - police_pos_[i][1]) / police_dis_[i];
				escape_pos_[0] += vector[i][0] / police_dis_[i];
				escape_pos_[1] += vector[i][1] / police_dis_[i];

				// ����ץ����ĳһ���򣬸���lock_[]
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

