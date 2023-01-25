#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <ctype.h>

#define SIZE_MAX 512                                // Arbitrary large number
size_t INPUT_BUFFER_SIZE = SIZE_MAX;                // Large input buffer
char error_message[30] = "An error has occurred\n"; // Stock error message
int MODE = 0;                                       // 0 is interactive, > 1 is batch mode
const char* delim = " \n\t\a\r";                    // Delimiters for strtok()
char* cmd_arr[SIZE_MAX];                            // Array for holding tokenized commands
char* path_arr[SIZE_MAX];                           // Array for holding paths

/////////////////////////
// FUNCTION PROTOTYPES //
/////////////////////////
void clean_exit(int code);
void print_error();
void initialize_path();
void clear_cmd_arr();
int process_cmd(char* in_buffer, int redirection, char* redirect_file);
int execute_program(char* cmds[SIZE_MAX], int redirection, char* redirect_file);
void redirect(char* in_buffer);
int check_if_cmd(char* cmd);

int main(int argc, char *argv[]){
    if(argc > 2){
        print_error();
        clean_exit(1);
    }

    initialize_path();

    MODE = (argc - 1); // Set interactive or batch mode with number of args

    // Open file for batch mode
    FILE* batch = fopen(argv[1], "r");
    if(MODE == 1 && batch == NULL){
        print_error();
        clean_exit(1);
    }
    FILE* input = batch ? batch : stdin;

    while(1){
        clear_cmd_arr();
        
        char* in_buffer; // Input characters get stored here
        if (MODE == 0) printf("wish> ");
        
        
        size_t input_count; // The number of characters read
        input_count = getline(&in_buffer, &INPUT_BUFFER_SIZE, input);
        
        if(input_count == EOF){ //EOF HIT
            clean_exit(0);
        }

        process_cmd(in_buffer, 0, NULL);
        
    }
}

int process_cmd(char* in_buffer, int redirection, char* redirect_file){    
    
    char input_copy[SIZE_MAX];
    strcpy(input_copy, in_buffer);

    char input_copy2[SIZE_MAX];
    strcpy(input_copy2, in_buffer);

    int num_if = check_if_cmd(input_copy);

    // Check for valid IF statement and count number of nested IFs
    if(num_if == -1){
        print_error();
        return -1;
    }

    // Check for valid redirect statement
    // If valid, execute redirection (which calls process_cmd() again).
    char* redirect_char = strchr(in_buffer, '>');
    if(redirect_char && (num_if < 1)){
        redirect(in_buffer);
        return -1;
    }

    char* token = strtok(in_buffer, delim);
    int i = 0;
    while(token != NULL){
        cmd_arr[i] = token;
        i++;
        token = strtok(NULL, delim);
        cmd_arr[i+1] = NULL;
    }

    if(num_if > 0){
        // Get the command between the first if and (== or !=)
            // Might have args, so everything in between. Args are space separated in all test cases

        // Loop from cmd_arr[1] to when cmd_arr[i]=="==" or "!="
        // concatenate a string into full cmd

        // Finds last non-Null entry in array
        int j = 0;
        while(cmd_arr[j]){
            j++;
        }
        // Checks if last entry is fi
        if(strcmp(cmd_arr[j-1], "fi") != 0){
            print_error();
            return 0;
        }

        char * next_cmd = strstr(input_copy2,"then ") + strlen("then ");
        next_cmd[strlen(next_cmd) - strlen(" fi")] = '\0';

        char extracted_cmd[SIZE_MAX] = "";
        char operator[10];
        char constant_str[100];
        int constant;
        for (int i = 1; cmd_arr[i]; i++){
            if(!((strcmp(cmd_arr[i], "==") == 0) || (strcmp(cmd_arr[i], "!=") == 0))){
                strcat(extracted_cmd, cmd_arr[i]);
                strcat(extracted_cmd, " ");
            } else {
                strcpy(operator, cmd_arr[i]);
                strcpy(constant_str, cmd_arr[i+1]);
                constant = atoi(constant_str);
                break;
            }
        }
        int ret_code = process_cmd(extracted_cmd, 0, NULL);

        if(strcmp(operator, "==") == 0){
            if (ret_code == constant){
                process_cmd(next_cmd, 0, NULL);
            }
        } else{
            if (ret_code != constant){
                process_cmd(next_cmd, 0, NULL);
            }
        }
        return 0;
    }

    if(redirection && cmd_arr[0] == NULL){
        print_error();
        return -1;
    }

    ///////////////////////
    // Built-in Commands //
    ///////////////////////
    if(cmd_arr[0] != NULL){        
        if(strcmp(cmd_arr[0], "exit") == 0){
            if(cmd_arr[1] != NULL){
                print_error();
            } else {
            clean_exit(0);
            }
        } else if(strcmp(cmd_arr[0], "cd") == 0){
            // 0 or >1 args should be signaled as an error
            if(cmd_arr[1]==NULL || cmd_arr[2] != NULL){
                print_error();
            } else if (chdir(cmd_arr[1]) != 0){
                print_error();
            }
        } else if(strcmp(cmd_arr[0], "path") == 0){
            for (int i = 1; i < SIZE_MAX; i++){
                if(NULL == cmd_arr[i]){
                    path_arr[i-1] = NULL;
                } else{
                    path_arr[i-1] = malloc((sizeof(char) * SIZE_MAX));
                    strcpy(path_arr[i-1], cmd_arr[i]);
                }
            }
        } else {
            return execute_program(cmd_arr, redirection, redirect_file);
        }
    }
    return 0;
}

