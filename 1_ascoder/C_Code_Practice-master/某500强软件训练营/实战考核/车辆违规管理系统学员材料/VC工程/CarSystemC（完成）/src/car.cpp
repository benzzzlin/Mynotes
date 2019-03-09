#include <string.h>
#include "stdio.h"
#include<stdlib.h>
#include "api.h"
#include "car.h"

/*
������״̬��Ϣ������ȫ�ֱ����С�
��Ҫ��ϵͳ��ʼ����ʱ�򣬽������á�
*/

//������
typedef struct violation_record_node
{
	QueryResult data;
	struct violation_record_node * next ;
}violation_record_node;

/***************ϵͳ״̬******************/

// 0 ��ʾû�б���ʼ��
int g_flag_init = 0;
violation_record_node * g_head = NULL;
int g_index = 1;

/***************����״̬��Ϣ******************/
//ʣ�����
int g_remain_score = MAX_SCORE_VALUE ;
//��¼����Υ�����Ŀ
int g_record_num = 0;
// ��¼����Ĳ���ʱ��
int g_last_op_time = 1;
// 0 ��ʾû�б�ע��
int g_flag_delete = 0 ;

// ����Υ���¼��ֻ��¼ָ��ڵ㼴�ɡ�
violation_record_node * wait_for_pay_records[MAX_PECCANCY_RECORD_NUM]; 


//���úͻ�����Ϣ,���ڲ��Һ��޸�
int g_Fees[5]={100,200,300,400,500};
int g_Scores[5] ={1,2,3,4,5};

/*****************************************************************************
 �� �� ��  : delete_record_all
 ��������  : ɾ�����еļ�¼��Ϣ��
             
 �������  : violation_record_node * head       :    ��¼�����ͷָ��
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  : ��
 ��������  : onInit

 �޸���ʷ      :
  1.��    ��   : 2015��12��25��
    ��    ��   : ����ΰ
    �޸�����   : �����ɺ���

*****************************************************************************/

void delete_record_all(violation_record_node * head )
{
	violation_record_node * tmp = head;
	while(tmp != NULL)
	{
		violation_record_node * delete_node =tmp;
		tmp = tmp->next;
		free(delete_node);
	}
}


/*****************************************************************************
 �� �� ��  : add_record
 ��������  : �����ʷ��¼��������
             
 �������  : violation_record_node * head       :    ��¼�����ͷָ��
 			 carInfo							: 	 Υ���¼��Ϣ
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  : ��
 ��������  : onRecord

 �޸���ʷ      :
  1.��    ��   : 2015��12��25��
    ��    ��   : ����ΰ
    �޸�����   : �����ɺ���

*****************************************************************************/
int add_record(violation_record_node * head, CarSysInfo carInfo)
{
	//�ҵ����һ������� 
	violation_record_node * tmp1 = head;
	violation_record_node * tmp2 = head;
	while(tmp1 != NULL)
	{
		tmp2 = tmp1;
		tmp1 = tmp1->next;
	}
	tmp1 = (violation_record_node *)malloc(sizeof(violation_record_node));
	tmp1->data.Fee = g_Fees[carInfo.Reason];
	tmp1->data.Index = g_index;
	tmp1->data.PayFlag = STAT_NO_PAY;
	//ע���Ƿ񳬳���Χ
	tmp1->data.Score = g_remain_score - g_Scores[carInfo.Reason];
	tmp1->data.stCarSysInfo.Reason = carInfo.Reason;
	tmp1->data.stCarSysInfo.Time = carInfo.Time;
	
	tmp1->next = NULL;
	if(tmp2 != NULL)
		tmp2->next = tmp1;
	if(head == NULL)
		g_head = tmp1;

	//����״̬
	wait_for_pay_records[g_record_num] = tmp1 ;
	g_record_num = g_record_num + 1;
	g_remain_score = g_remain_score - g_Scores[carInfo.Reason];
	g_last_op_time = carInfo.Time;
	if(g_remain_score <= 0)
	{
		g_flag_delete = 1;
	}
	return 0;
}


