#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#include "api.h"
#include "SubwayCharge.h"

#include "malloc.h"
#include <stdarg.h>

/*����ȫ�ֱ������忼��������Ҫ����*/

#define FILE_NAME   "SubwayCharge.txt"
#define MAX_FREE_STAY_TIME  30

#define MAX_CARD_NUM 10

HistoryInfoNode *pHead = NULL;
LogItem_ST arrayLogItem[10];
int log_num = 0 ;
//��¼9�ſ��Ƿ�ע��
int tag_delete[MAX_CARD_NUM] = {1,1,1,1,1,1,1,1,1,1};




/*****************************************************************************
 �� �� ��  : main
 ��������  : ����ڲ���(�����������)
 �������  : argc  ��������ʱ�Ĳ�������
             argv  ��������ʱ�Ĳ���
 �������  : ��
 �� �� ֵ  : ��
*****************************************************************************/
void main(int argc, char* argv[])
{
    /*����Socket��������5555�˿�(apiServerStart������lib����ʵ��)*/
    apiServerStart(argc, argv);
	//�����˳�ʱ��ɾ��malloc �ֱ��ȫ��ָ��ͷ��
	RemoveList(pHead);
	//free(pHead);
    return;
}

/*****************************************************************************
 �� �� ��  : opResetProc
 ��������  : ������Ҫʵ�ֵĽӿ�
             ��ɳ����ʼ��,������ܸ�λ����
             ���������Զ����øú���,r/R����ʱ�Զ����øú���
 �������  : ��
 �������  : ��
 �� �� ֵ  : ��
*****************************************************************************/
void opResetProc(void)
{
	//����ռ�ɾ����ע���ͷ����Ϣ�ռ�û��ɾ����
	if (NULL != pHead)
	{
		RemoveList(pHead);
	}

	pHead = CreateList();

	//
	log_num = 0;
	int i = 1;
	for(i = 1; i < MAX_CARD_NUM; i++)
	{
		tag_delete[i] = 1 ;
	}
	for(i = 1; i < MAX_CARD_NUM ; i++)
	{
		apiDeleteLog(i);
		
	}
}

