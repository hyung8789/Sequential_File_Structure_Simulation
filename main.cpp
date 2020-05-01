#include "header.h"

void main()
{
	int dpt_list_count = 0; //등록 된 학과 수
	Student_list* student_list = NULL; //학생 이중 연결 리스트
	char** dpt_list_array = allocate_dpt_list_array(dpt_list_count); //학과 리스트 배열할당

	if (dpt_list_array == NULL) //학과 리스트 파일이 존재하지 않거나 비어있을 경우
	{
		system("pause");
		exit(1);
	}

	while (1)
	{
		input_main_menu(&student_list, dpt_list_array, dpt_list_count);
	}
}