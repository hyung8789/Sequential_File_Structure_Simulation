#include "header.h"

/* 
	- �а� ����Ʈ ����, ������, Ʈ�����, �α� ���� ó�� �� ���, ������ ���� ���� �Լ� ����
	- ���� �� ���ĵ� �л� ��ü ����Ʈ�� ���Ͽ� ��� �� ����Ʈ ���
*/

char** allocate_dpt_list_array(int& dpt_list_count) //�а� ����Ʈ ���Ϸκ��� ���� �迭 �Ҵ�
{
	//row : ���Ϸκ��� �о���� �а� ��(dpt_list_count), col : �а� ���ڿ� ����(DPT_LENGTH + 1)
	
	FILE* dpt_list_fp = NULL;

	char** dpt_list_array = NULL; //�а� ����Ʈ�κ��� �����Ҵ��Ͽ� ����
	char dpt_buffer[SINGLE_LINE_BUFFER_SIZE] = { 0, }; //����Ʈ�� ��ϵ� �а� ���� �ľ� �� ���Ϸκ��� �а��� �ӽ������ϴ� ����
	int i = 0; //�а� �迭 �Ҵ� �ε���

	if ((dpt_list_fp = fopen("dpt_list.txt", "rt")) != NULL) //���������� open ��
	{
		while (!feof(dpt_list_fp)) //������ ������ 
		{
			fgets(dpt_buffer, SINGLE_LINE_BUFFER_SIZE, dpt_list_fp); //���๮�ڰ� ���� �� ���� �д´�
			memset(dpt_buffer, NULL, sizeof(dpt_buffer)); //���� �ʱ�ȭ
			dpt_list_count++; //����Ʈ�� �а� ���� ī����
		}

		if (dpt_list_count > 0) //����Ʈ�� ��ϵ� �а��� �����ϸ�
		{
			dpt_list_array = new char* [dpt_list_count]; //�а� �� ��ŭ�� ���� �Ҵ�(row)

			for (int i = 0; i < dpt_list_count; i++) //������ ������ ���� �ٽ� char�� �迭 �Ҵ�(col)
			{
				dpt_list_array[i] = new char[DPT_LENGTH + 1]; //�а� ���� ���̸�ŭ�� �迭 �Ҵ�(\0 ����)
			}
		}
		else
		{
			disp_error_msg("�а� ����Ʈ�� ����ֽ��ϴ�.");
			if (dpt_list_fp != NULL)
				fclose(dpt_list_fp);
			return NULL;
		}

		rewind(dpt_list_fp); //�а� ����Ʈ�� �ٽ� �о�鿩�� �迭�� �Ҵ��Ѵ�
		while (!feof(dpt_list_fp) && i < dpt_list_count) //������ ������
		{
			fscanf(dpt_list_fp, "%s\n", dpt_buffer);
			strcpy(dpt_list_array[i++], dpt_buffer);
			memset(dpt_buffer, NULL, sizeof(dpt_buffer)); //���� �ʱ�ȭ
		}
		fclose(dpt_list_fp);

		return dpt_list_array;
	}
	else
	{
		disp_error_msg("�а� ����Ʈ ����(dpt_list.txt)�� �������� �ʽ��ϴ�.");
		return NULL;
	}
}

