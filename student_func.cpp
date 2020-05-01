#include "header.h"

/* 
	- �л� ��ü ����, ���� ����� ���� �л� ���� ���� ���Ḯ��Ʈ ���� �� ���� ���� ����
	- �л� ����Ʈ�� Ÿ��(�迭, ���� ���� ����Ʈ)�� ���� �޸� ���� �Լ� ����
	- �Է¹��� �л� �����ͷκ��� Edit ���� ���� �Ϸ� �� �л� ��ü ����, �й� ������ �����Ͽ� ����Ʈ ���� �� ������ ����ϴ� file_func�� ����
*/

Student_string_data::Student_string_data() //�Է°� ���� ���� �˻縦 ���� �л� ������ ���ڿ� Ŭ����
{
	this->clear_all();
}

void Student_string_data::clear_all() //��� ���ڿ��� ���� �ʱ�ȭ
{
	this->str_op_code.clear();
	this->str_student_id.clear();
	this->str_name.clear();
	this->str_dpt.clear();
	this->str_grade.clear();
	this->str_score.clear();
}

Student::Student()
{
	strcpy(this->Student_ID, NULL_VALUE); //�й�
	strcpy(this->name, NULL_VALUE); //�̸�
	strcpy(this->department, NULL_VALUE); //�а�
	this->grade = EXCEPTION_HANDLING_VALUE; //�г�
	this->score = EXCEPTION_HANDLING_VALUE; //����
}

Student::Student(const char* ID, const char* name, const char* dpt, int grade, int score)
{
	strcpy(this->Student_ID, ID); //�й�
	
	//Ʈ����� ���� �ۼ� �� ���� ����(D) �ÿ��� �̸�, �а�, �г�, ������ �Է� ���� ����
	//����ó�� ��(EXCEPTION_HANDLING_VALUE, NULL_VALUE)�� �־� �� ������ ����
	if (name != NULL_VALUE && dpt != NULL_VALUE && grade != EXCEPTION_HANDLING_VALUE && score != EXCEPTION_HANDLING_VALUE)
	{
		strcpy(this->name, name); //�̸�
		strcpy(this->department, dpt); //�а�
		this->grade = grade; //�г�
		this->score = score; //����
	}
	else //�� �� �Ҵ�
	{
		strcpy(this->name, NULL_VALUE); //�̸�
		strcpy(this->department, NULL_VALUE); //�а�
		this->grade = EXCEPTION_HANDLING_VALUE; //�г�
		this->score = EXCEPTION_HANDLING_VALUE; //����
	}
}

void Student::get_Student_ID(char* dst_buffer) //�й� ��ȯ
{
	strcpy(dst_buffer,this->Student_ID);
}

Student_list::Student_list()
{
	this->op_code = OPERATION_CODE::no_operation; //�����ڵ� Enum �� �Ҵ�

	this->next = NULL;
	this->pre = NULL;
}

Student_list::Student_list(const char* op_code, const char* ID, const char* name, const char* dpt, int grade, int score)
{
	strcpy(this->Student_ID, ID); //�й�
	strcpy(this->name, name); //�̸�
	strcpy(this->department, dpt); //�а�
	this->grade = grade; //�г�
	this->score = score; //����

	if (op_code != NULL_VALUE) //�����ڵ� Enum �� �Ҵ�
	{
		if (strcmp(op_code, "I") == 0) //Insert
			this->op_code = OPERATION_CODE::Insert;
		else if (strcmp(op_code, "D") == 0) //Delete
			this->op_code = OPERATION_CODE::Delete;
		else //Correct
			 this->op_code = OPERATION_CODE::Correct;
	}
	else //�� �� �Ҵ�
		this->op_code = OPERATION_CODE::no_operation;

	this->next = NULL;
	this->pre = NULL;
}

bool chk_dpt_list(char** dpt_list_array, int dpt_list_count, char* user_input_dpt) //����ڷκ����� �а� �Է� �� �˻� �Լ�
{
	for (int i = 0; i < dpt_list_count; i++)
	{
		if (strcmp((const char*)dpt_list_array[i], user_input_dpt) == 0) //��ġ�ϸ�
		{
			return true; //�Է� ���� �ش��ϴ� �а� �̸� ����
		}
	}
	return false; //�Է� ���� �ش��ϴ� �а� �̸� �������� ����
}