/*****************************************************************************
 �� �� ��  : pay_one
 ��������  : �Ӵ��ɷѵ��б���ѡ��һ�����нɷѣ�
             
 �������  : violation_record_node * wait_for_pay_records[]       :    ������ɷѵ�����ڵ����顣
 			 int Money											  :    ʣ����ύ��Ǯ
 �������  : ��
 �� �� ֵ  : 
 			 -1	 : ��ʾʧ�ܣ�û���ҵ����Խ��ɵļ�¼
 			 > 0 : ��ʾ���ɳɹ�����ֵ��СΪ�Ѿ����ɵĽ��
 ���ú���  : ��
 ��������  : onPay

 �޸���ʷ      :
  1.��    ��   : 2015��12��25��
    ��    ��   : ����ΰ
    �޸�����   : �����ɺ���

*****************************************************************************/
int pay_one(violation_record_node * wait_for_pay_records[], int Money)
{
	//���ҵ�һ�����㸶�������Ľڵ�
	int pay_day = 0;
	int pay_index = -1;
	int i = 0;
	for( i = 0; i < g_record_num; i++)
	{
		if(wait_for_pay_records[i]->data.Fee <= Money)
		{
			
			pay_day = wait_for_pay_records[i]->data.stCarSysInfo.Time;
			pay_index = i;
			break;
		}
		
	}
	if(pay_index < 0)
	{
		return -1;
	}
	//����ͬһ������ɽ���ֵ���н���
	int max_pay = wait_for_pay_records[pay_index]->data.Fee;
	for( i = 0; i < g_record_num; i++)
	{
		if((wait_for_pay_records[i]->data.Fee <= Money) &&(pay_day == wait_for_pay_records[i]->data.stCarSysInfo.Time)
			&&(wait_for_pay_records[i]->data.Fee > max_pay ))
		{
			pay_index = i;
			max_pay = wait_for_pay_records[i]->data.Fee;
		}
		
	}

	wait_for_pay_records[pay_index]->data.PayFlag = STAT_HAVE_PAY ;
	for( i = pay_index+1; i < g_record_num; i++)
	{
		wait_for_pay_records[i-1] = wait_for_pay_records[i];
	}
	g_record_num--;
	return max_pay ;
}

/*****************************************************************************
 �� �� ��  : get_sum_wait_for_pay
 ��������  : ��ȡ�����ɽ�������
             
 �������  : violation_record_node * wait_for_pay_records[]       :    ������ɷѵ�����ڵ����顣
 �������  : result
 �� �� ֵ  : 
 			 0	 : ��ʾ�޼�¼
 			 > 0 : ��ʾ���ɿ��ܶ�
 ���ú���  : ��
 ��������  : onPay

 �޸���ʷ      :
  1.��    ��   : 2015��12��25��
    ��    ��   : ����ΰ
    �޸�����   : �����ɺ���

*****************************************************************************/

int get_sum_wait_for_pay(violation_record_node * wait_for_pay_records[])
{
	int result = 0;
	int i = 0;
	for( i = 0; i < g_record_num; i++)
	{
		result =result + wait_for_pay_records[i]->data.Fee;
	}
	return result ;
}

void main(int argc, char* argv[])
{
    /* ����Socket��������5555�˿�(sapi_server_start������lib����ʵ��)��
     * lib����ʵ�ִ�Socket���յ��ַ����������ַ�����
     * ����ֻ��Ҫʵ�ַַ���ĸ�����ɡ�
    */
    api_server_start(argc, argv);
	// �˴�����ִ�е���ע�ⲻҪ�ڴ˴���Ӵ���
}

/*****************************************************************************
 �� �� ��  : opInit
 ��������  : ������Ҫʵ�ֵĽӿ�
             ��ɡ�ϵͳ��ʼ��������
             ����ʵ����i
 �������  : ��
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2010��1��21��
    ��    ��   :
    �޸�����   : �����ɺ���

*****************************************************************************/
void opInit(void)
{
	//ϵͳ״̬��ʼ��
	g_flag_init = 1;
	
	//ɾ�����е�����
	delete_record_all(g_head);
	g_head = NULL;
	g_index = 1;

	g_remain_score = MAX_SCORE_VALUE ;
	g_record_num = 0;
	g_last_op_time = 1;
	g_flag_delete = 0 ;
	api_print_result(S001);
	return ;
	
}

