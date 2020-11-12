#include "header.h"

// 화면 입/출력 함수 정의

void disp_main_menu() //메인 메뉴 출력
{
	system("cls");
	std::cout << "==============================================================" << std::endl;
	std::cout << "	    Sequential File Structure Simulation" << std::endl;
	std::cout << "==============================================================" << std::endl;
	std::cout << " 1 : 초기 마스터 파일 생성" << std::endl;
	std::cout << " 2 : 마스터 파일 출력" << std::endl;
	std::cout << "--------------------------------------------------------------" << std::endl;
	std::cout << " 3 : 트랜잭션 파일 작성" << std::endl;
	std::cout << " 4 : 트랜잭션 파일 출력" << std::endl;
	std::cout << "--------------------------------------------------------------" << std::endl;
	//std::cout << "학번 순 정렬 & 데이터 오류 검출(Manually)" << std::endl;
	std::cout << " 5 : 마스터 파일 갱신 (Trans -> Master)" << std::endl;
	std::cout << "--------------------------------------------------------------" << std::endl;
	std::cout << " 6 : View Error Log" << std::endl;
	std::cout << " 7 : Exit" << std::endl;
	std::cout << "==============================================================" << std::endl;
	std::cout << ">>";
}

void disp_student_description_bar(int file_mode) //학생 정보 설명 출력
{
	system("cls");
	switch (file_mode)
	{
	case 0: //마스터 파일
		std::cout << "==============================================================" << std::endl;
		std::cout << "학번 ||  이름  ||  학과  ||  학년  ||  점수" << std::endl;
		std::cout << "==============================================================" << std::endl;
		break;
	case 1: //트랜잭션 파일
		std::cout << "==============================================================" << std::endl;
		std::cout << "학번 ||  연산코드 ||  이름  ||  학과  ||  학년  ||  점수" << std::endl;
		std::cout << "==============================================================" << std::endl;
		break;
	default:
		break;
	}
}

void disp_student_trans_input_bar(char** dpt_list_array, int dpt_list_count) //트랜잭션 파일 작성 시 입력해야 할 데이터 출력
{
	system("cls");
	std::cout << "==============================================================" << std::endl;
	std::cout << "Insert(I) : 삽입 연산 => I, 학번, 이름, 학과, 학년, 점수 순으로 입력" << std::endl;
	std::cout << "Delete(D) : 삭제 연산 => D, 학번 순으로 입력" << std::endl;
	std::cout << "Correct(C) : 수정 연산 => C, 학번, 이름, 학과, 학년, 점수 순으로 입력" << std::endl;
	std::cout << "--------------------------------------------------------------" << std::endl;
	std::cout << "※ 학번 글자 수 : " << ID_LENGTH << std::endl;
	std::cout << "※ 이름 글자 수 : " << NAME_LENGTH << std::endl;
	std::cout << "※ 학과 글자 수 : " << DPT_LENGTH << std::endl;
	std::cout << "※ 학년 범위 : 1 ~ 4" << std::endl;
	std::cout << "※ 점수 범위 : 1 ~ 100" << std::endl;
	std::cout << "--------------------------------------------------------------" << std::endl;
	std::cout << "※ 학과 목록 : ";

	for (int i = 0; i < dpt_list_count; i++) //학과 목록 출력
	{
		if (i == dpt_list_count - 1)
			std::cout << dpt_list_array[i] << std::endl;
		else
			std::cout << dpt_list_array[i] << ",";
		//printf("%02d: %s\n", i, dpt_list_array[i]);
	}
	std::cout << "--------------------------------------------------------------" << std::endl;
	std::cout << " 입력 종료 : 연산 코드 위치에 -1 입력" << std::endl;
	std::cout << "==============================================================" << std::endl;
	std::cout << ">>";
}