void create_student_list(Student_list** src_list, char** dpt_list_array, int dpt_list_count, bool flag_use_opcode) //����ڷκ��� �л� �����͸� �Է¹ް� Edit ���� ���� ���� �� �����͸� insertion_sort ����
{
	//flag_use_opcode : true - �����ڵ带 ������ �л� ����Ʈ ����, false - �л� ����Ʈ ����
	
	char op_code[OP_LENGTH + 1] = { 0, }; //���� �ڵ�(Insert, Delete, Correct)('\0' ���� 1�ڸ�)
	char student_id[ID_LENGTH + 1] = { 0, }; //�й�(����,���� ����, '\0' ���� 8�ڸ�)
	char name[NAME_LENGTH + 1] = { 0, }; //�̸� ('\0' ���� 10�ڸ�)
	char dpt[DPT_LENGTH + 1] = { 0, }; //�а� ('\0' ���� 2�ڸ�)
	int grade = EXCEPTION_HANDLING_VALUE;  //�г�
	int score = EXCEPTION_HANDLING_VALUE; //����

	std::string user_input; //����ڷκ��� ��ɾ �� �ٷ� �Է¹޴� ����
	Student_string_data* string_data = new Student_string_data(); //����ڷκ��� ���� ������ string���� �и��Ͽ� edit ���� �˻� �� �Ҵ�

	//��� ȭ�� ����
	if (flag_use_opcode == true)
	{
		disp_student_trans_input_bar(dpt_list_array, dpt_list_count);
	}
	else
	{
		disp_student_input_bar(dpt_list_array, dpt_list_count);
	}

	while (1)
	{
		string_data->clear_all(); //���� �� �Է� �ø� ���ؼ� �ʱ�ȭ

		std::getline(std::cin, user_input); //�� �ٷ� �Է¹ޱ�
		std::stringstream ss(user_input); //�и�

		switch (flag_use_opcode) //�Է� �� �� ��ȯ �� ����
		{
		case true: //�����ڵ带 ���� �л�����Ʈ ����(Ʈ����� ���� �ۼ�)
			ss >> string_data->str_op_code; //�����ڵ� �и�
			
			if (string_data->str_op_code.compare("-1") == 0)
			{	
				delete(string_data);
				return;
			}
			else if (string_data->str_op_code.compare("D") == 0) //���� ���� �� �й��� �ʿ�
			{
				ss >> string_data->str_student_id;
				if (chk_edit_error(src_list, &string_data, dpt_list_array, dpt_list_count) == true) //edit ���� �˻� �� �α� ���
				{
					strcpy(op_code, string_data->str_op_code.c_str()); //�Է��� ���ڿ��� �̻��� ������ ����
					strcpy(student_id, string_data->str_student_id.c_str()); //�Է��� ���ڿ��� �̻��� ������ ����
					//edit ������ ������� �ʾ��� ��� ���ο� �л� ��ü�� �����Ͽ� ����Ʈ�� ���ĵ� ��ġ�� ����
					insertion_sort(src_list, op_code, student_id, NULL_VALUE, NULL_VALUE, EXCEPTION_HANDLING_VALUE, EXCEPTION_HANDLING_VALUE);
				}
				break;
			}
			else //����, ���� ���� �Ǵ� �ٸ� �߸��� �Է�(EDit ���� �˻��� ���� ����)
			{
				ss >> string_data->str_student_id; //�й� �и�
				ss >> string_data->str_name; //�̸� �и�
				ss >> string_data->str_dpt; //�а� �и�
				ss >> string_data->str_grade; //�г� �и�
				ss >> string_data->str_score; //���� �и�

				if (chk_edit_error(src_list, &string_data, dpt_list_array, dpt_list_count) == true) //edit ���� �˻� �� �α� ���
				{
					strcpy(op_code, string_data->str_op_code.c_str()); //�Է��� ���ڿ��� �̻��� ������ ����
					strcpy(student_id, string_data->str_student_id.c_str()); //�Է��� ���ڿ��� �̻��� ������ ����
					strcpy(name, string_data->str_name.c_str()); //�Է��� ���ڿ��� �̻��� ������ ����
					strcpy(dpt, string_data->str_dpt.c_str()); //�Է��� ���ڿ��� �̻��� ������ ����
					grade = atoi(string_data->str_grade.c_str()); //�Է��� ���ڿ��� �̻��� ������ ����
					score = atoi(string_data->str_score.c_str()); //�Է��� ���ڿ��� �̻��� ������ ����

					//edit ������ ������� �ʾ��� ��� ���ο� �л� ��ü�� �����Ͽ� ����Ʈ�� ���ĵ� ��ġ�� ����
					insertion_sort(src_list, op_code, student_id, name, dpt, grade, score);
				}
				break;
			}
		
		case false: //�л� ����Ʈ ����
			ss >> string_data->str_student_id; //�й� �и�

			if (string_data->str_student_id.compare("-1") == 0)
			{
				delete(string_data);
				return;
			}
			ss >> string_data->str_name; //�̸� �и�
			ss >> string_data->str_dpt; //�а� �и�
			ss >> string_data->str_grade; //�г� �и�
			ss >> string_data->str_score; //���� �и�

			if (chk_edit_error(src_list, &string_data, dpt_list_array, dpt_list_count) == true) //edit ���� �˻� �� �α� ���
			{
				strcpy(student_id, string_data->str_student_id.c_str()); //�Է��� ���ڿ��� �̻��� ������ ����
				strcpy(name, string_data->str_name.c_str()); //�Է��� ���ڿ��� �̻��� ������ ����
				strcpy(dpt, string_data->str_dpt.c_str()); //�Է��� ���ڿ��� �̻��� ������ ����
				grade = atoi(string_data->str_grade.c_str()); //�Է��� ���ڿ��� �̻��� ������ ����
				score = atoi(string_data->str_score.c_str()); //�Է��� ���ڿ��� �̻��� ������ ����

				//edit ������ ������� �ʾ��� ��� ���ο� �л� ��ü�� �����Ͽ� ����Ʈ�� ���ĵ� ��ġ�� ����
				insertion_sort(src_list, NULL_VALUE, student_id, name, dpt, grade, score);
			}
			break;

		}
	}
	
	delete(string_data);
}