/*****************************************************************************
 �� �� ��  : opChargeProc
 ��������  : ������Ҫʵ�ֵĽӿ�
             �������۷ѵĹ���(���������˵��)
             c/C����ʱ�Զ����øú���
 �������  : pstTravelInfo  ���γ˳���¼��Ϣ
 �������  : ��
 �� �� ֵ  : ��
*****************************************************************************/
void opChargeProc(TravelInfo_ST* pstTravelInfo)
{
	//��Ҫ���жϿ��Ƿ����
	int distance = 0;
	int charge = 0;
	RetCode_EN retcode = RET_OK ;
	ErrCode_EN error_id= E01;
	int ret_if = 0;
	//�ж��Ƿ��ջ����ջ��
	if(((pstTravelInfo->nInHour*60)+(pstTravelInfo->nInMinute)) >
		((pstTravelInfo->nOutHour*60)+(pstTravelInfo->nOutMinute)) )
	{
		error_id = E02;
		goto cleanup;
		
	}
	//��ѯ��·�Ƿ����
	
	ret_if = apiGetDistanceBetweenTwoStation(pstTravelInfo->sInStation, pstTravelInfo->sOutStation, &distance);
	if(RET_ERROR == ret_if)
	{
		apiPrintOpStatusInfo(I10, pstTravelInfo->nCardNo, pstTravelInfo->nCardMoney);
		charge = 0;
		retcode = RET_ERROR;
		goto writeLog;
	}

	if ( 0 ==tag_delete[pstTravelInfo->nCardNo])
	{
		error_id = E22;
		goto cleanup;
	}
	//����ֻ����ý�����Ǯ
	//ͬ��վ������
	if(0 == strcmp(pstTravelInfo->sInStation,pstTravelInfo->sOutStation))
	{
		//С�ڶ�����ֵ 
		int time_diff = apiTimeDiff(pstTravelInfo->nOutHour,pstTravelInfo->nOutMinute,pstTravelInfo->nInHour,pstTravelInfo->nInMinute);
		//printf("%d\n",time_diff);
		if( time_diff <= MAX_FREE_STAY_TIME)
		{
			//���̿�
			if(CARDTYPE_A == pstTravelInfo->enCardType)
			{
				charge = pstTravelInfo->nCardMoney;
			}
			else
			{
				charge = 0 ;
			}
			
		}
		else
		{
			//���̿�
			if(CARDTYPE_A == pstTravelInfo->enCardType)
			{
				charge = (pstTravelInfo->nCardMoney >= 3) ?pstTravelInfo->nCardMoney : 3 ;
			}
			else
			{
				charge = 3;
			}
		}
	}
	//��ͬվ��
	else
	{
		int base_price = 0;
		base_price = get_base_price(pstTravelInfo->sInStation,pstTravelInfo->sOutStation);
		switch (pstTravelInfo->enCardType)
		{
			case CARDTYPE_A:
				charge = (base_price >pstTravelInfo->nCardMoney) ? base_price : pstTravelInfo->nCardMoney;
				break;
			case CARDTYPE_B:
				switch (get_time_tpye(pstTravelInfo->nInHour,pstTravelInfo->nInMinute))
				{
					case 0:
						charge = base_price*9/10;
						break;
					case 1:
						charge = base_price;
						break;
					case 2:
						charge = base_price*5/10;
						
				}
				break;
			case CARDTYPE_C:
				switch (get_time_tpye(pstTravelInfo->nInHour,pstTravelInfo->nInMinute))
				{
					case 0:
						charge = base_price;
						break;
					case 1:
						charge = base_price;
						break;
					case 2:
						charge = base_price*5/10;
						
				}
				
		}
	}
	//printf("%d",charge);
	//����
	if(charge <= (pstTravelInfo->nCardMoney))
	{
		
		if(CARDTYPE_A == pstTravelInfo->enCardType)
		{
			charge = pstTravelInfo->nCardMoney;
			apiPrintOpStatusInfo(I11, pstTravelInfo->nCardNo, 0);
		}
		else
		{
			if(((pstTravelInfo->nCardMoney)-charge) <20)
				apiPrintOpStatusInfo(I12, pstTravelInfo->nCardNo, (pstTravelInfo->nCardMoney)-charge);
			else
				apiPrintOpStatusInfo(I11, pstTravelInfo->nCardNo, (pstTravelInfo->nCardMoney)-charge);
			
		}
		//д��ʷ��¼
		HistoryItem * ptrHisItem = (HistoryItem *)malloc(sizeof(HistoryItem));
		ptrHisItem->nCardNo = pstTravelInfo->nCardNo;
		ptrHisItem->enCardType = pstTravelInfo->enCardType;
		ptrHisItem->nInHour = pstTravelInfo->nInHour;
		ptrHisItem->nInMin = pstTravelInfo->nInMinute;
		strcpy(ptrHisItem->sInStation , pstTravelInfo->sInStation);
		ptrHisItem->nOutHour = pstTravelInfo->nOutHour;
		ptrHisItem->nOutMin = pstTravelInfo->nOutMinute;
		strcpy(ptrHisItem->sOutStation ,pstTravelInfo->sOutStation);
		ptrHisItem->nMoney = charge;

		printf("good to here 1 \n");
		PushBackNode(pHead, ptrHisItem);
		printf("good to here 2 \n");
		
		retcode = RET_OK;
		goto writeLog;
	}
	//����
	else
	{
		apiPrintOpStatusInfo(I13,pstTravelInfo->nCardNo, pstTravelInfo->nCardMoney);
		charge = 0;
		retcode = RET_ERROR;
		goto writeLog;
		
	}
	

cleanup:
	apiPrintErrInfo(error_id);
	return ;
	
//д�ڴ���־
writeLog:
	apiWriteLog(charge, pstTravelInfo, retcode);
	return;
}

