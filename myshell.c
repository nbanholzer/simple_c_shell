#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>

struct _node {
    char* token;
    struct _node* next;
};
typedef struct _node node;

void myPrint(char *msg)
{
    write(STDOUT_FILENO, msg, strlen(msg));
}

node* build_token_list(node* head, char* cmd_buff) {
    // const char semi_newline[3] = ";\n";
    char* token = NULL;
    int i = 0, multi_flag = 0;
    for(i = 0; i < strlen(cmd_buff); i++) {
        if(cmd_buff[i] == ';') {
            multi_flag = 1;
            break;
        }
    }
    if(multi_flag) {
        token = strtok(cmd_buff, ";\n");
        head = malloc(sizeof(node));
        node* temp = head;
        temp->token = token;
        while(token != NULL) {
            // temp->token = token;
            temp->next = NULL;
            token = strtok(NULL, ";\n");
            int k = 0, pass_flag = 0;
            
            if(token) {
                for(k = 0; token[k] != '\0'; k++) {
                    if(token[k] != ' ' && token[k] != '\t' && token[k] != '\n') {
                        pass_flag = 1;
                    }
                }
                if(pass_flag) {
                    temp->next = malloc(sizeof(node));
                    temp = temp->next;
                    temp->token = token;
                }
            }

        }
        return head;
    } else {
        head = malloc(sizeof(node));
        // head->token = malloc(sizeof(char) * strlen(cmd_buff));
        head->token = cmd_buff;
        // myPrint(head->token);
        head->next = NULL;
        return head;
    }
}

void call_exit(char **args) {
    if(args[1]) {
        myPrint("An error has occurred\n");
    } else {
        exit(0);
    }
}
void call_cd(char** args) {
    if(!args[1]) {
        char* home = getenv("HOME");
        chdir(home);
    } else {
        char *path = args[1];
        if(args[2]) {
            myPrint("An error has occurred\n");
        } else {
            int err = chdir(path);
            if(err == -1)
                myPrint("An error has occurred\n");
        }
    }
}
void call_pwd(char** args) {
    if(args[1])
        myPrint("An error has occurred\n");
    else {
        char *cwd = NULL;
        cwd = getcwd(cwd, 514);
        strcat(cwd, "\n");
        write(STDOUT_FILENO, cwd, strlen(cwd));
    }
}



