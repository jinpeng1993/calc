#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <string.h>

#define DEBUG 1

#define MAX_STACK 100
#define NUM_LEN 32
#define MAX_EXPRE 200
#define END '\n'

typedef int bool;
#define true 1
#define false 0
#define pi  3.14159
#define exp 2.71828

char ops[MAX_STACK]; //�����ջ
int ops_top;       //�����ջ��
char *exps[MAX_STACK]; //���ʽջ
int exps_top;       //���ʽջ��
bool cal_flag = false; //��ʾ��ǰ�ַ���ջǰ�Ƿ��г�ջ����Ĺ���
char last_ch; //������һ�μ�¼���ַ�

char pop_expre[MAX_EXPRE];
char *separated_expre[2][MAX_EXPRE] = {NULL}; //�洢�ָ���Ķ������ʽ
														//һ�з��ӣ�һ�з�ĸ
int sep_expre_num = 0;
char *separated_op[MAX_EXPRE]; //�洢�ָ���Ĳ�����
int sep_op_num = 0;

char temp_separated_expre[MAX_EXPRE]; //��ʱ����ָ����ʽ�Ĺ�������

void init_ops() {
	ops_top = -1;
}

void init_exps() {
	while (exps_top != -1) {
		if (NULL != exps[exps_top]) {
			free(exps[exps_top]);
		}
		exps_top--;
	}
	exps_top = -1;
}

void push_ops(char op) {
	if (ops_top == MAX_STACK-1) {
		exit(-1);
	} else {
		ops_top++;
		ops[ops_top] = op;
	}
}

void push_exps(char *expre) {
	if (exps_top == MAX_STACK-1) {
		exit(-1);
	} else {
		exps_top++;
		exps[exps_top] = malloc(strlen(expre) + 1);
		memset(exps[exps_top], 0, strlen(expre) + 1);
		strcpy(exps[exps_top], expre);
	}
}

char pop_ops() {
	char pop_op;
	if(ops_top == -1) {
		exit(-1);
	} else {
		pop_op = ops[ops_top];
		ops_top--;
	}
	return pop_op;
}

char* pop_exps() {
	memset(pop_expre, 0, MAX_EXPRE);
	if(exps_top == -1) {
		exit(-1);
	} else {
		strcpy(pop_expre, exps[exps_top]);
		free(exps[exps_top]);
		exps_top--;
	}
	return pop_expre;
}


char show_ops_top() {
	if (ops_top != -1) {
		return ops[ops_top];
	} else {
		exit(-1);
	}
}

char* show_exps_top() {
	if (exps_top != -1) {
		return exps[exps_top];
	} else {
		exit(-1);
	}
}

bool is_constant(char ch) {
	if ((ch >= '0' && ch <= '9') || ch == '.' || ch == 'p' || ch == 'i' 
			|| ch =='e' || ch == 'x' || ch == 'p') {
		return true;
	} else
		return false;
}

bool is_algebra(char ch) {
	switch (ch) {
		case 'a':
		case 'b':
		case 'c':
		case 'd':
		case 'l':
		case 'm':
		case 'n':
		case 'x':
		case 'y':
		case 'z':
			return true;
		
		default:
			return false;
	}
}

bool is_last_ch_operator(char ch) {
	if (ch == show_ops_top())
		return true;
	else
		return false;
}

bool is_operator(char ch) {
	switch (ch) {
		case '+':
		case '-':
			if (!cal_flag && (is_last_ch_operator(last_ch) || last_ch == END)) {
				/*����ڴ������֮ǰ��û�н��й���ջ��������㣬����
				���ǵ�һ��������ַ�������һ��������ַ�Ҳ�ǲ�������
				�������ջѹ��һ��"0"��ʾ����������*/
				push_exps("0");
			}
			return true;
		case '(':
			if (!cal_flag) {
				if (last_ch == ')' || is_constant(last_ch)
						|| is_algebra(last_ch)) {
					
					exit(-1);
				}
			}
			return true;
		case ')':
			if (!cal_flag) {
				if (!(is_constant(last_ch) || is_algebra(last_ch))) {
					exit(-1);
				}
			}
			return true;
		case '*':
		case '/':
		case '^':
		case '=':
		case END:
			return true;
			
		default:
			return false;
	}
}

