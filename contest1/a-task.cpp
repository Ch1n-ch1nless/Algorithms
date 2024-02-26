#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#define FALSE 0

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

void    StackCtor(Stack* const stack);
void    StackDtor(Stack* const stack);

void    StackPush(Stack* const stack, const int number);
void    StackPop(Stack* const stack);
void    StackTop(Stack* const stack);
void    StackClear(Stack* const stack);

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
    int  number = 0;    	  //number, which will be pushed
    
    //Read the first command
    if (scanf("%6s", contest_command) == 0)
    {
	assert(FALSE && "Program can not read the string!");
    }
    

    while (strcmp(contest_command, EXIT_CMD) != 0)
    {
        if (strcmp(contest_command, PUSH_CMD) == 0)
        {
            //Read the number
            if (scanf("%d", &number) == 0)
            {
              	assert(FALSE && "Program can not read the number");
            }
            StackPush(stack, number);
        }
        else if (strcmp(contest_command, POP_CMD) == 0)
        {
            StackPop(stack);
        }
        else if (strcmp(contest_command, BACK_CMD) == 0)
        {
            StackTop(stack);
        }
        else if (strcmp(contest_command, SIZE_CMD) == 0)
        {
            printf("%ld\n", stack->size);  //Print size of stack
        }
        else if (strcmp(contest_command, CLEAR_CMD) == 0)
        {
            StackClear(stack);
        }

        //Read next command
        if (scanf("%6s", contest_command) == 0)
    	{
	    assert(FALSE && "Program can not read the string!");
    	}
    }
}

void StackCtor(Stack* const stack)
{
    assert(stack);

    stack->size     = 0;
    stack->capacity = MAX_VALUE_OF_CAPACITY;

    stack->data = (int*) calloc(stack->capacity, sizeof(int));
}

void StackDtor(Stack* const stack)
{
    assert((stack != NULL) && "Pointer to stack is NULL!!!\n");

    free(stack->data);

    stack->data     = NULL;
    stack->size     = 0;
    stack->capacity = 0;

}

void StackPush(Stack* const stack, const int number)
{
    assert((stack != NULL) && "Pointer to stack is NULL!!!\n");

    stack->data[stack->size] = number;

    stack->size++;

    printf("ok\n");
}

void StackPop(Stack* const stack)
{
    assert((stack != NULL) && "Pointer to stack is NULL!!!\n");

    if (stack->size == 0)
    {
        printf("error\n");
    }
    else
    {
        printf("%d\n", stack->data[stack->size-1]);
        stack->size--;
    }
}

void StackTop(Stack* const stack)
{
    assert((stack != NULL) && "Pointer to stack is NULL!!!\n");

    if (stack->size == 0)
    {
        printf("error\n");
    }
    else
    {
        printf("%d\n", stack->data[stack->size-1]);
    }
}

void StackClear(Stack* const stack)
{
    assert((stack != NULL) && "Pointer to stack is NULL!!!\n");

    memset(stack->data, 0, stack->size);
    stack->size = 0;

    printf("ok\n");
}