void execute_command_list(node* head) {
    if(!head)
        return;
    while(head != NULL) {
        // int redir_cnt = 0;
        char* temp_str = strdup(head->token);
        char* temp_str2 = strdup(head->token);
        // temp_str = strcpy(temp_str, head->token);
        // temp_str2 = strcpy(temp_str2, head->token);
        
        int k = 0, redir_cnt = 0, adv_redir_cnt = 0;
        for(k = 0; temp_str[k] != '\0'; k++) {
            if(temp_str[k] == '>') {
                if(!temp_str[k+1]) {}
                else if(temp_str[k+1] == '+') {
                    adv_redir_cnt++;
                    temp_str[k+1] = ' ';
                    temp_str2[k+1] = ' ';
                } else {
                    redir_cnt++;
                }
            }
        }
        
        
        // // myPrint(redir_cmd);
        
        // while((redir_file[redir_cnt] = strtok(NULL, "> \t\n")) != NULL) {
        //     redir_cnt++;
        // }
        if(redir_cnt > 0 && adv_redir_cnt == 0) {
            char* redir_cmd = NULL;
            char* redir_file[20];
            redir_cmd = strtok(temp_str, ">");
            redir_file[0] = strtok(NULL, "");
            redir_file[0] = strtok(redir_file[0], " \t\n");
            // myPrint(redir_file[0]);
            // myPrint("I made it to 6.");
          if(redir_cnt > 1 || !redir_file[0]) {
              myPrint("An error has occurred\n");
          } else {
              char* args[50];
              int i = 0;
              args[0] = strtok(redir_cmd, " \t\n");
            //   myPrint(args[0]);
              i = 1;
              while((args[i] = strtok(NULL, " \t\n")) != NULL)
                  i++;
              if(!args[0]) {
                  myPrint("An error has occurred\n");
                //   myPrint("I made it here.");
              } else {
                  int fd = 0, fail_flag = 0, err3 = 0;
                  pid_t proc_id;
                  int status;
                //   proc_id = fork();
                if(strcmp(args[0], "exit") == 0) {
                    myPrint("An error has occurred\n");
                    // call_exit(args);
                    // exit(0);
                } else if(strcmp(args[0], "pwd") == 0) {
                    myPrint("An error has occurred\n");
                    // call_exit(args);
                } else if(strcmp(args[0], "cd") == 0) {
                    myPrint("An error has occurred\n");
                    // call_exit(args);
                    
                } else if((proc_id = fork()) < 0) {
                    myPrint("An error has occurred\n");
                } else if(proc_id == 0) {
                    if((fd = open(redir_file[0], O_RDONLY)) >= 0) {
                        close(fd);
                        fail_flag = 1;
                        //   myPrint("I made it to 4.");
                    } else {
                          // mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
                        fd = open(redir_file[0], O_CREAT | O_WRONLY, S_IRWXU);
                        err3 = dup2(fd, 1);
                        //   myPrint("I made it to 5.");
                        //   close(fd);
                    }
                    if(err3 < 0 || fail_flag != 0) {
                          myPrint("An error has occurred\n");
                        //   myPrint("I made it to 1.");
                    } else {
                          int err = execvp(args[0], args);
                          if(err) {
                              myPrint("An error has occurred\n");
                            //   myPrint("I made it to 3.");
                          }
                          exit(0);
                      }
                      close(fd);
                  } else {
                      waitpid(proc_id, &status, 0);
                      fflush(stdout);
                  }
              }
          }
        //   free(redir_cmd);
        } else if(adv_redir_cnt > 0) {
            // int k = 0;
            // for(k = 0; k < strlen(temp_str2); k++) {
            //     if(temp_str2[k] == '+')
            //         temp_str2[k] = ' ';
            // }
            // myPrint("I made it here")
            char* adv_redir_cmd = NULL;
            adv_redir_cmd = strtok(temp_str2, ">");
            char* adv_redir_file[20];
            adv_redir_file[0] = strtok(NULL, "");
            adv_redir_file[0] = strtok(adv_redir_file[0], " \t\n");
            if(adv_redir_cnt > 1 || !adv_redir_file[0]) {
                myPrint("An error has occurred\n");
            } else {
                char* args[50];
                int i = 0, err3 = 0;
                args[0] = strtok(adv_redir_cmd, " \t\n");
                i = 1;
                while((args[i] = strtok(NULL, " \t\n")) != NULL)
                    i++;
                if(!args[0]) {
                    myPrint("An error has occurred\n");
                } else {
                    int fd = 0, exist_flag = 0;;
                    pid_t proc_id;
                    int status;
                    char* exist_cont = malloc(sizeof(char)*50);
                    int cur_mem = 50;
                    if((fd = open(adv_redir_file[0], O_RDONLY)) >= 0) {
                        exist_flag = 1;
                        // myPrint("I made it here.\n");
                        int l = 0;
                        int ret = 0;
                        while((ret = read(fd, &exist_cont[l], 1)) > 0) {
                            if(l > cur_mem - 1) {
                                exist_cont = realloc(exist_cont, cur_mem * 2);
                                cur_mem *= 2;
                            }
                            l++;
                        }
                        // myPrint("I made it here.\n");
                        exist_cont[l] = '\0';
                        close(fd);
                    }
                    // myPrint(exist_cont);
                    // myPrint("I made it here.\n");
                    if(strcmp(args[0], "exit") == 0) {
                        myPrint("An error has occurred\n");
                        // call_exit(args);
                        // exit(0);
                    } else if(strcmp(args[0], "pwd") == 0) {
                        myPrint("An error has occurred\n");
                        // call_exit(args);
                    } else if(strcmp(args[0], "cd") == 0) {
                        myPrint("An error has occurred\n");
                        // call_exit(args);    
                    } else if((proc_id = fork()) < 0) {
                        myPrint("An error has occurred\n");
                    } else if(proc_id == 0) {
                        if((fd = open(adv_redir_file[0], O_WRONLY | O_TRUNC, S_IRWXU)) >= 0) {}
                        else {
                            fd = open(adv_redir_file[0], O_CREAT | O_RDWR | O_TRUNC, S_IRWXU);
                        }
                        err3 = dup2(fd, 1);
                        if(err3 < 0) {
                            myPrint("An error has occurred\n");
                        } else {
                            int err = execvp(args[0], args);
                            if(err)
                                myPrint("An error has occurred\n");
                            exit(0);
                        }
                        close(fd);
                    } else {
                        waitpid(proc_id, &status, 0);
                        if(exist_flag) {
                            // myPrint("I made it to here.\n");
                            fd = open(adv_redir_file[0], O_WRONLY | O_APPEND, S_IRWXU);
                            // int stdout_d = dup(1);
                            // int err = dup2(fd, 1);
                            // myPrint(exist_cont);
                            write(fd, exist_cont, strlen(exist_cont));
                            // int err2 = dup2(stdout_d, 1);
                            close(fd);
                            // if(err || err2)
                            //     myPrint("An error has occurred\n");

                        }
                        fflush(stdout);
                    }
                }
                // myPrint("Advanced Redirect thing\n");
            }
        } else {
            // myPrint("I made it here.");
            char* args[50];
            int j = 0, count = 0;
            for(j = 0; j < strlen(head->token); j++) {
                if(head->token[j] == ' ' || head->token[j] == '\t') {
                    count++;
                }
            }
            if(!count) {
                args[0] = strtok(head->token, " \t\n");
                args[1] = NULL;
            } else {
                int i = 0;
                args[0] = strtok(head->token, " \t\n");
                for(i = 1; (args[i] = strtok(NULL, " \t\n")) != NULL; i++);
            }
            // args[0] = strtok(head->token, " \t\n");
            if(!args[0]) {
                myPrint("An error has occurred\n");
            } else {
                pid_t proc_id;
                int status;
                if(strcmp(args[0], "exit") == 0) {
                    // myPrint("CALLING EXIT");
                    call_exit(args);
                } else if(strcmp(args[0], "cd") == 0) {
                    call_cd(args);
                } else if(strcmp(args[0], "pwd") == 0) {
                    call_pwd(args);
                } else if((proc_id = fork()) < 0) {
                    myPrint("An error has occurred\n");
                } else if(proc_id == 0) {
                    int err = execvp(args[0], args);
                    if(err)
                        myPrint("An error has occurred\n");
                    call_exit(args);
                } else {
                    waitpid(proc_id, &status, 0);
                    fflush(stdout);
                    // exit(0);
                }
            }
        }
        node *temp = head;
        head = head->next;
        free(temp);
        // free(temp_str);
        // free(temp_str2);
        // free(redir_cmd);
    }

    // call_exit(args);
}


