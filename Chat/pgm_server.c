#include "pgm_msg.h"

int const USERS_MAX = 5;

int send_all_users(int msg_id, struct MsgBuf *msg, int users)
{
    for (int i = 1; i <= users; ++i)
    {
        msg->type_ = i;
        
        int e = msgsnd(msg_id, msg, MSG_SIZE, 0);
        ERROR_CHECK(e);
    }
}

int main()
{
    int users = 0;
    
    int msg_key = ftok(FILE_NAME, FILE_ID);
    ERROR_CHECK(msg_key);
    
    // clear msg queue
    int msg_id  = msgget(msg_key, IPC_CREAT | 0666); 
    ERROR_CHECK(msg_id);

    int e = msgctl(msg_id, IPC_RMID, 0);
    ERROR_CHECK(e);
    
    // creates msg queue
    msg_id  = msgget(msg_key, IPC_CREAT | 0666); 
    ERROR_CHECK(msg_id);
    
    // main cycle
    while (1)
    {
        struct MsgBuf msg; 
        memset(msg.text_, '\0', TEXT_SIZE);
        
        e = msgrcv(msg_id, &msg, MSG_SIZE, TYPE_SERVER , 0);
        ERROR_CHECK(e);
        
        char text[TEXT_SIZE];
        memset(text, '\0', TEXT_SIZE);
        
        if (msg.first_ == TRUE)
        {   
            ++users;    
            msg.first_ = users;
            msg.type_  = TYPE_SYSTEM;
            
            e = msgsnd(msg_id, &msg, MSG_SIZE, 0);
            ERROR_CHECK(e);
            
            sprintf(text, C_RED"$ User %d connected"C_RESET, users);
            printf("%s\n", text);
            
            memset(msg.text_, '\0', TEXT_SIZE);
            strcat(msg.text_, text);
            send_all_users(msg_id, &msg, users);
        }
        else
        {   
            if (msg.text_[0] == '/')
            {
                char c    = 0;
                long type = 0;
                char cmd[TEXT_SIZE];
                memset(cmd, '\0', TEXT_SIZE);
               
                int n = sscanf(msg.text_, "%c%d", &c, &type);
                if (n == 2) msg.type_ = type;
                else 
                {   
                    sscanf(msg.text_, "%s", cmd);
                    if (strcmp(cmd, END_CMD) == 0)
                    {
                        sprintf(text, C_RED"$ User %d disconnected"C_RESET, msg.first_);
                        
                        memset(msg.text_, '\0', TEXT_SIZE);
                        strcat(msg.text_, text);
                        
                        msg.first_ = -msg.first_;
                        
                        send_all_users(msg_id, &msg, users);
                    } 
                    
                    continue;
                }
                sprintf(text, C_BLUE"%2d->%d: %s"C_RESET, msg.first_, msg.type_, msg.text_ + 3 + type / 10);  
                
                memset(msg.text_, '\0', TEXT_SIZE);
                strcat(msg.text_, text);
         
                e = msgsnd(msg_id, &msg, MSG_SIZE, 0);
                ERROR_CHECK(e);
                
                if (msg.first_ != msg.type_)
                {
                    msg.type_ = msg.first_;
                    e = msgsnd(msg_id, &msg, MSG_SIZE, 0);
                    ERROR_CHECK(e);
                }
            }
            else
            {
                msg.type_  = TYPE_SYSTEM;
        
                sprintf(text, C_GREEN"%2d: %s"C_RESET, msg.first_, msg.text_);        
                memset(msg.text_, '\0', TEXT_SIZE);
                strcat(msg.text_, text);
         
                send_all_users(msg_id, &msg, users);
            }
        }
    }

    return 0;
}
