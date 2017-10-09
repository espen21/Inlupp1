
#ifndef __UTILS_H__
#define __UTILS_H__



extern char *strdup(const char *);

typedef union {
  int   i;
  float f;
  char  c;
  char *s;
} answer_t;

typedef bool(*check_func)(char *);
typedef answer_t(*convert_func)(char *);


int read_string(char *buf, int buf_siz);
bool is_number(char *str);
bool right_menu(char *c);
int ask_question_int(char *question);
char *ask_question_string(char *question);
char ask_question_char(char *question);
bool not_empty(char *str);
answer_t ask_question(char *question, check_func check, convert_func convert);
char *ask_question_shelf(char *question);
bool is_shelf(char *str);
bool right_answer(char *c);
char ask_question_ware();

//, char *buf, int buf_siz);

#endif
