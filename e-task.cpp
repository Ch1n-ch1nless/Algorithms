#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

/*===Structs_&_Constants===*/

typedef struct Stack
{
    int*    data;
    size_t  size;
    size_t  capacity;
} Stack;

const size_t MAX_VALUE_OF_CAPACITY          = 1e5;
const size_t MAX_LENGTH_OF_BRACKET_SEQUENCE = 2 * 1e5;

const int TRUE  = 1;
const int FALSE = 0;

/*===Function_Declaration===*/

void    StackCtor(Stack* const stack);
void    StackDtor(Stack* const stack);

void    StackPush(Stack* const stack, const int number);
void    StackPop(Stack* const stack);

int     ConvertParenthesisToNumber(const char bracket);

/*===Function_Definition===*/

int main()
{
    Stack my_stack = {};    // <-+
    StackCtor(&my_stack);   // <-+- Create the stack.

    char bracket_sequence[MAX_LENGTH_OF_BRACKET_SEQUENCE] = {};

    scanf("%s", bracket_sequence);

    int index = 0;
    int SEQUENCE_IS_CORRECT = TRUE;

    while (bracket_sequence[index] != '\0')
    {
        int number = ConvertParenthesisToNumber(bracket_sequence[index]);

        if (number > 0)
        {
            StackPush(&my_stack, number);
        }
        else
        {
            if (my_stack.size == 0 || number + my_stack.data[my_stack.size-1] != 0)
            {
                SEQUENCE_IS_CORRECT = FALSE;
                break;
            }

            StackPop(&my_stack);
        }

        index++;
    }

    if (SEQUENCE_IS_CORRECT == TRUE && my_stack.size == 0)
    {
        printf("YES\n");
        StackDtor(&my_stack);
        return 0;
    }

    printf("NO\n");
    StackDtor(&my_stack);

    return 0;
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
    assert(stack);

    free(stack->data);

    stack->data     = NULL;
    stack->size     = 0;
    stack->capacity = 0;

}

void StackPush(Stack* const stack, const int number)
{
    assert(stack);

    stack->data[stack->size] = number;

    stack->size++;
}

void StackPop(Stack* const stack)
{
    assert(stack);

    if (stack->size == 0)
    {
        return;
    }
    
    stack->size--;
    stack->data[stack->size] = 0;
}

int ConvertParenthesisToNumber(const char bracket)
{
    switch(bracket)
    {
        case '(':
        {
            return 1;
        }
        case ')':
        {
            return -1;
        }
        case '{':
        {
            return 2;
        }
        case '}':
        {
            return -2;
        }
        case '[':
        {
            return 3;
        }
        case ']':
        {
            return -3;
        }

        default:
            assert(FALSE && "Incorrect symbol!\n");
            break;
    }
}

