#include "value.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
int main()
{
    char judge[6];
    int index=-1;
    TNODE head, ptr;
    head = (TNODE)malloc(sizeof(TREE));
    head->next = NULL;
    ptr = head;
    while(1)
    {
        printf("Input key_value pairs or Get key_value or Delete key_value pairs\n");
        scanf("%s",judge);
        if (strcmp(judge, "INPUT") == 0)
        {
            printf("index ");
            scanf("%d", &index);

            int c;
            while ((c = getchar()) != '\n' && c != EOF);

            char cmd[1000];
            printf("Input key_value pair\n");
            fgets(cmd, sizeof(cmd), stdin);

            size_t cmd_len = strlen(cmd);
            if (cmd_len > 0 && cmd[cmd_len - 1] == '\n') {
                cmd[cmd_len - 1] = '\0';
            }

            createTreelink(head, cmd, index);
        }

        else if(strcmp(judge, "GET") == 0)
        {
            printf("Input INDEX ");
            scanf("%d",&index);
            TdbGET(head,index);
        }
        else if(strcmp(judge, "DEL") == 0)
        {
            printf("Input INDEX ");
            scanf("%d",&index);
            TdbDEL(head,index);
        }
    }
    return 0;
}
