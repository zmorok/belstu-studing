struct Stack {
    int data;
    Stack* next;
};

void push(Stack*& top, int data);
int pop(Stack*& top);
void clear(Stack*& top);
int deleteFirstElementMoreThan100(Stack*& top);
void saveToFile(Stack* top, const char* filename);
Stack* loadFromFile(const char* filename);
void printStack(Stack* top);