#include "header.h"

/* 
	- 학생 객체 생성, 파일 기록을 위한 학생 정보 이중 연결리스트 생성 및 삽입 정렬 정의
	- 학생 리스트의 타입(배열, 이중 연결 리스트)에 따른 메모리 해제 함수 정의
	- 입력받은 학생 데이터로부터 Edit 오류 검출 완료 후 학생 객체 생성, 학번 순으로 정렬하여 리스트 생성 후 파일을 기록하는 file_func에 전달
*/

Student_string_data::Student_string_data() //입력값 길이 제한 검사를 위한 학생 데이터 문자열 클래스
{
	this->clear_all();
}

void Student_string_data::clear_all() //모든 문자열의 내용 초기화
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
	strcpy(this->Student_ID, NULL_VALUE); //학번
	strcpy(this->name, NULL_VALUE); //이름
	strcpy(this->department, NULL_VALUE); //학과
	this->grade = EXCEPTION_HANDLING_VALUE; //학년
	this->score = EXCEPTION_HANDLING_VALUE; //점수
}

Student::Student(const char* ID, const char* name, const char* dpt, int grade, int score)
{
	strcpy(this->Student_ID, ID); //학번
	
	//트랜잭션 파일 작성 시 삭제 연산(D) 시에는 이름, 학과, 학년, 점수를 입력 받지 않음
	//예외처리 값(EXCEPTION_HANDLING_VALUE, NULL_VALUE)을 주어 빈 값으로 생성
	if (name != NULL_VALUE && dpt != NULL_VALUE && grade != EXCEPTION_HANDLING_VALUE && score != EXCEPTION_HANDLING_VALUE)
	{
		strcpy(this->name, name); //이름
		strcpy(this->department, dpt); //학과
		this->grade = grade; //학년
		this->score = score; //점수
	}
	else //빈 값 할당
	{
		strcpy(this->name, NULL_VALUE); //이름
		strcpy(this->department, NULL_VALUE); //학과
		this->grade = EXCEPTION_HANDLING_VALUE; //학년
		this->score = EXCEPTION_HANDLING_VALUE; //점수
	}
}

void Student::get_Student_ID(char* dst_buffer) //학번 반환
{
	strcpy(dst_buffer,this->Student_ID);
}

Student_list::Student_list()
{
	this->op_code = OPERATION_CODE::no_operation; //연산코드 Enum 값 할당

	this->next = NULL;
	this->pre = NULL;
}

Student_list::Student_list(const char* op_code, const char* ID, const char* name, const char* dpt, int grade, int score)
{
	strcpy(this->Student_ID, ID); //학번
	strcpy(this->name, name); //이름
	strcpy(this->department, dpt); //학과
	this->grade = grade; //학년
	this->score = score; //점수

	if (op_code != NULL_VALUE) //연산코드 Enum 값 할당
	{
		if (strcmp(op_code, "I") == 0) //Insert
			this->op_code = OPERATION_CODE::Insert;
		else if (strcmp(op_code, "D") == 0) //Delete
			this->op_code = OPERATION_CODE::Delete;
		else //Correct
			 this->op_code = OPERATION_CODE::Correct;
	}
	else //빈 값 할당
		this->op_code = OPERATION_CODE::no_operation;

	this->next = NULL;
	this->pre = NULL;
}

bool chk_dpt_list(char** dpt_list_array, int dpt_list_count, char* user_input_dpt) //사용자로부터의 학과 입력 값 검사 함수
{
	for (int i = 0; i < dpt_list_count; i++)
	{
		if (strcmp((const char*)dpt_list_array[i], user_input_dpt) == 0) //일치하면
		{
			return true; //입력 값에 해당하는 학과 이름 존재
		}
	}
	return false; //입력 값에 해당하는 학과 이름 존재하지 않음
}

