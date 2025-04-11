

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h> 
 
int main(void)
{
   pid_t iPid;
   int iStatus;
 
 
   if( (iPid = fork())<0) /* cria um processo filho */
   {
      perror("Erro no fork");
      return 0;
   }
 
   if( iPid != 0) /* no processo pai*/
   {
      printf("\nCriado o processo %d", iPid);
      while(1)
      {
         printf("\nEsperando o status do filho.");
         waitpid(iPid,&iStatus,WUNTRACED);
         printf("\nStatus do filho pego.");
 
         if( WIFEXITED(iStatus) )
         {
            printf("\nFilho terminou normalmente");
            printf("\nO status de término foi %d.", WEXITSTATUS(iStatus));
            break;
         }
         if( WIFSIGNALED(iStatus) )
         {
            printf("\nFilho recebeu sinal e terminou");
            printf("\nO sinal que terminou o filho foi %d.", WTERMSIG(iStatus));
            break;
         }
 
         if( WIFSTOPPED(iStatus) )
         {
            printf("\nFilho recebeu sinal stop");
            printf("\nO sinal que parou o filho foi %d.", WSTOPSIG(iStatus));
         }
     }
   }
   else /* no processo filho */
   {
      /* fica em loop esperando um sinal via comando kill ou ate terminar */
      int i;
      printf("\nFilho %d em execucao", getpid());
      for(i=0;i<20;i++)
      {
         printf("\nFilho faltam %d segundos para terminar.", 20-i);
         sleep(1);
      }
   }
   return 0;
}

