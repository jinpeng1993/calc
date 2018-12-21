#include "algebra_cal.c"

#define CON_MAXSIZE 200
#define CON_END '\n'

char con_input_last = CON_END; //用来记录上一次的输入
bool con_cal_flag = false; //用来计算当前字符入栈前是否有计算过程

char con_ops[CON_MAXSIZE]; //运算符栈
int con_ops_top;       //运算符栈顶
double con_ovs[CON_MAXSIZE]; //操作数栈
int con_ovs_top;       //操作数栈顶

char con_result_str[CON_MAXSIZE];
char alg_result_str[CON_MAXSIZE];

void init_con_ops() {
	con_ops_top = -1;
}

void init_con_ovs() {
	con_ovs_top = -1;
}

void push_con_ops(char op) {
	if (con_ops_top == CON_MAXSIZE-1) {
		exit(-1);
	} else {
		con_ops_top++;
		con_ops[con_ops_top] = op;
	}
}

void push_con_ovs(double val) {
	if (con_ovs_top == CON_MAXSIZE-1) {
		exit(-1);
	} else {
		con_ovs_top++;
		con_ovs[con_ovs_top] = val;
	}
}

char pop_con_ops() {
	char ret;
	if(con_ops_top == -1) {
		exit(-1);
	} else {
		ret = con_ops[con_ops_top];
		con_ops_top--;
	}
	return ret;
}

double pop_con_ovs() {
	double ret;
	if(con_ovs_top == -1) {
		exit(-1);
	} else {
		ret=con_ovs[con_ovs_top];
		con_ovs_top--;
	}
	return ret;
}

char show_con_ops_top() {
	if (con_ops_top != -1) {
		return con_ops[con_ops_top];
	} else {
		exit(-1);
	}
}

double show_con_ovs_top() {
	if (con_ovs_top != -1) {
		return con_ovs[con_ovs_top];
	} else {
		exit(-1);
	}
}

bool con_is_operator_last_ch(char last_ch) {
	if (last_ch == show_con_ops_top())
		return true;
	else
		return false;
}

bool con_is_cur_ch_constant(char ch) {
	if ((ch >= '0' && ch <= '9') || (ch == '.') || (ch == 'p') || (ch == 'i')
			|| (ch =='e') || (ch == 'x') || (ch == 'p')) {
		return true;
	} else
		return false;
}

bool con_is_operator(char ch) {
	switch (ch) {
		case '+':
			if ((!con_cal_flag) && (con_is_operator_last_ch(con_input_last) ||
					con_input_last == CON_END)) {
				/*如果在此运算符之前，没有进行过出栈计算的运算，并且
				这是第一个输入的字符或者上一次输入的字符也是操作符，
				向操作数栈压入一个0表示正数运算*/
				push_con_ovs(0);
			}
			return true;
		case '-':
			if (!con_cal_flag && (con_is_operator_last_ch(con_input_last) ||
					con_input_last == CON_END)) {
				/*如果在此运算符之前，没有进行过出栈计算的运算，并且
				这是第一个输入的字符或者上一次输入的字符也是操作符，
				向操作数栈压入一个0表示负数运算*/
				push_con_ovs(0);
			}
			return true;
		case '(':
			if ((!con_cal_flag)) {
				if (con_input_last  == ')' || (con_input_last >= '0'
						&& con_input_last <= '9') || con_input_last=='.') {
					exit(-1);
				}
			}			
			return true;
		case ')':
			if (!con_cal_flag) {
				if (!con_is_cur_ch_constant(con_input_last)) {
					exit(-1);
				}
			}				
			return true;
		case '*':
		case '/':
		case '^':
		case CON_END:
			return true;
			
		default :
			return false;
	}
}

char con_judge_op_prior(char op_last, char op_cur) {
	char ret;
	switch (op_cur) {
		case '+':
		case '-':
			if (op_last == '(' || op_last == CON_END)
				ret = '<';
			else
				ret = '>';
			break;
		case '*':
		case '/':
			if (op_last == '*' || op_last == '/' || op_last == '^')
				ret = '>';
			else
				ret = '<';
			break;
		case '^':
			ret = '<';
			break;
		case '(':
			if (op_last == ')') {
				exit(-1);
			} else
				ret = '<';
			break;
		case ')':
			switch (op_last) {
				case '(':
					ret = '=';
					break;
				case CON_END:
					exit(-1);
				default:
					ret = '>';
			}
			break;
		case CON_END:
			switch (op_last) {
				case CON_END:
					ret = '=';
					break;
				case '(':
					exit(-1);
				default:
					ret = '>';
			}
	}
	return ret;
}