/*****************************************************************************
 �� �� ��  : opQueryLogProc
 ��������  : ������Ҫʵ�ֵĽӿ�
             ��ɲ�ѯ�˳���¼��־�Ĺ���(���������˵��)
             q/Q����ʱ�Զ����øú���
 �������  : pstQueryCond  ��־��ѯ����
 �������  : ��
 �� �� ֵ  : ��
*****************************************************************************/
void opQueryLogProc(QueryCond_ST* pstQueryCond)
{
	//log ��ָ�ڴ�
	ErrCode_EN error_id = E01;
	int logNum = apiGetLogNum();
	int startTime = pstQueryCond->nStartHour*60+pstQueryCond->nStartMinute;
	int endTime = pstQueryCond->nEndHour*60+pstQueryCond->nEndMinute;
	printf("%d\n",logNum);
	
	int delete_count = 0;
	int k = 0;
	for ( k = 1;k<MAX_CARD_NUM;k++)
	{
		if(0 == tag_delete[k])
			delete_count++;
	}
	if(9 == delete_count)
	{
		error_id = E22;
		goto cleanup;
	}
	if ( 0 ==tag_delete[pstQueryCond->nCardNo])
	{
		error_id = E22;
		goto cleanup;
	}
	
	if (0 == logNum)
	{
		error_id = E21 ;
		goto cleanup;
	}
	//ʱ�����

	
	
	if(startTime > endTime )
	{
		error_id = E02 ;
		goto cleanup;
	}
	else
	{
		LogItem_ST * headLogAddr = apiGetLogAddr();
		int i = 0;
		int j = 0;
		int count = 0;
		//��ӡ���еĿ���
		int tag_print_all = 0 ;
		if(0 == pstQueryCond->nCardNo )
		{
			tag_print_all = 1;
			int k = 0;
			for( k=1;k<MAX_CARD_NUM;k++)
			{
				for(i = 0;i<logNum;i++)
				{
					int outTime = ((headLogAddr+i)->nOutHour)*60 + (headLogAddr+i)->nOutMin;
					//����log
					if(outTime>= startTime && outTime<= endTime && (((headLogAddr+i)->nCardNo == k)))
					{
						arrayLogItem[j].nCardNo = (headLogAddr+i)->nCardNo ;
						arrayLogItem[j].nInHour = (headLogAddr+i)->nInHour;
						arrayLogItem[j].nInMin = (headLogAddr+i)->nInMin;
						strcpy(arrayLogItem[j].sInStation , (headLogAddr+i)->sInStation);
						arrayLogItem[j].nOutHour = (headLogAddr+i)->nOutHour;
						arrayLogItem[j].nOutMin = (headLogAddr+i)->nOutMin ;
						strcpy(arrayLogItem[j].sOutStation , (headLogAddr+i)->sOutStation);
						arrayLogItem[j].nMoney = (headLogAddr+i)->nMoney;
						arrayLogItem[j].enOpFlg = (headLogAddr+i)->enOpFlg;
						count++;
						j++;
						
					}
				}
			}
		}
		else
		{
			for(i = 0;i<logNum;i++)
			{
				int outTime = ((headLogAddr+i)->nOutHour)*60 + (headLogAddr+i)->nOutMin;
				//����log
				if(outTime>= startTime && outTime<= endTime && (((headLogAddr+i)->nCardNo == pstQueryCond->nCardNo)))
				{
					arrayLogItem[j].nCardNo = (headLogAddr+i)->nCardNo ;
					arrayLogItem[j].nInHour = (headLogAddr+i)->nInHour;
					arrayLogItem[j].nInMin = (headLogAddr+i)->nInMin;
					strcpy(arrayLogItem[j].sInStation , (headLogAddr+i)->sInStation);
					arrayLogItem[j].nOutHour = (headLogAddr+i)->nOutHour;
					arrayLogItem[j].nOutMin = (headLogAddr+i)->nOutMin ;
					strcpy(arrayLogItem[j].sOutStation , (headLogAddr+i)->sOutStation);
					arrayLogItem[j].nMoney = (headLogAddr+i)->nMoney;
					arrayLogItem[j].enOpFlg = (headLogAddr+i)->enOpFlg;
					count++;
					j++;
					
				}
			}
		}
		if(0 == count)
		{
			error_id = E21 ;
			goto cleanup;
		}
		else
		{
			apiPrintLog(arrayLogItem, count);
			return ;
		}
	}
	//��־��Ϣ������һ�����鱣�棬��Ϊ�������󣬲�10����ࡣ
	
cleanup:
	apiPrintErrInfo(error_id);
	return ;
	
}

/*****************************************************************************
 �� �� ��  : opQueryHistoryChargeListProc
 ��������  : ������Ҫʵ�ֵĽӿ�
             ��ɲ�ѯָ�����ŵ�Ʊ��������ʷ��¼����(���������˵��)
 �������  : iCardNo  ����ѯ��Ʊ������
 �������  : ��
 �� �� ֵ  : ��
*****************************************************************************/
void opQueryHistoryChargeListProc(int iCardNo)
{
	//��ʷ����ָ����
	
	if(0 == iCardNo)
	{
		//��ѯ����
		HistoryInfoNode* ptr_node = pHead->pNext;
		if(NULL == ptr_node)
		{
			apiPrintErrInfo(E21);
			return ;
		}
		else
		{
			while(NULL != ptr_node)
			{
				apiPrintHistoryChargeList(&(ptr_node->data));
				ptr_node=ptr_node->pNext;
			}
		}
	}
	else
	{
		HistoryInfoNode* ptr_node = pHead;
		//�����ж�ÿһ��
		int count = 0;
		if(NULL == ptr_node)
		{
			apiPrintErrInfo(E21);
			return ;
		}
		if ( 0 ==tag_delete[iCardNo])
		{
			apiPrintErrInfo(E22);
			return;
		}
		
		while(NULL != ptr_node)
		{
			ptr_node = FindNodeByCardNo(ptr_node,iCardNo);
			if(NULL != ptr_node)
			{
				apiPrintHistoryChargeList(&(ptr_node->data));
				count++; 
			}
			//ptr_node->pNext;
		}
		
		printf("here count : %d\n",count);
		if(count == 0)
		{	
			//�Ѿ���ɾ��
			if(0 == tag_delete[iCardNo])
				apiPrintErrInfo(E22);
			else
				apiPrintErrInfo(E21);
		}
		
		
	}
}