void disp_student_input_bar(char** dpt_list_array, int dpt_list_count) //마스터 파일 작성 시 입력해야 할 데이터 출력
{
	system("cls");
	std::cout << "==============================================================" << std::endl;
	std::cout << "※ 학번, 이름, 학과, 학년, 점수 순으로 입력" << std::endl;
	std::cout << "※ 학번 글자 수 : " << ID_LENGTH << std::endl;
	std::cout << "※ 이름 글자 수 : " << NAME_LENGTH << std::endl;
	std::cout << "※ 학과 글자 수 : " << DPT_LENGTH << std::endl;
	std::cout << "※ 학년 범위 : 1 ~ 4" << std::endl;
	std::cout << "※ 점수 범위 : 1 ~ 100" << std::endl;
	std::cout << "--------------------------------------------------------------" << std::endl;
	std::cout << "※ 학과 목록 : ";
	
	for (int i = 0; i < dpt_list_count; i++) //학과 목록 출력
	{
		if (i == dpt_list_count - 1)
			std::cout << dpt_list_array[i] << std::endl;
		else
			std::cout << dpt_list_array[i] << ",";
		//printf("%02d: %s\n", i, dpt_list_array[i]);
	}
	std::cout << "--------------------------------------------------------------" << std::endl;
	std::cout << " 입력 종료 : 학번 위치에 -1 입력" << std::endl;
	std::cout << "==============================================================" << std::endl;
	std::cout << ">>";
}

void disp_log_description_bar() //로그 타입에 따른 설명
{
	system("cls");
	std::cout << "==============================================================" << std::endl;
	std::cout << "ERROR_TYPE ||		 시간		||	 내용" << std::endl;
	std::cout << "==============================================================" << std::endl;
}

void disp_trans_log_description_bar() //로그 타입에 따른 설명
{
	system("cls");
	std::cout << "=============================================================================" << std::endl;
	std::cout << " ERROR_TYPE  ||  시간  ||  연산 코드  || Master Key || Trans Key ||  내용" << std::endl;
	std::cout << "=============================================================================" << std::endl;
}

void disp_error_msg(const char* message) //오류 메세지 출력
{
	std::cout << "오류 : " << message << std::endl;
}

int input_log_file_mode() //로그 출력을 위한 로그 타입 입력
{
	//file_mode : 0 - 마스터 파일, 1 - 트랜잭션 파일, 2 - Edit 오류 로그, 3 - Transaction 오류 로그
	while (1)
	{
		system("cls");
		std::cout << "==============================================================" << std::endl;
		std::cout << " 1 : Edit 오류 로그 출력" << std::endl;
		std::cout << " 0 : Transaction 오류 로그 출력" << std::endl;
		std::cout << "--------------------------------------------------------------" << std::endl;
		std::cout << " -1 : return" << std::endl;
		std::cout << "==============================================================" << std::endl;
		std::cout << ">>";

		int user_input = 0;
		std::cin >> user_input;
		
		if (user_input != -1 && user_input != 0 && user_input != 1)
		{
			std::cin.clear(); //오류스트림 초기화
			std::cin.ignore(INT_MAX, '\n'); //입력버퍼비우기
		}
		else
		{
			if (user_input == 1) return 2; //print_file(2) 호출
			if (user_input == 0) return 3; //print_file(1) 호출
		}
	}
}

bool input_init_master_confirm() //초기 마스터 파일 생성 확인
{
	while (1)
	{
		system("cls");
		std::cout << "==============================================================" << std::endl;
		std::cout << "기존 마스터 파일 삭제 후 새로운 마스터 파일을 생성합니다." << std::endl;
		std::cout << " 1 : continue" << std::endl;
		std::cout << " 0 : return" << std::endl;
		std::cout << "==============================================================" << std::endl;
		std::cout << ">>";

		int user_input = 0;
		std::cin >> user_input;
		if (user_input == 0)
		{
			std::cin.clear(); //오류스트림 초기화
			std::cin.ignore(INT_MAX, '\n'); //입력버퍼비우기

			return false;
			break;
		}
		else if (user_input < 0 || user_input > 1)
		{
			std::cin.clear(); //오류스트림 초기화
			std::cin.ignore(INT_MAX, '\n'); //입력버퍼비우기
		}
		else
		{
			std::cin.clear(); //오류스트림 초기화
			std::cin.ignore(INT_MAX, '\n'); //입력버퍼비우기

			return true;
			break;
		}
	}
}

