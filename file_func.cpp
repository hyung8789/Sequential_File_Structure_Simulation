#include "header.h"

/* 
	- 학과 리스트 갱신, 마스터, 트랜잭션, 로그 파일 처리 및 출력, 마스터 파일 갱신 함수 정의
	- 생성 및 정렬된 학생 객체 리스트를 파일에 기록 후 리스트 폐기
*/

char** allocate_dpt_list_array(int& dpt_list_count) //학과 리스트 파일로부터 동적 배열 할당
{
	//row : 파일로부터 읽어들인 학과 수(dpt_list_count), col : 학과 문자열 길이(DPT_LENGTH + 1)
	
	FILE* dpt_list_fp = NULL;

	char** dpt_list_array = NULL; //학과 리스트로부터 동적할당하여 리턴
	char dpt_buffer[SINGLE_LINE_BUFFER_SIZE] = { 0, }; //리스트에 등록된 학과 수를 파악 및 파일로부터 학과를 임시저장하는 버퍼
	int i = 0; //학과 배열 할당 인덱스

	if ((dpt_list_fp = fopen("dpt_list.txt", "rt")) != NULL) //성공적으로 open 시
	{
		while (!feof(dpt_list_fp)) //파일의 끝까지 
		{
			fgets(dpt_buffer, SINGLE_LINE_BUFFER_SIZE, dpt_list_fp); //개행문자가 나올 때 까지 읽는다
			memset(dpt_buffer, NULL, sizeof(dpt_buffer)); //버퍼 초기화
			dpt_list_count++; //리스트의 학과 개수 카운팅
		}

		if (dpt_list_count > 0) //리스트에 등록된 학과가 존재하면
		{
			dpt_list_array = new char* [dpt_list_count]; //학과 수 만큼의 공간 할당(row)

			for (int i = 0; i < dpt_list_count; i++) //각각의 공간에 대해 다시 char형 배열 할당(col)
			{
				dpt_list_array[i] = new char[DPT_LENGTH + 1]; //학과 문자 길이만큼의 배열 할당(\0 포함)
			}
		}
		else
		{
			disp_error_msg("학과 리스트가 비어있습니다.");
			if (dpt_list_fp != NULL)
				fclose(dpt_list_fp);
			return NULL;
		}

		rewind(dpt_list_fp); //학과 리스트를 다시 읽어들여서 배열에 할당한다
		while (!feof(dpt_list_fp) && i < dpt_list_count) //파일의 끝까지
		{
			fscanf(dpt_list_fp, "%s\n", dpt_buffer);
			strcpy(dpt_list_array[i++], dpt_buffer);
			memset(dpt_buffer, NULL, sizeof(dpt_buffer)); //버퍼 초기화
		}
		fclose(dpt_list_fp);

		return dpt_list_array;
	}
	else
	{
		disp_error_msg("학과 리스트 파일(dpt_list.txt)이 존재하지 않습니다.");
		return NULL;
	}
}