void create_student_list(Student_list** src_list, char** dpt_list_array, int dpt_list_count, bool flag_use_opcode) //사용자로부터 학생 데이터를 입력받고 Edit 오류 검출 실행 후 데이터를 insertion_sort 전달
{
	//flag_use_opcode : true - 연산코드를 포함한 학생 리스트 생성, false - 학생 리스트 생성
	
	char op_code[OP_LENGTH + 1] = { 0, }; //연산 코드(Insert, Delete, Correct)('\0' 제외 1자리)
	char student_id[ID_LENGTH + 1] = { 0, }; //학번(숫자,문자 조합, '\0' 제외 8자리)
	char name[NAME_LENGTH + 1] = { 0, }; //이름 ('\0' 제외 10자리)
	char dpt[DPT_LENGTH + 1] = { 0, }; //학과 ('\0' 제외 2자리)
	int grade = EXCEPTION_HANDLING_VALUE;  //학년
	int score = EXCEPTION_HANDLING_VALUE; //점수

	std::string user_input; //사용자로부터 명령어를 한 줄로 입력받는 변수
	Student_string_data* string_data = new Student_string_data(); //사용자로부터 받은 값들을 string으로 분리하여 edit 오류 검사 후 할당

	//출력 화면 지정
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
		string_data->clear_all(); //다음 번 입력 시를 위해서 초기화

		std::getline(std::cin, user_input); //한 줄로 입력받기
		std::stringstream ss(user_input); //분리

		switch (flag_use_opcode) //입력 값 및 반환 값 지정
		{
		case true: //연산코드를 포함 학생리스트 생성(트랜잭션 파일 작성)
			ss >> string_data->str_op_code; //연산코드 분리
			
			if (string_data->str_op_code.compare("-1") == 0)
			{	
				delete(string_data);
				return;
			}
			else if (string_data->str_op_code.compare("D") == 0) //삭제 연산 시 학번만 필요
			{
				ss >> string_data->str_student_id;
				if (chk_edit_error(src_list, &string_data, dpt_list_array, dpt_list_count) == true) //edit 오류 검사 및 로그 기록
				{
					strcpy(op_code, string_data->str_op_code.c_str()); //입력한 문자열에 이상이 없으면 복사
					strcpy(student_id, string_data->str_student_id.c_str()); //입력한 문자열에 이상이 없으면 복사
					//edit 오류가 검출되지 않았을 경우 새로운 학생 객체를 생성하여 리스트에 정렬된 위치에 삽입
					insertion_sort(src_list, op_code, student_id, NULL_VALUE, NULL_VALUE, EXCEPTION_HANDLING_VALUE, EXCEPTION_HANDLING_VALUE);
				}
				break;
			}
			else //삽입, 수정 연산 또는 다른 잘못된 입력(EDit 오류 검사을 통해 검출)
			{
				ss >> string_data->str_student_id; //학번 분리
				ss >> string_data->str_name; //이름 분리
				ss >> string_data->str_dpt; //학과 분리
				ss >> string_data->str_grade; //학년 분리
				ss >> string_data->str_score; //점수 분리

				if (chk_edit_error(src_list, &string_data, dpt_list_array, dpt_list_count) == true) //edit 오류 검사 및 로그 기록
				{
					strcpy(op_code, string_data->str_op_code.c_str()); //입력한 문자열에 이상이 없으면 복사
					strcpy(student_id, string_data->str_student_id.c_str()); //입력한 문자열에 이상이 없으면 복사
					strcpy(name, string_data->str_name.c_str()); //입력한 문자열에 이상이 없으면 복사
					strcpy(dpt, string_data->str_dpt.c_str()); //입력한 문자열에 이상이 없으면 복사
					grade = atoi(string_data->str_grade.c_str()); //입력한 문자열에 이상이 없으면 복사
					score = atoi(string_data->str_score.c_str()); //입력한 문자열에 이상이 없으면 복사

					//edit 오류가 검출되지 않았을 경우 새로운 학생 객체를 생성하여 리스트에 정렬된 위치에 삽입
					insertion_sort(src_list, op_code, student_id, name, dpt, grade, score);
				}
				break;
			}
		
		case false: //학생 리스트 생성
			ss >> string_data->str_student_id; //학번 분리

			if (string_data->str_student_id.compare("-1") == 0)
			{
				delete(string_data);
				return;
			}
			ss >> string_data->str_name; //이름 분리
			ss >> string_data->str_dpt; //학과 분리
			ss >> string_data->str_grade; //학년 분리
			ss >> string_data->str_score; //점수 분리

			if (chk_edit_error(src_list, &string_data, dpt_list_array, dpt_list_count) == true) //edit 오류 검사 및 로그 기록
			{
				strcpy(student_id, string_data->str_student_id.c_str()); //입력한 문자열에 이상이 없으면 복사
				strcpy(name, string_data->str_name.c_str()); //입력한 문자열에 이상이 없으면 복사
				strcpy(dpt, string_data->str_dpt.c_str()); //입력한 문자열에 이상이 없으면 복사
				grade = atoi(string_data->str_grade.c_str()); //입력한 문자열에 이상이 없으면 복사
				score = atoi(string_data->str_score.c_str()); //입력한 문자열에 이상이 없으면 복사

				//edit 오류가 검출되지 않았을 경우 새로운 학생 객체를 생성하여 리스트에 정렬된 위치에 삽입
				insertion_sort(src_list, NULL_VALUE, student_id, name, dpt, grade, score);
			}
			break;

		}
	}
	
	delete(string_data);
}

