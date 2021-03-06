#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/*
 * main.c
 *
 *  Created on: 31 мая 2019 г.
 *      Author: anna
 */

#include <stdio.h>
/
#include <locale.h> 
#include <stdlib.h> 

struct element
{
  char *task;
  struct element *next;
};

typedef struct element LIST_NODE;
typedef struct element QUEUE_NODE;

typedef struct stack
{
  int data;
  struct stack *next;
} stack;

// описываем очередь
struct Queue
{
  QUEUE_NODE *queueHead;
  QUEUE_NODE *queueTail;
  unsigned int size;
};

void init_queue(struct Queue *q)
{
  q->queueHead = NULL;
  q->queueTail = NULL;
  q->size = 0;
}

char* queueHead(struct Queue *q)
{
  return q->queueHead->task;
}

QUEUE_NODE* getElement(struct Queue *q)
{
  if (q->size > 0)
  {
    q->size--;

    QUEUE_NODE *tmp = q->queueHead;
    q->queueHead = q->queueHead->next;
    return tmp;
  }
  return NULL;
}

void putElement(struct Queue *q, char* task)
{
  q->size++;

  if (q->queueHead == NULL)
  {
    q->queueHead = (QUEUE_NODE *) malloc(sizeof(QUEUE_NODE));
    q->queueHead->task = task;
    q->queueHead->next = NULL;
    q->queueTail = q->queueHead;
  }
  else
  {
    q->queueTail->next = (QUEUE_NODE *) malloc(sizeof(QUEUE_NODE));
    q->queueTail->next->task = task;
    q->queueTail->next->next = NULL;
    q->queueTail = q->queueTail->next;
  }
}

// объявляем функции для работы с листом
void freeList(struct element* head);
//void add_to_list(LIST_NODE *head, LIST_NODE *current, char *line);

// объявляем функции для работы с векторами
void convert_vector_to_str_vector(int size, int *vector_1, int *vector_2,
                                  float *vector_result, char *vector_1_str,
                                  char *vector_2_str, char *vector_result_str);

// объявляем функции для работы со стеком
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

void freeList(LIST_NODE *head)
{
  LIST_NODE *tmp;

  while (head != NULL)
  {
    tmp = head;
    head = head->next;
    free(tmp);
  }

}