bool chk_edit_error(Student_list** src_list, Student_string_data** src_data, char** dpt_list_array, int dpt_list_count) //Editing ���� ���� �� �α� ��� �Լ�
{
	FILE* logger = NULL;

	Student_list* student_list_indexer = NULL; //������ �й� �˻縦 ���� �ε���
	char buffer_id[9] = { 0, }; //�й� �� �ӽ�����
	bool flag_no_error = true; //���� �÷���

	if ((logger = fopen("error_log.log", "at")) != NULL) //append mode
	{
		//�����ڵ�, �й�, �̸�, �а�, �г�, ���� ���� ����
		//�μ��� ���� �л� ����Ʈ�κ��� ���� �й� �˻�
		//����ڷκ��� ���� �Է� ��(src_data)�� ��Ŀ� �°� �Է��Ͽ����� �˻�
		
		//������ ���� �� Ʈ����� ���� �ۼ� �� ���������� �����ϰ� �ݵ�� �������� �й�, �̸�, �а�, �г�, ������ �Է¹޾ƾ��� 
		//Ʈ����� ���� �ۼ� �� ���� ���� �ÿ��� �̸�, �а�, �г�, ������ �Է� ���� ����

		/*** ���� �й��� ���� ����ó�� �� ���� �Է¿� ���� ����ó�� ***/
		if ((*src_data)->str_student_id.empty() != true) //���� �й� �˻�
		{
			if (*src_list != NULL)
			{
				for (student_list_indexer = (*src_list); student_list_indexer != NULL; student_list_indexer = student_list_indexer->next)
				{
					student_list_indexer->get_Student_ID(buffer_id); //�й� �� ���� �Ҵ�
					if ((*src_data)->str_student_id.compare(buffer_id) == 0) //��ġ�ϸ�
					{
						fprintf(logger, "edit_error || %s || �й� '%s'�� �̹� �����մϴ�.\n", get_current_time().c_str(), (*src_data)->str_student_id.c_str());
						flag_no_error = false;
					}
				}
			}
		}
		else
		{
			fprintf(logger, "edit_error || %s || �й��� �����Դϴ�.\n", get_current_time().c_str());
			flag_no_error = false;
		}
		/*** ���� �й��� ���� ����ó�� �� ���� �Է¿� ���� ����ó�� �� ***/

		/*** ���� �ڵ忡 ���� ����ó�� ***/
		//������ ���� ���� �ÿ��� �����ڵ带 �Է¹��� �����Ƿ� ���� �ڵ�� �������� ó��
		if ((*src_data)->str_op_code.empty() != true) //���� �ڵ� �˻�
		{
			if((*src_data)->str_op_code.compare("I") != 0 && (*src_data)->str_op_code.compare("D") != 0 && (*src_data)->str_op_code.compare("C") != 0)
			{
				fprintf(logger, "edit_error || %s || '%s'�� �߸� �� ���� �ڵ� �Դϴ�.\n", get_current_time().c_str(), (*src_data)->str_op_code.c_str());
				flag_no_error = false;
			}
		}
		/*** ���� �ڵ忡 ���� ����ó�� �� ***/

		/*** �й��� ���� ����ó�� ***/
		if ((*src_data)->str_student_id.empty() != true)
		{
			if ((*src_data)->str_student_id.length() > ID_LENGTH)
			{
				fprintf(logger, "edit_error || %s || �й� '%s'�� �ִ���� '%d'�� �Ѿ����ϴ�.\n", get_current_time().c_str(), (*src_data)->str_student_id.c_str(), ID_LENGTH);
				flag_no_error = false;
			}
		}
		else //�й��� ����
		{
			fprintf(logger, "edit_error || %s || �й��� �����Դϴ�.\n", get_current_time().c_str());
			flag_no_error = false;
		}
		/*** �й��� ���� ����ó�� �� ***/

		/*** �̸��� ���� ����ó�� ***/
		if ((*src_data)->str_op_code.empty() != true && (*src_data)->str_op_code.compare("D") != 0) //Ʈ����� ���� �ۼ� ����, �����ڵ尡 ������ �ƴϰ� ���� ������ �ƴ� ���
		{
			if ((*src_data)->str_name.empty() != true) //�̸��� ������ �ƴϰ�, �����ڵ尡 ������ �ƴ� ���, �� Ʈ����� ���� �ۼ� ��
			{
				if ((*src_data)->str_name.length() > NAME_LENGTH)
				{
					fprintf(logger, "edit_error || %s || �̸� '%s'�� �ִ���� '%d'�� �Ѿ����ϴ�.\n", get_current_time().c_str(), (*src_data)->str_name.c_str(), NAME_LENGTH);
					flag_no_error = false;
				}
			}
		}

		if ((*src_data)->str_op_code.empty() != true && (*src_data)->str_op_code.compare("D") == 0) //���� ������ ��� ����ڰ� ���� �Է��ϸ� ����
		{
			if ((*src_data)->str_name.empty() != true)
			{
				fprintf(logger, "edit_error || %s || �̸� �Է� ���� : ���� ����ÿ� �й��� �Է��ؾ��մϴ�.\n", get_current_time().c_str());
				flag_no_error = false;
			}
		}
		/*** �̸��� ���� ����ó�� �� ***/

		/*** �а��� ���� ����ó�� ***/
		if ((*src_data)->str_dpt.empty() != true && (*src_data)->str_op_code.compare("D") != 0) //�а����� ������� �ʰ�, ���� ������ �ƴ� ���
		{
			if ((*src_data)->str_dpt.length() > DPT_LENGTH || chk_dpt_list(dpt_list_array, dpt_list_count, (char*)(*src_data)->str_dpt.c_str()) == false) //�Է��� �а��� �������� ������
			{
				fprintf(logger, "edit_error || %s || '%s'�� ��ϵ� �а� �̸��� �ƴϰų� �ִ���� '%d'�� �ʰ��Ͽ����ϴ�.\n", get_current_time().c_str(), (*src_data)->str_dpt.c_str(), DPT_LENGTH);
				flag_no_error = false;
			}
		}
		if ((*src_data)->str_op_code.empty() != true && (*src_data)->str_op_code.compare("D") == 0) //���� ������ ���
		{
			if ((*src_data)->str_dpt.empty() != true)
			{
				fprintf(logger, "edit_error || %s || �а� �� �Է� ���� : ���� ����ÿ� �й��� �Է��ؾ��մϴ�.\n", get_current_time().c_str());
				flag_no_error = false;
			}
		}
		/*** �а��� ���� ����ó�� �� ***/

		/*** �г⿡ ���� ����ó�� ***/
		if ((*src_data)->str_grade.empty() != true && (*src_data)->str_op_code.compare("D") != 0) //�г��� ������� �ʰ�, ���� ������ �ƴ� ���
		{
			if (atoi((*src_data)->str_grade.c_str()) < 1 || atoi((*src_data)->str_grade.c_str()) > 4) //�г� ������ �����
			{
				fprintf(logger, "edit_error || %s || '%s'�� ��ȿ�� �г� ������ �ƴմϴ�.\n", get_current_time().c_str(), (*src_data)->str_grade.c_str());
				flag_no_error = false;
			}
		}
		if ((*src_data)->str_op_code.empty() != true && (*src_data)->str_op_code.compare("D") == 0) //���� ������ ���
		{
			if ((*src_data)->str_grade.empty() != true)
			{
				fprintf(logger, "edit_error || %s || �г� �Է� ���� : ���� ����ÿ� �й��� �Է��ؾ��մϴ�.\n", get_current_time().c_str());
				flag_no_error = false;
			}
		}
		/*** �г⿡ ���� ����ó�� �� ***/

		/*** ������ ���� ����ó�� ***/
		if ((*src_data)->str_score.empty() != true && (*src_data)->str_op_code.compare("D") != 0) //������ ������� �ʰ�, ���� ������ �ƴ� ���
		{
			if (atoi((*src_data)->str_score.c_str()) < 0 || atoi((*src_data)->str_score.c_str()) > 100) //���� ������ �����
			{
				fprintf(logger, "edit_error || %s || '%s'�� ��ȿ�� ���� ������ �ƴմϴ�.\n", get_current_time().c_str(), (*src_data)->str_score.c_str());
				flag_no_error = false;
			}
		}
		if ((*src_data)->str_op_code.empty() != true && (*src_data)->str_op_code.compare("D") == 0) //���� ������ ���
		{
			if ((*src_data)->str_score.empty() != true)
			{
				fprintf(logger, "edit_error || %s || ���� �Է� ���� : ���� ����ÿ� �й��� �Է��ؾ��մϴ�.\n", get_current_time().c_str());
				flag_no_error = false;
			}
		}
		/*** ������ ���� ����ó�� �� ***/

		if (flag_no_error == true)
		{
			fclose(logger);
			return true; //������ ������� ����
		}
	}
	else
	{
		disp_error_msg("error_log.log ��� �Ұ���");
		return false;
	}
	
	fclose(logger);
	return false;
}

