#include "header.h"

void main()
{
	int dpt_list_count = 0; //��� �� �а� ��
	Student_list* student_list = NULL; //�л� ���� ���� ����Ʈ
	char** dpt_list_array = allocate_dpt_list_array(dpt_list_count); //�а� ����Ʈ �迭�Ҵ�

	if (dpt_list_array == NULL) //�а� ����Ʈ ������ �������� �ʰų� ������� ���
	{
		system("pause");
		exit(1);
	}

	while (1)
	{
		input_main_menu(&student_list, dpt_list_array, dpt_list_count);
	}
}