int execute_program(char* cmds[SIZE_MAX], int redirection, char* redirect_file){
    // Check if file exists
    char file_path[SIZE_MAX];
    int valid_path = 0;
    for (int i = 0; path_arr[i]; i++){
        // Create the correct path {PATH/CMD}
        strcpy(file_path, path_arr[i]);
        strcat(file_path, "/");
        strcat(file_path, cmds[0]);
        if (access(file_path, X_OK) == 0){
            valid_path = 1;
            break;
        }
    }
    if(valid_path != 1){
        print_error();
    } else {
        pid_t pid = fork();
        if(pid < 0){
            print_error();
            clean_exit(1);
        } else if (pid == 0){
            if(redirection != 0){
                int open_ret = open(redirect_file, O_CREAT | O_TRUNC | O_WRONLY, S_IRWXU);
                if(open_ret != -1){
                    dup2(open_ret, STDOUT_FILENO);
                }
            }
            execv(file_path, cmds);
            print_error();
            clean_exit(1);
        } else {
            int status;
            waitpid(pid, &status, 0);
            return WEXITSTATUS(status);
        }
    }
    return -1;
}

void redirect(char* in_buffer){
    char* cmd = NULL;
    char* output_file_int = NULL;
    char* extra_arrow = NULL;
    cmd = strsep(&in_buffer, ">");
    output_file_int = strsep(&in_buffer, ">");
    extra_arrow = strsep(&in_buffer, ">");
    
    char* output_file = strtok(output_file_int, delim); //remove whitespaces
    char* extra_file = strtok(NULL, delim);

    if(extra_file){
        print_error();
        return;
    }

    if(output_file == NULL){
        print_error();
        return;
    }
    
    if(extra_arrow){
        print_error();
        return;
    } else{
        process_cmd(cmd, 1, output_file);
    }
}

int check_if_cmd(char* cmd){
    char* inputs[SIZE_MAX];
    char* token = strtok(cmd, delim);
    int i = 0;
    inputs[0] = NULL;
    while(token != NULL){
        inputs[i] = token;
        i++;
        token = strtok(NULL, delim);
        inputs[i+1] = NULL;
    }

    int num_if = 0;
    int num_fi = 0;
    int num_then = 0;
    int num_op = 0;
    if(inputs[0] == NULL){
        return 0;
    }
    if(0 == strcmp(inputs[0], "if")){
        num_if++;
    } else{
        return 0; // Automatically return if the command does not have if
    }

    // Loop through all the commands and count all the if, fi, then, and operators
    for (int i = 1; inputs[i]; i++){
        if(0 == strcmp(inputs[i], "if")){
            num_if++;
            continue;
        }

        if(0 == strcmp(inputs[i], "fi")){
            num_fi++;
            continue;
        }

        if(0 == strcmp(inputs[i], "then")){
            num_then++;
            continue;
        }

        if(0 == strcmp(inputs[i], "==") || 0 == strcmp(inputs[i], "!=")){
            num_op++;
            continue;
        }
    }

    int sum = num_fi + num_if + num_op + num_then;

    if((sum % 4) == 0){
        return num_if;
    } else {
        return -1; // If they are not all equal, it is a bad input
    }
    return 0;
}

void clean_exit(int code){
    for(int i = 0; i < SIZE_MAX; i++){
        free(cmd_arr[i]);
        free(path_arr[i]);
    }
    exit(code);
}

void print_error(){
    write(STDERR_FILENO, error_message, strlen(error_message)); 
}

void initialize_path(){
    // Your initial shell path should contain one directory: /bin
    path_arr[0] = malloc(sizeof(char) * SIZE_MAX);
    strcpy(path_arr[0], "/bin");    
    for (int i = 1; i < SIZE_MAX; i++){
        path_arr[i] = malloc(sizeof(char) * SIZE_MAX);
        path_arr[i] = NULL;
    }
}

void clear_cmd_arr(){
    for (int i = 0; i < SIZE_MAX; i++){
        cmd_arr[i] = NULL;
    }
}