bool chk_trans_error(const char* masterKey, const char* transKey, OPERATION_CODE op_code) //Transaction ���� ���� �� �α� ��� �Լ�
{
	//correct, delete, insert

	FILE* logger = NULL;
	bool flag_no_error = true; //���� �÷���

	if ((logger = fopen("error_log.log", "at")) != NULL) //append mode
	{
		/***
		3 - 1) masterKey < transKey
			Ʈ����� ������ Ű ���� ũ�ٴ� ���� ���� ������ ���Ͽ� ������ ���ڵ尡 ���ٴ� ���̹Ƿ� ������ ���ڵ带 ���ο� ������ ȭ�Ϸ� �����ϰ�, ���� ������ ���ڵ带 �о�´�
			masterKey++

		3 - 2) masterKey == transKey
			Ʈ����� ���ڵ��� �����ڵ忡 ����

			- ����(C) : ���� ������ ���ڵ��� ������ Ʈ����� ���ڵ��� �������� ���ο� ������ ���Ͽ� ���, ���� �����Ϳ� Ʈ����� ���ڵ带 �о�´�
			copy trans_record to new_master
			masterKey++
			transKEy++

			- ����(D) : ���� ������ ���ڵ� ����, �ƹ��� ó���� ���� �ʰ�, ���� �����Ϳ� Ʈ����� ���ڵ带 �о�´�
			masterKey++
			transKey++

			- ����(I) : ������ ���Ͽ� �̹� ���� Ű ���� ���� ���ڵ尡 �����Ƿ� �ߺ� ���ڵ� ����, ���� Ʈ����� ���ڵ带 �о��
			output error_log
			transKey++

		3 - 3) masterKey > transKey
			Ʈ����� ���ڵ忡 ��ġ�ϴ� ������ ���ڵ尡 ���� ���

			- ����(C) : ��ġ�ϴ� ������ ���ڵ尡 �����Ƿ� ���� ���, ���� Ʈ����� ���ڵ带 �о��
			output error_log
			transKey++

			- ����(D) : ��ġ�ϴ� ������ ���ڵ尡 �����Ƿ� ���� ���, ���� Ʈ����� ���ڵ带 �о��
			output error_log
			transKey++

			- ����(I) : ���ο� ������ ���Ϸ� Ʈ����� ���ڵ带 ����, ���� Ʈ����� ���ڵ带 �о��
			copy trans_record to new_master
			transKey++
		***/

		//���� : ������ ���� ��Ÿ���� EOF�� � ���ڵ� Ű ������ ũ��
		
		if(strcmp(masterKey, EOF_KEY_VALUE) == 0)
		{
			//������ Ű ���� Ʈ����� Ű ������ ũ��
			switch (op_code)
			{
			case OPERATION_CODE::Insert: //���ο� ������ ���Ϸ� Ʈ����� ���ڵ带 ����, ���� Ʈ����� ���ڵ带 �о��
				flag_no_error = true;
				break;

			case OPERATION_CODE::Delete: //��ġ�ϴ� ������ ���ڵ尡 �����Ƿ� ���� ���, ���� Ʈ����� ���ڵ带 �о��
				fprintf(logger, "trans_error || %s || Delete || masterKey : %s || transKey : %s || ��ġ�ϴ� ������ ���ڵ尡 �������� �ʽ��ϴ�.\n", get_current_time().c_str(), masterKey, transKey);
				flag_no_error = false;
				break;

			case OPERATION_CODE::Correct: //��ġ�ϴ� ������ ���ڵ尡 �����Ƿ� ���� ���, ���� Ʈ����� ���ڵ带 �о��
				fprintf(logger, "trans_error || %s || Correct || masterKey : %s || transKey : %s || ��ġ�ϴ� ������ ���ڵ尡 �������� �ʽ��ϴ�.\n", get_current_time().c_str(), masterKey, transKey);
				flag_no_error = false;
				break;

			default: //����
				disp_error_msg("�����ڵ尡 �Ҵ���� �ʾҽ��ϴ�. Ʈ����� ������ ���Ƿ� ������ �� �����ϴ�.");
				system("pause");
				exit(1);
				break;
			}
		}
		else if (strcmp(transKey, EOF_KEY_VALUE) == 0)
		{
			//Ʈ����� Ű ���� ������ Ű ������ ũ��
			//���ο� ������ ���Ͽ� ���� ������ ������ ���ڵ带 ����, ���� ������ ���ڵ带 �о��
			flag_no_error = true;
		}
		else //���� ������ ���� �������� ���� ���
		{
			if (strcmp(masterKey, transKey) < 0) //masterKey < transKey
			{
				switch (op_code)
				{
				case OPERATION_CODE::Insert: //���ο� ������ ���Ͽ� ���� ������ ������ ���ڵ带 ����, ���� ������ ���ڵ带 �о��
					flag_no_error = true;
					break;

				case OPERATION_CODE::Delete: //������ ���ڵ带 ���ο� ������ ȭ�Ϸ� �����ϰ�, ���� ������ ���ڵ带 �о��
					flag_no_error = true;
					break;

				case OPERATION_CODE::Correct: //������ ���ڵ带 ���ο� ������ ȭ�Ϸ� �����ϰ�, ���� ������ ���ڵ带 �о��
					flag_no_error = true;
					break;

				default: //����
					disp_error_msg("�����ڵ尡 �Ҵ���� �ʾҽ��ϴ�. Ʈ����� ������ ���Ƿ� ������ �� �����ϴ�.");
					system("pause");
					exit(1);
					break;
				}

			}
			else if (strcmp(masterKey, transKey) == 0) //masterKey == transKey
			{
				switch (op_code)
				{
				case OPERATION_CODE::Insert: //������ ���Ͽ� �̹� ���� Ű ���� ���� ���ڵ尡 �����Ƿ� �ߺ� ���ڵ� ����, ���� Ʈ����� ���ڵ带 �о��
					fprintf(logger, "trans_error || %s || Insert || masterKey : %s || transKey : %s || ������ ���Ͽ� ���� Ű ���� �����մϴ�.\n", get_current_time().c_str(), masterKey, transKey);
					flag_no_error = false;
					break;

				case OPERATION_CODE::Delete: //���� ������ ���ڵ� ����, �ƹ��� ó���� ���� �ʰ�, ���� �����Ϳ� Ʈ����� ���ڵ带 �о��
					flag_no_error = true;
					break;

				case OPERATION_CODE::Correct: //���� ������ ���ڵ��� ������ Ʈ����� ���ڵ��� �������� ���ο� ������ ���Ͽ� ���, ���� �����Ϳ� Ʈ����� ���ڵ带 �о��
					flag_no_error = true;
					break;

				default: //����
					disp_error_msg("�����ڵ尡 �Ҵ���� �ʾҽ��ϴ�. Ʈ����� ������ ���Ƿ� ������ �� �����ϴ�.");
					system("pause");
					exit(1);
					break;
				}
			}
			else //masterKey > transKey(Ʈ����� ���ڵ忡 ��ġ�ϴ� ������ ���ڵ尡 ���� ���)
			{
				switch (op_code)
				{
				case OPERATION_CODE::Insert: //���ο� ������ ���Ϸ� Ʈ����� ���ڵ带 ����, ���� Ʈ����� ���ڵ带 �о��
					flag_no_error = true;
					break;

				case OPERATION_CODE::Delete: //��ġ�ϴ� ������ ���ڵ尡 �����Ƿ� ���� ���, ���� Ʈ����� ���ڵ带 �о��
					fprintf(logger, "trans_error || %s || Delete || masterKey : %s || transKey : %s || ��ġ�ϴ� ������ ���ڵ尡 �������� �ʽ��ϴ�.\n", get_current_time().c_str(), masterKey, transKey);
					flag_no_error = false;
					break;

				case OPERATION_CODE::Correct: //��ġ�ϴ� ������ ���ڵ尡 �����Ƿ� ���� ���, ���� Ʈ����� ���ڵ带 �о��
					fprintf(logger, "trans_error || %s || Correct || masterKey : %s || transKey : %s || ��ġ�ϴ� ������ ���ڵ尡 �������� �ʽ��ϴ�.\n", get_current_time().c_str(), masterKey, transKey);
					flag_no_error = false;
					break;

				default: //����
					disp_error_msg("�����ڵ尡 �Ҵ���� �ʾҽ��ϴ�. Ʈ����� ������ ���Ƿ� ������ �� �����ϴ�.");
					system("pause");
					exit(1);
					break;
				}
			}
		}

		if (flag_no_error == true)
		{
			fclose(logger);
			return true; //������ ������� ����
		}
	}
	else
	{
		disp_error_msg("error_log.log ��� �Ұ���");
		return false;
	}

	fclose(logger);
	return false;
}

