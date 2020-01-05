#pragma once
#include <cmath>
#include <iostream>
#include "GL/glut.h"
using namespace std;

class Police
{
public:
	// �ٶ�
	static constexpr double v_police = 0.004;
	// ��Ⱦ����İ뾶
	static constexpr double r = 0.5;
	// ����뾶
	static constexpr double alert_threshold = 2.5;
	// ��Ұ�뾶
	static constexpr double view_threshold = 3.5;
	// ���һ�θ��µ�λ��
	static double last_found_pos[2];
	// �Ƿ��ټ���������ǰ��last_found_pos
	static bool all_action;
	// �캽������
	static int leader;
	// ������
	int id;
	// ����Ŀ��ƫ��
	double offset[2];
	// λ��
	double pos[2];
	// ����λ��
	double origin_pos[2];
	// ״̬ 0-Ѳ�� 1-��· 2-��λ 3-׷��
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
			dx = this->v_police * (thief_pos[0] - (this->pos[0] + offset[0])) / dis;
			dz = this->v_police * (thief_pos[1] - (this->pos[1] + offset[1])) / dis;
		}

		/* 3. ����̬�ϰ���andС͵����ײ */
		// �Ѿ������������dx��dy������Ƿ���Ҫ����
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
			}
		}
		if (sqrt(pow(this->pos[0] + dx - thief_pos[0], 2) + pow(this->pos[1] + dx - thief_pos[1], 2)) < 1.95 * r)
		{
			dx = 0;
			dz = 0;
		}


		/* 4. ������ʵ��λ�� */
		this->pos[0] += dx;
		this->pos[1] += dz;
		return;
	}


	void advancedMove(const double thief_pos[], const bool grid_flag[20][20])
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
			// ȷ���Լ���θ�·
			double arr[2] = {};
			if (leader == this->id) // �캽��·
			{
				arr[0] = last_found_pos[0] + offset[0];
				arr[1] = last_found_pos[1] + offset[1];
				dx = this->v_police * (-this->pos[0] + arr[0]) / getDistance(arr);
				dz = this->v_police * (-this->pos[1] + arr[1]) / getDistance(arr);
			}
			else if (leader != this->id)			// ��Χ��·
			{
				arr[0] = last_found_pos[0] + 2.5 * offset[0];
				arr[1] = last_found_pos[1] + 2.5 * offset[1];
				dx = this->v_police * (-this->pos[0] + arr[0]) / getDistance(arr);
				dz = this->v_police * (-this->pos[1] + arr[1]) / getDistance(arr);
			}
			else
			{
				cout << "eeeeeeeeeeeeeeeeeeeeee" << endl;
			}
			
			if (getDistance(arr) < 0.1)
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
			// ȷ���캽������
			if (leader == -1) {
				leader = this->id;
			}
			// ʵʱ���������λ��
			last_found_pos[0] = thief_pos[0];
			last_found_pos[1] = thief_pos[1];
			if (leader == this->id)
			{
				dx = this->v_police * (thief_pos[0] - (this->pos[0])) / dis;
				dz = this->v_police * (thief_pos[1] - (this->pos[1])) / dis;
			}
			else
			{
				dx = this->v_police * (thief_pos[0] - (this->pos[0] + offset[0])) / dis;
				dz = this->v_police * (thief_pos[1] - (this->pos[1] + offset[1])) / dis;
			}
		}

		/* 3. ����̬�ϰ���andС͵����ײ */
		// �Ѿ������������dx��dy������Ƿ���Ҫ����
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
			}
		}
		if (sqrt(pow(this->pos[0] + dx - thief_pos[0], 2) + pow(this->pos[1] + dx - thief_pos[1], 2)) < 1.95 * r)
		{
			dx = 0;
			dz = 0;
		}


		/* 4. ������ʵ��λ�� */
		this->pos[0] += dx;
		this->pos[1] += dz;

		/* 5. ��ע */
		if (leader == this->id)
		{
			glColor3d(0.0, 1.0, 0.0);
			glBegin(GL_LINES);
			glVertex3d(this->pos[0], 0, this->pos[1]);
			glVertex3d(this->pos[0], 1, this->pos[1]);
			glEnd();
		}

		return;
	}

};