double calculate_double(double val1, char op, double val2) {
	switch (op) {
		case '+':
			val1 = val1 + val2;
			break;
		case '-':
			val1 = val1 - val2;
			break;
		case '*':
			val1 = val1 * val2;
			break;
		case '/':
			val1 = val1 / val2;
			break;
		case '^':
			val1 = pow(val1, val2);
			break;
		
		default:
			exit(-1);
	}
	return val1;
}

char * float2str(double val, char *buf)
{
    char *cur, *end;
    sprintf(buf, "%.6f", val);
    cur = buf + strlen(buf) - 1;
    end = cur - 6;
    while ((cur > end) && (*cur == '0')) {
        *cur = '\0';
        cur--;
    }
    if (*cur == '.') {
    	*cur = '\0';
    }
    return buf;
}

char* con_parse_expression(char *p_str) {
	char input_ch, top_con_ops_ch;
	char num_buff[CON_MAXSIZE], op_last;  
	int num_len = 0; //连续输入数字的个数
	double val1, val2; //con_ovs出栈的操作数
	double num_cur;
	
	init_con_ops();
	push_con_ops(CON_END);
	init_con_ovs();
	if ( p_str != NULL) {
		input_ch = *p_str;
	} else {
		exit(-1);
	}
	top_con_ops_ch = show_con_ops_top();
	
	
	while (input_ch != CON_END || top_con_ops_ch != CON_END) {
		if (con_is_operator(input_ch)) {
			if (con_cal_flag) {
				con_input_last = '0';
			}
			if (num_len) {
				push_con_ovs(num_cur);  //将解析出来的数字入栈
			}
			memset(num_buff, 0, num_len);
			num_len = 0;
			
			switch (con_judge_op_prior(top_con_ops_ch, input_ch)) {
				case '<': 
					/*若上一次的运算符优先级小于当前运算符的优先级，
					则将当前运算符入栈*/
					con_cal_flag = false;
					push_con_ops(input_ch);
					if (*(p_str++) != '\0') {
						con_input_last = input_ch;
						input_ch = *p_str;
					}
					//input_ch = getchar();
					break;
				case '=': 
					/*只有')'或者CON_END时才会相等，
					弹出'('或者初始化时的CON_END即可*/
					con_cal_flag = false;
					pop_con_ops();
					if (*(p_str++) != '\0') {
						con_input_last ='0'; //这时候不单将操作符栈里面的'('
										 //丢弃，当前记录的')'也会丢弃，
										 //所以con_input_last不能保存之前的
										 //input_ch,而是默认赋给一个数值
						input_ch = *p_str;
					}
					//input_ch = getchar();
					break;
				case '>':
					/*若上一次的运算符优先级大于当前运算符的优先级，
					弹出两个操作数和一个操作符进行计算，然后将计算结果
					压回操作数栈*/
					con_cal_flag = true;
					op_last = pop_con_ops();
					val2 = pop_con_ovs();
					val1 = pop_con_ovs();
					push_con_ovs(calculate_double(val1, op_last, val2));
					break;
			}
		} else if (con_is_cur_ch_constant(input_ch)) {
			if (con_cal_flag) {
				con_input_last = '0';
			}
			if (con_input_last == ')') {
				/*上一次输入为')',则本次输入不能为数字*/
				exit(-1);
			}
			num_buff[num_len] = input_ch;
			num_len++;
			switch (input_ch) { //判断输入的是否为pi或者exp常数
				case 'p':
					if (*(p_str++) != '\0') {
						con_input_last = input_ch;
						input_ch = *p_str;
						if (input_ch == 'i') {
							num_cur = (double)pi;
							break;
						}
					}
					exit(-1);
				case 'e':
					if (*(p_str++) != '\0') {
						con_input_last = input_ch;
						input_ch = *p_str;
						if (input_ch == 'x') {
							if (*(p_str++) != '\0') {
								con_input_last = input_ch;
								input_ch = *p_str;
								if (input_ch == 'p') {
									num_cur = (double)exp;
									break;
								}
							}
						}
					}
					exit(-1);
				default:
					num_cur = (double)atof(num_buff);
			}
			if (*(p_str++) != '\0') {
				con_input_last = input_ch;
				input_ch = *p_str;
			}
			//input_ch = getchar();
		} else if (input_ch == '=') {
			input_ch = '\n';
			//input_ch = getchar();
			continue;
		} else {
			exit(-1);
		}
		top_con_ops_ch = show_con_ops_top();
	}
	memset(con_result_str, 0, CON_MAXSIZE);
	return (float2str(show_con_ovs_top(), con_result_str));
}
char* parse_expression(char *p_str) {
	char * ptr_temp = p_str;
	while (*ptr_temp != '=') {
		if (is_algebra(*ptr_temp)) {
			memset(alg_result_str, 0, CON_MAXSIZE);
			return algebra_cal_parse_expression(p_str, alg_result_str);
		}
		ptr_temp++;
	}
	return con_parse_expression(p_str);
}