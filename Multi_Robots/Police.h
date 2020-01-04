#pragma once
#include<cmath>
#include<iostream>
using namespace std;

class Police
{
public:
	// �ٶ�
	static constexpr double v_police = 0.001;
	// ��Ⱦ����İ뾶
	static constexpr double r = 1.0;
	// ����뾶
	static constexpr double alert_threshold = 200.0;
	// ��Ұ�뾶
	static constexpr double view_threshold = 200.0;
	// ���һ�θ��µ�λ��
	static double last_found_pos[2];
	// �Ƿ��ټ���������ǰ��last_found_pos
	static bool all_action;
	// λ��
	double pos[2];
	// ����λ��
	double origin_pos[2];
	// ״̬ 0-Ѳ�� 1-��· 2-��λ 3-׷��
	static enum { PATROL, HEADING, RETURN, CAPTURE };
	int curr_status, last_status;

public:
	Police(const double x, const double z)
	{
		this->pos[0] = x;
		this->pos[1] = z;
		this->origin_pos[0] = x;
		this->origin_pos[1] = z;
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
		int x = this->pos[0] + dx, z = this->pos[1] + dz;
		/*if (dx != 0)
		{
			x = this->pos[0] + dx + (dx / abs(dx)) * r;
		}
		if (dz != 0)
		{
			z = this->pos[1] + dz + (dz / abs(dz)) * r;
		}*/
		cout << dz << endl;
		if (grid_flag[int(20 - floor(x + r))][int(floor(z + r))] || grid_flag[int(20 - floor(x - r))][int(floor(z + r))]
			|| grid_flag[int(20 - floor(x + r))][int(floor(z - r))] || grid_flag[int(20 - floor(x - r))][int(floor(z - r))])
		{
			cout << "True" << endl;
			return true;
		}
		else
		{
			return false;
		}
		//return grid_flag[int(20 - floor(x))][int(floor(z))];
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

		/* 1. ȷ����ǰ״̬ */
		// �ȼ���Ƿ��ܵ��ٻ�
		if (this->curr_status != CAPTURE && all_action == true)
		{
			this->curr_status = HEADING;		// ״̬����Ϊ��·
		}
		else if (all_action == false && this->last_status == HEADING)
		{
			this->curr_status = RETURN;			// ״̬����Ϊ��λ
		}
		// �ټ���Ƿ�����Ұ��Χ֮��
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
					this->curr_status = PATROL;	// ״̬����ΪѲ��
				}
			}
		}
		else
		{
			// ������Ƿ��ھ��䷶Χ��
			if (dis <= alert_threshold || this->last_status == CAPTURE)
			{
				this->curr_status = CAPTURE;	// ״̬����Ϊץ��
				all_action = true;
			}
		}
		this->last_status = this->curr_status;


		/* 2. ִ����Ӧ���� */
		if (this->curr_status == PATROL)		// Ѳ��
		{
			patrol();
		}
		else if (this->curr_status == HEADING)	// ��·
		{
			dx = this->v_police * (last_found_pos[0] - this->pos[0]) / getDistance(last_found_pos);
			dz = this->v_police * (last_found_pos[1] - this->pos[1]) / getDistance(last_found_pos);
			if (getDistance(last_found_pos) < 0.1) 
			{
				all_action = false;
			}
		}
		else if (this->curr_status == RETURN)	// ��λ
		{
			dx = this->v_police * (this->origin_pos[0] - this->pos[0]) / getDistance(this->origin_pos);
			dz = this->v_police * (this->origin_pos[1] - this->pos[1]) / getDistance(this->origin_pos);
		}
		else if (this->curr_status == CAPTURE)	// ׷��
		{
			// ʵʱ���������λ��
			last_found_pos[0] = thief_pos[0];
			last_found_pos[1] = thief_pos[1];
			dx = this->v_police * (thief_pos[0] - this->pos[0]) / dis;
			dz = this->v_police * (thief_pos[1] - this->pos[1]) / dis;
		}

		/* 3. ����̬�ϰ���andС͵����ײ */
		// �Ѿ������������dx��dy������Ƿ���Ҫ����
		if (checkBarrier(dx, dz, grid_flag))
		{
			static auto cnt = 0;
			auto x_ba = checkBarrier(v_police * dx / abs(dx), 0, grid_flag);
			auto z_ba = checkBarrier(0, v_police * dz / abs(dz), grid_flag);
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
				cout << "123123123" << endl;
			}
			//cout << cnt++ << " Barrier: " << x_ba << z_ba << endl;
		}


		/* 4. ������ʵ��λ�� */
		this->pos[0] += dx;
		this->pos[1] += dz;
		// cout << grid_flag[int(20 - floor(this->pos[0]))][int(floor(this->pos[1]))] << endl;
		return;
	}
};

