#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

/*===Structs_&_Constants===*/

typedef struct StackElem
{
    int value;
    int min;
} stk_elem_t;

typedef struct Stack
{
    stk_elem_t* data;
    size_t      size;
    size_t      capacity;
} Stack;

typedef struct Queue
{
    Stack* stack_in;
    Stack* stack_out;
} Queue;

const size_t MAX_VALUE_OF_CAPACITY  = 1e6;

const char* const   ENQUEUE_CMD     = "enqueue";
const char* const   DEQUEUE_CMD     = "dequeue";
const char* const   FRONT_CMD       = "front";
const char* const   SIZE_CMD        = "size";
const char* const   CLEAR_CMD       = "clear";
const char* const   MIN_CMD         = "min";

const int OK    = 1;
const int ERROR = 0;

/*===Function_Declaration===*/

//<----------------------------------------------------- STACK_FUNCTIONS
void    StackCtor(Stack* const stack);
void    StackDtor(Stack* const stack);

int    StackPush(Stack* const stack, const int number);
int    StackPop(Stack* const stack, int* const buffer);
int    StackTop(Stack* const stack, int* const buffer);
int    StackClear(Stack* const stack);
int    StackGetMin(Stack* const stack, int* const buffer);
//------------------------------------------------------>

//<------------------------------------------------------ QUEUE_FUNCTIONS
void    QueueCtor(Queue* const queue);
void    QueueDtor(Queue* const queue);

void    QueuePush(Queue* const queue, const int number);
void    QueueReallocateStack(Queue* const queue);
void    QueuePop(Queue* const queue);
void    QueueTop(Queue* const queue);
void    QueueClear(Queue* const queue);
void    QueueGetMin(Queue* const queue);

/*===Function_Definition===*/

int main()
{
    char contest_command[10] = {};

    int  number = 0;    //number, which will be pushed
    int  number_of_commands = 0;

    Queue my_queue = {};    // <-+
    QueueCtor(&my_queue);   // <-+- Create the queue.

    scanf("%d", &number_of_commands); //Read the number of commands

    for (int i = 0; i < number_of_commands; i++)
    {
        scanf("%s", contest_command);   //Read the command

        if (strcmp(contest_command, ENQUEUE_CMD) == 0)
        {
            scanf("%d", &number);
            QueuePush(&my_queue, number);
        }
        else if (strcmp(contest_command, DEQUEUE_CMD) == 0)
        {
            QueuePop(&my_queue);
        }
        else if (strcmp(contest_command, FRONT_CMD) == 0)
        {
            QueueTop(&my_queue);
        }
        else if (strcmp(contest_command, SIZE_CMD) == 0)
        {
            printf("%ld\n", my_queue.stack_in->size + my_queue.stack_out->size);  //Print size of stack
        }
        else if (strcmp(contest_command, CLEAR_CMD) == 0)
        {
            QueueClear(&my_queue);
        }
        else 
        {
            QueueGetMin(&my_queue);
        }
    }

    QueueDtor(&my_queue);

    return 0;
}

/*================STACK_SEGMENT===============*/

void StackCtor(Stack* const stack)
{
    assert(stack);

    stack->size     = 0;
    stack->capacity = MAX_VALUE_OF_CAPACITY;

    stack->data = (stk_elem_t*) calloc(stack->capacity, sizeof(stk_elem_t));
}

void StackDtor(Stack* const stack)
{
    assert(stack);

    free(stack->data);

    stack->data     = NULL;
    stack->size     = 0;
    stack->capacity = 0;

}

int StackPush(Stack* const stack, const int number)
{
    assert(stack);

    stack->data[stack->size].value = number;

    if (stack->size == 0)
    {
        stack->data[stack->size].min = number;
        stack->size += 1;
        return OK;
    }

    stack->data[stack->size].min = (number < stack->data[stack->size - 1].min) ? number : stack->data[stack->size - 1].min;
    stack->size += 1;

    return OK;
}

