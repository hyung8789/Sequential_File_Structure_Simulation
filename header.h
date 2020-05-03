#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h> //fscanf,fprintf,fwrite,fread
#include <stdlib.h> //standard lib
#include <iostream> //입출력
#include <string.h> //strcpy
#include <sstream> //stringstream
#include <windows.h> //sys command
#include <time.h> //time
#include <stdbool.h> //boolean
#include <type_traits> //std::underlying_type

//NULL_VALUE : 연산코드, 학번, 이름, 학과에 대해 값을 할당하지 않기 위한 문자 상수 선언
//EOF_KEY_VALUE : (가정)파일의 끝을 나타내는 EOF는 어떤 레코드 키 값보다 크다
constexpr auto NULL_VALUE = "";
constexpr auto EOF_KEY_VALUE = "-1"; //파일의 끝일 경우 레코드 키에 할당하는 값

#define EXCEPTION_HANDLING_VALUE -999 //학년, 점수에 나올 수 없는 예외 값 선언
#define SINGLE_LINE_BUFFER_SIZE 512 //효율성을 위해 한 줄 읽을 때 버퍼 크기를 byte단위의 블록 경계치 크기로 설정

#define OP_LENGTH 1 //연산 코드 문자 길이
#define DPT_LENGTH 2 //학과 문자 길이
#define NAME_LENGTH 10 //이름 문자 길이
#define ID_LENGTH 8 //학번 문자 길이

//Insert, Delete, Correct 열거형 선언
enum class OPERATION_CODE : char{
	//학생 객체 생성 시에 연산코드를 정의된 OPERATION_CODE중에 할당
	no_operation = NULL,
	Insert = 'I',
	Delete = 'D',
	Correct = 'C'
};

//열거형 => char 변환 탬플릿
template<typename Enum>
constexpr auto to_char(Enum value) -> typename std::underlying_type<Enum>::type
{
	return static_cast<typename std::underlying_type<Enum>::type>(value);
};

class Student_string_data //입력값 검사를 위한 학생 데이터 문자열 클래스
{
public:
	Student_string_data();

	std::string str_op_code; //연산코드
	std::string str_student_id; //학번
	std::string str_name; //이름
	std::string str_dpt; //학과
	std::string str_grade; //학년
	std::string str_score; //점수

	void clear_all(); //모든 문자열의 내용 삭제
};

class Student
{
public:
	//EDIT 오류 검출을 마친 후에 생성

	Student();
	Student(const char* ID, const char* name, const char* dpt, int grade, int score);
	
	char name[NAME_LENGTH + 1]; //이름 ('\0' 제외 10자리)
	char department[DPT_LENGTH + 1]; //학과 ('\0' 제외 2자리)
	int grade; //학년(1 ~ 4)
	int score; //점수(0 ~ 100)

	void get_Student_ID(char* dst_buffer); //dst_buffer에 학번 반환

protected: //학번은 사전 순 비교를 위해 숫자,문자로 구성된 고정된 길이의 값
	char Student_ID[ID_LENGTH + 1]; //학번(숫자,문자 조합, '\0' 제외 8자리)
};

class Student_list : public Student //파일 기록을 위한 학생 정보 이중 연결리스트 및 연산 코드를 포함
{
public:
	Student_list();
	Student_list(const char* op_code, const char* ID, const char* name, const char* dpt, int grade, int score);
	
	enum OPERATION_CODE op_code; //연산 코드(Insert, Delete, Correct)

	class Student_list* next; //다음 학생 정보
	class Student_list* pre; //이전 학생 정보
};

/*** student function ***/
bool chk_dpt_list(char** dpt_list_array, int dpt_list_count, char* user_input_dpt); //사용자로부터의 학과 입력 값 검사 함수
void insertion_sort(Student_list** src_list, const char* op_code, const char* ID, const char* name, const char* dpt, int grade, int score); //Edit 오류 검출을 마친 데이터들로부터 새로운 학생 객체를 생성 및 삽입정렬하여 리스트 생성
void create_student_list(Student_list** src_list, char** dpt_list_array, int dpt_list_count, bool flag_use_opcode); //사용자로부터 학생 데이터를 입력받고 Edit 오류 검출 실행 후 데이터를 insertion_sort 전달
void deallocate_memory(Student_list** student_list, char** dpt_list_array, int dpt_list_count, int type); //메모리 해제

/*** time function ***/
const std::string get_current_time(); //현재 시스템 시간 반환 함수

/*** screen I/O function ***/
//flag_use_opcode : true - 연산코드를 포함한 학생 리스트 생성, false - 학생 리스트 생성
void disp_main_menu(); //메인 메뉴 출력
void disp_student_description_bar(int file_mode); //학생 정보 설명 출력
void disp_student_trans_input_bar(char** dpt_list_array, int dpt_list_count); //트랜잭션 파일 작성 시 입력해야 할 데이터 출력
void disp_student_input_bar(char** dpt_list_array, int dpt_list_count); //마스터 파일 작성 시 입력해야 할 데이터 출력
void disp_log_description_bar(); //로그 타입에 따른 설명
void disp_trans_log_description_bar(); //로그 타입에 따른 설명
void disp_error_msg(const char* message); //오류 메세지 출력
int input_log_file_mode(); //로그 출력을 위한 로그 타입 입력
bool input_replace_master_confirm(); //마스터 파일 갱신 대체 확인
bool input_init_master_confirm(); //초기 마스터 파일 생성 확인
void input_main_menu(Student_list** src_list, char** dpt_list_array, int dpt_list_count); //메인 메뉴에 대한 사용자 입력

/*** file function : 학생 정보 입력, 마스터 파일 업데이트 시 호출 ***/
//file_mode : 0 - 마스터 파일, 1 - 트랜잭션 파일, 2 - Edit 오류 로그, 3 - Transaction 오류 로그
bool chk_edit_error(Student_list** src_list, Student_string_data** src_data, char** dpt_list_array, int dpt_list_count); //Editing 오류 검출 및 로그 기록 함수
bool chk_trans_error(const char* masterKey, const char* transKey, const char* op_code); //Transaction 오류 검출 및 로그 기록 함수
//file function : 메인에서 호출
char** allocate_dpt_list_array(int& dpt_list_count); //학과 리스트 파일로부터 동적 배열 할당
bool create_file(Student_list** src_list, char** dpt_list_array, int dpt_list_count, int file_mode); //초기 마스터 파일 생성 또는 트랜잭션 파일 작성 함수
void print_file(int file_mode); //마스터 파일, 트랜잭션 파일, 로그 파일 출력 함수
void update_master(bool flag_replace_master); //마스터 파일 갱신 함수
