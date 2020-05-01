#include "header.h"

// ȭ�� ��/��� �Լ� ����

void disp_main_menu() //���� �޴� ���
{
	system("cls");
	std::cout << "==============================================================" << std::endl;
	std::cout << "	    Sequential File Structure Simulation" << std::endl;
	std::cout << "==============================================================" << std::endl;
	std::cout << " 1 : �ʱ� ������ ���� ����" << std::endl;
	std::cout << " 2 : ������ ���� ���" << std::endl;
	std::cout << "--------------------------------------------------------------" << std::endl;
	std::cout << " 3 : Ʈ����� ���� �ۼ�" << std::endl;
	std::cout << " 4 : Ʈ����� ���� ���" << std::endl;
	std::cout << "--------------------------------------------------------------" << std::endl;
	//std::cout << "�й� �� ���� & ������ ���� ����(Manually)" << std::endl;
	std::cout << " 5 : ������ ���� ���� (Trans -> Master)" << std::endl;
	std::cout << "--------------------------------------------------------------" << std::endl;
	std::cout << " 6 : View Error Log" << std::endl;
	std::cout << " 7 : Exit" << std::endl;
	std::cout << "==============================================================" << std::endl;
	std::cout << ">>";
}

void disp_student_description_bar(int file_mode) //�л� ���� ���� ���
{
	system("cls");
	switch (file_mode)
	{
	case 0: //������ ����
		std::cout << "==============================================================" << std::endl;
		std::cout << "�й� ||  �̸�  ||  �а�  ||  �г�  ||  ����" << std::endl;
		std::cout << "==============================================================" << std::endl;
		break;
	case 1: //Ʈ����� ����
		std::cout << "==============================================================" << std::endl;
		std::cout << "�й� ||  �����ڵ� ||  �̸�  ||  �а�  ||  �г�  ||  ����" << std::endl;
		std::cout << "==============================================================" << std::endl;
		break;
	default:
		break;
	}
}

void disp_student_trans_input_bar(char** dpt_list_array, int dpt_list_count) //Ʈ����� ���� �ۼ� �� �Է��ؾ� �� ������ ���
{
	system("cls");
	std::cout << "==============================================================" << std::endl;
	std::cout << "Insert(I) : ���� ���� => I, �й�, �̸�, �а�, �г�, ���� ������ �Է�" << std::endl;
	std::cout << "Delete(D) : ���� ���� => D, �й� ������ �Է�" << std::endl;
	std::cout << "Correct(C) : ���� ���� => C, �й�, �̸�, �а�, �г�, ���� ������ �Է�" << std::endl;
	std::cout << "--------------------------------------------------------------" << std::endl;
	std::cout << "�� �й� ���� �� : " << ID_LENGTH << std::endl;
	std::cout << "�� �̸� ���� �� : " << NAME_LENGTH << std::endl;
	std::cout << "�� �а� ���� �� : " << DPT_LENGTH << std::endl;
	std::cout << "�� �г� ���� : 1 ~ 4" << std::endl;
	std::cout << "�� ���� ���� : 1 ~ 100" << std::endl;
	std::cout << "--------------------------------------------------------------" << std::endl;
	std::cout << "�� �а� ��� : ";

	for (int i = 0; i < dpt_list_count; i++) //�а� ��� ���
	{
		if (i == dpt_list_count - 1)
			std::cout << dpt_list_array[i] << std::endl;
		else
			std::cout << dpt_list_array[i] << ",";
		//printf("%02d: %s\n", i, dpt_list_array[i]);
	}
	std::cout << "--------------------------------------------------------------" << std::endl;
	std::cout << " �Է� ���� : ���� �ڵ� ��ġ�� -1 �Է�" << std::endl;
	std::cout << "==============================================================" << std::endl;
	std::cout << ">>";
}

void disp_student_input_bar(char** dpt_list_array, int dpt_list_count) //������ ���� �ۼ� �� �Է��ؾ� �� ������ ���
{
	system("cls");
	std::cout << "==============================================================" << std::endl;
	std::cout << "�� �й�, �̸�, �а�, �г�, ���� ������ �Է�" << std::endl;
	std::cout << "�� �й� ���� �� : " << ID_LENGTH << std::endl;
	std::cout << "�� �̸� ���� �� : " << NAME_LENGTH << std::endl;
	std::cout << "�� �а� ���� �� : " << DPT_LENGTH << std::endl;
	std::cout << "�� �г� ���� : 1 ~ 4" << std::endl;
	std::cout << "�� ���� ���� : 1 ~ 100" << std::endl;
	std::cout << "--------------------------------------------------------------" << std::endl;
	std::cout << "�� �а� ��� : ";
	
	for (int i = 0; i < dpt_list_count; i++) //�а� ��� ���
	{
		if (i == dpt_list_count - 1)
			std::cout << dpt_list_array[i] << std::endl;
		else
			std::cout << dpt_list_array[i] << ",";
		//printf("%02d: %s\n", i, dpt_list_array[i]);
	}
	std::cout << "--------------------------------------------------------------" << std::endl;
	std::cout << " �Է� ���� : �й� ��ġ�� -1 �Է�" << std::endl;
	std::cout << "==============================================================" << std::endl;
	std::cout << ">>";
}