bool judge_op_prior(char op_last, char op_cur) {
	char prior;
	switch (op_cur) {
		case '+':
		case '-':
			if (op_last == '(' || op_last == END)
				prior = '<';
			else
				prior = '>';
			break;
		case '*':
		case '/':
			if (op_last == '*' || op_last == '/' || op_last == '^')
				prior = '>';
			else
				prior = '<';
			break;
		case '^':
			prior = '<';
			break;
		case '(':
			if (op_last == ')') {
				exit(-1);
			} else
				prior = '<';
			break;
		case ')':
			switch (op_last) {
				case '(':
					prior = '=';
					break;
				case END:
					exit(-1);
				default:
					prior = '>';
			}
			break;
		case END:
			switch (op_last) {
				case END:
					prior = '=';
					break;
				case '(':
					exit(-1);
				default:
					prior = '>';
			}
			break;
		case '=':
			prior = '=';
			break;

		default:
			exit(-1);
	}
	return prior;
}

char* calculate(char *expre1, char *expre_op, char *expre2) {
	strcat(expre1, expre_op);
	strcat(expre1, expre2);
	return expre1;
}

bool is_seperated_expression() {
	int ops_top_temp = ops_top;
	if (show_ops_top() != '+' && show_ops_top() != '-') {
		return false;
	}
	while (--ops_top_temp != -1) {
		if (ops[ops_top_temp] == '(') {
			return false;
		}
	}
	return true;
}

bool is_have_divisor(char *p_str) {
	if (NULL == strchr(p_str, '/')) {
		return false;
	} else {
		return true;
	}
}

void before_divisor(char *dst_str, char *src_str) {
	unsigned int tmp;
	tmp = (unsigned int)(strchr(src_str, '/') - src_str);
	strncpy(dst_str, src_str, tmp);
}

void after_divisor(char *dst_str, char *src_str) {
	char *str_temp = strchr(src_str, '/');
	str_temp++; //��������/��������ָ�������һλ
	strcpy(dst_str, str_temp);
}

