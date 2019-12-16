#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define assume_size 7
#define davai_size 5
#define sqr_size 4
#define fuck_size 5
#define pognali_size 8
#define exit_size 4

typedef char* elem_t;

char* s = NULL;

char* func_arr[10];
int func_count = 0;

enum types {
    condition = 1,
    var = 2,
    plus = 3,
    minus = 4,
    mul = 5,
    division = 6,
    number = 7,
    beginning = 8,
    less = 9,
    more = 10,
    equal = 11,
    assign = 12,
    operator = 13,
    davai = 14,
    connecting_op = 15,
    sqrt = 16,
    function = 17,
    param = 18,
    call_param = 19,
    call_func = 20,
    exit_program = 99
};

char* free_regs[10] = {"ax", "bx", "cx", "dx", "R1", "R2", "R3", "R4", "R5", "R6"};

int free_regs_count = 0;

char* getName();


struct Node {

    elem_t data;
    int type;
    struct Node *left;
    struct Node *right;
};

struct string {
    char* str;
    int length;
};

void get_str(char* buffer, struct string* place) {
    place->str = buffer;
    char* old = buffer;
    char* neww = strchr(buffer, '\n');
    while (place != NULL && neww != NULL) {
        *(neww) = '\0';
        ++neww;
        place->length = neww - old - 1;
        old = neww;
        neww = strchr(neww, '\n');
        ++place;
        place->str = old;
    }
}

FILE* open_file (char* name, char* type) {
    FILE* file;
    file = fopen(name, type);
    if (file == NULL) {
        printf("%s cannot be opened", name);
    }
    return file;
}


void close_file(FILE* file) {
    if (fclose(file)) {
        printf("File cannot be closed, error\n");
    }
    return;
}

int count_str (char* buffer, const int size) {
    int count = 1;
    char* p = strchr(buffer, '\n');
    while (p - buffer < size && p != NULL) {
        p = strchr(p + 1, '\n');
        ++count;
    }
    return count;
}

void copy_file(char* place, char* name) {
    int size = 0;
    FILE* file = open_file(name, "r");
    fseek(file, 0, SEEK_END);
    size = ftell(file);
    rewind(file);
    fread(place, sizeof(char), size, file);
    close_file(file);
    return;
}

int count_file(char* name) {
    FILE* file = open_file(name, "r");

    int count = 0;
    fseek(file, 0, SEEK_END);
    count = ftell(file);
    rewind(file);

    close_file(file);

    return count;
}

char* create_buffer(char* name) {
    FILE* file;
    int size = count_file(name);

    file = open_file(name, "r");
    char* buffer = (char*) calloc(size, sizeof(char));
    close_file(file);
    copy_file(buffer, name);

    return buffer;
}

void print_in_file(char* name, char const* buffer, int size) {
    FILE* file = open_file(name, "w");
    fwrite(buffer, sizeof(char), size, file);
    close_file(file);
    return;
}

struct Node* getE();
struct Node* GetV();
struct Node* GetEE();
struct Node* GetO();

struct Node* getN() {
    int size = 0;
    char* val;
    struct Node* val2 = (struct Node*) calloc(1, sizeof(struct Node));
    val = (char*) calloc(10, sizeof(char));

    char* op = val;
    do {
        *op = *s;
        op++;
        s++;
        ++size;
    } while (*s >= '0' && *s <= '9');

    val[size] = '\0';

    val2->type = number;
    val2->data = val;

    return val2;
}

struct Node* getP() {
    struct Node* val = NULL;
    struct Node* val2 = NULL;

    if (*s == '(') {
        ++s;
        val = getE();
        //val2 = (struct Node*) calloc(1, sizeof(struct Node));
        //val2->right = val;
        assert(*s == ')');
        s++;
        return val;
    }
    if (*s >= 'a' && *s <= 'z') {
        return GetV();
    }

    return getN();
}

//struct Node* getL() {
//    struct Node* val = NULL;
//    struct Node* val2 = NULL;
//    struct Node* val3 = NULL;
//
//    val = getP();
//
//    s++;
//    if (*s == '^') {
//        val2 = getP();
//
//        if (val3 != NULL) {
//            val3 = createNode(val3, val2, power);
//        } else {
//            val3 = createNode(val, val2, power);
//        }
//    }
//    if (val3 == NULL) {
//        return getP();
//    }
//    return val3;
//}