void add_to_list(LIST_NODE *head, LIST_NODE *current, char *line)
{
  LIST_NODE *node = malloc(sizeof(LIST_NODE));
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
  // выводим результат
  int cur_pos = 0;
  int i = 0;
  memset(vector_1_str, 0, 255);
  memset(vector_2_str, 0, 255);
  memset(vector_result_str, 0, 255);
  // пишем значения 1 вектора через пробел

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
  LIST_NODE *in_current, *in_head;
  LIST_NODE *out_current, *out_head;
  struct Queue in_q;
  struct Queue out_q;
  char operation_type = 0; // в эту переменную сохранится тип операции. 
  char data_type = 0; // в эту переменную сохраним какой тип данных содержится в файле после того как его прочитаем
  int i = 0; // переменная, которую будем использовать как счетчик, для операций
  // начинаем работать с файлами
  // сначало нужно объявить файловые дескрипторы для входного и выходного файла. 
  FILE *in_file;
  FILE *out_file; // дескриптор выходного файла (куда запишем результат)

  setlocale(LC_ALL, "Russian"); // включаем поддержку русских букв в консоли, т.к. консолью будем пользоваться для вывода всяких полезных сообщений

  in_head = in_current = NULL;
  out_head = out_current = NULL;
  in_file = fopen("input.txt", "r");
  out_file = fopen("output.txt", "w");
  memset(line, 0, sizeof(line));

  // инициализируем очередь
  init_queue(&in_q);
  init_queue(&out_q);

  while (fgets(line, sizeof(line), in_file))
  {
    putElement(&in_q, strdup(line));
  }
  fclose(in_file);

  while (in_q.size > 0)
  {
    QUEUE_NODE* cur_node = getElement(&in_q);

    printf("Current task: %s\n", cur_node->task);

    sscanf(cur_node->task, "%c %c", &operation_type, &data_type);

    printf("operation_type= %c\n", operation_type);
    printf("data_type= %c\n", data_type);

    switch (data_type)
    // теперь смотрим, какой же у нас тип данных в файле, который мы вычитали
    {
      case 's': // значит у нас обычные числа в файле
      {
        int number_1; // теперь говорим, что в нашей программе будет использоваться переменная number_1 типа int (integer, т.е. целое число. Целое число это 1,2,3,4,5..N)
        int number_2; // в нашей программе будет переменная number_2
        float result; // в эту переменную сохраним результат операции. Она типа float - десятичное ну или дробное число, например "2.342"

        sscanf(&cur_node->task[0] + 4, "%d %d", &number_1, &number_2);

        switch (operation_type)
        {
          case '+':
            result = number_1 + number_2; // складываем два числа и пишем в переменную result
            // функция fprintf работает точно так же как функция printf, только выводит результат не в консоль, а в файл, поэтому в нее нужно передать дескриптор на файл, куда хотим записать информацию
            memset(line, 0, sizeof(line));
            sprintf(line, "результат: %d + %d = %f \n", number_1, number_2,
                    result);
            putElement(&out_q, strdup(line));

            break;
          case '-':
            result = number_1 - number_2;
            memset(line, 0, sizeof(line));
            sprintf(line, "результат: %d - %d = %f \n", number_1, number_2,
                    result);
            putElement(&out_q, strdup(line));

            break;
          case '*':
            result = number_1 * number_2;
            memset(line, 0, sizeof(line));
            sprintf(line, "результат: %d * %d = %f \n", number_1, number_2,
                    result);
            putElement(&out_q, strdup(line));

            break;
          case '/':
            result = number_1 / number_2;
            memset(line, 0, sizeof(line));
            sprintf(line, "результат: %d / %d = %f \n", number_1, number_2,
                    result);
            putElement(&out_q, strdup(line));

            break;
          case '^':
            result = 1; // нужно задать значение result, чтобы не испортить первый шаг цикла
            
            for (i = 0; i < number_2; i++)
            {
              result = result * number_1;
            }
            memset(line, 0, sizeof(line));
            sprintf(line, "результат: %d ^ %d = %f \n", number_1, number_2,
                    result);
            putElement(&out_q, strdup(line));

            break;
          case '!':
            if (number_1 == 0) // если number == 0, то факториал считать не нужно, ответ 1
            {
              result = 1;
            }
            else // если number != 0
            {
              result = 1;
              for (i = 1; i <= number_1; i++)
              {
                result = result * i;
              }
            }
            memset(line, 0, sizeof(line));
            sprintf(line, "результат: %d! = %f \n", number_1, result);
            putElement(&out_q, strdup(line));

            break;
          default:
            puts(
                "Ошибка - введен недопустимый символ операции. Исправте файл и попробуйте снова");
            break;
        }
        break; // break относится к "case 's':", т.е. выходим из него
      }
      case 'v': // значит у нас векторы в файле
      {
        int *vector_1; // указатель на первый вектор. В этой переменной хранится адрес начала массива
        int *vector_2; // указатель на второй вектор. В этой переменной хранится адрес начала массива
        float *vector_result; // указатель на вектор, содержащий результат. В этой переменной хранится адрес начала массива
        char *vector_1_str; // указатель на первый вектор. В этой переменной хранится адрес начала массива
        char *vector_2_str; // указатель на второй вектор. В этой переменной хранится адрес начала массива
        char *vector_result_str; // указатель на вектор, содержащий результат. В этой переменной хранится адрес начала массива
        int size = 0; // размер всех векторов. Прочитаем его из файла

        sscanf(&cur_node->task[0] + 4, "%d", &size);

        // выделяем память под векторы
        vector_1 = calloc(size, sizeof(int)); // функция calloc выделяет память для вектора. size - количество элементов, sizeof(int) - размер одного элемента
        vector_2 = calloc(size, sizeof(int)); // функция calloc выделяет память для вектора. size - количество элементов, sizeof(int) - размер одного элемента
        vector_result = calloc(size, sizeof(float)); // функция calloc выделяет память для вектора. size - количество элементов, sizeof(int) - размер одного элемента
        vector_1_str = calloc(255, sizeof(char)); // функция calloc выделяет память для вектора. size - количество элементов, sizeof(int) - размер одного элемента
        vector_2_str = calloc(255, sizeof(char)); // функция calloc выделяет память для вектора. size - количество элементов, sizeof(int) - размер одного элемента
        vector_result_str = calloc(255, sizeof(char)); // функция calloc выделяет память для вектора. size - количество элементов, sizeof(int) - размер одного элемента

        // читаем значения 1 вектора из файла
        for (i = 0; i < size; i++)
        {
          sscanf(&cur_node->task[0] + 6, "%d ", &vector_1[i]);
        }

        for (i = 0; i < size; i++)
        {
          printf("vector_1[%d] = %d ", i, vector_1[i]);
        }
        printf("\n");

        // читаем значения 2 вектора из файла
        for (i = 0; i < size; i++)
        {
          sscanf(&cur_node->task[0] + 6 + size + 1, "%d ", &vector_2[i]);
        }

        for (i = 0; i < size; i++)
        {
          printf("vector_2[%d] = %d ", i, vector_2[i]);
        }
        printf("\n");

        // оператор switch сравнивает значение внутри (operation_type) с каждым из case. И если оно совпало, то идет в эту ветку
        switch (operation_type)
        {
          case '+':
            // выполняем сложее векторов
            for (i = 0; i < size; i++)
            {
              vector_result[i] = vector_1[i] + vector_2[i];
            }

            convert_vector_to_str_vector(size, vector_1, vector_2,
                                         vector_result, vector_1_str,
                                         vector_2_str, vector_result_str);

            memset(line, 0, sizeof(line));
            sprintf(line, "результат: (%s) + (%s) = (%s) \n", vector_1_str,
                    vector_2_str, vector_result_str);
            putElement(&out_q, strdup(line));

            break;
          case '-':
            // выполняем вычитание векторов
            for (i = 0; i < size; i++)
            {
              vector_result[i] = vector_1[i] - vector_2[i]; // складываем два числа и пишем в переменную result
            }

            convert_vector_to_str_vector(size, vector_1, vector_2,
                                         vector_result, vector_1_str,
                                         vector_2_str, vector_result_str);

            memset(line, 0, sizeof(line));
            sprintf(line, "результат: (%s) + (%s) = (%s) \n", vector_1_str,
                    vector_2_str, vector_result_str);
            putElement(&out_q, strdup(line));

            break;
          case '*':
            // выполняем умножение векторов
            for (i = 0; i < size; i++)
            {
              vector_result[i] = vector_1[i] * vector_2[i];
            }

            convert_vector_to_str_vector(size, vector_1, vector_2,
                                         vector_result, vector_1_str,
                                         vector_2_str, vector_result_str);

            memset(line, 0, sizeof(line));
            sprintf(line, "результат: (%s) + (%s) = (%s) \n", vector_1_str,
                    vector_2_str, vector_result_str);
            putElement(&out_q, strdup(line));

            break;
          case '/':
            // выполняем деление векторов
            for (i = 0; i < size; i++)
            {
              vector_result[i] = vector_1[i] / vector_2[i]; // складываем два числа и пишем в переменную result
            }

            convert_vector_to_str_vector(size, vector_1, vector_2,
                                         vector_result, vector_1_str,
                                         vector_2_str, vector_result_str);

            memset(line, 0, sizeof(line));
            sprintf(line, "результат: (%s) + (%s) = (%s) \n", vector_1_str,
                    vector_2_str, vector_result_str);
            putElement(&out_q, strdup(line));

            break;
          default:
            puts(
                "Ошибка - введен недопустимый символ операции. Исправте файл и попробуйте снова");
            break;
        }
        // free - освобождает память, которую выделили, используя calloc
        free(vector_1);
        free(vector_2);
        free(vector_result);
        free(vector_1_str);
        free(vector_2_str);
        free(vector_result_str);

        break; // break относится к "case 'v':"
      }
      case 'p': // значит у нас калькулятор с обратной польской нотацией
      {
        char str[128];
        int tmp, stacksize = 0;
        stack* head = NULL;

        memset(str, 0, sizeof(str));

        strcpy(str, &cur_node->task[0] + 4);

        if (str[strlen(str) - 1] == '\n')
        {
          str[strlen(str) - 1] = 0; // уберем перенос строки
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
        sprintf(line, "результат: %s = %d \n", str, head->data);
        putElement(&out_q, strdup(line));

        break;
      }
      default:
        puts("Ошибка - неправильно введен тип данных в файле");
        break;
    }
    free(cur_node);
  }

  while (out_q.size > 0)
  {
    QUEUE_NODE *cur_node = getElement(&out_q);
    fprintf(out_file, "%s", cur_node->task); // пишем значение с пробелом в конце. %.2f означает, что нужно записать тип float, но только 2 знакак после '.'
    free(cur_node);
  }

  fclose(out_file);
  freeList(in_head);
  freeList(out_head);
  return 0;
}