void to_seperated_expression(char *p_str) {
	char get_ch, top_ops_ch;
	int num_len = 0; //�����������ֵĸ���
	char num_buff[NUM_LEN];
	char expre1[MAX_EXPRE];
	char expre2[MAX_EXPRE];
	char expre_op[MAX_EXPRE];

	init_ops();
	push_ops(END);
	init_exps();
	
	if (p_str != NULL) {
		get_ch = *p_str;
	} else {
	}
	top_ops_ch = show_ops_top();
		
	while (get_ch != END || top_ops_ch != END) {
		if (is_operator(get_ch)) {
			if (cal_flag) { //����ո��г�ջ����Ĺ��̣���ô��һ��ȡ�����ַ�
							//�Ѿ���������Ĭ�ϸ��ϴε��ַ�����'0',��ʾ�������
				last_ch = '0';
			}
			if (num_len) {
				push_exps(num_buff);
			}
			memset(num_buff, 0, num_len);
			num_len = 0;
			
			switch (judge_op_prior(top_ops_ch, get_ch)) {
				case '<':
					cal_flag = false;
					push_ops(get_ch);
					if (*(++p_str) != '\0') {
						last_ch = get_ch;
						get_ch = *p_str;
					}
					break;
				case '=': //ֻ��')'����ENDʱ����ȣ������')',��Ҫ��������ջ���
						  //'('������������ͬ��ǰ��')'��ӵ����ʽ������
					cal_flag = false;
					if (get_ch == ')') {
						memset(expre_op, 0, MAX_EXPRE);
						memset(expre1, 0, MAX_EXPRE);
						memset(expre2, 0, MAX_EXPRE);
						expre_op[0] = pop_ops();
						expre_op[1] = '\0';
						strcpy(expre2, pop_exps());
						strcat(expre_op, expre2);
						strcpy(expre1, expre_op);
						expre_op[0] = get_ch;
						expre_op[1] = '\0';
						strcat(expre1, expre_op);
						push_exps(expre1);
					} else if (get_ch == '=') {
						get_ch = END;
						continue;
					} else if (get_ch == END) {
						pop_ops();
					}
					if (*(++p_str) != '\0') {
						last_ch = '0'; //����Ĳ��轫��һ�εĲ����������ˣ�����
									   //����Ĭ�ϸ���ֵһ���ǲ���������
						get_ch = *p_str;
					}
					break;
				case '>':
					cal_flag = true;
					memset(expre_op, 0, MAX_EXPRE);
					memset(expre1, 0, MAX_EXPRE);
					memset(expre2, 0, MAX_EXPRE);
					expre_op[0] = pop_ops();
					expre_op[1] = '\0';
					strcpy(expre2, pop_exps());
					strcpy(expre1, pop_exps());
					push_exps(calculate(expre1, expre_op, expre2));
					break;
				
				default:
					break;
			}
			if (is_seperated_expression()) {
				memset(expre1, 0, MAX_EXPRE);
				strcpy(expre1,pop_exps());
				separated_expre[0][sep_expre_num] = malloc(MAX_EXPRE);
				memset(separated_expre[0][sep_expre_num], 0, MAX_EXPRE);
				strcpy(separated_expre[0][sep_expre_num], expre1);
				
				expre_op[0] = pop_ops();
				expre_op[1] = '\0';
				separated_op[sep_op_num] = malloc(strlen(expre_op)+1);
				memset(separated_op[sep_op_num], 0, strlen(expre_op)+1);
				strcpy(separated_op[sep_op_num], expre_op);
				
				sep_expre_num++;
				sep_op_num++;
			}
		} else if (is_constant(get_ch)) {
			if (cal_flag) {
				last_ch = '0';
			}
			if (last_ch == ')') { //��һ������Ϊ')',�򱾴����벻��Ϊ����
				exit(-1);
			}
			num_buff[num_len] = get_ch;
			num_len++;
			num_buff[num_len] = '\0';
			if (*(++p_str) != '\0') {
				last_ch = get_ch;
				get_ch = *p_str;
			}
		} else if (is_algebra(get_ch)) {
			if (cal_flag) {
				last_ch = '0';
			}
			if (last_ch == ')') { //��һ������Ϊ')',�򱾴����벻��Ϊ����
				exit(-1);
			}
			num_buff[num_len] = get_ch;
			num_len++;
			num_buff[num_len] = '\0';
			if (*(++p_str) != '\0') {
				last_ch = get_ch;
				get_ch = *p_str;
			}
		} else {
			exit(-1);
		}
		top_ops_ch = show_ops_top();
	}
	memset(expre1, 0, MAX_EXPRE);
	strcpy(expre1,pop_exps());
	separated_expre[0][sep_expre_num] = malloc(MAX_EXPRE);
	memset(separated_expre[0][sep_expre_num], 0, MAX_EXPRE);
	strcpy(separated_expre[0][sep_expre_num], expre1);
	sep_expre_num++;
}