bool chk_edit_error(Student_list** src_list, Student_string_data** src_data, char** dpt_list_array, int dpt_list_count) //Editing 오류 검출 및 로그 기록 함수
{
	FILE* logger = NULL;

	Student_list* student_list_indexer = NULL; //동일한 학번 검사를 위해 인덱싱
	char buffer_id[9] = { 0, }; //학번 값 임시저장
	bool flag_no_error = true; //오류 플래그

	if ((logger = fopen("error_log.log", "at")) != NULL) //append mode
	{
		//연산코드, 학번, 이름, 학과, 학년, 점수 오류 검출
		//인수로 받은 학생 리스트로부터 동일 학번 검사
		//사용자로부터 받은 입력 값(src_data)을 양식에 맞게 입력하였는지 검사
		
		//마스터 파일 및 트랜잭션 파일 작성 시 삭제연산을 제외하고 반드시 공통으로 학번, 이름, 학과, 학년, 점수를 입력받아야함 
		//트랜잭션 파일 작성 시 삭제 연산 시에는 이름, 학과, 학년, 점수를 입력 받지 않음

		/*** 동일 학번에 대한 예외처리 및 공백 입력에 대한 예외처리 ***/
		if ((*src_data)->str_student_id.empty() != true) //동일 학번 검사
		{
			if (*src_list != NULL)
			{
				for (student_list_indexer = (*src_list); student_list_indexer != NULL; student_list_indexer = student_list_indexer->next)
				{
					student_list_indexer->get_Student_ID(buffer_id); //학번 값 버퍼 할당
					if ((*src_data)->str_student_id.compare(buffer_id) == 0) //일치하면
					{
						fprintf(logger, "edit_error || %s || 학번 '%s'가 이미 존재합니다.\n", get_current_time().c_str(), (*src_data)->str_student_id.c_str());
						flag_no_error = false;
					}
				}
			}
		}
		else
		{
			fprintf(logger, "edit_error || %s || 학번이 공백입니다.\n", get_current_time().c_str());
			flag_no_error = false;
		}
		/*** 동일 학번에 대한 예외처리 및 공백 입력에 대한 예외처리 끝 ***/

		/*** 연산 코드에 대한 예외처리 ***/
		//마스터 파일 생성 시에는 연산코드를 입력받지 않으므로 연산 코드는 공백으로 처리
		if ((*src_data)->str_op_code.empty() != true) //연산 코드 검사
		{
			if((*src_data)->str_op_code.compare("I") != 0 && (*src_data)->str_op_code.compare("D") != 0 && (*src_data)->str_op_code.compare("C") != 0)
			{
				fprintf(logger, "edit_error || %s || '%s'는 잘못 된 연산 코드 입니다.\n", get_current_time().c_str(), (*src_data)->str_op_code.c_str());
				flag_no_error = false;
			}
		}
		/*** 연산 코드에 대한 예외처리 끝 ***/

		/*** 학번에 대한 예외처리 ***/
		if ((*src_data)->str_student_id.empty() != true)
		{
			if ((*src_data)->str_student_id.length() > ID_LENGTH)
			{
				fprintf(logger, "edit_error || %s || 학번 '%s'가 최대길이 '%d'를 넘었습니다.\n", get_current_time().c_str(), (*src_data)->str_student_id.c_str(), ID_LENGTH);
				flag_no_error = false;
			}
		}
		else //학번이 공백
		{
			fprintf(logger, "edit_error || %s || 학번이 공백입니다.\n", get_current_time().c_str());
			flag_no_error = false;
		}
		/*** 학번에 대한 예외처리 끝 ***/

		/*** 이름에 대한 예외처리 ***/
		if ((*src_data)->str_op_code.empty() != true && (*src_data)->str_op_code.compare("D") != 0) //트랜잭션 파일 작성 과정, 연산코드가 공백이 아니고 삭제 연산이 아닌 경우
		{
			if ((*src_data)->str_name.empty() != true) //이름이 공백이 아니고, 연산코드가 공백이 아닌 경우, 즉 트랜잭션 파일 작성 중
			{
				if ((*src_data)->str_name.length() > NAME_LENGTH)
				{
					fprintf(logger, "edit_error || %s || 이름 '%s'가 최대길이 '%d'를 넘었습니다.\n", get_current_time().c_str(), (*src_data)->str_name.c_str(), NAME_LENGTH);
					flag_no_error = false;
				}
			}
		}

		if ((*src_data)->str_op_code.empty() != true && (*src_data)->str_op_code.compare("D") == 0) //삭제 연산인 경우 사용자가 값을 입력하면 오류
		{
			if ((*src_data)->str_name.empty() != true)
			{
				fprintf(logger, "edit_error || %s || 이름 입력 오류 : 삭제 연산시에 학번만 입력해야합니다.\n", get_current_time().c_str());
				flag_no_error = false;
			}
		}
		/*** 이름에 대한 예외처리 끝 ***/

		/*** 학과명에 대한 예외처리 ***/
		if ((*src_data)->str_dpt.empty() != true && (*src_data)->str_op_code.compare("D") != 0) //학과명이 비어있지 않고, 삭제 연산이 아닌 경우
		{
			if ((*src_data)->str_dpt.length() > DPT_LENGTH || chk_dpt_list(dpt_list_array, dpt_list_count, (char*)(*src_data)->str_dpt.c_str()) == false) //입력한 학과가 존재하지 않으면
			{
				fprintf(logger, "edit_error || %s || '%s'는 등록된 학과 이름이 아니거나 최대길이 '%d'를 초과하였습니다.\n", get_current_time().c_str(), (*src_data)->str_dpt.c_str(), DPT_LENGTH);
				flag_no_error = false;
			}
		}
		if ((*src_data)->str_op_code.empty() != true && (*src_data)->str_op_code.compare("D") == 0) //삭제 연산인 경우
		{
			if ((*src_data)->str_dpt.empty() != true)
			{
				fprintf(logger, "edit_error || %s || 학과 명 입력 오류 : 삭제 연산시에 학번만 입력해야합니다.\n", get_current_time().c_str());
				flag_no_error = false;
			}
		}
		/*** 학과명에 대한 예외처리 끝 ***/

		/*** 학년에 대한 예외처리 ***/
		if ((*src_data)->str_grade.empty() != true && (*src_data)->str_op_code.compare("D") != 0) //학년이 비어있지 않고, 삭제 연산이 아닌 경우
		{
			if (atoi((*src_data)->str_grade.c_str()) < 1 || atoi((*src_data)->str_grade.c_str()) > 4) //학년 범위를 벗어나면
			{
				fprintf(logger, "edit_error || %s || '%s'는 유효한 학년 범위가 아닙니다.\n", get_current_time().c_str(), (*src_data)->str_grade.c_str());
				flag_no_error = false;
			}
		}
		if ((*src_data)->str_op_code.empty() != true && (*src_data)->str_op_code.compare("D") == 0) //삭제 연산인 경우
		{
			if ((*src_data)->str_grade.empty() != true)
			{
				fprintf(logger, "edit_error || %s || 학년 입력 오류 : 삭제 연산시에 학번만 입력해야합니다.\n", get_current_time().c_str());
				flag_no_error = false;
			}
		}
		/*** 학년에 대한 예외처리 끝 ***/

		/*** 점수에 대한 예외처리 ***/
		if ((*src_data)->str_score.empty() != true && (*src_data)->str_op_code.compare("D") != 0) //점수가 비어있지 않고, 삭제 연산이 아닌 경우
		{
			if (atoi((*src_data)->str_score.c_str()) < 0 || atoi((*src_data)->str_score.c_str()) > 100) //점수 범위를 벗어나면
			{
				fprintf(logger, "edit_error || %s || '%s'는 유효한 점수 범위가 아닙니다.\n", get_current_time().c_str(), (*src_data)->str_score.c_str());
				flag_no_error = false;
			}
		}
		if ((*src_data)->str_op_code.empty() != true && (*src_data)->str_op_code.compare("D") == 0) //삭제 연산인 경우
		{
			if ((*src_data)->str_score.empty() != true)
			{
				fprintf(logger, "edit_error || %s || 점수 입력 오류 : 삭제 연산시에 학번만 입력해야합니다.\n", get_current_time().c_str());
				flag_no_error = false;
			}
		}
		/*** 점수에 대한 예외처리 끝 ***/

		if (flag_no_error == true)
		{
			fclose(logger);
			return true; //오류가 검출되지 않음
		}
	}
	else
	{
		disp_error_msg("error_log.log 기록 불가능");
		return false;
	}
	
	fclose(logger);
	return false;
}