struct Node* getT() {

    struct Node* val1 = NULL;
    struct Node* val2 = NULL;
    struct Node* val3 = NULL;

    val1 = getP();

    while (*s == '*' || *s == '/') {
        char op = *s;
        s++;
        val2 = getP();

        val3 = (struct Node *) calloc(1, sizeof(struct Node));

        if (op == '*') {
            val3->type = mul;
        } else {
            val3->type = division;
        }

        struct Node *temp = val1;
        val1 = val3;
        val3 = temp;

        val1->right = val2;

        val1->left = val3;
    }

    if (val3 == NULL) {
        return val1;
    }
    return val1;
}
struct Node* getE() {

    struct Node* val1 = NULL;
    struct Node* val2 = NULL;
    struct Node* val3 = NULL;

    val1 = getT();

    while (*s == '+' || *s == '-') {
        char op = *s;
        s++;
        val2 = getT();

        val3 = (struct Node *) calloc(1, sizeof(struct Node));

        if (op == '+') {
            val3->type = plus;
        } else {
            val3->type = minus;
        }

        val3->data = 0;

        struct Node *temp = val1;
        val1 = val3;
        val3 = temp;

        val1->right = val2;

        val1->left = val3;
    }

    if (val3 == NULL) {
        return val1;
    }

    return val1;

}

struct Node* GetEE() {
    struct Node* val1 = NULL;
    struct Node* val2 = NULL;
    struct Node* val3 = NULL;

    val1 = getE();
    assert(*s == '<' || *s == '>' || *s == '$');
    val3 = (struct Node*) calloc(1, sizeof(struct Node));

    if (*s == '<') {
        ++s;
        val2 = getE();
        val3->type = less;
    }
    if (*s == '>') {
        ++s;
        val2 = getE();
        val3->type = more;
    }
    if (*s == '$') {
        ++s;
        val2 = getE();
        val3->type = equal;
    }


    val3->left = val1;
    val3->right = val2;

    return val3;
}

struct Node* GetV() {
    int size = 0;


    char* val = (char*) calloc(10, sizeof(char));

    char* op = val;
    do {
        *op = *s;
        op++;
        s++;
        ++size;
    } while (*s >= 'a' && *s <= 'z');
    val[size] = '\0';

    struct Node* val3 = (struct Node*) calloc(1, sizeof(struct Node));

    val3->type = var;
    val3->data = val;

    return val3;
}

struct Node* GetA() {
    struct Node* val1 = NULL;
    struct Node* val2 = NULL;
    struct Node *val3 = NULL;

    val1 = GetV();
    assert(*s == '=');
    ++s;


    if (*s == 's' && *(s + 1) == 'q' && *(s + 2) == 'r') {
        val2 = GetO();
    } else {
        val2 = getE();
    }

    val3 = (struct Node*) calloc(1, sizeof(struct Node));
    val3->type = assign;

    val3->left = val1;
    val3->right = val2;

    return val3;

}

struct Node* getIf() {
    struct Node* val1 = NULL;
    struct Node* val2 = NULL;
    struct Node *val3 = NULL;

    assert(*s == 'a' && *(s + 1) == 's');

    s += assume_size;

    val1 = GetEE();
    ++s;
//    while(*s == '\n' || *s == '}') {
//        ++s;
//    }

    if (*s == '{') {
        ++s;
        ++s;
        while (*s != '}')  {

            val2 = GetO();
            val3 = (struct Node *) calloc(1, sizeof(struct Node));
            val3->data = 0;
            val3->type = connecting_op;

            struct Node *temp = val1;
            val1 = val3;
            val3 = temp;

            val1->right = val2;

            val1->left = val3;
            ++s;
        }
        val1->type = condition;
        ++s;
        return val1;
    }
//    val2 = GetO();
//
//    val3 = (struct Node*) calloc(1, sizeof(struct Node));
//
//    val3->type = condition;
//    val3->data = "assume";
//
//    val3->left = val1;
//    val3->right = val2;

