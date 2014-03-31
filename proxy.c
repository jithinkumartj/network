#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<pthread.h>
int main(int argc,char *argv[])
{
        int sockfd,newsockfd,portno,clilen,i,j,port=80,sockfd1,newsockfd1,pid,flag=0,prime=0;
        char buffer[1000],f[500],s[500],l[500],host[500],path[500],*p,cport[100];
        
        struct sockaddr_in serv_addr,cli_addr,addr;
        struct hostent *server;
        int n;
        sockfd=socket(AF_INET,SOCK_STREAM,0);
			if(sockfd<0)
        {
                perror("ERror");
                return 1;
        }
        if(argc<1)
        {
			 printf("usage ./p <port>");
			return 1;
		}
        bzero((char *) &serv_addr,sizeof(serv_addr));
        portno=atoi(argv[1]);
        //printf("%d",portno);
        serv_addr.sin_family=AF_INET;
        serv_addr.sin_addr.s_addr=INADDR_ANY;
        serv_addr.sin_port=htons(portno);
        if(bind(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr))<0)
        {
                perror("Error on binding");
                return 1;
        }
        listen(sockfd,5);
        clilen=sizeof(cli_addr);
       accepting:
        newsockfd=accept(sockfd,(struct sockaddr *) &cli_addr,&clilen);
        if(newsockfd<0)
        {
                perror("Erorr on accept");
        }
        pid=fork();
		if(pid==0)
		{
        bzero(buffer,1000);
        n=read(newsockfd,buffer,1000);
		printf("%s",buffer);		
        sscanf(buffer,"%s%s%s",f,s,l);
		if(((strncmp(f,"GET",3)==0))&&((strncmp(l,"HTTP/1.1",8)==0)||(strncmp(l,"HTTP/1.0",8)==0))&&(strncmp(s,"http://",7)==0))
		{
        i=7;
        while(i)
        {
			if(s[i]=='/'||s[i]=='\0')
				break;
			i++;
		}
		
		j=7;
		bzero(host,1000);
        bzero(path,1000);
		flag=0;
		while(j<i)
		{
			host[j-7]=s[j];
			j++;
			if(s[j]==':')
			{
				flag=1;
			}
		}
		host[j]='\0';
		printf("%d",flag);
		if(flag==1)
		{
		path[0]='\0';
		p=strtok(host,":");
		p=strtok(NULL,":");
		strcpy(cport,p);
		port=atoi(&cport);
		printf("PORT is:%d",port);
		printf("a%d",j);
		}
		j=0,prime=0;
		while(s[i]!='\0'&&flag==0)
		{
			path[j]=s[i];
			j++;
			i++;
			if(s[i]==':')
			{
				prime=1;
				
			}
			
		}
		path[j]='\0';
		if(prime==1)
		{
		p=strtok(path,":");
		p=strtok(NULL,":");
		strcpy(cport,p);
		port=atoi(&cport);
		printf("PORT is:%d",port);
		}	
			
		
		
		/*
		if(flag==1);
		{
		p=strtok(host,":");
		p=strtok(NULL,":");
		strcpy(cport,p);
		port=atoi(&cport);
		printf("PORT is:%d",port);
		}
		*/
		printf("Host is : %s\n",host);
		printf("path is : %s\n",path);	
		printf("PORT is:%d",port);

		server=gethostbyname(host);
		
		if(server==NULL)
		{
			fprintf(stderr,"ErroR\n");
			return 0;
		}
		bzero((char *) &addr,sizeof(addr));
        addr.sin_family=AF_INET;
        bcopy((char *)server->h_addr,(char *)&addr.sin_addr.s_addr,server->h_length);
        addr.sin_port=htons(port);
        sockfd1=socket(AF_INET,SOCK_STREAM,0);
        
        if(connect(sockfd1,&addr,sizeof(addr))<0)
        {
                perror("error connecting");
                return 1;
        }
        sprintf(buffer,"\nConnected to %s  IP - %s\n",host,inet_ntoa(addr.sin_addr));
		printf("%s",buffer);
		
        bzero((char*)buffer,1000);
        
        if(strcmp(path,"")==0)
			sprintf(buffer,"GET / HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n",host);
			

        else
			sprintf(buffer,"GET %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n",path,host);
			
		printf("%s",buffer);

		n=write(sockfd1,buffer,1000);
	
		if(n<0)
        {
                perror("Error on read");
                return 1;
        }
	
        do
        {
		//printf("here");
        bzero((char*)buffer,1000);
		n=read(sockfd1,buffer,1000);
		//printf("%s",buffer);
		if(n<0)
        {
                perror("Error on read");
                return 1;
        }
		else	
		write(newsockfd,buffer,1000);
		}while(n>0);	
		
		}

		else
		{
			send(newsockfd,"400 : BAD REQUEST\nONLY HTTP REQUESTS ALLOWED",18,0);
		}
		close(newsockfd);
		}
		else
			goto accepting;
        close(sockfd);
                
               return 0;
               
}
