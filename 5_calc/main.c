#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/*
 * main.c
 *
 *  Created on: 24 ��� 2019 �.
 *      Author: anna
 */

#include <stdio.h>

#include <locale.h> 
#include <stdlib.h> 

struct element
{
  char *task;
  struct element *next;
};

typedef struct element LIST;

typedef struct stack
{
  int data;
  struct stack *next;
} stack;

void freeList(struct element* head);
void convert_vector_to_str_vector(int size, int *vector_1, int *vector_2,
                                  float *vector_result, char *vector_1_str,
                                  char *vector_2_str, char *vector_result_str);
//void add_to_list(LIST *head, LIST *current, char *line);
int check_number(char number);
int need_operation(char op);
int do_operation(char op, stack** head);
int pop(stack **head);

void push(stack **head, int value)
{
  stack* node = malloc(sizeof(stack));

  printf("push = %d \n", value);

  if (node == NULL)
  {
    fputs("Error: Out of memory\n", stderr);
    exit(1);
  }
  else
  {
    node->data = value;
    node->next = *head;
    *head = node;
  }
}

int pop(stack **head)
{
  if (*head == NULL)
  {
    fputs("Error: bottom of stack!\n", stderr);
    exit(1);
  }
  else
  {
    stack* top = *head;
    int value = top->data;
    *head = top->next;
    free(top);
    printf("pop value = %d \n", value);
    return value;
  }
}

int do_operation(char op, stack** head)
{
  int result = 0;
  int tmp;
  switch (op)
  {
    case '+':
      result = pop(head) + pop(head);
      printf("pop(head) + pop(head) = %d \n", result);
      break;
    case '*':
      result = pop(head) * pop(head);
      printf("pop(head) * pop(head) = %d \n", result);
      break;
    case '-':
      tmp = pop(head);
      result = pop(head) - tmp;
      printf("operation - result = %d tmp = %d\n", result, tmp);
      break;
    case '/':
      tmp = pop(head);
      result = pop(head) / tmp;
      printf("operation / result = %d tmp = %d\n", result, tmp);
      break;
    default:
      break;
  }
  return result;
}

int need_operation(char op)
{
  switch (op)
  {
    case '+':
    case '*':
    case '-':
    case '/':
      return 2;
    default:
      printf("Invalid operand!\n");
      exit(1);
  }
}

int check_number(char number)
{
  if (number < '0' || number > '9') return 0;

  return 1;
}

void freeList(LIST *head)
{
  LIST *tmp;

  while (head != NULL)
  {
    tmp = head;
    head = head->next;
    free(tmp);
  }

}

void add_to_list(LIST *head, LIST *current, char *line)
{
  LIST *node = malloc(sizeof(LIST));
  node->task = strdup(line); //note : strdup is not standard function
  node->next = NULL;

  if (head == NULL)
  {
    current = head = node;
  }
  else
  {
    current = current->next = node;
  }
}

void convert_vector_to_str_vector(int size, int *vector_1, int *vector_2,
                                  float *vector_result, char *vector_1_str,
                                  char *vector_2_str, char *vector_result_str)
{
  // ������� ���������
  int cur_pos = 0;
  int i = 0;
  memset(vector_1_str, 0, 255);
  memset(vector_2_str, 0, 255);
  memset(vector_result_str, 0, 255);
  // ����� �������� 1 ������� ����� ������

  for (i = 0; i < size; i++)
  {
    if (i != size - 1)
    {
      cur_pos += sprintf(&vector_1_str[cur_pos], "%d ", vector_1[i]);
    }
    else
    {
      cur_pos += sprintf(&vector_1_str[cur_pos], "%d", vector_1[i]);
    }
  }
  cur_pos = 0;

  for (i = 0; i < size; i++)
  {
    if (i != size - 1)
    {
      cur_pos += sprintf(&vector_2_str[cur_pos], "%d ", vector_2[i]);
    }
    else
    {
      cur_pos += sprintf(&vector_2_str[cur_pos], "%d", vector_2[i]);
    }
  }

  cur_pos = 0;

  for (i = 0; i < size; i++)
  {
    if (i != size - 1)
    {
      cur_pos += sprintf(&vector_result_str[cur_pos], "%.2f ",
                         vector_result[i]);
    }
    else
    {
      cur_pos += sprintf(&vector_result_str[cur_pos], "%.2f)",
                         vector_result[i]);
    }
  }
  printf("\n");
}