/*****************************************************************************
 �� �� ��  : opDestroyCardProc
 ��������  : ������Ҫʵ�ֵĽӿ�
             ���ע��ָ�����ŵ�Ʊ��������ʷ��¼����(���������˵��)
 �������  : iCardNo  ��ע����Ʊ������
 �������  : ��
 �� �� ֵ  : ��
*****************************************************************************/
void opDestroyCardProc(int iCardNo)
{
	//ɾ������
	if(0 ==iCardNo)
	{
		int i = 1;
		for ( i = 1; i < MAX_CARD_NUM; i++ )
		{
			int delete_count = 0;
			int k = 0;
			for ( k = 1;k<MAX_CARD_NUM;k++)
			{
				if(0 == tag_delete[k])
					delete_count++;
			}
			if(9 == delete_count)
			{
				apiPrintErrInfo(E22);
				return ;
			}
			//���֮ǰû�б�ɾ����
			if(1 == tag_delete[i])
			{
				tag_delete[i] = 0 ;
				RemoveNodeByCardNo(pHead, i);
				apiDeleteLog(i);
				
			}
		}
		apiPrintOpStatusInfo(I22, 0, 0);
	}
	
	else 
	{
		//�Ѿ���ע������
		if(0 == tag_delete[iCardNo])
		{
			apiPrintErrInfo(E22);
		}
		else
		{
			//��ʷ������Ϣ��Ҫɾ��
			tag_delete[iCardNo] = 0;
			RemoveNodeByCardNo(pHead, iCardNo);
			apiDeleteLog(iCardNo);
			apiPrintOpStatusInfo(I22, iCardNo, 0);
		}
	}
}



/*�����������忼�����ݹ�����Ҫ����*/


/*************************************************
Function      : CreateList
Description   : ��������ͷ�ڵ�,ֻ����Ҫ��ʱ����ӣ���һ����ʱ��� 
				���������ͷ�ڵ㲻���Ƿ�������ݣ����Ǵ��ڵġ�
Return        : �����ͷָ��
*************************************************/
HistoryInfoNode* CreateList(void)
{
    
    pHead = (HistoryInfoNode *)malloc(sizeof(HistoryInfoNode));
    if (NULL == pHead)
    {
        apiPrintErrInfo(E99);
        return NULL;
    }

    pHead->data.nCardNo = 0;
    pHead->pNext = NULL;

    return pHead;
}

/*************************************************
Function      : FindNodeByCardNo
Description   : ���ݿ��ţ�����ĳ���ڵ�
Input         : pHead �����ͷ�ڵ�ָ��
                Ҫ���ҵĿ���
Return        : ��ȷ:����ָ���ڵ��ָ��
                ʧ��:���ؿ�ָ��
*************************************************/
HistoryInfoNode* FindNodeByCardNo(HistoryInfoNode *pHead, int iCradNo)
{
    HistoryInfoNode *pNode = NULL;

    if ((NULL == pHead) || (iCradNo < 0))
    {
        //apiPrintErrInfo(E99);
        return NULL;
    }

    pNode = pHead->pNext;
	//pNode = pHead;
    while ((NULL != pNode))
    {
    	printf("card no is : %d\n",pNode->data.nCardNo);
        if (pNode->data.nCardNo == iCradNo)
        {
            break;
        }
        pNode = pNode->pNext;
    }

    return pNode;
}


