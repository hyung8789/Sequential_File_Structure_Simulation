#include "header.h"

// �ý����� ���� �ð� ��ȯ�ϴ� �Լ� ����

const std::string get_current_time() { //���� �ð� ���ϴ� �Լ�
	time_t now = time(0); //����ð��� time_tŸ������ ����
	struct tm tstruct;
	char buf[80];
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct); // YYYY-MM-DD.HH:mm:ss ������ string

	return buf;
}