
#ifndef _STRUCT_H_
#define _STRUCT_H_

typedef unsigned short WORD;
// �洢����
struct STravel{
	// ����id
	int m_iId;
	// ����url
	char m_cUrl[128];
	// ������Ŀ
	WORD m_wTitle[32];
	// ����ʱ��
	char m_cDate[11];
	// ��������
	WORD m_wPeople[8];
	// ��������
	short int m_siDays;
	// ������ʽ
	WORD m_wType[8];
	// �������ݵĴ洢�ļ���
	char m_cFileName[16];
	// ��������
	// �����������ݱȽϴ�������Ҫ��̬�����ڴ�
	char *m_cContext;
	// ���´�С
	int m_iSize;
};

enum ETravel
{
	EN_T_ID = 0,
	EN_T_URL,
	EN_T_TITLE,
	EN_T_DATE,
	EN_T_PEOPLE,
	EN_T_DAYS,
	EN_T_TYPE,
	EN_T_CONTEXT,

};

#endif