void insertion_sort(Student_list** src_list, const char* op_code, const char* ID, const char* name, const char* dpt, int grade, int score)  //Edit 오류 검출을 마친 데이터들로부터 새로운 학생 객체를 생성 및 삽입정렬하여 리스트 생성
{
	Student_list* new_student = NULL; //리스트에 연결할 새로운 학생 정보 생성
	Student_list* tmp_student_head = NULL; //학번 순으로 정렬을 위해 리스트의 헤드 노드를 저장

	int student_counter = 0; //입력받아 생성된 학생 수
	char current_buffer_id[ID_LENGTH + 1] = { 0, }; //기존 학생 노드의 학번 값 임시저장
	char new_buffer_id[ID_LENGTH + 1] = { 0, }; //새로운 학생 노드의 학번 값 임시저장

	//오류가 없을 시 기록을 위한 학생 정보 리스트에 학번 순으로 추가
	if (*src_list == NULL) //초기 노드 생성
	{
		(*src_list) = new Student_list(op_code, ID, name, dpt, grade, score);
	}
	else
	{
		new_student = new Student_list(op_code, ID, name, dpt, grade, score); //리스트에 삽입할 새로운 학생 정보
		tmp_student_head = (*src_list); //기존 리스트의 첫 번째 학생 정보 임시저장

		while (1) //리스트의 끝까지 수행, 학번 순으로 삽입 정렬하여 리스트를 만든다
		{
			//다음 번 입력을 위한 버퍼 초기화
			memset(current_buffer_id, NULL, sizeof(current_buffer_id));
			memset(new_buffer_id, NULL, sizeof(new_buffer_id));

			(*src_list)->get_Student_ID(current_buffer_id); //기존 학생 노드의 학번 값 버퍼 할당
			new_student->get_Student_ID(new_buffer_id); //새로운 학생 노드의 학번 값 버퍼 할당

			if (strcmp(new_buffer_id, current_buffer_id) < 0) //strcmp(A,B) < 0 => A < B
			{
				//새로 입력하고자 하는 노드의 키 값이 기존 노드보다 작은 수 일 경우 앞에 기록
				new_student->next = (*src_list);
				if ((*src_list)->pre != NULL) //현재 노드의 이전 노드가 존재하면
				{
					new_student->pre = (*src_list)->pre;
					(*src_list)->pre->next = new_student;
				}
				(*src_list)->pre = new_student;

				if (new_student->pre == NULL) //새로운 학생 정보가 삽입된 위치가 맨 처음인 경우
					(*src_list) = new_student; //학생 정보 리스트가 새로운 학생 정보 노드를 가리키도록 변경
				else //새로운 학생 정보가 중앙에 삽입된 경우
					(*src_list) = tmp_student_head; //기존 헤드로 다시 변경하여 리스트가 맨 처음을 가리키도록 함
				break;
			}

			if ((*src_list)->next != NULL)
				(*src_list) = (*src_list)->next;
			else
				break;
		} //리스트 끝까지 수행 완료

		//노드 끝까지 비교하여 삽입이 되지 않은 경우 맨 뒤에 삽입
		if (new_student->next == NULL && new_student->pre == NULL)
		{
			new_student->pre = (*src_list);
			(*src_list)->next = new_student;
			(*src_list) = tmp_student_head; //기존 헤드로 다시 변경하여 리스트가 맨 처음을 가리키도록 함
		}
	}
}

void deallocate_memory(Student_list** student_list, char** dpt_list_array, int dpt_list_count, int type) //메모리 해제
{
	//type : 0 - Dynamic Array, 1 - Double Linked List
	//type에 따른 메모리 해제
	Student_list* student_list_indexer = NULL; //원본 리스트 인덱서
	Student_list* del_this = NULL; //student_list_indexer로부터 할당하여 순차적으로 해제

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
			student_list_indexer = student_list_indexer->next; //인덱서를 이동 후 삭제
			delete(del_this);
		}
		(*student_list) = NULL; //링크 초기화

		break;

	default:
		break;
	}
}