bool input_replace_master_confirm() //마스터 파일 갱신 대체 확인
{
	while (1)
	{
		system("cls");
		std::cout << "==============================================================" << std::endl;
		std::cout << "기존 마스터 파일을 갱신 후 새로운 마스터 파일로 대체하시겠습니까?" << std::endl;
		std::cout << "기존 마스터 파일 : master.txt" << std::endl;
		std::cout << "새 마스터 파일 : new_master.txt" << std::endl;
		std::cout << "--------------------------------------------------------------" << std::endl;
		std::cout << " 2 : 갱신하여 새로운 마스터 파일 생성만 수행(new_master.txt)" << std::endl;
		std::cout << " 1 : 갱신 및 기존 마스터 파일 대체(new_master.txt -> master.txt)" << std::endl;
		std::cout << " 0 : return" << std::endl;
		std::cout << "==============================================================" << std::endl;
		std::cout << ">>";

		int user_input = 0;
		std::cin >> user_input;

		if (user_input == 2) //갱신만
		{
			std::cin.clear(); //오류스트림 초기화
			std::cin.ignore(INT_MAX, '\n'); //입력버퍼비우기

			return false;
			break;
		}
		else if (user_input == 1) //갱신 및 대체
		{
			std::cin.clear(); //오류스트림 초기화
			std::cin.ignore(INT_MAX, '\n'); //입력버퍼비우기
			
			return true;
			break;
		}
		else
		{
			std::cin.clear(); //오류스트림 초기화
			std::cin.ignore(INT_MAX, '\n'); //입력버퍼비우기
		}
	}
}

void input_main_menu(Student_list** src_list, char** dpt_list_array, int dpt_list_count) //메인 메뉴에 대한 사용자 입력
{
	while (1)
	{
		int user_input = 0; //사용자 입력 값

		/***debug***/
		Student_list* tmp_student_head = NULL; //기존 리스트의 첫 번째 학생 정보 임시저장
		char buffer_id[9] = { 0, };
		/***debug***/

		disp_main_menu();
		std::cin >> user_input;

		std::cin.clear(); //오류스트림 초기화
		std::cin.ignore(INT_MAX, '\n'); //입력버퍼비우기

		switch (user_input)
		{
		case EXCEPTION_HANDLING_VALUE: //debug
			system("cls");
			(*src_list) = new Student_list("D", "12345", "KIM", "DP", 1, 100); //add 			
		
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

		case 1: //초기 마스터 파일 생성
			create_file(src_list, dpt_list_array, dpt_list_count, 0);
			system("pause");
			break;

		case 2: //마스터 파일 출력
			print_file(0);
			system("pause");
			break;

		case 3: //트랜잭션 파일 작성
			create_file(src_list, dpt_list_array, dpt_list_count, 1);
			system("pause");
			break;

		case 4: //트랜잭션 파일 출력
			print_file(1);
			system("pause");
			break;

		case 5: //마스터 파일 갱신
			update_master(input_replace_master_confirm());
			system("pause");
			break;

		case 6: //로그 파일 출력
			print_file(input_log_file_mode());
			system("pause");
			break;

		case 7: //exit
			deallocate_memory(src_list, dpt_list_array, dpt_list_count, 1); //학생 리스트 메모리 해제
			deallocate_memory(src_list, dpt_list_array, dpt_list_count, 0); //학과 배열 메모리 해제
			exit(1);
				
		default:
			continue;
		}

	}
}
