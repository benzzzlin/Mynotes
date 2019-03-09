#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#include "api.h"
#include "SubwayCharge.h"
#include "malloc.h"
#include <stdarg.h>


/*************************************************
Function      : CreateList
Description   : ��������ͷ�ڵ�
Return        : �����ͷָ��
*************************************************/
HistoryInfoNode* CreateList(void)
{
    HistoryInfoNode *pHead = NULL;

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
        apiPrintErrInfo(E99);
        return NULL;
    }

    pNode = pHead->pNext;
    while ((NULL != pNode))
    {
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
        apiPrintErrInfo(E99);
        return NULL;
    }

    pNode = pHead;
    while (pNode->pNext != NULL)
    {
        pNode = pNode->pNext;
    }

    pNewNode = (HistoryInfoNode *)malloc(sizeof(HistoryInfoNode));
    if (NULL == pNewNode)
    {
        apiPrintErrInfo(E99);
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
        apiPrintErrInfo(E99);
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
        apiPrintErrInfo(E99);
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

void WriteToFile(char * pstLogBuff)
{
    unsigned int    uiStrLen    = 0;
    FILE            *fp         = NULL;

    //����֮��򿪶�ȡ
    fp = fopen(FILE_NAME, "a+");
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

