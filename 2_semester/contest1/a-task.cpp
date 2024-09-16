#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#define FALSE   0
#define ERROR   0
#define OK      1

/*===Structs_&_Constants===*/

typedef struct Stack
{
    int*    data;
    size_t  size;
    size_t  capacity;
} Stack;

const size_t MAX_VALUE_OF_CAPACITY  = 1e5;

const char* const   PUSH_CMD   = "push";
const char* const   POP_CMD    = "pop";
const char* const   BACK_CMD   = "back";
const char* const   SIZE_CMD   = "size";
const char* const   CLEAR_CMD  = "clear";
const char* const   EXIT_CMD   = "exit";

/*===Function_Declaration===*/

void 	ReadCommands(Stack* const stack);

int     StackCtor(Stack* const stack);
int     StackDtor(Stack* const stack);

int     StackPush(Stack* const stack, const int number);
int     StackPop(Stack* const stack, int* const buffer);
int     StackTop(Stack* const stack, int* const buffer);
int     StackClear(Stack* const stack);

/*===Function_Definition===*/

int main()
{
    Stack my_stack = {};    // <-+
    StackCtor(&my_stack);   // <-+- Create the stack.

    ReadCommands(&my_stack);

    StackDtor(&my_stack);

    printf("bye\n");

    return 0;
}

void ReadCommands(Stack* const stack)
{
    assert(stack);
    
    char contest_command[6] = {}; //Char array, in which the command is stored
    int  buffer = 0;    	      //number, which will be pushed
    
    do
    {
        //Read the command
        if (scanf("%6s", contest_command) == 0)
        {
	        assert(FALSE && "Program can not read the string!");
        }

        if (strcmp(contest_command, PUSH_CMD) == 0)
        {
            //Read the number
            if (scanf("%d", &buffer) == 0)
            {
              	assert(FALSE && "Program can not read the number");
            }
            if (StackPush(stack, buffer) == OK)
            {
                printf("ok\n");
            }
        }
        else if (strcmp(contest_command, POP_CMD) == 0)
        {
            if (StackPop(stack, &buffer) == ERROR)
            {
                printf("error\n");
            }
            else
            {
                printf("%d\n", buffer);
            }
        }
        else if (strcmp(contest_command, BACK_CMD) == 0)
        {
            if (StackTop(stack, &buffer) == ERROR)
            {
                printf("error\n");
            }
            else
            {
                printf("%d\n", buffer);
            }
        }
        else if (strcmp(contest_command, SIZE_CMD) == 0)
        {
            printf("%ld\n", stack->size);  //Print size of stack
        }
        else if (strcmp(contest_command, CLEAR_CMD) == 0)
        {
            if (StackClear(stack) == OK)
            {
                printf("ok\n");
            }
        }
    }
    while (strcmp(contest_command, EXIT_CMD) != 0);
}

int StackCtor(Stack* const stack)
{
    assert(stack);

    stack->size     = 0;
    stack->capacity = MAX_VALUE_OF_CAPACITY;

    stack->data = (int*) calloc(stack->capacity, sizeof(int));
    assert((stack->data != NULL) && "Program can not allocate memory!\n");

    return OK;
}

int StackDtor(Stack* const stack)
{
    assert((stack != NULL) && "Pointer to stack is NULL!!!\n");

    free(stack->data);

    stack->data     = NULL;
    stack->size     = 0;
    stack->capacity = 0;

    return OK;

}

int StackPush(Stack* const stack, const int number)
{
    assert((stack != NULL) && "Pointer to stack is NULL!!!\n");

    stack->data[stack->size] = number;

    stack->size++;

    return OK;
}

int StackPop(Stack* const stack, int* const buffer)
{
    assert((stack != NULL) && "Pointer to stack is NULL!!!\n");

    if (stack->size == 0)
    {
        return ERROR;
    }
    else
    {
        *buffer = stack->data[stack->size-1];
        stack->size--;
        return OK;
    }
}

int StackTop(Stack* const stack, int* const buffer)
{
    assert((stack != NULL) && "Pointer to stack is NULL!!!\n");

    if (stack->size == 0)
    {
        return ERROR;
    }
    else
    {
        *buffer = stack->data[stack->size-1];
        stack->size--;
        return OK;
    }
}

int StackClear(Stack* const stack)
{
    assert((stack != NULL) && "Pointer to stack is NULL!!!\n");

    memset(stack->data, 0, stack->size);
    stack->size = 0;

    return OK;
}