    return GetO();

}

struct Node* getD() {
    struct Node* val1 = NULL;
    s += davai_size;

    assert(*s == '(');
    val1 = getE();

    val1->type = davai;
    return val1;

}
struct Node* GetS() {
    struct Node* val1 = NULL;
    s += sqr_size;

    assert(*s == '(');
    val1 = getE();

    val1->type = sqrt;
    return val1;
}

struct Node* getParam() {
    struct Node* val1 = NULL;
    val1 = (struct Node *) calloc(1, sizeof(struct Node));

    val1->data = getName();
    val1->type = param;

    return val1;
}
struct Node* getParamCall() {
    struct Node* val1 = NULL;
    val1 = (struct Node *) calloc(1, sizeof(struct Node));

    val1->data = getName();
    val1->type = call_param;

    return val1;
}

struct Node* getParameters() {

    struct Node *val1 = NULL;
    struct Node *val2 = NULL;
    struct Node *val3 = NULL;


    val1 = getParamCall();
    while (*s != ')' && *(s + 1) != ')') {
        ++s;

        val2 = getParamCall();
        val3 = (struct Node *) calloc(1, sizeof(struct Node));
        val3->data = 0;
        val3->type = connecting_op;

        struct Node *temp = val1;
        val1 = val3;
        val3 = temp;

        val1->right = val2;

        val1->left = val3;
    }
    ++s;
    return val1;
}

struct Node* getCall(char* p) {
    struct Node *val1 = NULL;
    struct Node *val2 = NULL;
    struct Node *val3 = NULL;

    //s += strlen(p) - 1;
    char* param = NULL;
    //assert(*s == '(');
    //++s;

    val1 = (struct Node*) calloc(1, sizeof(struct Node));


    val1->type = call_func;
    val1->data = p;


    val1->right = getParameters();
    //s++;
    //val1->left = GetMO();
//    val1 = getParamCall();
//    while (*s != ')' && *(s + 1) != ')') {
//        ++s;
//
//        val2 = getParamCall();
//        val3 = (struct Node *) calloc(1, sizeof(struct Node));
//        val3->data = 0;
//        val3->type = connecting_op;
//
//        struct Node *temp = val1;
//        val1 = val3;
//        val3 = temp;
//
//        val1->right = val2;
//
//        val1->left = val3;
//    }
//    ++s;
    return val1;
}

struct Node* GetEx() {
    struct Node* val1 = NULL;

    val1 = (struct Node*) calloc(1, sizeof(struct Node));

    val1->type = exit_program;
    return val1;
}

struct Node* GetO() {

    struct Node *val1 = NULL;
    struct Node *val2 = NULL;
    struct Node *val3 = NULL;



    if (*s == '{') {
        s += 2;
    }

    char* p = NULL;
    if (*s == ':') {
        ++s;
        p = getName();
        ++s;
        for (int i = 0; i < 10; ++i) {
            if (strcmp(func_arr[i], p) == 0) {
                val1 = getCall(p);
                return val1;
            }
        }
    }

//    if (*s == '\n') {
//        ++s;
//    }

    if (*s == 'a' && *(s + 1) == 's') {
        val1 = getIf();
    } else if (*s == 'd' && *(s + 1) == 'a') {
        val1 = getD();
    } else if (*s == 's' && *(s + 1) == 'q') {
        val1 = GetS();
    } else if (*s == 'e' && *(s + 1) == 'x' && *(s + 2) == 'i' && *(s + 3) == 't') {
        val1 = GetEx();
        s += exit_size;
    } else {
        val1 = GetA();
    }
    assert(*s == '\n' || *s == '#');
    return val1;
}

struct Node* GetMO() {

    struct Node *val1 = NULL;
    struct Node *val2 = NULL;
    struct Node *val3 = NULL;

    if (*s == '{') {

        val1 = GetO();
        while (*s != '}' && *(s + 1) != '}') {
            ++s;

            val2 = GetO();
            val3 = (struct Node *) calloc(1, sizeof(struct Node));
            val3->data = 0;
            val3->type = connecting_op;

            struct Node *temp = val1;
            val1 = val3;
            val3 = temp;

            val1->right = val2;

            val1->left = val3;
        }

        return val1;
    } else {
        return GetO();
    }
}