void abstract_denominator() {
	int i; //ѭ��������
	int i_sub; //��ѭ��������
	char temp_expre[MAX_EXPRE];
	char *temp_str = NULL;
	char *temp_str_last = NULL;

	for (i=0; i < sep_expre_num; i++) {
		memset(temp_expre, 0, MAX_EXPRE);
		strcpy(temp_expre, separated_expre[0][i]);
		if (is_have_divisor(temp_expre)) {
			/*����֮ǰ����Ϊ���ӣ�����֮��Ľ�һ���ж�*/
			memset(separated_expre[0][i], 0, MAX_EXPRE);
			separated_expre[1][i] = malloc(MAX_EXPRE);
			memset(separated_expre[1][i], 0, MAX_EXPRE);
			before_divisor(separated_expre[0][i], temp_expre);
			after_divisor(separated_expre[1][i], temp_expre);

			// memset(temp_expre, 0, MAX_EXPRE);
			// strcpy(temp_expre, separated_expre[1][i]);
			temp_str = separated_expre[1][i];
			while (*temp_str != '\0') {
				if (is_constant(*temp_str)) {
					do {
						temp_str++;
					} while (is_constant(*temp_str)); 	
					temp_str++;
				} else if (*temp_str == '(') {
					do {
						temp_str++;
					} while (*temp_str != ')');
					temp_str++;
				} else if (*temp_str == '/') {
					/*�����ǰ�ַ��ǳ��ţ�ת��Ϊ�˺�*/
					memset(temp_str, '*', 1);
					temp_str++;
				} else if (*temp_str == '*') {
					/*�����ǰ�ַ��ǳ˺ţ��ѳ˺��Լ������ŵı��ʽ�Ƶ����Ӳ��֣�
					��ĸ����ɾ����һ�����ַ�*/
					i_sub = 0;
					memset(temp_expre, 0, MAX_EXPRE);
					temp_expre[i_sub] = *temp_str;
					i_sub++;
					temp_expre[i_sub] = '\0';
					memset(temp_str, 0, 1);
					temp_str_last = temp_str; // ���浱ǰ��λ��

					temp_str++;
					if (is_constant(*(temp_str+1))) {
						do {
							temp_expre[i_sub] = *temp_str;
							i_sub++;
							temp_expre[i_sub] = '\0';
							memset(temp_str, 0, 1);
							temp_str++;
						} while (is_constant(*(temp_str+1))); 	
					} else if (*temp_str == '(') {
						do {
							temp_expre[i_sub] = *temp_str;
							i_sub++;
							temp_expre[i_sub] = '\0';
							memset(temp_str, 0, 1);
							temp_str++;
						} while (*temp_str != ')');
					}
					temp_expre[i_sub] = *temp_str;
					i_sub++;
					temp_expre[i_sub] = '\0';
					memset(temp_str, 0, 1);
					temp_str++;
					strcat(separated_expre[0][i], temp_expre);
					strcat(separated_expre[1][i], temp_str);  //ƴ���ַ����Ժ�temp_strָ���λ�ûᱻ�ı�
					temp_str = temp_str_last;				  //����ʹ��֮ǰ�����λ�ó�ֵtemp_str
				} else {
					temp_str++;
				}
			}

		} else {
			/*���û�г��ţ���ĸĬ��Ϊ1*/
			separated_expre[1][i] = malloc(MAX_EXPRE);
			memset(separated_expre[1][i], 0, MAX_EXPRE);
			strcpy(separated_expre[1][i], "1");
		}
	}
}

void total_denominator(char *deno, char *str) {
	char *ptr_deno = deno;
	char *ptr_str = str;
	char deno_element[MAX_EXPRE];
	char str_element[MAX_EXPRE];
	char denominator_temp[MAX_EXPRE];
	int i_deno, i_str;
	memset(deno_element, 0, MAX_EXPRE);
	memset(str_element, 0, MAX_EXPRE);
	memset(denominator_temp, 0, MAX_EXPRE);
	strcpy(denominator_temp, deno);
	while (*ptr_str != '\0') {
		i_str = 0;
		if (is_constant(*ptr_str)) {
			while (is_constant(*ptr_str)) {
				str_element[i_str] = *ptr_str;
				i_str++;
				str_element[i_str] = '\0';
				ptr_str++;
			}
		} else if(*ptr_str == '(') {
			while (*ptr_str != ')') {
				str_element[i_str] = *ptr_str;
				i_str++;
				str_element[i_str] = '\0';
				ptr_str++;
			}
			str_element[i_str] = *ptr_str;
			i_str++;
			str_element[i_str] = '\0';
			ptr_str++;
		} else {
			str_element[i_str] = *ptr_str;
			i_str++;
			str_element[i_str] = '\0';
			ptr_str++;
		}
			
		ptr_deno = deno;
		while (*ptr_deno != '\0') {
			i_deno = 0;
			if (is_constant(*ptr_deno)) {
				while (is_constant(*ptr_deno)) {
					deno_element[i_deno] = *ptr_deno;
					i_deno++;
					deno_element[i_deno] = '\0';
					ptr_deno++;
				}
			} else if(*ptr_deno == '(') {
				while (*ptr_deno != ')') {
					deno_element[i_deno] = *ptr_deno;
					i_deno++;
					deno_element[i_deno] = '\0';
					ptr_deno++;
				}
				deno_element[i_deno] = *ptr_deno;
				i_deno++;
				deno_element[i_deno] = '\0';
				ptr_deno++;
			} else {
				deno_element[i_deno] = *ptr_deno;
				i_deno++;
				deno_element[i_deno] = '\0';
				ptr_deno++;
			}
				
			if (!strcmp(deno_element, str_element)) {
				break;
			}
		}
		if (strcmp(deno_element, str_element)) {
			strcat(denominator_temp, "*");
			strcat(denominator_temp, str_element);
		}
	}
	strcpy(deno, denominator_temp);
}