bool create_file(Student_list** src_list, char** dpt_list_array, int dpt_list_count, int file_mode) //�ʱ� ������ ���� ���� �Ǵ� Ʈ����� ���� �ۼ� �Լ�
{
	//file_mode : 0 - ������ ����, 1 - Ʈ����� ����
	FILE* fp = NULL;
	Student_list* student_list_indexer = NULL; //������ ���� ù ��° ���� �������� �ʾ����Ƿ� ������ ���ؼ��� �ε����� ���� �ؾ��Ѵ�
	char buffer_id[9] = { 0, }; //�й� �� �ӽ�����

	switch (file_mode)
	{
	case 0: //�ʱ� ������ ���� ����
		if ((fp = fopen("master.txt", "wt")) != NULL) //���� ������ ������ ������ ���
		{
			if (input_init_master_confirm() == true) //������ ���� �ʱ�ȭ ���� ���� Ȯ��
			{
				//������ ���� �ʱ� ����
				create_student_list(src_list, dpt_list_array, dpt_list_count, false); //src_list�� �Ѱ� �л� ����Ʈ�� ����

				if (*src_list != NULL)
				{
					student_list_indexer = (*src_list);

					fprintf(fp, "%s\n", get_current_time().c_str()); //���Ͽ� �ý��� ���� �ð��� �����Ͽ� ���

					while (student_list_indexer != NULL)
					{
						student_list_indexer->get_Student_ID(buffer_id); //�й��� ���� �Ҵ�
						fprintf(fp, "%s %s %s %d %d\n", buffer_id, student_list_indexer->name, student_list_indexer->department, student_list_indexer->grade, student_list_indexer->score);
						student_list_indexer = student_list_indexer->next;
					}

					deallocate_memory(src_list, dpt_list_array, dpt_list_count, 1); //�л� ����Ʈ �޸� ����
					fclose(fp);

					return true;
				}
				break;
			}
			else break;
		}
		else
		{
			disp_error_msg("������ ������ ���� ���� �� �� �����ϴ�.");
			break;
		}
		break;

	case 1: //Ʈ����� ���� �ۼ�
		if ((fp = fopen("trans.txt", "wt")) != NULL) //���� Ʈ����� ������ ������ ���
		{
			create_student_list(src_list, dpt_list_array, dpt_list_count, true); //src_list�� �Ѱ� �л� ����Ʈ�� ����

			if (*src_list != NULL)
			{
				student_list_indexer = (*src_list);

				fprintf(fp, "%s\n", get_current_time().c_str()); //���Ͽ� �ý��� ���� �ð��� �����Ͽ� ���

				while (student_list_indexer != NULL)
				{
					student_list_indexer->get_Student_ID(buffer_id); //�й��� ���� �Ҵ�

					if (student_list_indexer->op_code == OPERATION_CODE::Delete) //���� ���� �� �й� ���� �Է�
					{
						fprintf(fp, "%s %c\n", buffer_id, to_char(student_list_indexer->op_code)); //�й�, �����ڵ� ������ ���
						student_list_indexer = student_list_indexer->next;
					}
					else
					{
						fprintf(fp, "%s %c %s %s %d %d\n", buffer_id, to_char(student_list_indexer->op_code), student_list_indexer->name, student_list_indexer->department, student_list_indexer->grade, student_list_indexer->score);
						student_list_indexer = student_list_indexer->next;
					}
				}

				deallocate_memory(src_list, dpt_list_array, dpt_list_count, 1); //�л� ����Ʈ �޸� ����
				fclose(fp);

				return true;
			}
			break;
		}
		else
		{
			disp_error_msg("Ʈ����� ������ ���� ���� �� �� �����ϴ�.");
			break;
		}
		break;

	default:
		break;
	}

	if(fp!=NULL)
		fclose(fp);

	return false;
}

