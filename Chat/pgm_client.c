#include "pgm_msg.h"

int main()
{
    //creates msg queue
    int msg_key = ftok(FILE_NAME, FILE_ID);
    ERROR_CHECK(msg_key);
    
    int msg_id  = msgget(msg_key, IPC_CREAT | 0666); 
    ERROR_CHECK(msg_id);
    int e = 0;    
    
    //creates connection between client and server 
    struct MsgBuf msg; 
    
    msg.type_  = TYPE_SERVER;
    msg.first_ = TRUE;
        
    e = msgsnd(msg_id, &msg, MSG_SIZE, 0); 
    ERROR_CHECK(e);
    
    printf(C_YELLOW"@ CONNECTING TO SERVER\n"C_RESET);
    e = msgrcv(msg_id, &msg, MSG_SIZE, TYPE_SYSTEM , 0);
    ERROR_CHECK(e);
    printf(C_YELLOW"@ CONNECTED CORRECTLY \n"C_RESET);
    
    const int type_client = msg.first_; 
    printf(C_YELLOW"You are user %d\n"C_RESET, type_client);
    
    //main cycle
    int cycle = 1;
    int pid = fork();
    ERROR_CHECK(pid)
    
    if (pid == 0) while (cycle)
    {
        struct MsgBuf msg_out;
        memset(msg_out.text_, '\0', TEXT_SIZE);
        
        char text_out [TEXT_SIZE];
        gets(text_out, TEXT_SIZE); 
        printf(CLEAR);
        strcat(msg_out.text_, text_out);
        msg_out.type_  = TYPE_SERVER;
        msg_out.first_ = type_client;
        
        e = msgsnd(msg_id, &msg_out, MSG_SIZE, 0); 
        ERROR_CHECK(e);
        
        if (msg_out.text_[0] == '/')
        {
            char cmd[TEXT_SIZE];
            memset(cmd, '\0', TEXT_SIZE);
            
            sscanf(msg_out.text_, "%s", cmd);
            if (strcmp(cmd, END_CMD) == 0)
            {
                cycle = 0;
            }
        }
    }
    else while (cycle)
    {
        struct MsgBuf msg_in;
        memset(msg_in.text_, '\0', TEXT_SIZE);
        msg_in.first_ = FALSE;
                
        e = msgrcv(msg_id, &msg_in, MSG_SIZE, type_client, 0);
        ERROR_CHECK(e);
        
        if (type_client == -msg_in.first_)
        {
            printf(C_YELLOW"@ YOU HAVE DISCONNECTED\n"C_RESET);
            cycle = 0;
        }
        else
        {
            printf("%s\n", msg_in.text_);
        }
    }
    
    //msgsnd(msg_id, &msg, msg_size, 0);  
    return 0;
}