/*****************************************************************************
 �� �� ��  : opRecord
 ��������  : ������Ҫʵ�ֵĽӿ�
             ��ɡ�¼��Υ���¼������
             ����ʵ����r 0-1
 �������  : int Peccancy  :    Υ������
             int Days      :    ʱ��
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2010��1��21��
    ��    ��   :
    �޸�����   : �����ɺ���

*****************************************************************************/
void opRecord(int PeccancyType, int Days)
{
	//������û�г�ʼ����
	if(0 == g_flag_init)
	{
		api_print_result(E001);
		return ;
	}
	//�Ƿ񱻵���
	if(g_flag_delete == 1)
	{
		api_print_result(E004);
		return ;
	}
	//��¼����
	if(g_record_num == MAX_PECCANCY_RECORD_NUM)
	{
		api_print_result(E009);
		return ;
	}
	//ʱ�䲻�ڷ�Χ֮��
	if(Days < 1 || Days > MAX_DAYS_VALUE)
	{
		api_print_result(E002);
		return ;
	}
	//ʱ�����һ�εĻ���
	if(Days < g_last_op_time)
	{
		api_print_result(E008);
		return ;
	}
	//���ʹ���
	if(PeccancyType<0 || PeccancyType >4)
	{
		api_print_result(E003);
		return ;
	}
	
	CarSysInfo carInfo;
	carInfo.Reason = PeccancyType;
	carInfo.Time = Days;

	if(0 == add_record(g_head, carInfo))
	{
		g_index = g_index + 1;
		api_print_result(S002);
		return ;
	}

}

/*****************************************************************************
 �� �� ��  : opQuery
 ��������  : ������Ҫʵ�ֵĽӿ�
             ��ɲ�ѯΥ���¼����
             ����ʵ����q
 �������  : ��
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2010��1��21��
    ��    ��   :
    �޸�����   : �����ɺ���

*****************************************************************************/
void opQuery(void)
{
	//������û�г�ʼ����
	if(0 == g_flag_init)
	{
		api_print_result(E001);
		return ;
	}
	//�Ƿ񱻵���
	if(1 == g_flag_delete)
	{
		api_print_result(E004);
		return ;
	}
	//û�м�¼
	if(g_head == NULL)
	{
		api_print_result(E010);
		return ;
	}
	else
	{
		violation_record_node * tmp1 = g_head;
		while(tmp1!=NULL)
		{
			api_print_query_info(&(tmp1->data));
			tmp1=tmp1->next;
		}
		return ;
	}
}

/*****************************************************************************
 �� �� ��  : opPay
 ��������  : ������Ҫʵ�ֵĽӿ�
             ��ɽ���Υ�淣�����
             ����ʵ����p 100
 �������  : int  Money       :    ���ɽ��
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2010��1��21��
    ��    ��   :
    �޸�����   : �����ɺ���

*****************************************************************************/
void opPay(int Money)
{
	//������û�г�ʼ����
	if(0 == g_flag_init)
	{
		api_print_result(E001);
		return ;
	}
	//�Ƿ񱻵���
	if(1 == g_flag_delete)
	{
		api_print_result(E004);
		return ;
	}
	//�Ƿ��д����ɵļ�¼
	if(0 == g_record_num)
	{
		api_print_result(E007);
		return ;
	}
	//�жϽ���Ƿ��ڷ�Χ֮��
	if(Money < 1 || Money > MAX_MONEY_VALUE)
	{
		api_print_result(E005);
		return ;
	}
	// ��¼��ǰ���ɳɹ��ı���
	int pay_num = -1;
	int pay_money = 0;
	int money_remain = Money;
	do
	{
		money_remain = money_remain - pay_money;
		pay_money =pay_one(wait_for_pay_records, money_remain);
		pay_num++;
	}while(pay_money != -1);
	
	if(pay_num < 1)
	{
		api_print_result(E006);
		return ;
	}
	else
	{
		PayResult result ;
		result.Fee = get_sum_wait_for_pay(wait_for_pay_records);
		result.PeccancyNo = g_record_num;
		result.Score = g_remain_score;
		result.ReturnMoney = money_remain ;
		api_print_pay_info(&result);
		return ;
	}
	
}