void print_file(int file_mode) //������ ���� �Ǵ� Ʈ����� ���� ��� �Լ�
{
	//file_mode : 0 - ������ ����, 1 - Ʈ����� ����, 2 - Edit ���� �α�, 3 - Transaction ���� �α�
	FILE* fp = NULL;
	char read_buffer[SINGLE_LINE_BUFFER_SIZE] = { 0, };

	disp_student_description_bar(file_mode);

	switch (file_mode)
	{
	case 0: //������ ����
		if ((fp = fopen("master.txt", "r")) != NULL)
		{
			//������ ���� ���� �ð� ���
			fscanf(fp, "%s\n", read_buffer);
			printf("������ ���� ���� �ð� : %s\n", read_buffer);
			std::cout << "--------------------------------------------------------------" << std::endl;
			memset(read_buffer, NULL, sizeof(read_buffer));
		}
		else
		{
			disp_error_msg("������ ������ �������� �ʽ��ϴ�.");
			return;
			break;
		}
		break;

	case 1: //Ʈ����� ����
		if ((fp = fopen("trans.txt", "r")) != NULL)
		{
			//Ʈ����� ���� ���� �ð� ���
			fscanf(fp, "%s\n", read_buffer);
			printf("Ʈ����� ���� ���� �ð� : %s\n", read_buffer);
			std::cout << "--------------------------------------------------------------" << std::endl;
			memset(read_buffer, NULL, sizeof(read_buffer));
		}
		else
		{
			disp_error_msg("Ʈ����� ������ �������� �ʽ��ϴ�.");
			return;
			break;
		}
		break;

	case 2: //Edit ���� �α�
		//type == edit_error�� ��츸 ���
		if ((fp = fopen("error_log.log", "r")) != NULL)
		{
			disp_log_description_bar();

			while (!feof(fp))
			{
				fscanf(fp, "%s", read_buffer); //�� ���ڿ��� �о���δ�
				if (strcmp(read_buffer, "edit_error") == 0) //��ġ�� ��츸
				{
					printf("%s", read_buffer);
					fgets(read_buffer, SINGLE_LINE_BUFFER_SIZE, fp); //���� ���ڰ� ���� �� ����
					std::cout << read_buffer;
					memset(read_buffer, NULL, sizeof(read_buffer));
				}
				else
				{
					//���� ���ڰ� ���� �� ���� �о �ѱ��
					fgets(read_buffer, SINGLE_LINE_BUFFER_SIZE, fp); //���� ���ڰ� ���� �� ����
					memset(read_buffer, NULL, sizeof(read_buffer));
				}
			}
			fclose(fp);
			return;
		}
		else
		{
			disp_error_msg("�α� ������ �������� �ʽ��ϴ�.");
			return;
			break;
		}
		break;

	case 3: //Transaction ���� �α�
		//type == trans_error�� ��츸 ���
		if ((fp = fopen("error_log.log", "r")) != NULL)
		{
			disp_trans_log_description_bar();

			while (!feof(fp))
			{
				fscanf(fp, "%s", read_buffer); //�� ���ڿ��� �о���δ�
				if (strcmp(read_buffer, "trans_error") == 0) //��ġ�� ��츸
				{
					printf("%s", read_buffer);
					fgets(read_buffer, SINGLE_LINE_BUFFER_SIZE, fp); //���� ���ڰ� ���� �� ����
					std::cout << read_buffer;
					memset(read_buffer, NULL, sizeof(read_buffer));
				}
				else //���� ���ڰ� ���� �� ���� �о �ѱ��
				{
					fgets(read_buffer, SINGLE_LINE_BUFFER_SIZE, fp); //���� ���ڰ� ���� �� ����
					memset(read_buffer, NULL, sizeof(read_buffer));
				}
			}
			fclose(fp);
			return;
		}
		else
		{
			disp_error_msg("�α� ������ �������� �ʽ��ϴ�.");
			return;
			break;
		}
		break;

	default:
		return;
		break;
	}

	//������ ���� �Ǵ� Ʈ����� ������ ���� ���
	while (!feof(fp))
	{
		fread(read_buffer, sizeof(char), sizeof(read_buffer) - 1, fp); //'\0' ����� ���� �����Ͽ� ����� 1 �۰�
		printf("%s", read_buffer);
		memset(read_buffer, NULL, sizeof(read_buffer));
	}

	if(fp != NULL)
		fclose(fp);
	
	return;
}