void disp_log_description_bar() //�α� Ÿ�Կ� ���� ����
{
	system("cls");
	std::cout << "==============================================================" << std::endl;
	std::cout << "ERROR_TYPE ||		 �ð�		||	 ����" << std::endl;
	std::cout << "==============================================================" << std::endl;
}

void disp_trans_log_description_bar() //�α� Ÿ�Կ� ���� ����
{
	system("cls");
	std::cout << "=============================================================================" << std::endl;
	std::cout << " ERROR_TYPE  ||  �ð�  ||  ���� �ڵ�  || Master Key || Trans Key ||  ����" << std::endl;
	std::cout << "=============================================================================" << std::endl;
}

void disp_error_msg(const char* message) //���� �޼��� ���
{
	std::cout << "���� : " << message << std::endl;
}

int input_log_file_mode() //�α� ����� ���� �α� Ÿ�� �Է�
{
	//file_mode : 0 - ������ ����, 1 - Ʈ����� ����, 2 - Edit ���� �α�, 3 - Transaction ���� �α�
	while (1)
	{
		system("cls");
		std::cout << "==============================================================" << std::endl;
		std::cout << " 1 : Edit ���� �α� ���" << std::endl;
		std::cout << " 0 : Transaction ���� �α� ���" << std::endl;
		std::cout << "--------------------------------------------------------------" << std::endl;
		std::cout << " -1 : return" << std::endl;
		std::cout << "==============================================================" << std::endl;
		std::cout << ">>";

		int user_input = 0;
		std::cin >> user_input;
		
		if (user_input != -1 && user_input != 0 && user_input != 1)
		{
			std::cin.clear(); //������Ʈ�� �ʱ�ȭ
			std::cin.ignore(INT_MAX, '\n'); //�Է¹��ۺ���
		}
		else
		{
			if (user_input == 1) return 2; //print_file(2) ȣ��
			if (user_input == 0) return 3; //print_file(1) ȣ��
		}
	}
}

bool input_init_master_confirm() //�ʱ� ������ ���� ���� Ȯ��
{
	while (1)
	{
		system("cls");
		std::cout << "==============================================================" << std::endl;
		std::cout << "���� ������ ���� ���� �� ���ο� ������ ������ �����մϴ�." << std::endl;
		std::cout << " 1 : continue" << std::endl;
		std::cout << " 0 : return" << std::endl;
		std::cout << "==============================================================" << std::endl;
		std::cout << ">>";

		int user_input = 0;
		std::cin >> user_input;
		if (user_input == 0)
		{
			std::cin.clear(); //������Ʈ�� �ʱ�ȭ
			std::cin.ignore(INT_MAX, '\n'); //�Է¹��ۺ���

			return false;
			break;
		}
		else if (user_input < 0 || user_input > 1)
		{
			std::cin.clear(); //������Ʈ�� �ʱ�ȭ
			std::cin.ignore(INT_MAX, '\n'); //�Է¹��ۺ���
		}
		else
		{
			std::cin.clear(); //������Ʈ�� �ʱ�ȭ
			std::cin.ignore(INT_MAX, '\n'); //�Է¹��ۺ���

			return true;
			break;
		}
	}
}

bool input_replace_master_confirm() //������ ���� ���� ��ü Ȯ��
{
	while (1)
	{
		system("cls");
		std::cout << "==============================================================" << std::endl;
		std::cout << "���� ������ ������ ���� �� ���ο� ������ ���Ϸ� ��ü�Ͻðڽ��ϱ�?" << std::endl;
		std::cout << "���� ������ ���� : master.txt" << std::endl;
		std::cout << "�� ������ ���� : new_master.txt" << std::endl;
		std::cout << "--------------------------------------------------------------" << std::endl;
		std::cout << " 2 : �����Ͽ� ���ο� ������ ���� ������ ����(new_master.txt)" << std::endl;
		std::cout << " 1 : ���� �� ���� ������ ���� ��ü(new_master.txt -> master.txt)" << std::endl;
		std::cout << " 0 : return" << std::endl;
		std::cout << "==============================================================" << std::endl;
		std::cout << ">>";

		int user_input = 0;
		std::cin >> user_input;

		if (user_input == 2) //���Ÿ�
		{
			std::cin.clear(); //������Ʈ�� �ʱ�ȭ
			std::cin.ignore(INT_MAX, '\n'); //�Է¹��ۺ���

			return false;
			break;
		}
		else if (user_input == 1) //���� �� ��ü
		{
			std::cin.clear(); //������Ʈ�� �ʱ�ȭ
			std::cin.ignore(INT_MAX, '\n'); //�Է¹��ۺ���
			
			return true;
			break;
		}
		else
		{
			std::cin.clear(); //������Ʈ�� �ʱ�ȭ
			std::cin.ignore(INT_MAX, '\n'); //�Է¹��ۺ���
		}
	}
}