void sub_denominator(char *deno_for_element, char *self_deno) {
	char *ptr_deno_for_element = deno_for_element;
	char *ptr_self_deno = self_deno;
	char *temp_deno_for_element = NULL;
	int i_deno_for_element;
	int i_self_deno;

	char part_of_deno_for_element[MAX_EXPRE];
	char part_of_self_deno[MAX_EXPRE];
	memset(part_of_deno_for_element, 0, MAX_EXPRE);
	memset(part_of_self_deno, 0, MAX_EXPRE);

	while (*ptr_self_deno != '\0') {
		i_self_deno = 0;
		if (is_constant(*ptr_self_deno)) {
			while (is_constant(*ptr_self_deno)) {
				part_of_self_deno[i_self_deno] = *ptr_self_deno;
				i_self_deno++;
				part_of_self_deno[i_self_deno] = '\0';
				ptr_self_deno++;
			}
		} else if(*ptr_self_deno == '(') {
			while (*ptr_self_deno != ')') {
				part_of_self_deno[i_self_deno] = *ptr_self_deno;
				i_self_deno++;
				part_of_self_deno[i_self_deno] = '\0';
				ptr_self_deno++;
			}
			part_of_self_deno[i_self_deno] = *ptr_self_deno;
			i_self_deno++;
			part_of_self_deno[i_self_deno] = '\0';
			ptr_self_deno++;
		} else {
			part_of_self_deno[i_self_deno] = *ptr_self_deno;
			i_self_deno++;
			part_of_self_deno[i_self_deno] = '\0';
			ptr_self_deno++;
		}
			
		ptr_deno_for_element = deno_for_element;
		while (*ptr_deno_for_element != '\0') {
			i_deno_for_element = 0;
			if (is_constant(*ptr_deno_for_element)) {
				while (is_constant(*ptr_deno_for_element)) {
					part_of_deno_for_element[i_deno_for_element] = *ptr_deno_for_element;
					i_deno_for_element++;
					part_of_deno_for_element[i_deno_for_element] = '\0';
					ptr_deno_for_element++;
				}
			} else if(*ptr_deno_for_element == '(') {
				while (*ptr_deno_for_element != ')') {
					part_of_deno_for_element[i_deno_for_element] = *ptr_deno_for_element;
					i_deno_for_element++;
					part_of_deno_for_element[i_deno_for_element] = '\0';
					ptr_deno_for_element++;
				}
				part_of_deno_for_element[i_deno_for_element] = *ptr_deno_for_element;
				i_deno_for_element++;
				part_of_deno_for_element[i_deno_for_element] = '\0';
				ptr_deno_for_element++;
			} else {
				part_of_deno_for_element[i_deno_for_element] = *ptr_deno_for_element;
				i_deno_for_element++;
				part_of_deno_for_element[i_deno_for_element] = '\0';
				ptr_deno_for_element++;
			}
				
			if (!strcmp(part_of_deno_for_element, part_of_self_deno)) {
				temp_deno_for_element = ptr_deno_for_element;
				while (i_deno_for_element > 0) {
					temp_deno_for_element--;
					i_deno_for_element--;
				}
				if (!strcmp(temp_deno_for_element, deno_for_element)) {
					ptr_deno_for_element++;
					strcpy(deno_for_element,ptr_deno_for_element);
				} else {
					if (*(temp_deno_for_element-1) == '*') {
						temp_deno_for_element--;
					} else if (*(temp_deno_for_element-1) == '*') {
						temp_deno_for_element--;
					}
					memset(temp_deno_for_element, 0, 1);
					strcat(deno_for_element, ptr_deno_for_element);
				}
				break;
			}
		}
	}
}