bool chk_trans_error(const char* masterKey, const char* transKey, OPERATION_CODE op_code) //Transaction 오류 검출 및 로그 기록 함수
{
	//correct, delete, insert

	FILE* logger = NULL;
	bool flag_no_error = true; //오류 플래그

	if ((logger = fopen("error_log.log", "at")) != NULL) //append mode
	{
		/***
		3 - 1) masterKey < transKey
			트랜잭션 파일의 키 값이 크다는 것은 기존 마스터 파일에 적용할 레코드가 없다는 것이므로 마스터 레코드를 새로운 마스터 화일로 복사하고, 다음 마스터 레코드를 읽어온다
			masterKey++

		3 - 2) masterKey == transKey
			트랜잭션 레코드의 갱신코드에 따라

			- 수정(C) : 기존 마스터 레코드의 내용을 트랜잭션 레코드의 내용으로 새로운 마스터 파일에 기록, 다음 마스터와 트랜잭션 레코드를 읽어온다
			copy trans_record to new_master
			masterKey++
			transKEy++

			- 삭제(D) : 기존 마스터 레코드 무시, 아무런 처리도 하지 않고, 다음 마스터와 트랜잭션 레코드를 읽어온다
			masterKey++
			transKey++

			- 삽입(I) : 마스터 파일에 이미 같은 키 값을 가진 레코드가 있으므로 중복 레코드 오류, 다음 트랜잭션 레코드를 읽어옴
			output error_log
			transKey++

		3 - 3) masterKey > transKey
			트랜잭션 레코드에 일치하는 마스터 레코드가 없는 경우

			- 수정(C) : 일치하는 마스터 레코드가 없으므로 오류 출력, 다음 트랜잭션 레코드를 읽어옴
			output error_log
			transKey++

			- 삭제(D) : 일치하는 마스터 레코드가 없으므로 오류 출력, 다음 트랜잭션 레코드를 읽어옴
			output error_log
			transKey++

			- 삽입(I) : 새로운 마스터 파일로 트랜잭션 레코드를 삽입, 다음 트랜잭션 레코드를 읽어옴
			copy trans_record to new_master
			transKey++
		***/

		//가정 : 파일의 끝을 나타내는 EOF는 어떤 레코드 키 값보다 크다
		
		if(strcmp(masterKey, EOF_KEY_VALUE) == 0)
		{
			//마스터 키 값은 트랜잭션 키 값보다 크다
			switch (op_code)
			{
			case OPERATION_CODE::Insert: //새로운 마스터 파일로 트랜잭션 레코드를 삽입, 다음 트랜잭션 레코드를 읽어옴
				flag_no_error = true;
				break;

			case OPERATION_CODE::Delete: //일치하는 마스터 레코드가 없으므로 오류 출력, 다음 트랜잭션 레코드를 읽어옴
				fprintf(logger, "trans_error || %s || Delete || masterKey : %s || transKey : %s || 일치하는 마스터 레코드가 존재하지 않습니다.\n", get_current_time().c_str(), masterKey, transKey);
				flag_no_error = false;
				break;

			case OPERATION_CODE::Correct: //일치하는 마스터 레코드가 없으므로 오류 출력, 다음 트랜잭션 레코드를 읽어옴
				fprintf(logger, "trans_error || %s || Correct || masterKey : %s || transKey : %s || 일치하는 마스터 레코드가 존재하지 않습니다.\n", get_current_time().c_str(), masterKey, transKey);
				flag_no_error = false;
				break;

			default: //오류
				disp_error_msg("연산코드가 할당되지 않았습니다. 트랜잭션 파일이 임의로 수정된 것 같습니다.");
				system("pause");
				exit(1);
				break;
			}
		}
		else if (strcmp(transKey, EOF_KEY_VALUE) == 0)
		{
			//트랜잭션 키 값은 마스터 키 값보다 크다
			//새로운 마스터 파일에 기존 마스터 파일의 레코드를 복사, 다음 마스터 레코드를 읽어옴
			flag_no_error = true;
		}
		else //아직 파일의 끝에 도달하지 않은 경우
		{
			if (strcmp(masterKey, transKey) < 0) //masterKey < transKey
			{
				switch (op_code)
				{
				case OPERATION_CODE::Insert: //새로운 마스터 파일에 기존 마스터 파일의 레코드를 복사, 다음 마스터 레코드를 읽어옴
					flag_no_error = true;
					break;

				case OPERATION_CODE::Delete: //마스터 레코드를 새로운 마스터 화일로 복사하고, 다음 마스터 레코드를 읽어옴
					flag_no_error = true;
					break;

				case OPERATION_CODE::Correct: //마스터 레코드를 새로운 마스터 화일로 복사하고, 다음 마스터 레코드를 읽어옴
					flag_no_error = true;
					break;

				default: //오류
					disp_error_msg("연산코드가 할당되지 않았습니다. 트랜잭션 파일이 임의로 수정된 것 같습니다.");
					system("pause");
					exit(1);
					break;
				}

			}
			else if (strcmp(masterKey, transKey) == 0) //masterKey == transKey
			{
				switch (op_code)
				{
				case OPERATION_CODE::Insert: //마스터 파일에 이미 같은 키 값을 가진 레코드가 있으므로 중복 레코드 오류, 다음 트랜잭션 레코드를 읽어옴
					fprintf(logger, "trans_error || %s || Insert || masterKey : %s || transKey : %s || 마스터 파일에 같은 키 값이 존재합니다.\n", get_current_time().c_str(), masterKey, transKey);
					flag_no_error = false;
					break;

				case OPERATION_CODE::Delete: //기존 마스터 레코드 무시, 아무런 처리도 하지 않고, 다음 마스터와 트랜잭션 레코드를 읽어옴
					flag_no_error = true;
					break;

				case OPERATION_CODE::Correct: //기존 마스터 레코드의 내용을 트랜잭션 레코드의 내용으로 새로운 마스터 파일에 기록, 다음 마스터와 트랜잭션 레코드를 읽어옴
					flag_no_error = true;
					break;

				default: //오류
					disp_error_msg("연산코드가 할당되지 않았습니다. 트랜잭션 파일이 임의로 수정된 것 같습니다.");
					system("pause");
					exit(1);
					break;
				}
			}
			else //masterKey > transKey(트랜잭션 레코드에 일치하는 마스터 레코드가 없는 경우)
			{
				switch (op_code)
				{
				case OPERATION_CODE::Insert: //새로운 마스터 파일로 트랜잭션 레코드를 삽입, 다음 트랜잭션 레코드를 읽어옴
					flag_no_error = true;
					break;

				case OPERATION_CODE::Delete: //일치하는 마스터 레코드가 없으므로 오류 출력, 다음 트랜잭션 레코드를 읽어옴
					fprintf(logger, "trans_error || %s || Delete || masterKey : %s || transKey : %s || 일치하는 마스터 레코드가 존재하지 않습니다.\n", get_current_time().c_str(), masterKey, transKey);
					flag_no_error = false;
					break;

				case OPERATION_CODE::Correct: //일치하는 마스터 레코드가 없으므로 오류 출력, 다음 트랜잭션 레코드를 읽어옴
					fprintf(logger, "trans_error || %s || Correct || masterKey : %s || transKey : %s || 일치하는 마스터 레코드가 존재하지 않습니다.\n", get_current_time().c_str(), masterKey, transKey);
					flag_no_error = false;
					break;

				default: //오류
					disp_error_msg("연산코드가 할당되지 않았습니다. 트랜잭션 파일이 임의로 수정된 것 같습니다.");
					system("pause");
					exit(1);
					break;
				}
			}
		}

		if (flag_no_error == true)
		{
			fclose(logger);
			return true; //오류가 검출되지 않음
		}
	}
	else
	{
		disp_error_msg("error_log.log 기록 불가능");
		return false;
	}

	fclose(logger);
	return false;
}

