struct Stack {
    char data;
    Stack* next;
};

void push(Stack*& top, char data);
int pop(Stack*& top);
void clear(Stack*& top);
bool hasDuplicateNext(Stack* top);
void saveToFile(Stack* top, const char* filename);
Stack* loadFromFile(const char* filename);
void printStack(Stack* top);