void update_master(bool flag_replace_master) //������ ���� ���� �Լ�
{
	FILE* new_master_fp = NULL; //���ο� ������ ���� ������
	FILE* master_fp = NULL; //������ ���� ������
	FILE* trans_fp = NULL; //Ʈ����� ���� ������

	//�� ������ ���ų�¥�� ���Ͽ� ������ ������ Ʈ����� ���Ϻ��� �ֽ��� ��� �������� ����(up-to-date)

	/***
	- masterKey : ������ ������ ���� ���ڵ��� Ű ��
	- transKey : Ʈ����� ������ ���� ���ڵ��� Ű ��

	1) �� ���Ͽ� ���� ��ϵ� ���� ��¥ ��
	2) ���ο� ������ ���� new_master.txt ����
	3) �� ������ �й� ������ ���������� ���� �Ǿ� �����Ƿ� ������ ���� �˰��� ���� ���ο� ������ ���Ͽ� �����Ѵ�

		3-1) masterKey < transKey
			Ʈ����� ������ Ű ���� ũ�ٴ� ���� ���� ������ ���Ͽ� ������ ���ڵ尡 ���ٴ� ���̹Ƿ� ������ ���ڵ带 ���ο� ������ ȭ�Ϸ� �����ϰ�, ���� ������ ���ڵ带 �о�´�
			masterKey++

		3-2) masterKey == transKey
			Ʈ����� ���ڵ��� �����ڵ忡 ����

			- ����(C) : ���� ������ ���ڵ��� ������ Ʈ����� ���ڵ��� �������� ���ο� ������ ���Ͽ� ���, ���� �����Ϳ� Ʈ����� ���ڵ带 �о�´�
			copy trans_record to new_master
			masterKey++
			transKEy++

			- ����(D) : ���� ������ ���ڵ� ����, �ƹ��� ó���� ���� �ʰ�, ���� �����Ϳ� Ʈ����� ���ڵ带 �о�´�
			masterKey++
			transKey++

			- ����(I) : ������ ���Ͽ� �̹� ���� Ű ���� ���� ���ڵ尡 �����Ƿ� �ߺ� ���ڵ� ����, ���� Ʈ����� ���ڵ带 �о��
			output error_log
			transKey++

		3-3) masterKey > transKey
			Ʈ����� ���ڵ忡 ��ġ�ϴ� ������ ���ڵ尡 ���� ���

			- ����(C) : ��ġ�ϴ� ������ ���ڵ尡 �����Ƿ� ���� ���, ���� Ʈ����� ���ڵ带 �о��
			output error_log
			transKey++

			- ����(D) : ��ġ�ϴ� ������ ���ڵ尡 �����Ƿ� ���� ���, ���� Ʈ����� ���ڵ带 �о��
			output error_log
			transKey++

			- ����(I) : ���ο� ������ ���Ϸ� Ʈ����� ���ڵ带 ����, ���� Ʈ����� ���ڵ带 �о��
			copy trans_record to new_master
			transKey++

	4) ���� ������ ���� master.txt�� new_master.txt�� ��ü

	�� �߰����� ���� ó�� ���� :
		- masterKey�� ���� �����Ͽ����� transKey�� ���� ���� �������� ���� ���, �����ִ� Ʈ����� ���ڵ���� ���Կ����� �ƴ� ��� ����
		- ���� ������ ���� �� Ʈ����� ���ڵ忡 ���� �ߺ��� �й��� ���ڵ� �߻� ������ Edit ���� ������ ���� �Էµ��� �ʵ��� �Ͽ����Ƿ� �߻����� ����

		��, ����ڰ� ���Ƿ� ������ ���� �� Ʈ����� ������ ����
			1) �й� �� ��, Ű ���� ���ĵ��� ���� ���·� �Է��ϴ� ���
			2) ���ڵ��� �� ������(�����ڵ�, �й�, �̸�, �а�, �г�, ����)�� ���� ���������� �ʰ��Ͽ� �Է��ϴ� ���
			3) ���ڵ��� �� �����Ϳ� ���� ��ȿ���� �ʴ� ��(�߸��� �����ڵ�, ��Ͽ� ���� �а���, ������ �ʰ��� �г� �Ǵ� ����)�� �Է��ϴ� ���

		�� ���ؼ��� ���� ó�� �Լ��� ����� ��� �� �����͵鿡 ���ؼ� Edit ���� ���θ� �Ǵ� �� �ش� �����͸� ���� �Ǵ� �����ϵ��� �� �� ������,
		���� ���� ������ Ư�� ��, ��� ���ڵ�鿡 ���� ���������� ����, �� ���Ƿ� ������ ���� ����,
		Ʈ����� ������ ���� ������ �����ϸ� ���� ������ ���ڵ�� ���Ͽ� ���ο� ������ ���Ͽ� ����ϴ� ������ ������ �����Ƿ�,
		�� �κп� ���ؼ��� �� ���α׷��� ���ؼ��� ����ڰ� ���ڵ带 �Է��Ѵٴ� ���� �Ͽ����� ��������Ƿ�,
		�ش� ���� ó���� �� ���α׷��� ������ ���� �ʴ´ٰ� �Ǵ��Ͽ� �������� �ʾ���
	***/

	/***
	Ʈ����� ���� ���� �Լ��� �Ѱܾ� �ϴ� ����
	1) ������ ���Ϸκ��� �о���� masterKey��
	2) Ʈ����� ���Ϸκ��� �о���� transKey��
	2) Ʈ����� ���Ϸκ��� �о���� �����ڵ� ��
	***/

	char masterKey[ID_LENGTH + 1] = { 0, }; //������ ������ �й� ��
	char transKey[ID_LENGTH + 1] = { 0, }; //Ʈ����� ������ �й� ��
	OPERATION_CODE trans_op_code = OPERATION_CODE::no_operation; //Ʈ����� ������ �����ڵ�

	//get_current_time() buffer size : 80byte
	char master_renewal_date[80] = { 0, };
	char trans_renewal_date[80] = { 0, };

	char read_buffer[SINGLE_LINE_BUFFER_SIZE] = { 0, }; //���Ͽ������� �о�ͼ� �Ҵ��ϴ� ����

	bool flag_no_error = true; //Ʈ����� ���� �˻縦 ���� ����� ���� ���� �÷���
	
	/**
		������ ���� �� ���� �÷���
		flag_read_next_masterKey : ���� ������ Ű ���� �о�´�
		flag_read_next_transKey : ���� Ʈ����� Ű ���� �о�´�
		�� ������ ���� �� �� ������� ���� ����
	**/
	bool flag_read_next_masterKey = true;
	bool flag_read_next_transKey = true;

	/**
		flag_read_next_master_line : ���� ������ ���ڵ带 ���� �� �ٹٲ�
		flag_read_next_trans_line : ���� Ʈ����� ���ڵ带 ���� �� �ٹٲ�

		���� ���� ���� �ÿ� ���� Ű ���� ���� �� ���� ���� ������ ����
		�����͸� ���� ���� ���� �����Ϳ� ���ؼ��� ���� Ű�� �б� ���ؼ� �ٹٲ��� �ؾ� �Ѵ�.
		Delete ������ ��� �й��� �Է¹޾����Ƿ� �ٹٲ��� �ϸ� �ȵ�
	**/
	bool flag_read_next_master_line = false;
	bool flag_read_next_trans_line = false;
	/**
		flag_eof_master : ������ ������ ��
		flag_eof_trans : Ʈ����� ������ ��
		
		- �ش� ������ �� �̻� ���� �� ���� ��� ����
		- ��, �ش� ���Ͽ� ���ؼ� ���� ���ڵ带 �о�鿴�� �� �� �̻� ���� �� ���� ���
		- �����Ϳ� Ʈ����� ��� ������ ���� �����ϸ� while���� ����
	**/
	bool flag_eof_master = false;
	bool flag_eof_trans = false;
	
	if ((master_fp = fopen("master.txt", "rt")) != NULL && (trans_fp = fopen("trans.txt", "rt")) != NULL
		&& (new_master_fp = fopen("new_master.txt", "wt")) != NULL)
	{ //������ ���� �б� ���, Ʈ����� ���� �б� ���, ���ο� ������ ���� ���� ���

		//������ ���ϰ� Ʈ����� ������ ���� ��¥ ��
		fscanf(master_fp, "%s\n", master_renewal_date);
		fscanf(trans_fp, "%s\n", trans_renewal_date);

		if (strcmp(master_renewal_date, trans_renewal_date) < 0) //������ ������ Ʈ����� ���Ϻ��� ������ ������ ���(A < B)
		{
			fprintf(new_master_fp, "%s\n", get_current_time().c_str()); //���ο� ������ ���Ͽ� �ý��� ���� �ð��� �����Ͽ� ���

			while (1)
			{
				memset(read_buffer, NULL, sizeof(read_buffer)); //���� �ʱ�ȭ

				/*** ���� ���� �÷��� ���� ���� ������ ���� �б� ***/
				if (flag_read_next_masterKey == true && flag_eof_master != true)
				{
					if (flag_read_next_master_line == true) //���� ���ڵ带 �а��� ���� ���ڵ��� ����
					{
						fgets(read_buffer, SINGLE_LINE_BUFFER_SIZE, master_fp); //���๮�ڰ� ���� �� ���� �д´�
						memset(read_buffer, NULL, sizeof(read_buffer)); //���� �ʱ�ȭ
					}

					fscanf(master_fp, "%s ", masterKey); //������ Ű �� �о��

					if (feof(master_fp))
					{
						flag_eof_master = true; //�б� ����(������ ���� ���)
						memset(masterKey, NULL, sizeof(masterKey)); //������ Ű �� �ʱ�ȭ
						strcpy(masterKey, EOF_KEY_VALUE); //������ ���� ��Ÿ���� ������ ���� ����(� ���ڵ��� Ű ������ �׻� ũ��)
					}
				}

				/*** ���� ���� �÷��� ���� ���� Ʈ����� ���� �б� ***/
				if (flag_read_next_transKey == true && flag_eof_trans != true)
				{
					if (flag_read_next_trans_line == true) //���� ���ڵ带 �а��� ���� ���ڵ��� ����
					{
						fgets(read_buffer, SINGLE_LINE_BUFFER_SIZE, trans_fp); //���๮�ڰ� ���� �� ���� �д´�
						memset(read_buffer, NULL, sizeof(read_buffer)); //���� �ʱ�ȭ
					}
					
					fscanf(trans_fp, "%s %c ", transKey, &trans_op_code); //Ʈ����� Ű�� ���� �ڵ尪 �о��

					if (feof(trans_fp))
					{
						flag_eof_trans = true; //�б� ����(������ ���� ���)
						memset(transKey, NULL, sizeof(transKey)); //Ʈ����� Ű �� �ʱ�ȭ
						strcpy(transKey, EOF_KEY_VALUE); //������ ���� ��Ÿ���� ������ ���� ����(� ���ڵ��� Ű ������ �׻� ũ��)
					}
				}

				if (flag_eof_master == true && flag_eof_trans == true) //�� �� ������ ���� �����ϸ� ���� ����
					break;

				flag_no_error = chk_trans_error(masterKey, transKey, trans_op_code); //Ʈ����� ���� ����
			
				//���� : ������ ���� ��Ÿ���� EOF�� � ���ڵ� Ű ������ ũ��

				if (strcmp(masterKey, EOF_KEY_VALUE) == 0) //������ ���� �� ����
				{
					//������ Ű ���� Ʈ����� Ű ������ ũ��
					switch (flag_no_error)
					{
					case true: //���ο� ������ ���Ϸ� Ʈ����� ���ڵ带 ����, ���� Ʈ����� ���ڵ带 �о��
						fgets(read_buffer, SINGLE_LINE_BUFFER_SIZE, trans_fp); //���๮�ڰ� ���� �� ���� �д´�
						read_buffer[strlen(read_buffer) - 1] = '\0'; //������ ���๮�� ����
						fprintf(new_master_fp, "%s %s\n", transKey, read_buffer); //���� Ʈ����� Ű ���� read_buffer�� ���ļ� new_master�� ���
						flag_read_next_masterKey = false;
						flag_read_next_transKey = true;

						//���� Ʈ����� Ű �� ���� �ÿ� �ٹٲ� �ʿ����
						flag_read_next_master_line = false;
						flag_read_next_trans_line = false;
						break;

					case false: //��ġ�ϴ� ������ ���ڵ尡 �����Ƿ� ���� ���, ���� Ʈ����� ���ڵ带 �о��
						flag_read_next_masterKey = false;
						flag_read_next_transKey = true;

						//Delete ������ �ƴ� ��� �����ڵ� ������ �����Ͱ� �����Ƿ� ���� ���ڵ带 �б����� �ٹٲ�
						flag_read_next_master_line = false;
						if (trans_op_code == OPERATION_CODE::Delete)
							flag_read_next_trans_line = false;
						else flag_read_next_trans_line = true;
						break;
					}

					continue; //���� while�� ó�� �ǳʶ�

				}
				else if (strcmp(transKey, EOF_KEY_VALUE) == 0) //Ʈ����� ���� �� ����
				{
					//Ʈ����� Ű ���� ������ Ű ������ ũ��
					//���ο� ������ ���Ͽ� ���� ������ ������ ���ڵ带 ����, ���� ������ ���ڵ带 �о��
					fgets(read_buffer, SINGLE_LINE_BUFFER_SIZE, master_fp); //���๮�ڰ� ���� �� ���� �д´�
					read_buffer[strlen(read_buffer) - 1] = '\0'; //������ ���๮�� ����
					fprintf(new_master_fp, "%s %s\n", masterKey, read_buffer); //���� ������ Ű ���� read_buffer�� ���ļ� new_master�� ���
					flag_read_next_masterKey = true;
					flag_read_next_transKey = false;

					//���� ������ Ű �� ���� �ÿ� �ٹٲ� �ʿ� ����
					flag_read_next_master_line = false;
					flag_read_next_trans_line = false;

					continue; //���� while�� ó�� �ǳʶ�
				}

				if (strcmp(masterKey, transKey) < 0) //masterKey < transKey
				{
					fgets(read_buffer, SINGLE_LINE_BUFFER_SIZE, master_fp); //���๮�ڰ� ���� �� ���� �д´�
					read_buffer[strlen(read_buffer) - 1] = '\0'; //������ ���๮�� ����
					fprintf(new_master_fp, "%s %s\n", masterKey, read_buffer); //���� ������ Ű ���� read_buffer�� ���ļ� new_master�� ���
					flag_read_next_masterKey = true;
					flag_read_next_transKey = false;

					//���� ������ Ű �� ���� �ÿ� �ٹٲ� �ʿ� ����
					flag_read_next_master_line = false;
					flag_read_next_trans_line = false;
				}
				else if (strcmp(masterKey, transKey) == 0) //masterKey == transKey
				{
					switch (flag_no_error)
					{
					case true: //Delete, Correct
						if (trans_op_code == OPERATION_CODE::Delete) //���� ������ ���ڵ� ����, �ƹ��� ó���� ���� �ʰ�, ���� �����Ϳ� Ʈ����� ���ڵ带 �о��
						{
							flag_read_next_masterKey = true;
							flag_read_next_transKey = true;

							//���� ������ Ű �� ���� �ÿ� �ٹٲ� �ؾ� ��
							flag_read_next_master_line = true;
							flag_read_next_trans_line = false;
						}
						else //Correct, ���� ������ ���ڵ��� ������ Ʈ����� ���ڵ��� �������� ���ο� ������ ���Ͽ� ���, ���� �����Ϳ� Ʈ����� ���ڵ带 �о��
						{
							fgets(read_buffer, SINGLE_LINE_BUFFER_SIZE, trans_fp); //���๮�ڰ� ���� �� ���� �д´�
							read_buffer[strlen(read_buffer) - 1] = '\0'; //������ ���๮�� ����
							fprintf(new_master_fp, "%s %s\n", transKey, read_buffer); //���� Ʈ����� Ű ���� read_buffer�� ���ļ� new_master�� ���
							flag_read_next_masterKey = true;
							flag_read_next_transKey = true;

							//���� ������ Ű �� ���� �ÿ� �ٹٲ� �ؾ� ��
							flag_read_next_master_line = true;
							flag_read_next_trans_line = false;
						}
						break;

					case false: //������ ���Ͽ� �̹� ���� Ű ���� ���� ���ڵ尡 �����Ƿ� �ߺ� ���ڵ� ����, ���� Ʈ����� ���ڵ带 �о��
						flag_read_next_masterKey = false;
						flag_read_next_transKey = true;

						//Delete ������ �ƴ� ��� �����ڵ� ������ �����Ͱ� �����Ƿ� ���� ���ڵ带 �б����� �ٹٲ�
						flag_read_next_master_line = false;
						if (trans_op_code == OPERATION_CODE::Delete)
							flag_read_next_trans_line = false;
						else flag_read_next_trans_line = true;

						break;
					}
				}
				else //masterKey > transKey(Ʈ����� ���ڵ忡 ��ġ�ϴ� ������ ���ڵ尡 ���� ���)
				{
					switch (flag_no_error)
					{
					case true: //���ο� ������ ���Ϸ� Ʈ����� ���ڵ带 ����, ���� Ʈ����� ���ڵ带 �о��
						fgets(read_buffer, SINGLE_LINE_BUFFER_SIZE, trans_fp); //���๮�ڰ� ���� �� ���� �д´�
						read_buffer[strlen(read_buffer) - 1] = '\0'; //������ ���๮�� ����
						fprintf(new_master_fp, "%s %s\n", transKey, read_buffer); //���� Ʈ����� Ű ���� read_buffer�� ���ļ� new_master�� ���
						flag_read_next_masterKey = false;
						flag_read_next_transKey = true;

						//���� Ʈ����� Ű �� ���� �ÿ� �ٹٲ� �ʿ����
						flag_read_next_master_line = false;
						flag_read_next_trans_line = false;
						break;

					case false: //��ġ�ϴ� ������ ���ڵ尡 �����Ƿ� ���� ���, ���� Ʈ����� ���ڵ带 �о��
						flag_read_next_masterKey = false;
						flag_read_next_transKey = true;

						//Delete ������ �ƴ� ��� �����ڵ� ������ �����Ͱ� �����Ƿ� ���� ���ڵ带 �б����� �ٹٲ�
						flag_read_next_master_line = false;
						if (trans_op_code == OPERATION_CODE::Delete)
							flag_read_next_trans_line = false;
						else flag_read_next_trans_line = true;
						break;
					}
				}
			}

		}
		else
		{
			std::cout << "������ ���� ���� ���� : " << master_renewal_date << std::endl;
			std::cout << "Ʈ����� ���� ���� ���� : " << trans_renewal_date << std::endl;
			disp_error_msg("������ ������ Ʈ����� ���Ϻ��� �ֽŹ����Դϴ�.");
			return;
		}
	}
	else
	{
		if (master_fp == NULL)
			disp_error_msg("������ ������ �������� �ʽ��ϴ�.");
		if (trans_fp == NULL)
			disp_error_msg("Ʈ����� ������ �������� �ʽ��ϴ�.");
		if (new_master_fp == NULL)
			disp_error_msg("���ο� ������ ������ �ۼ� �� �� �����ϴ�.");

		return;
	}

	if (master_fp != NULL)
		fclose(master_fp);
	if (trans_fp != NULL)
		fclose(trans_fp);
	if (new_master_fp != NULL)
		fclose(new_master_fp);
	
	switch (flag_replace_master) //���� ������ ������ ���ο� ������ ���Ϸ� ��ü
	{
	case true: //��ü
		remove("master.txt");
		rename("new_master.txt", "master.txt");
		break;
	case false: //��ü���� ����
		break;
	}

	std::cout << "������ ���� ���� ����" << std::endl;
	return;
}