bool create_file(Student_list** src_list, char** dpt_list_array, int dpt_list_count, int file_mode) //초기 마스터 파일 생성 또는 트랜잭션 파일 작성 함수
{
	//file_mode : 0 - 마스터 파일, 1 - 트랜잭션 파일
	FILE* fp = NULL;
	Student_list* student_list_indexer = NULL; //마지막 노드와 첫 번째 노드는 연결하지 않았으므로 삭제를 위해서는 인덱싱을 따로 해야한다
	char buffer_id[9] = { 0, }; //학번 값 임시저장

	switch (file_mode)
	{
	case 0: //초기 마스터 파일 생성
		if ((fp = fopen("master.txt", "wt")) != NULL) //기존 마스터 파일이 존재할 경우
		{
			if (input_init_master_confirm() == true) //마스터 파일 초기화 수행 여부 확인
			{
				//마스터 파일 초기 생성
				create_student_list(src_list, dpt_list_array, dpt_list_count, false); //src_list를 넘겨 학생 리스트를 생성

				if (*src_list != NULL)
				{
					student_list_indexer = (*src_list);

					fprintf(fp, "%s\n", get_current_time().c_str()); //파일에 시스템 현재 시간을 포함하여 기록

					while (student_list_indexer != NULL)
					{
						student_list_indexer->get_Student_ID(buffer_id); //학번값 버퍼 할당
						fprintf(fp, "%s %s %s %d %d\n", buffer_id, student_list_indexer->name, student_list_indexer->department, student_list_indexer->grade, student_list_indexer->score);
						student_list_indexer = student_list_indexer->next;
					}

					deallocate_memory(src_list, dpt_list_array, dpt_list_count, 1); //학생 리스트 메모리 해제
					fclose(fp);

					return true;
				}
				break;
			}
			else break;
		}
		else
		{
			disp_error_msg("마스터 파일을 쓰기 모드로 열 수 없습니다.");
			break;
		}
		break;

	case 1: //트랜잭션 파일 작성
		if ((fp = fopen("trans.txt", "wt")) != NULL) //기존 트랜잭션 파일이 존재할 경우
		{
			create_student_list(src_list, dpt_list_array, dpt_list_count, true); //src_list를 넘겨 학생 리스트를 생성

			if (*src_list != NULL)
			{
				student_list_indexer = (*src_list);

				fprintf(fp, "%s\n", get_current_time().c_str()); //파일에 시스템 현재 시간을 포함하여 기록

				while (student_list_indexer != NULL)
				{
					student_list_indexer->get_Student_ID(buffer_id); //학번값 버퍼 할당

					if (student_list_indexer->op_code == OPERATION_CODE::Delete) //삭제 연산 시 학번 값만 입력
					{
						fprintf(fp, "%s %c\n", buffer_id, to_char(student_list_indexer->op_code)); //학번, 연산코드 순으로 기록
						student_list_indexer = student_list_indexer->next;
					}
					else
					{
						fprintf(fp, "%s %c %s %s %d %d\n", buffer_id, to_char(student_list_indexer->op_code), student_list_indexer->name, student_list_indexer->department, student_list_indexer->grade, student_list_indexer->score);
						student_list_indexer = student_list_indexer->next;
					}
				}

				deallocate_memory(src_list, dpt_list_array, dpt_list_count, 1); //학생 리스트 메모리 해제
				fclose(fp);

				return true;
			}
			break;
		}
		else
		{
			disp_error_msg("트랜잭션 파일을 쓰기 모드로 열 수 없습니다.");
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

void print_file(int file_mode) //마스터 파일 또는 트랜잭션 파일 출력 함수
{
	//file_mode : 0 - 마스터 파일, 1 - 트랜잭션 파일, 2 - Edit 오류 로그, 3 - Transaction 오류 로그
	FILE* fp = NULL;
	char read_buffer[SINGLE_LINE_BUFFER_SIZE] = { 0, };

	disp_student_description_bar(file_mode);

	switch (file_mode)
	{
	case 0: //마스터 파일
		if ((fp = fopen("master.txt", "r")) != NULL)
		{
			//마스터 파일 갱신 시간 출력
			fscanf(fp, "%s\n", read_buffer);
			printf("마스터 파일 갱신 시간 : %s\n", read_buffer);
			std::cout << "--------------------------------------------------------------" << std::endl;
			memset(read_buffer, NULL, sizeof(read_buffer));
		}
		else
		{
			disp_error_msg("마스터 파일이 존재하지 않습니다.");
			return;
			break;
		}
		break;

	case 1: //트랜잭션 파일
		if ((fp = fopen("trans.txt", "r")) != NULL)
		{
			//트랜잭션 파일 갱신 시간 출력
			fscanf(fp, "%s\n", read_buffer);
			printf("트랜잭션 파일 갱신 시간 : %s\n", read_buffer);
			std::cout << "--------------------------------------------------------------" << std::endl;
			memset(read_buffer, NULL, sizeof(read_buffer));
		}
		else
		{
			disp_error_msg("트랜잭션 파일이 존재하지 않습니다.");
			return;
			break;
		}
		break;

	case 2: //Edit 오류 로그
		//type == edit_error인 경우만 출력
		if ((fp = fopen("error_log.log", "r")) != NULL)
		{
			disp_log_description_bar();

			while (!feof(fp))
			{
				fscanf(fp, "%s", read_buffer); //한 문자열을 읽어들인다
				if (strcmp(read_buffer, "edit_error") == 0) //일치할 경우만
				{
					printf("%s", read_buffer);
					fgets(read_buffer, SINGLE_LINE_BUFFER_SIZE, fp); //개행 문자가 나올 때 까지
					std::cout << read_buffer;
					memset(read_buffer, NULL, sizeof(read_buffer));
				}
				else
				{
					//개행 문자가 나올 때 까지 읽어서 넘긴다
					fgets(read_buffer, SINGLE_LINE_BUFFER_SIZE, fp); //개행 문자가 나올 때 까지
					memset(read_buffer, NULL, sizeof(read_buffer));
				}
			}
			fclose(fp);
			return;
		}
		else
		{
			disp_error_msg("로그 파일이 존재하지 않습니다.");
			return;
			break;
		}
		break;

	case 3: //Transaction 오류 로그
		//type == trans_error인 경우만 출력
		if ((fp = fopen("error_log.log", "r")) != NULL)
		{
			disp_trans_log_description_bar();

			while (!feof(fp))
			{
				fscanf(fp, "%s", read_buffer); //한 문자열을 읽어들인다
				if (strcmp(read_buffer, "trans_error") == 0) //일치할 경우만
				{
					printf("%s", read_buffer);
					fgets(read_buffer, SINGLE_LINE_BUFFER_SIZE, fp); //개행 문자가 나올 때 까지
					std::cout << read_buffer;
					memset(read_buffer, NULL, sizeof(read_buffer));
				}
				else //개행 문자가 나올 때 까지 읽어서 넘긴다
				{
					fgets(read_buffer, SINGLE_LINE_BUFFER_SIZE, fp); //개행 문자가 나올 때 까지
					memset(read_buffer, NULL, sizeof(read_buffer));
				}
			}
			fclose(fp);
			return;
		}
		else
		{
			disp_error_msg("로그 파일이 존재하지 않습니다.");
			return;
			break;
		}
		break;

	default:
		return;
		break;
	}

	//마스터 파일 또는 트랜잭션 파일의 내용 출력
	while (!feof(fp))
	{
		fread(read_buffer, sizeof(char), sizeof(read_buffer) - 1, fp); //'\0' 기록할 공간 제외하여 사이즈를 1 작게
		printf("%s", read_buffer);
		memset(read_buffer, NULL, sizeof(read_buffer));
	}

	if(fp != NULL)
		fclose(fp);
	
	return;
}


void update_master(bool flag_replace_master) //마스터 파일 갱신 함수
{
	FILE* new_master_fp = NULL; //새로운 마스터 파일 포인터
	FILE* master_fp = NULL; //마스터 파일 포인터
	FILE* trans_fp = NULL; //트랜잭션 파일 포인터

	//두 파일의 갱신날짜를 비교하여 마스터 파일이 트랜잭션 파일보다 최신일 경우 갱신하지 않음(up-to-date)

	/***
	- masterKey : 마스터 파일의 현재 레코드의 키 값
	- transKey : 트랜잭션 파일의 현재 레코드의 키 값

	1) 각 파일에 대해 기록된 갱신 날짜 비교
	2) 새로운 마스터 파일 new_master.txt 생성
	3) 각 파일은 학번 순으로 순차적으로 정렬 되어 있으므로 다음과 같은 알고리즘에 따라 새로운 마스터 파일에 삽입한다

		3-1) masterKey < transKey
			트랜잭션 파일의 키 값이 크다는 것은 기존 마스터 파일에 적용할 레코드가 없다는 것이므로 마스터 레코드를 새로운 마스터 화일로 복사하고, 다음 마스터 레코드를 읽어온다
			masterKey++

		3-2) masterKey == transKey
			트랜잭션 레코드의 갱신코드에 따라

			- 수정(C) : 기존 마스터 레코드의 내용을 트랜잭션 레코드의 내용으로 새로운 마스터 파일에 기록, 다음 마스터와 트랜잭션 레코드를 읽어온다
			copy trans_record to new_master
			masterKey++
			transKEy++

			- 삭제(D) : 기존 마스터 레코드 무시, 아무런 처리도 하지 않고, 다음 마스터와 트랜잭션 레코드를 읽어온다
			masterKey++
			transKey++

			- 삽입(I) : 마스터 파일에 이미 같은 키 값을 가진 레코드가 있으므로 중복 레코드 오류, 다음 트랜잭션 레코드를 읽어옴
			output error_log
			transKey++

		3-3) masterKey > transKey
			트랜잭션 레코드에 일치하는 마스터 레코드가 없는 경우

			- 수정(C) : 일치하는 마스터 레코드가 없으므로 오류 출력, 다음 트랜잭션 레코드를 읽어옴
			output error_log
			transKey++

			- 삭제(D) : 일치하는 마스터 레코드가 없으므로 오류 출력, 다음 트랜잭션 레코드를 읽어옴
			output error_log
			transKey++

			- 삽입(I) : 새로운 마스터 파일로 트랜잭션 레코드를 삽입, 다음 트랜잭션 레코드를 읽어옴
			copy trans_record to new_master
			transKey++

	4) 기존 마스터 파일 master.txt을 new_master.txt로 대체

	※ 추가적인 오류 처리 사항 :
		- masterKey가 끝에 도달하였지만 transKey가 아직 끝에 도달하지 못한 경우, 남아있는 트랜잭션 레코드들이 삽입연산이 아닌 경우 오류
		- 기존 마스터 파일 및 트랜잭션 레코드에 대해 중복된 학번의 레코드 발생 여부은 Edit 오류 검출을 통해 입력되지 않도록 하였으므로 발생하지 않음

		단, 사용자가 임의로 마스터 파일 및 트랜잭션 파일을 열어
			1) 학번 순 즉, 키 값이 정렬되지 않은 상태로 입력하는 경우
			2) 레코드의 각 데이터(연산코드, 학번, 이름, 학과, 학년, 점수)에 대해 길이제한을 초과하여 입력하는 경우
			3) 레코드의 각 데이터에 대해 유효하지 않는 값(잘못된 연산코드, 목록에 없는 학과명, 범위를 초과한 학년 또는 점수)을 입력하는 경우

		에 대해서는 오류 처리 함수를 만들어 기록 된 데이터들에 대해서 Edit 오류 여부를 판단 후 해당 데이터를 수정 또는 삭제하도록 할 수 있지만,
		순차 파일 구조의 특성 상, 모든 레코드들에 대해 순차적으로 접근, 즉 임의로 접근할 수가 없고,
		트랜잭션 파일을 통해 연산을 결정하며 기존 마스터 레코드와 비교하여 새로운 마스터 파일에 기록하는 구조를 가지고 있으므로,
		이 부분에 대해서는 본 프로그램을 통해서만 사용자가 레코드를 입력한다는 가정 하에서만 만들었으므로,
		해당 오류 처리는 본 프로그램의 목적에 맞지 않는다고 판단하여 구현하지 않았음
	***/

	/***
	트랜잭션 오류 검출 함수에 넘겨야 하는 값들
	1) 마스터 파일로부터 읽어들인 masterKey값
	2) 트랜잭션 파일로부터 읽어들인 transKey값
	2) 트랜잭션 파일로부터 읽어들인 연산코드 값
	***/

	char masterKey[ID_LENGTH + 1] = { 0, }; //마스터 파일의 학번 값
	char transKey[ID_LENGTH + 1] = { 0, }; //트랜잭션 파일의 학번 값
	OPERATION_CODE trans_op_code = OPERATION_CODE::no_operation; //트랜잭션 파일의 연산코드

	//get_current_time() buffer size : 80byte
	char master_renewal_date[80] = { 0, };
	char trans_renewal_date[80] = { 0, };

	char read_buffer[SINGLE_LINE_BUFFER_SIZE] = { 0, }; //파일에서부터 읽어와서 할당하는 버퍼

	bool flag_no_error = true; //트랜잭션 오류 검사를 통해 결과를 받을 오류 플래그
	
	/**
		다음에 수행 할 연산 플래그
		flag_read_next_masterKey : 다음 마스터 키 값을 읽어온다
		flag_read_next_transKey : 다음 트랜잭션 키 값을 읽어온다
		매 연산이 수행 된 후 결과값에 따라 변동
	**/
	bool flag_read_next_masterKey = true;
	bool flag_read_next_transKey = true;

	/**
		flag_read_next_master_line : 다음 마스터 레코드를 읽을 때 줄바꿈
		flag_read_next_trans_line : 다음 트랜잭션 레코드를 읽을 때 줄바꿈

		이전 연산 수행 시에 다음 키 값을 읽을 때 다음 줄을 읽을지 설정
		데이터를 읽지 않은 파일 포인터에 대해서는 다음 키를 읽기 위해서 줄바꿈을 해야 한다.
		Delete 연산의 경우 학번만 입력받았으므로 줄바꿈을 하면 안됨
	**/
	bool flag_read_next_master_line = false;
	bool flag_read_next_trans_line = false;
	/**
		flag_eof_master : 마스터 파일의 끝
		flag_eof_trans : 트랜잭션 파일의 끝
		
		- 해당 파일을 더 이상 읽을 수 없을 경우 변경
		- 즉, 해당 파일에 대해서 다음 레코드를 읽어들였을 때 더 이상 읽을 수 없을 경우
		- 마스터와 트랜잭션 모두 파일의 끝에 도달하면 while연산 종료
	**/
	bool flag_eof_master = false;
	bool flag_eof_trans = false;
	
	if ((master_fp = fopen("master.txt", "rt")) != NULL && (trans_fp = fopen("trans.txt", "rt")) != NULL
		&& (new_master_fp = fopen("new_master.txt", "wt")) != NULL)
	{ //마스터 파일 읽기 모드, 트랜잭션 파일 읽기 모드, 새로운 마스터 파일 쓰기 모드

		//마스터 파일과 트랜잭션 파일의 갱신 날짜 비교
		fscanf(master_fp, "%s\n", master_renewal_date);
		fscanf(trans_fp, "%s\n", trans_renewal_date);

		if (strcmp(master_renewal_date, trans_renewal_date) < 0) //마스터 파일이 트랜잭션 파일보다 오래된 버전일 경우(A < B)
		{
			fprintf(new_master_fp, "%s\n", get_current_time().c_str()); //새로운 마스터 파일에 시스템 현재 시간을 포함하여 기록

			while (1)
			{
				memset(read_buffer, NULL, sizeof(read_buffer)); //버퍼 초기화

				/*** 다음 연산 플래그 값에 따른 마스터 파일 읽기 ***/
				if (flag_read_next_masterKey == true && flag_eof_master != true)
				{
					if (flag_read_next_master_line == true) //다음 레코드를 읽고자 이전 레코드을 무시
					{
						fgets(read_buffer, SINGLE_LINE_BUFFER_SIZE, master_fp); //개행문자가 나올 때 까지 읽는다
						memset(read_buffer, NULL, sizeof(read_buffer)); //버퍼 초기화
					}

					fscanf(master_fp, "%s ", masterKey); //마스터 키 값 읽어옴

					if (feof(master_fp))
					{
						flag_eof_master = true; //읽기 실패(파일의 끝일 경우)
						memset(masterKey, NULL, sizeof(masterKey)); //마스터 키 값 초기화
						strcpy(masterKey, EOF_KEY_VALUE); //파일의 끝을 나타내는 임의의 값을 대입(어떤 레코드의 키 값보다 항상 크다)
					}
				}

				/*** 다음 연산 플래그 값에 따른 트랜잭션 파일 읽기 ***/
				if (flag_read_next_transKey == true && flag_eof_trans != true)
				{
					if (flag_read_next_trans_line == true) //다음 레코드를 읽고자 이전 레코드을 무시
					{
						fgets(read_buffer, SINGLE_LINE_BUFFER_SIZE, trans_fp); //개행문자가 나올 때 까지 읽는다
						memset(read_buffer, NULL, sizeof(read_buffer)); //버퍼 초기화
					}
					
					fscanf(trans_fp, "%s %c ", transKey, &trans_op_code); //트랜잭션 키와 연산 코드값 읽어옴

					if (feof(trans_fp))
					{
						flag_eof_trans = true; //읽기 실패(파일의 끝일 경우)
						memset(transKey, NULL, sizeof(transKey)); //트랜잭션 키 값 초기화
						strcpy(transKey, EOF_KEY_VALUE); //파일의 끝을 나타내는 임의의 값을 대입(어떤 레코드의 키 값보다 항상 크다)
					}
				}

				if (flag_eof_master == true && flag_eof_trans == true) //둘 다 파일의 끝에 도달하면 연산 종료
					break;

				flag_no_error = chk_trans_error(masterKey, transKey, trans_op_code); //트랜잭션 오류 검출
			
				//가정 : 파일의 끝을 나타내는 EOF는 어떤 레코드 키 값보다 크다

				if (strcmp(masterKey, EOF_KEY_VALUE) == 0) //마스터 파일 끝 도달
				{
					//마스터 키 값은 트랜잭션 키 값보다 크다
					switch (flag_no_error)
					{
					case true: //새로운 마스터 파일로 트랜잭션 레코드를 삽입, 다음 트랜잭션 레코드를 읽어옴
						fgets(read_buffer, SINGLE_LINE_BUFFER_SIZE, trans_fp); //개행문자가 나올 때 까지 읽는다
						read_buffer[strlen(read_buffer) - 1] = '\0'; //버퍼의 개행문자 제거
						fprintf(new_master_fp, "%s %s\n", transKey, read_buffer); //현재 트랜잭션 키 값과 read_buffer를 합쳐서 new_master에 기록
						flag_read_next_masterKey = false;
						flag_read_next_transKey = true;

						//다음 트랜잭션 키 값 읽을 시에 줄바꿈 필요없음
						flag_read_next_master_line = false;
						flag_read_next_trans_line = false;
						break;

					case false: //일치하는 마스터 레코드가 없으므로 오류 출력, 다음 트랜잭션 레코드를 읽어옴
						flag_read_next_masterKey = false;
						flag_read_next_transKey = true;

						//Delete 연산이 아닌 경우 연산코드 다음에 데이터가 있으므로 다음 레코드를 읽기위해 줄바꿈
						flag_read_next_master_line = false;
						if (trans_op_code == OPERATION_CODE::Delete)
							flag_read_next_trans_line = false;
						else flag_read_next_trans_line = true;
						break;
					}

					continue; //현재 while문 처리 건너뜀

				}
				else if (strcmp(transKey, EOF_KEY_VALUE) == 0) //트랜잭션 파일 끝 도달
				{
					//트랜잭션 키 값은 마스터 키 값보다 크다
					//새로운 마스터 파일에 기존 마스터 파일의 레코드를 복사, 다음 마스터 레코드를 읽어옴
					fgets(read_buffer, SINGLE_LINE_BUFFER_SIZE, master_fp); //개행문자가 나올 때 까지 읽는다
					read_buffer[strlen(read_buffer) - 1] = '\0'; //버퍼의 개행문자 제거
					fprintf(new_master_fp, "%s %s\n", masterKey, read_buffer); //현재 마스터 키 값과 read_buffer를 합쳐서 new_master에 기록
					flag_read_next_masterKey = true;
					flag_read_next_transKey = false;

					//다음 마스터 키 값 읽을 시에 줄바꿈 필요 없음
					flag_read_next_master_line = false;
					flag_read_next_trans_line = false;

					continue; //현재 while문 처리 건너뜀
				}

				if (strcmp(masterKey, transKey) < 0) //masterKey < transKey
				{
					fgets(read_buffer, SINGLE_LINE_BUFFER_SIZE, master_fp); //개행문자가 나올 때 까지 읽는다
					read_buffer[strlen(read_buffer) - 1] = '\0'; //버퍼의 개행문자 제거
					fprintf(new_master_fp, "%s %s\n", masterKey, read_buffer); //현재 마스터 키 값과 read_buffer를 합쳐서 new_master에 기록
					flag_read_next_masterKey = true;
					flag_read_next_transKey = false;

					//다음 마스터 키 값 읽을 시에 줄바꿈 필요 없음
					flag_read_next_master_line = false;
					flag_read_next_trans_line = false;
				}
				else if (strcmp(masterKey, transKey) == 0) //masterKey == transKey
				{
					switch (flag_no_error)
					{
					case true: //Delete, Correct
						if (trans_op_code == OPERATION_CODE::Delete) //기존 마스터 레코드 무시, 아무런 처리도 하지 않고, 다음 마스터와 트랜잭션 레코드를 읽어옴
						{
							flag_read_next_masterKey = true;
							flag_read_next_transKey = true;

							//다음 마스터 키 값 읽을 시에 줄바꿈 해야 함
							flag_read_next_master_line = true;
							flag_read_next_trans_line = false;
						}
						else //Correct, 기존 마스터 레코드의 내용을 트랜잭션 레코드의 내용으로 새로운 마스터 파일에 기록, 다음 마스터와 트랜잭션 레코드를 읽어옴
						{
							fgets(read_buffer, SINGLE_LINE_BUFFER_SIZE, trans_fp); //개행문자가 나올 때 까지 읽는다
							read_buffer[strlen(read_buffer) - 1] = '\0'; //버퍼의 개행문자 제거
							fprintf(new_master_fp, "%s %s\n", transKey, read_buffer); //현재 트랜잭션 키 값과 read_buffer를 합쳐서 new_master에 기록
							flag_read_next_masterKey = true;
							flag_read_next_transKey = true;

							//다음 마스터 키 값 읽을 시에 줄바꿈 해야 함
							flag_read_next_master_line = true;
							flag_read_next_trans_line = false;
						}
						break;

					case false: //마스터 파일에 이미 같은 키 값을 가진 레코드가 있으므로 중복 레코드 오류, 다음 트랜잭션 레코드를 읽어옴
						flag_read_next_masterKey = false;
						flag_read_next_transKey = true;

						//Delete 연산이 아닌 경우 연산코드 다음에 데이터가 있으므로 다음 레코드를 읽기위해 줄바꿈
						flag_read_next_master_line = false;
						if (trans_op_code == OPERATION_CODE::Delete)
							flag_read_next_trans_line = false;
						else flag_read_next_trans_line = true;

						break;
					}
				}
				else //masterKey > transKey(트랜잭션 레코드에 일치하는 마스터 레코드가 없는 경우)
				{
					switch (flag_no_error)
					{
					case true: //새로운 마스터 파일로 트랜잭션 레코드를 삽입, 다음 트랜잭션 레코드를 읽어옴
						fgets(read_buffer, SINGLE_LINE_BUFFER_SIZE, trans_fp); //개행문자가 나올 때 까지 읽는다
						read_buffer[strlen(read_buffer) - 1] = '\0'; //버퍼의 개행문자 제거
						fprintf(new_master_fp, "%s %s\n", transKey, read_buffer); //현재 트랜잭션 키 값과 read_buffer를 합쳐서 new_master에 기록
						flag_read_next_masterKey = false;
						flag_read_next_transKey = true;

						//다음 트랜잭션 키 값 읽을 시에 줄바꿈 필요없음
						flag_read_next_master_line = false;
						flag_read_next_trans_line = false;
						break;

					case false: //일치하는 마스터 레코드가 없으므로 오류 출력, 다음 트랜잭션 레코드를 읽어옴
						flag_read_next_masterKey = false;
						flag_read_next_transKey = true;

						//Delete 연산이 아닌 경우 연산코드 다음에 데이터가 있으므로 다음 레코드를 읽기위해 줄바꿈
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
			std::cout << "마스터 파일 갱신 일자 : " << master_renewal_date << std::endl;
			std::cout << "트랜잭션 파일 갱신 일자 : " << trans_renewal_date << std::endl;
			disp_error_msg("마스터 파일이 트랜잭션 파일보다 최신버전입니다.");
			return;
		}
	}
	else
	{
		if (master_fp == NULL)
			disp_error_msg("마스터 파일이 존재하지 않습니다.");
		if (trans_fp == NULL)
			disp_error_msg("트랜잭션 파일이 존재하지 않습니다.");
		if (new_master_fp == NULL)
			disp_error_msg("새로운 마스터 파일을 작성 할 수 없습니다.");

		return;
	}

	if (master_fp != NULL)
		fclose(master_fp);
	if (trans_fp != NULL)
		fclose(trans_fp);
	if (new_master_fp != NULL)
		fclose(new_master_fp);
	
	switch (flag_replace_master) //기존 마스터 파일을 새로운 마스터 파일로 대체
	{
	case true: //대체
		remove("master.txt");
		rename("new_master.txt", "master.txt");
		break;
	case false: //대체하지 않음
		break;
	}

	std::cout << "마스터 파일 갱신 성공" << std::endl;
	return;
}