void free_resource() {
	/*Ϊ����һ�μ��㣬����ȫ�ֱ���*/
	memset(ops, 0, MAX_STACK);
	cal_flag = false;
	last_ch = END;
	memset(pop_expre, 0, MAX_EXPRE);
	memset(temp_separated_expre, 0, MAX_EXPRE);

	/*�ͷű��ʽջ*/
	while (exps_top != -1) {
		if (exps[exps_top] != NULL) {
			free(exps[exps_top]);
		}
		exps_top--;
	}

	/*�ͷŴ洢�ָ���ı��ʽ�Ķ�ά����*/
	while (sep_expre_num >= 0) {
		if (NULL != separated_expre[0][sep_expre_num]) {
			free(separated_expre[0][sep_expre_num]);
		}
		if (NULL != separated_expre[1][sep_expre_num]) {
			free(separated_expre[1][sep_expre_num]);
		}
		sep_expre_num--;
	}
	sep_expre_num = 0;

	/*�ͷŴ洢�������ʽ֮��ķ��ŵ�����*/
	while (sep_op_num >= 0) {
		if (NULL != separated_op[sep_op_num]) {
			free(separated_op[sep_op_num]);
		}
		sep_op_num--;
	}
	sep_op_num = 0;
}

char* algebra_cal_parse_expression(char *p_str, char *ret_str) {
	int i = 0;
	char denominator[MAX_EXPRE];
	char denominator_for_element[MAX_EXPRE]; // ÿ�������ı��ʽ�ķ��ӣ���Ҫ��������ַ�����ͨ�� 
	to_seperated_expression(p_str);
	abstract_denominator();
	memset(denominator, 0, MAX_EXPRE);
	while (i < sep_expre_num) {
		if (*separated_expre[1][i] == '1' && strlen(separated_expre[1][i]) == 1) {
			i++;
		} else {
			break;
		}
	}
	if (i < sep_expre_num) {
		strcpy(denominator,separated_expre[1][i]);
		i++;
		for (; i<sep_expre_num; i++) {
			if (*separated_expre[1][i] == '1' && strlen(separated_expre[1][i]) == 1) {
				continue;
			} else {
				total_denominator(denominator, separated_expre[1][i]);
			}
		}
	} else {
		strcpy(denominator,separated_expre[1][0]);
	}
	if (strcmp(denominator, "1"))
	{
		for (i=0; i<sep_expre_num; i++) {
			if (*separated_expre[1][i] == '1' && strlen(separated_expre[1][i]) == 1) {
				memset(separated_expre[1][i], 0, MAX_EXPRE);
				strcpy(separated_expre[1][i], denominator);

				strcat(separated_expre[0][i], "*");
				strcat(separated_expre[0][i], denominator);
			} else {
				memset(denominator_for_element, 0, MAX_EXPRE);
				strcpy(denominator_for_element, denominator);
				sub_denominator(denominator_for_element, separated_expre[1][i]);

				memset(separated_expre[1][i], 0, MAX_EXPRE);
				strcpy(separated_expre[1][i], denominator);

				if (strlen(denominator_for_element) > 0) {
					strcat(separated_expre[0][i], "*");
					strcat(separated_expre[0][i], denominator_for_element);
				}
			}
		}
		strcpy(ret_str, "(");
		for (i=0; i<sep_expre_num; i++) {
			strcat(ret_str, separated_expre[0][i]);
			if (i != sep_op_num) {
				strcat(ret_str, separated_op[i]);
			}
		}
		strcat(ret_str, ")/(");
		strcat(ret_str, separated_expre[1][0]);
		strcat(ret_str, ")");
	} else {
		for (i=0; i<sep_expre_num; i++) {
			strcat(ret_str, separated_expre[0][i]);
			if (i != sep_op_num) {
				strcat(ret_str, separated_op[i]);
			}
		}
	}
	free_resource();

	return ret_str;
}