void insertion_sort(Student_list** src_list, const char* op_code, const char* ID, const char* name, const char* dpt, int grade, int score)  //Edit ���� ������ ��ģ �����͵�κ��� ���ο� �л� ��ü�� ���� �� ���������Ͽ� ����Ʈ ����
{
	Student_list* new_student = NULL; //����Ʈ�� ������ ���ο� �л� ���� ����
	Student_list* tmp_student_head = NULL; //�й� ������ ������ ���� ����Ʈ�� ��� ��带 ����

	int student_counter = 0; //�Է¹޾� ������ �л� ��
	char current_buffer_id[ID_LENGTH + 1] = { 0, }; //���� �л� ����� �й� �� �ӽ�����
	char new_buffer_id[ID_LENGTH + 1] = { 0, }; //���ο� �л� ����� �й� �� �ӽ�����

	//������ ���� �� ����� ���� �л� ���� ����Ʈ�� �й� ������ �߰�
	if (*src_list == NULL) //�ʱ� ��� ����
	{
		(*src_list) = new Student_list(op_code, ID, name, dpt, grade, score);
	}
	else
	{
		new_student = new Student_list(op_code, ID, name, dpt, grade, score); //����Ʈ�� ������ ���ο� �л� ����
		tmp_student_head = (*src_list); //���� ����Ʈ�� ù ��° �л� ���� �ӽ�����

		while (1) //����Ʈ�� ������ ����, �й� ������ ���� �����Ͽ� ����Ʈ�� �����
		{
			//���� �� �Է��� ���� ���� �ʱ�ȭ
			memset(current_buffer_id, NULL, sizeof(current_buffer_id));
			memset(new_buffer_id, NULL, sizeof(new_buffer_id));

			(*src_list)->get_Student_ID(current_buffer_id); //���� �л� ����� �й� �� ���� �Ҵ�
			new_student->get_Student_ID(new_buffer_id); //���ο� �л� ����� �й� �� ���� �Ҵ�

			if (strcmp(new_buffer_id, current_buffer_id) < 0) //strcmp(A,B) < 0 => A < B
			{
				//���� �Է��ϰ��� �ϴ� ����� Ű ���� ���� ��庸�� ���� �� �� ��� �տ� ���
				new_student->next = (*src_list);
				if ((*src_list)->pre != NULL) //���� ����� ���� ��尡 �����ϸ�
				{
					new_student->pre = (*src_list)->pre;
					(*src_list)->pre->next = new_student;
				}
				(*src_list)->pre = new_student;

				if (new_student->pre == NULL) //���ο� �л� ������ ���Ե� ��ġ�� �� ó���� ���
					(*src_list) = new_student; //�л� ���� ����Ʈ�� ���ο� �л� ���� ��带 ����Ű���� ����
				else //���ο� �л� ������ �߾ӿ� ���Ե� ���
					(*src_list) = tmp_student_head; //���� ���� �ٽ� �����Ͽ� ����Ʈ�� �� ó���� ����Ű���� ��
				break;
			}

			if ((*src_list)->next != NULL)
				(*src_list) = (*src_list)->next;
			else
				break;
		} //����Ʈ ������ ���� �Ϸ�

		//��� ������ ���Ͽ� ������ ���� ���� ��� �� �ڿ� ����
		if (new_student->next == NULL && new_student->pre == NULL)
		{
			new_student->pre = (*src_list);
			(*src_list)->next = new_student;
			(*src_list) = tmp_student_head; //���� ���� �ٽ� �����Ͽ� ����Ʈ�� �� ó���� ����Ű���� ��
		}
	}
}

void deallocate_memory(Student_list** student_list, char** dpt_list_array, int dpt_list_count, int type) //�޸� ����
{
	//type : 0 - Dynamic Array, 1 - Double Linked List
	//type�� ���� �޸� ����
	Student_list* student_list_indexer = NULL; //���� ����Ʈ �ε���
	Student_list* del_this = NULL; //student_list_indexer�κ��� �Ҵ��Ͽ� ���������� ����

	switch (type)
	{
	case 0: //dynamic arrray
		if (dpt_list_array != NULL)
		{
			for (int i = 0; i < dpt_list_count; i++)
			{
				delete[] dpt_list_array[i];
			}
			delete[] dpt_list_array;
		}
		break;

	case 1: //double linked list
		student_list_indexer = (*student_list);

		while (student_list_indexer != NULL)
		{
			del_this = student_list_indexer;
			student_list_indexer = student_list_indexer->next; //�ε����� �̵� �� ����
			delete(del_this);
		}
		(*student_list) = NULL; //��ũ �ʱ�ȭ

		break;

	default:
		break;
	}
}