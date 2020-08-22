#include "stdafx.h"
#include <windows.h> 
//#include "orangeos.h"
//#include "stringdef.h"
#include "OrangeOSAPI.h"

typedef struct tag_SKY_APIStruct
{
	char* strAPIName;		//�Լ� �̸�
	int _APIIndex;
	void* ptrAPIFunction;		//�Լ� ������
	tag_SKY_APIStruct * Next;
}SKY_APIStruct;
SKY_APIStruct* g_pRegisteredSkyAPIEntries = nullptr;

void RegisterOrangeOSAPI(int APIIndex, char* strAPIName, void * ptrAPIFunction)
{
	SKY_APIStruct* newAPIStruct;
	char *strName = nullptr;

	//printf("Register API : %s %x\n", strAPIName, (unsigned int)ptrAPIFunction);

	newAPIStruct = new SKY_APIStruct;
	strName = new char[strlen(strAPIName) + 1];

	strcpy(strName, strAPIName);

	//�Լ��̸��� �Լ� �����͸� ����
	newAPIStruct->_APIIndex = APIIndex;
	newAPIStruct->strAPIName = strName;
	newAPIStruct->ptrAPIFunction = ptrAPIFunction;
	newAPIStruct->Next = nullptr;

	if (g_pRegisteredSkyAPIEntries) //��Ʈ ��Ʈ���� �����ϸ� �˻��� �ؼ� �������� �� ��Ʈ���� �����ϰ� �׷��� ������ �� ��Ʈ���� ��Ʈ ��Ʈ����
	{
		SKY_APIStruct* curAPIStruct = g_pRegisteredSkyAPIEntries;
		while (curAPIStruct->Next)
		{
			if (strcmp(curAPIStruct->strAPIName, strAPIName) == 0) //already the function exists update the pointer value
				break;
			curAPIStruct = curAPIStruct->Next;
		}
		curAPIStruct->Next = newAPIStruct;
	}
	else
		g_pRegisteredSkyAPIEntries = newAPIStruct;
}

void* GetOrangeOSAPI(char * strAPIName)
{
	SKY_APIStruct* curAPIStruct = g_pRegisteredSkyAPIEntries;
	while (curAPIStruct)
	{
		printf("[%s] [%s]\n", strAPIName, curAPIStruct->strAPIName);

		if (strcmp(curAPIStruct->strAPIName, strAPIName) == 0)
			return curAPIStruct->ptrAPIFunction;

		curAPIStruct = curAPIStruct->Next;
	}

	return 0;
}

void* GetOrangeOSAPIByIndex(int index)
{
	SKY_APIStruct* curAPIStruct = g_pRegisteredSkyAPIEntries;
	while (curAPIStruct)
	{
		
		if (curAPIStruct->_APIIndex == index)
			return curAPIStruct->ptrAPIFunction;

		curAPIStruct = curAPIStruct->Next;
	}

	return 0;
}