int main(int argc, char *argv[])
{
    if(argc == 1) {
        char cmd_buff[514];
        int cur_mem  = 513;
        int i = 0;
        while (1) {
            // myPrint("I made it there.");
            char *pinput = malloc(sizeof(char) * 514);
            myPrint("myshell> ");
            while((pinput[i] = fgetc(stdin)) != '\n') {
                i++;
                if(i > cur_mem) {
                    pinput = realloc(pinput, cur_mem * 2);
                    cur_mem *= 2;
                }
            }
            // pinput[i] = '\n';
            pinput[i+1] = '\0';
            if (*pinput == '\0') {
                continue;
            } else if(i > 513) {
                myPrint("An error has occurred\n");
                write(STDOUT_FILENO, pinput, strlen(pinput));
            } else {
                for(i = 0; pinput[i] != '\0'; i++) {
                    cmd_buff[i] = pinput[i];
                }
                cmd_buff[i] = '\0';
                node *head = NULL;
                head = build_token_list(head, cmd_buff);
                execute_command_list(head);
                i = 0;
                free(pinput);
            }
        }
    } else if(argc == 2) {
        char* filename = argv[1];
        int fd = open(filename, O_RDONLY);
        if(fd < 0) {
            myPrint("An error has occurred\n");
        } else {
            // char cmd_buff[514];
            int cur_mem  = 513;
            int i = 0, err_flag = 0;
            // ssize_t read;
            // size_t len = 0;
            //write(STDOUT_FILENO, "YOOOOOOOOOOO\n", 14);
            int ret = 1;
            while(ret > 0) {
                char *pinput = malloc(sizeof(char) * 514);
                while((ret = read(fd, &pinput[i], 1)) > 0 && pinput[i] != '\n') {
                    i++;
                    if(i > cur_mem) {
                        pinput = realloc(pinput, cur_mem * 2);
                        cur_mem *= 2;
                        err_flag = 1;
                    }
                }
                pinput[i] = '\n';
                pinput[i+1] = '\0';
                char *cmd_copy = malloc(sizeof(char) * strlen(pinput));
                cmd_copy = strcpy(cmd_copy, pinput);
                char *cmd_copy2 = malloc(sizeof(char) * strlen(pinput));
                cmd_copy2 = strcpy(cmd_copy2, pinput);
                if((strtok(cmd_copy, " \t\b\v\r\f\n")) == NULL) {}
                else if((strtok(cmd_copy2, " \t\n;")) == NULL) {
                    myPrint(pinput);
                }
                else {
                    // pinput[i] = '\n';
                    // pinput[i+1] = '\0';
                    if(err_flag) {
                        write(STDOUT_FILENO, pinput, strlen(pinput));
                        myPrint("An error has occurred\n");
                    } else {
                        node* head = NULL;
                        write(STDOUT_FILENO, pinput, strlen(pinput));
                        // myPrint("An error has occurrYY\n");
                        head = build_token_list(head, pinput);
                        execute_command_list(head);
                    }
                }

                free(pinput);
                i = 0;
                err_flag = 0;
                cur_mem = 513;
                // full_line = 0;
            }
        }
        close(fd);
    } else {
        myPrint("An error has occurred\n");
    }
    return 0;

}