int StackPop(Stack* const stack, int* const buffer)
{
    assert(stack);

    if (stack->size == 0)
    {
        return ERROR;
    }
    else
    {
        *buffer = stack->data[stack->size - 1].value;
        stack->size--;
        return OK;
    }
}

int StackTop(Stack* const stack, int* const buffer)
{
    assert(stack);

    if (stack->size == 0)
    {
        return ERROR;
    }
        
    *buffer = stack->data[stack->size - 1].value;
    return OK;
}

int StackGetMin(Stack* const stack, int* const buffer)
{
    assert(stack);

    if (stack->size == 0)
    {
        return ERROR;
    }

    *buffer = stack->data[stack->size - 1].min;
    return OK;
}

int StackClear(Stack* const stack)
{
    assert(stack);

    memset(stack->data, 0, stack->size * sizeof(stk_elem_t));
    stack->size = 0;

    return OK;
}

/*============================================*/

/*================QUEUE_SEGMENT===============*/

void QueueCtor(Queue* const queue)
{
    assert(queue);

    queue->stack_in  = (Stack*) calloc(1, sizeof(Stack));
    queue->stack_out = (Stack*) calloc(1, sizeof(Stack));

    StackCtor(queue->stack_in);
    StackCtor(queue->stack_out);
}

void QueueDtor(Queue* const queue)
{
    assert(queue);
    assert(queue->stack_in);
    assert(queue->stack_out);

    StackDtor(queue->stack_in);
    StackDtor(queue->stack_out);

    free(queue->stack_in);
    free(queue->stack_out);

    queue->stack_in  = NULL;
    queue->stack_out = NULL;
}

void QueuePush(Queue* const queue, const int number)
{
    assert(queue);
    assert(queue->stack_in);
    assert(queue->stack_out);

    if (StackPush(queue->stack_in, number) == ERROR)
    {
        printf("error\n");
        return;
    }

    printf("ok\n");
}

void QueueReallocateStack(Queue* const queue)
{
    assert(queue);
    assert(queue->stack_in);
    assert(queue->stack_out);

    int buffer = 0;

    while (StackPop(queue->stack_in, &buffer) != ERROR)
    {
        StackPush(queue->stack_out, buffer);
    }
}

void QueuePop(Queue* const queue)
{
    assert(queue);
    assert(queue->stack_in);
    assert(queue->stack_out);

    int buffer = 0;

    if (StackPop(queue->stack_out, &buffer) == ERROR)
    {
        QueueReallocateStack(queue);
        
        if (StackPop(queue->stack_out, &buffer) == ERROR)
        {
            printf("error\n");
            return;
        }
    }

    printf("%d\n", buffer);
}

void QueueTop(Queue* const queue)
{
    assert(queue);
    assert(queue->stack_in);
    assert(queue->stack_out);

    int buffer = 0;

    if (StackTop(queue->stack_out, &buffer) == ERROR)
    {
        QueueReallocateStack(queue);
        
        if (StackTop(queue->stack_out, &buffer) == ERROR)
        {
            printf("error\n");
            return;
        }
    }

    printf("%d\n", buffer);
}
void QueueClear(Queue* const queue)
{
    assert(queue);
    assert(queue->stack_in);
    assert(queue->stack_out);

    StackClear(queue->stack_in);
    StackClear(queue->stack_out);

    printf("ok\n");
}

void QueueGetMin(Queue* const queue)
{
    assert(queue);
    assert(queue->stack_in);
    assert(queue->stack_out);

    int buffer = 0;

    if (StackGetMin(queue->stack_out, &buffer) == ERROR)
    {
        if (StackGetMin(queue->stack_in, &buffer) == ERROR)
        {
            printf("error\n");
            return;
        }

        printf("%d\n", buffer);
        return;
    }

    int min_elem = buffer;

    if (StackGetMin(queue->stack_in, &buffer) == ERROR)
    {
        printf("%d\n", min_elem);
        return;
    }

    printf("%d\n", (buffer < min_elem) ? buffer : min_elem);
    return;
}

/*============================================*/