char* getName() {
    char* p = NULL;
    char* name = (char*) calloc(10, sizeof(char));

    p = name;

    while (*s != ')' && *s != ',' && (*s != '(') && (*s != '\n')) {
        *p = *s;
        ++p;
        ++s;
    }
    *p = '\0';
    return name;
}

struct Node* GetPar() {
    struct Node* val1 = NULL;
    ++s;

    val1 = (struct Node*) calloc(1, sizeof(struct Node));
    char* p = NULL;
    p = getName();

    assert(*s == ',' || *s == ')');
    val1->type = param;
    val1->data = p;
    return val1;

}

struct Node* GetMPAR() {

    struct Node* val1 = NULL;
    struct Node* val2 = NULL;
    struct Node* val3 = NULL;


    val1 = GetPar();

    while (*s != ')') {

        val2 = GetPar();

        val3 = (struct Node *) calloc(1, sizeof(struct Node));

        val3->data = 0;
        val3->type = connecting_op;

        struct Node *temp = val1;
        val1 = val3;
        val3 = temp;

        val1->right = val2;

        val1->left = val3;
    }
    return val1;

}

struct Node* GetF() {

    struct Node *val1 = NULL;

    val1 = (struct Node*) calloc(1, sizeof(struct Node));
    //val1->left = (struct Node*) calloc(1, sizeof(struct Node));
    //val1->right = (struct Node*) calloc(1, sizeof(struct Node));

    char* p = NULL;
    p = getName();

    val1->type = function;
    val1->data = p;

    func_arr[func_count] = p;
    ++func_count;

    val1->right = GetMPAR();
    s++;
    val1->left = GetMO();

    return val1;


}

struct Node* GetMF() {

    struct Node *val1 = NULL;
    struct Node *val2 = NULL;
    struct Node *val3 = NULL;

    s += fuck_size;

    val1 = GetF();
    while (*s == '\n' || *s == '}') {
        s++;
    }

    while (*s == 'f' && *(s + 1) == 'u' && *(s + 2) == 'c' && *(s + 3) == 'k') {

        s += fuck_size;
        //while (*s != '*' && *(s + 1) != '*' && *(s + 2) != '*') {

                val2 = GetF();
                val3 = (struct Node *) calloc(1, sizeof(struct Node));
                val3->data = 0;
                val3->type = connecting_op;

                struct Node *temp = val1;
                val1 = val3;
                val3 = temp;

                val1->right = val2;

                val1->left = val3;
                //s += 3;

            return val1;
        }
    return val1;
}

struct Node* GetG(struct Node* tree) {

    struct Node *val1 = NULL;
    struct Node *val2 = NULL;
    struct Node *val3 = NULL;

    val1 = GetMF();

    while(*s == '\n' || *s == '}') {
        ++s;
    }

    while (*s != '#') {

//        while(*s == '\n' || *s == '}') {
//            ++s;
//        }
//        if (*s == 'f' && *(s + 1) == 'u' && *(s + 2) == '*') {
//
//        }
       // ++s; // careful
        val2 = GetMF();

        val3 = (struct Node *) calloc(1, sizeof(struct Node));

        val3->data = 0;
        val3->type = connecting_op;

        struct Node *temp = val1;
        val1 = val3;
        val3 = temp;

        val1->right = val2;

        val1->left = val3;
//        if (*s == '#' || *(s + 1) == '#' || *(s + 2) == '#') {
//            s += 2;
//            break;
//        }
    }
//    if (val3 == NULL) {
//
//        val3 = (struct Node *) calloc(1, sizeof(struct Node));
//        val3->data = 0;
//        val3->type = operator;
//
//        struct Node *temp = val1;
//        val1 = val3;
//        val3 = temp;
//
//        val1->right = val2;
//
//        val1->left = val3;
//    }
    assert(*s == '#');

    return val1;
}