/*************************************************
Function      : PushBackNode
Description   : ��������β������ĳ���ڵ�
Input         : pHead        �����ͷ�ڵ�ָ��
                pCardInfo    ���Ѽ�¼��Ϣ
Output        : ��
Return        : ��ȷ:����ͷ�ڵ�ָ��
                ʧ��:���ؿ�ָ��
*************************************************/
HistoryInfoNode* PushBackNode(HistoryInfoNode *pHead, HistoryItem *pCardInfo)
{
    HistoryInfoNode* pNode      = NULL;
    HistoryInfoNode* pNewNode   = NULL;

    if ((NULL == pHead) || (NULL == pCardInfo))
    {
        //apiPrintErrInfo(E99);
        return NULL;
    }

    pNode = pHead;
    while (NULL != pNode->pNext)
    {
        pNode = pNode->pNext;
    }
    pNewNode = (HistoryInfoNode *)malloc(sizeof(HistoryInfoNode));
    if (NULL == pNewNode)
    {
        //apiPrintErrInfo(E99);
        return NULL;
    }

    pNode->pNext     = pNewNode;
    pNewNode->pNext = NULL;
	
    memcpy(&(pNewNode->data), pCardInfo, sizeof(LogItem_ST));

    return pHead;
}

/*************************************************
Function      : RemoveNodeByCardNo
Description   : ��������ɾ��ָ�����ŵļ�¼
Input         : pHead       �����ͷ�ڵ�ָ��
                iCradNo     ��ɾ���Ľڵ�Ŀ���
Return        : ��ȷ:��������ͷ�ڵ��ָ��
                ʧ��:���ؿ�ָ��
*************************************************/
HistoryInfoNode* RemoveNodeByCardNo(HistoryInfoNode *pHead, int iCradNo)
{
    HistoryInfoNode* pNode      = NULL;
    HistoryInfoNode* pDelNode   = NULL;

    if ((NULL == pHead) || (iCradNo < 0))
    {
        //apiPrintErrInfo(E99);
        return NULL;
    }

    pNode = pHead;
    while (NULL != pNode->pNext)
    {
        if (pNode->pNext->data.nCardNo == iCradNo)
        {
            break;
        }

        pNode = pNode->pNext;
    }

    pDelNode = pNode->pNext;
    if (NULL == pDelNode)
    {
        //apiPrintErrInfo(E99);
        return NULL;
    }

    pNode->pNext = pDelNode->pNext;
    free(pDelNode);

    pDelNode = NULL;

    return pHead;
}


/*************************************************
Function      : RemoveList
Description   : ɾ����������
Input         : pHead �����ͷ�ڵ�ָ��
Return        : ��ȷ:RET_OK
                ʧ��:RET_ERROR
*************************************************/
int RemoveList(HistoryInfoNode *pHead)
{
    HistoryInfoNode *pNode  = NULL;
    HistoryInfoNode *pb     = NULL;

    if (NULL == pHead)
    {
        apiPrintErrInfo(E99);
        return RET_ERROR;
    }

    pNode = pHead;

    pb = pNode->pNext;
    if (NULL == pb)
    {
        free(pNode);
    }
    else
    {
        while (NULL != pb)
        {
            free(pNode);
            pNode = pb;
            pb = pb->pNext;
        }

        free(pNode);
    }

    pNode = NULL;
    return RET_OK;
}

//д���ļ�,ǰ������Ҫд����ı����ݻ����Ѿ�׼�����ˡ�
void WriteToFile(char * pstLogBuff)
{
    unsigned int    uiStrLen    = 0;
    FILE            *fp         = NULL;

    //����֮��򿪶�ȡ
    fp = fopen(FILE_NAME, "w");
    if (NULL == fp)
    {
        apiPrintErrInfo(E99);
        return;
    }

    uiStrLen = fwrite(pstLogBuff, 1, strlen(pstLogBuff), fp);
    if (uiStrLen != strlen(pstLogBuff))
    {
        apiPrintErrInfo(E99);
        fclose(fp);
        return;
    }

    fclose(fp);

    return;
}




//get base price
int get_base_price(char *intStation ,char * outStation )
{
	int distance = 0;
	int base_price = 0 ; 
	apiGetDistanceBetweenTwoStation(intStation, outStation,&distance);
	if (distance <= 3)
		base_price = 2;
	else if(distance <= 5)
		base_price = 3;
	else if(distance <= 10)
		base_price = 4;
	else if (distance > 10)
		base_price = 5;
	else
		base_price = -1;

	return base_price;
		
}

/********************************
//�жϽ�վ��ʱ���
//0������ʱ��
//1��æʱ
//2����ʱ
//-1������
*/
int get_time_tpye(int inHour, int InMinute)
{
	int type = 0;
	int totalMinute = inHour*60+InMinute;
	
	if((totalMinute>=(7*60))&&(totalMinute<(9*60)))
		type = 1 ;
	if((totalMinute>=(16*60+30))&&(totalMinute<(18*60+30)))
		type = 1;
	if((totalMinute>=(10*60))&&(totalMinute<(11*60)))
		type = 2;
	if((totalMinute>=(15*60))&&(totalMinute<(16*60)))
		type = 2;

	return type;
	
}