int main(void)
{
  char line[128];
  LIST *in_current, *in_head;
  LIST *out_current, *out_head;
  char operation_type = 0; // � ��� ���������� ���������� ��� ��������. 
  char data_type = 0; // � ��� ���������� �������� ����� ��� ������ ���������� � ����� ����� ���� ��� ��� ���������
  int i = 0; // ����������, ������� ����� ������������ ��� �������, ��� ��������
  // �������� �������� � �������
  // ������� ����� �������� �������� ����������� ��� �������� � ��������� �����. 
  FILE *in_file;
  FILE *out_file; // ���������� ��������� ����� (���� ������� ���������)

  setlocale(LC_ALL, "Russian"); 

  in_head = in_current = NULL;
  out_head = out_current = NULL;
  in_file = fopen("input.txt", "r");
  out_file = fopen("output.txt", "w");
  memset(line, 0, sizeof(line));

  while (fgets(line, sizeof(line), in_file))
  {
    //add_to_list(in_head, in_current, line);

    LIST *in_node = malloc(sizeof(LIST));
    in_node->task = strdup(line); //note : strdup is not standard function
    in_node->next = NULL;

    if (in_head == NULL)
    {
      in_current = in_head = in_node;
    }
    else
    {
      in_current = in_current->next = in_node;
    }

  }
  fclose(in_file);

  for (in_current = in_head; in_current; in_current = in_current->next)
  {
    printf("Current task: %s\n", in_current->task);
    LIST *out_node = malloc(sizeof(LIST));

    sscanf(in_current->task, "%c %c", &operation_type, &data_type);

    printf("operation_type= %c\n", operation_type);
    printf("data_type= %c\n", data_type);

    switch (data_type)
    // ������ �������, ����� �� � ��� ��� ������ � �����, ������� �� ��������
    {
      case 's': // ������ � ��� ������� ����� � �����
      {
        int number_1; // ������ �������, ��� � ����� ��������� ����� �������������� ���������� number_1 ���� int (integer, �.�. ����� �����. ����� ����� ��� 1,2,3,4,5..N)
        int number_2; // � ����� ��������� ����� ���������� number_2
        float result; // � ��� ���������� �������� ��������� ��������. ��� ���� float - ���������� �� ��� ������� �����, �������� "2.342"

        sscanf(&in_current->task[0] + 4, "%d %d", &number_1, &number_2);

        switch (operation_type)
        {
          case '+':
            result = number_1 + number_2; // ���������� ��� ����� � ����� � ���������� result
            // ������� fprintf �������� ����� ��� �� ��� ������� printf, ������ ������� ��������� �� � �������, � � ����, ������� � ��� ����� �������� ���������� �� ����, ���� ����� �������� ����������
            memset(line, 0, sizeof(line));
            sprintf(line, "���������: %d + %d = %f \n", number_1, number_2,
                    result);
            //add_to_list(out_head, out_current, line);
            out_node->task = strdup(line); //note : strdup is not standard function
            out_node->next = NULL;

            if (out_head == NULL)
            {
              out_current = out_head = out_node;
            }
            else
            {
              out_current = out_current->next = out_node;
            }
            break;
          case '-':
            result = number_1 - number_2;
            memset(line, 0, sizeof(line));
            sprintf(line, "���������: %d - %d = %f \n", number_1, number_2,
                    result);
            //add_to_list(out_head, out_current, line);
            out_node->task = strdup(line); //note : strdup is not standard function
            out_node->next = NULL;

            if (out_head == NULL)
            {
              out_current = out_head = out_node;
            }
            else
            {
              out_current = out_current->next = out_node;
            }

            break;
          case '*':
            result = number_1 * number_2;
            memset(line, 0, sizeof(line));
            sprintf(line, "���������: %d * %d = %f \n", number_1, number_2,
                    result);
            //add_to_list(out_head, out_current, line);
            out_node->task = strdup(line); //note : strdup is not standard function
            out_node->next = NULL;

            if (out_head == NULL)
            {
              out_current = out_head = out_node;
            }
            else
            {
              out_current = out_current->next = out_node;
            }

            break;
          case '/':
            result = number_1 / number_2;
            memset(line, 0, sizeof(line));
            sprintf(line, "���������: %d / %d = %f \n", number_1, number_2,
                    result);
            //add_to_list(out_head, out_current, line);
            out_node->task = strdup(line); //note : strdup is not standard function
            out_node->next = NULL;

            if (out_head == NULL)
            {
              out_current = out_head = out_node;
            }
            else
            {
              out_current = out_current->next = out_node;
            }

            break;
          case '^':
            result = 1; // ����� ������ �������� result, ����� �� ��������� ������ ��� �����
            // ��� �������� ���� for
            // ������ �� for. ������ i == 0. ���������, ���� "i < number_2", �� ������ i++ (�.�. ����������� i �� 1) � ���� �� "{"
            // ��������� ������������ ����� � ������� �� "}"
            // ����� ���� �� for. ��������� ���� "i < number_2", �� ������ i++, ���� ���, �� ������� �� �����
            for (i = 0; i < number_2; i++)
            {
              result = result * number_1;
            }
            memset(line, 0, sizeof(line));
            sprintf(line, "���������: %d ^ %d = %f \n", number_1, number_2,
                    result);
            //add_to_list(out_head, out_current, line);
            out_node->task = strdup(line); //note : strdup is not standard function
            out_node->next = NULL;

            if (out_head == NULL)
            {
              out_current = out_head = out_node;
            }
            else
            {
              out_current = out_current->next = out_node;
            }
            break;
          case '!':
            if (number_1 == 0) // ���� number == 0, �� ��������� ������� �� �����, ����� 1
            {
              result = 1;
            }
            else // ���� number != 0
            {
              result = 1;
              for (i = 1; i <= number_1; i++)
              {
                result = result * i;
              }
            }
            memset(line, 0, sizeof(line));
            sprintf(line, "���������: %d! = %f \n", number_1, result);
            //add_to_list(out_head, out_current, line);
            out_node->task = strdup(line); //note : strdup is not standard function
            out_node->next = NULL;

            if (out_head == NULL)
            {
              out_current = out_head = out_node;
            }
            else
            {
              out_current = out_current->next = out_node;
            }

            break;
          default:
            puts(
                "������ - ������ ������������ ������ ��������. �������� ���� � ���������� �����");
            break;
        }
        break; // break ��������� � "case 's':", �.�. ������� �� ����
      }
      case 'v': // ������ � ��� ������� � �����
      {
        int *vector_1; // ��������� �� ������ ������. � ���� ���������� �������� ����� ������ �������
        int *vector_2; // ��������� �� ������ ������. � ���� ���������� �������� ����� ������ �������
        float *vector_result; // ��������� �� ������, ���������� ���������. � ���� ���������� �������� ����� ������ �������
        char *vector_1_str; // ��������� �� ������ ������. � ���� ���������� �������� ����� ������ �������
        char *vector_2_str; // ��������� �� ������ ������. � ���� ���������� �������� ����� ������ �������
        char *vector_result_str; // ��������� �� ������, ���������� ���������. � ���� ���������� �������� ����� ������ �������
        int size = 0; // ������ ���� ��������. ��������� ��� �� �����

        sscanf(&in_current->task[0] + 4, "%d", &size);

        // �������� ������ ��� �������
        vector_1 = calloc(size, sizeof(int)); // ������� calloc �������� ������ ��� �������. size - ���������� ���������, sizeof(int) - ������ ������ ��������
        vector_2 = calloc(size, sizeof(int)); // ������� calloc �������� ������ ��� �������. size - ���������� ���������, sizeof(int) - ������ ������ ��������
        vector_result = calloc(size, sizeof(float)); // ������� calloc �������� ������ ��� �������. size - ���������� ���������, sizeof(int) - ������ ������ ��������
        vector_1_str = calloc(255, sizeof(char)); // ������� calloc �������� ������ ��� �������. size - ���������� ���������, sizeof(int) - ������ ������ ��������
        vector_2_str = calloc(255, sizeof(char)); // ������� calloc �������� ������ ��� �������. size - ���������� ���������, sizeof(int) - ������ ������ ��������
        vector_result_str = calloc(255, sizeof(char)); // ������� calloc �������� ������ ��� �������. size - ���������� ���������, sizeof(int) - ������ ������ ��������

        // ������ �������� 1 ������� �� �����
        for (i = 0; i < size; i++)
        {
          sscanf(&in_current->task[0] + 6, "%d ", &vector_1[i]);
        }

        for (i = 0; i < size; i++)
        {
          printf("vector_1[%d] = %d ", i, vector_1[i]);
        }
        printf("\n");

        // ������ �������� 2 ������� �� �����
        for (i = 0; i < size; i++)
        {
          sscanf(&in_current->task[0] + 6 + size + 1, "%d ", &vector_2[i]);
        }

        for (i = 0; i < size; i++)
        {
          printf("vector_2[%d] = %d ", i, vector_2[i]);
        }
        printf("\n");

        // �������� switch ���������� �������� ������ (operation_type) � ������ �� case. � ���� ��� �������, �� ���� � ��� �����
        switch (operation_type)
        {
          case '+':
            // ��������� ������ ��������
            for (i = 0; i < size; i++)
            {
              vector_result[i] = vector_1[i] + vector_2[i];
            }

            convert_vector_to_str_vector(size, vector_1, vector_2,
                                         vector_result, vector_1_str,
                                         vector_2_str, vector_result_str);

            memset(line, 0, sizeof(line));
            sprintf(line, "���������: (%s) + (%s) = (%s) \n", vector_1_str, vector_2_str,
                    vector_result_str);
            //add_to_list(out_head, out_current, line);
            LIST *out_node = malloc(sizeof(LIST));
            out_node->task = strdup(line); //note : strdup is not standard function
            out_node->next = NULL;

            if (out_head == NULL)
            {
              out_current = out_head = out_node;
            }
            else
            {
              out_current = out_current->next = out_node;
            }

            break;
          case '-':
            // ��������� ��������� ��������
            for (i = 0; i < size; i++)
            {
              vector_result[i] = vector_1[i] - vector_2[i]; // ���������� ��� ����� � ����� � ���������� result
            }

            convert_vector_to_str_vector(size, vector_1, vector_2,
                                         vector_result, vector_1_str,
                                         vector_2_str, vector_result_str);

            memset(line, 0, sizeof(line));
            sprintf(line, "���������: (%s) + (%s) = (%s) \n", vector_1_str, vector_2_str,
                    vector_result_str);
            out_node->task = strdup(line); //note : strdup is not standard function
            out_node->next = NULL;

            if (out_head == NULL)
            {
              out_current = out_head = out_node;
            }
            else
            {
              out_current = out_current->next = out_node;
            }

            break;
          case '*':
            // ��������� ��������� ��������
            for (i = 0; i < size; i++)
            {
              vector_result[i] = vector_1[i] * vector_2[i];
            }

            convert_vector_to_str_vector(size, vector_1, vector_2,
                                         vector_result, vector_1_str,
                                         vector_2_str, vector_result_str);

            memset(line, 0, sizeof(line));
            sprintf(line, "���������: (%s) + (%s) = (%s) \n", vector_1_str, vector_2_str,
                    vector_result_str);
            //add_to_list(out_head, out_current, line);
            out_node->task = strdup(line); //note : strdup is not standard function
            out_node->next = NULL;

            if (out_head == NULL)
            {
              out_current = out_head = out_node;
            }
            else
            {
              out_current = out_current->next = out_node;
            }

            break;
          case '/':
            // ��������� ������� ��������
            for (i = 0; i < size; i++)
            {
              vector_result[i] = vector_1[i] / vector_2[i]; // ���������� ��� ����� � ����� � ���������� result
            }

            convert_vector_to_str_vector(size, vector_1, vector_2,
                                         vector_result, vector_1_str,
                                         vector_2_str, vector_result_str);

            memset(line, 0, sizeof(line));
            sprintf(line, "���������: (%s) + (%s) = (%s) \n", vector_1_str, vector_2_str,
                    vector_result_str);
            //add_to_list(out_head, out_current, line);
            out_node->task = strdup(line); //note : strdup is not standard function
            out_node->next = NULL;

            if (out_head == NULL)
            {
              out_current = out_head = out_node;
            }
            else
            {
              out_current = out_current->next = out_node;
            }

            break;
          default:
            puts(
                "������ - ������ ������������ ������ ��������. �������� ���� � ���������� �����");
            break;
        }
        // free - ����������� ������, ������� ��������, ��������� calloc
        free(vector_1);
        free(vector_2);
        free(vector_result);
        free(vector_1_str);
        free(vector_2_str);
        free(vector_result_str);

        break; // break ��������� � "case 'v':"
      }
      case 'p': // ������ � ��� ����������� � �������� �������� ��������
      {
        char str[128];
        int tmp, stacksize = 0;
        stack* head = NULL;

        memset(str, 0, sizeof(str));

        strcpy(str, &in_current->task[0] + 4);

        if (str[strlen(str)-1] == '\n')
        {
          str[strlen(str)-1]=0; // ������ ������� ������
        }

        printf("calc task: %s\n", str);

        for (i = 0; i < strlen(str); i++)
        {
          char symbol = str[i];
          char op;
          if (symbol == ' ')
          {
            continue;
          }

          if (check_number(symbol))
          {
            /* We have a valid number. */
            printf("digit symbol = %c\n", symbol);
            tmp = symbol - '0';
            push(&head, tmp);
            ++stacksize;
          }
          else
          {
            /* We have an operand */
            printf("operand symbol = %c\n", symbol);

            op = symbol;

            if (stacksize < need_operation(op))
            {
              printf("Too few arguments on stack.\n");
              exit(1);
            }

            push(&head, do_operation(op, &head));
            stacksize -= need_operation(op) - 1;
          }
        }

        if (stacksize != 1)
        {
          printf("Too many arguments on stack.\n");
          exit(1);
        }

        memset(line, 0, sizeof(line));
        sprintf(line, "���������: %s = %d \n", str, head->data);
        //add_to_list(out_head, out_current, line);
        out_node->task = strdup(line); //note : strdup is not standard function
        out_node->next = NULL;

        if (out_head == NULL)
        {
          out_current = out_head = out_node;
        }
        else
        {
          out_current = out_current->next = out_node;
        }
        break;
      }
      default:
        puts("������ - ����������� ������ ��� ������ � �����");
        break;
    }

  }

  for (out_current = out_head; out_current; out_current = out_current->next)
  {
    fprintf(out_file, "%s", out_current->task); // ����� �������� � �������� � �����. %.2f ��������, ��� ����� �������� ��� float, �� ������ 2 ������ ����� '.'
  }
  fclose(out_file);
  freeList(in_head);
  freeList(out_head);
  return 0;
}
