struct Node {
    int data;
    Node* next;
};

struct Stack {
    Node* top;
};

Stack* createStack();
void push(Stack* stack, int data);
int pop(Stack* stack);
void clear(Stack* stack);
void saveToFile(Stack* stack, const char* filename);
Stack* loadFromFile(const char* filename);
Stack* loadFromFile(Stack* stack, const char* filename);
void printStack(Stack* stack);
void printStack(Stack* stack, Stack* stack1, Stack* stack2);
bool isExistInStack(Stack* stack, int data);