void calculate_tree(struct Node* tree, char* array_labels[], FILE* outfile) {

    static char* temp = NULL;

    static int i = 0;
    char* op = NULL;
    char* c = NULL;

    if (tree->type == function) {
        free_regs_count = 0;
        //fprintf(outfile, "jmp main\n");
        fprintf(outfile, ":%s\n", tree->data);
    }

    if (tree->left) {
        calculate_tree(tree->left, array_labels, outfile);
    }

    if (tree->right) {
        calculate_tree(tree->right, array_labels, outfile);
    }
    if (tree->type == function) {
        //free_regs_count = 0;
        //fprintf(outfile, "jmp main\n");
        fprintf(outfile, "ret\n");
    }

    switch (tree->type) {
        case var:
            fprintf(outfile, "push %s\n", tree->data);
            break;
        case less:
            if (tree->type == less) {

                fprintf(outfile, "jb %s\n", array_labels[i]);
                temp = array_labels[i];
            }
            break;
        case more:
            if (tree->type == more) {

                fprintf(outfile, "ja %s\n", array_labels[i]);
                temp = array_labels[i];
            }
            break;
        case equal:
            if (tree->type == equal) {

                fprintf(outfile, "je %s\n", array_labels[i]);
                temp = array_labels[i];
            }
            break;

        case condition:
            fprintf(outfile, ":%s\n", temp);//todo
            ++i;
            break;
        case plus:
            fprintf(outfile, "add\n");
            break;
        case assign:
            fprintf(outfile, "pop %s\n", tree->left->data);//todo
            break;
        case mul:
            fprintf(outfile, "mul\n");
            break;
        case division:
            fprintf(outfile, "dvd\n");
            break;
        case number:
            if (tree->data != NULL) {
                fprintf(outfile, "push %s\n", tree->data);
            }
            break;
        case call_param:
            fprintf(outfile, "push %s\n", tree->data);
            fprintf(outfile, "pop %s\n", free_regs[free_regs_count]);
            free_regs_count++;
            break;
        case call_func:
            fprintf(outfile, "call %s\n", tree->data);
            free_regs_count = 0;
            break;
        case davai:
            fprintf(outfile, "out %s\n", tree->data);
            break;
        case sqrt:
            fprintf(outfile, "sqrt %s\n", tree->data);
            break;
        case minus:
            fprintf(outfile, "sub\n");
            break;
        case exit_program:
            fprintf(outfile, "end\n");
            break;

    }

}




int main() {

    char* buffer = create_buffer("input.txt");
    s = buffer;
    int file_size = count_file("input.txt");

    int var_count = 0;

    char* p = NULL;

    p = s;

    while ((*p == 'p' && *(p + 1) == 'o' && *(p + 2) == 'g') == 0) {
        while (*p != '\n') {
            ++p;
        }
        ++p;
        ++var_count;
    }
    char** vars = (char**) calloc(var_count, sizeof(char*));

    for (int i = 0; i < var_count; ++i) {
        vars[i] = (char*) calloc(15, sizeof(char));
    }

    char* var = NULL;
    p = s;
    var = p;

    int count = 0;

    for (int i = 0; i < var_count; ++i) {
        s += 3;
        while (*s != '\n') {
            *p = *s;
            ++p;
            ++s;
        }
        *p = '\0';
        vars[count] = var;
        p = s;
        var = p;
        ++s;
        ++count;
    }

    FILE* vars_file = fopen("vars.txt", "w");
    fprintf(vars_file, "%d\n", var_count);
    for (int i = 0; i < var_count; ++i) {
        fprintf(vars_file, "%s\n", vars[i]);
    }
    fclose(vars_file);
    s += pognali_size;


    for (int i = 0; i < 10; ++i) {
        func_arr[i] = (char*) calloc(50, sizeof(char));
    }

    char* array_labels[10] = {"lab1", "lab2","lab3", "lab4", "lab5", "lab6", "lab7", "lab8","lab9","lab10"};


    struct Node tree = *GetG(&tree);

    FILE* outfile = open_file("outfile.txt", "w");
    fprintf(outfile, "jmp main\n");
    calculate_tree(&tree, array_labels, outfile);
    fprintf(outfile, "end");
    fclose(outfile);



    return 0;
}