void input_main_menu(Student_list** src_list, char** dpt_list_array, int dpt_list_count) //���� �޴��� ���� ����� �Է�
{
	while (1)
	{
		int user_input = 0; //����� �Է� ��

		/***debug***/
		Student_list* tmp_student_head = NULL; //���� ����Ʈ�� ù ��° �л� ���� �ӽ�����
		char buffer_id[9] = { 0, };
		/***debug***/

		disp_main_menu();
		std::cin >> user_input;

		std::cin.clear(); //������Ʈ�� �ʱ�ȭ
		std::cin.ignore(INT_MAX, '\n'); //�Է¹��ۺ���

		switch (user_input)
		{
		case EXCEPTION_HANDLING_VALUE: //debug
			system("cls");
			(*src_list) = new Student_list("D", "12345", "KIM", "DP", 1, 100); //add 

			std::cout << "sizeof dpt_list_array : " << sizeof(dpt_list_array) <<"\n"<< std::endl;
			std::cout << "sizeof *(&dpt_list_array) : " << sizeof * (&dpt_list_array) << "\n" << std::endl;
			std::cout << "sizeof &dpt_list_array : " << sizeof&(dpt_list_array) << "\n" << std::endl;
			std::cout << "sizeof (*dpt_list_array) : " << sizeof(*dpt_list_array) << "\n" << std::endl;

			std::cout << "sizeof src_list : " << sizeof(src_list) << "\n" << std::endl;
			std::cout << "sizeof *(&src_list) : " << sizeof * (&src_list) << "\n" << std::endl;
			std::cout << "sizeof &src_list : " << sizeof&(src_list) << "\n" << std::endl;
			std::cout << "sizeof (*src_list) : " << sizeof(*src_list) << "\n" << std::endl;
			
			if (src_list != NULL) //src_list == *(&src_list)
				std::cout << "src_list exist :"<<src_list<< "\n" << std::endl;
			if (&src_list != NULL) //src_list�� ���۷���(������), ������ ����
				std::cout << "&src_list exist :"<<&src_list << "\n" << std::endl;
			if(*src_list != NULL) //*src_list�� ����Ű�� ��ġ�� ����, Student_list������ src_list�� �ּҰ�
				std::cout << "(*src_list) exist :"<<(*src_list) << "\n" <<std::endl;
			if (*(&src_list) != NULL) //src_list == *(&src_list)
				std::cout << "*(&src_list) exist :" << *(&src_list) << "\n" << std::endl;
			
		
			std::cout << "dpt_list_count:"<<dpt_list_count << "\n";
			for (int i = 0; i < dpt_list_count;i++)
			{
				std::cout << dpt_list_array[i]<<"\n";
				printf("%02d: %s\n", i, dpt_list_array[i]);
			}


			if ((*src_list) != NULL)
			{
				tmp_student_head = (*src_list);

				while (1)
				{
					(*src_list)->get_Student_ID(buffer_id);
					std::cout << "==============================================================" << std::endl;
					switch ((*src_list)->op_code)
					{
					case OPERATION_CODE::no_operation:
						printf("no op:%c\n", OPERATION_CODE::no_operation);
						break;
					case OPERATION_CODE::Insert:
						printf("insert:%c\n", OPERATION_CODE::Insert);
						break;
					case OPERATION_CODE::Delete:
						printf("delete:%c\n", OPERATION_CODE::Delete);
						break;
					case OPERATION_CODE::Correct:
						printf("correct:%c\n", OPERATION_CODE::Correct);
						break;

					}
					std::cout << "StudentID: " << buffer_id << std::endl;
					std::cout << "NAME: " << (*src_list)->name << std::endl;
					std::cout << "DPT: " << (*src_list)->department << std::endl;
					std::cout << "Grade: " << (*src_list)->grade << std::endl;
					std::cout << "Score: " << (*src_list)->score << std::endl;
					std::cout << "==============================================================" << std::endl;
					if ((*src_list)->next != NULL)
						(*src_list) = (*src_list)->next;
					else break;
				}
			}
			(*src_list) = tmp_student_head;

			system("pause");
			break;

		case 1: //�ʱ� ������ ���� ����
			create_file(src_list, dpt_list_array, dpt_list_count, 0);
			system("pause");
			break;

		case 2: //������ ���� ���
			print_file(0);
			system("pause");
			break;

		case 3: //Ʈ����� ���� �ۼ�
			create_file(src_list, dpt_list_array, dpt_list_count, 1);
			system("pause");
			break;

		case 4: //Ʈ����� ���� ���
			print_file(1);
			system("pause");
			break;

		case 5: //������ ���� ����
			update_master(input_replace_master_confirm());
			system("pause");
			break;

		case 6: //�α� ���� ���
			print_file(input_log_file_mode());
			system("pause");
			break;

		case 7: //exit
			deallocate_memory(src_list, dpt_list_array, dpt_list_count, 1); //�л� ����Ʈ �޸� ����
			deallocate_memory(src_list, dpt_list_array, dpt_list_count, 0); //�а� �迭 �޸� ����
			exit(1);
				
		default:
			continue;
		}

	}
}