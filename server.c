/*
  name: Claire Hough
  pawprint: cnhfg6
  Student ID: 12538741
  
  Links used for reference:
  https://www.geeksforgeeks.org/tcp-server-client-implementation-in-c/
  https://stackoverflow.com/questions/3501338/c-read-file-line-by-line
  https://www.tutorialspoint.com/data_structures_algorithms/linked_list_program_in_c.htm
  https://www.geeksforgeeks.org/how-to-split-a-string-in-cc-python-and-java/
  
*/


#include <stdio.h> 
#include <stdbool.h>
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#define MAX 80 
#define PORT 18741 
#define SA struct sockaddr 

//define struct for user info linked list
//struct UserInfo {
//  char* userId;
//  char* password;
//  bool loggedIn;
//  struct UserInfo* next;
//};


// Function designed for chat between client and server. 
void func(int sockfd) 
{ 
  char temp[80] = "received";
	char buff[MAX]; 
	int n; 
	// infinite loop for chat 
	for (;;) { 
		bzero(buff, MAX); 

		// read the message from client and copy it in buffer 
		read(sockfd, buff, sizeof(buff)); 
		// print buffer which contains the client contents 
		printf("From client: %s\t To client : ", buff); 
		bzero(buff, MAX); 
		n = 0; 
   
   if( strncmp("login", buff, 5) == 0 ){
     buff = temp;
   }
   else if( strncmp("logout", buff, 6) == 0 ){
     buff = temp;
   }
   
		// copy server message in the buffer 
		//while ((buff[n++] = getchar()) != '\n') 
		//	; 

		// and send that buffer to client 
		write(sockfd, buff, sizeof(buff)); 

		// if msg contains "Exit" then server exit and chat ended. 
		if (strncmp("exit", buff, 4) == 0) { 
			printf("Server Exit...\n"); 
			break; 
		} 
	} 
} 

/*
// This function prints contents of linked list starting from head 
void printList(struct UserInfo *node) 
{ 
  while (node != NULL) 
  { 
     printf(" %s, %s, %b ", node->userId, node->password, node->loggedIn); 
     node = node->next; 
  } 
} 
*/


/* Given a reference (pointer to pointer) to the head 
   of a list and an int, appends a new node at the end  
void append(struct UserInfo** head_ref, char* userid, char* passWord) 
{ 
    // 1. allocate node 
    struct UserInfo* new_node = (struct UserInfo*) malloc(sizeof(struct UserInfo)); 
  
    struct UserInfo *last = *head_ref;  // used in step 5
  
    // 2. put in the data  
    new_node->userId  = userid; 
    new_node->password = passWord;
    new_node->loggedIn = false;
  
    // 3. This new node is going to be the last node, so make next of 
          it as NULL
    new_node->next = NULL; 
  
    // 4. If the Linked List is empty, then make the new node as head
    if (*head_ref == NULL) 
    { 
       *head_ref = new_node; 
       return; 
    } 
  
    // 5. Else traverse till the last node 
    while (last->next != NULL) 
        last = last->next; 
  
    // 6. Change the next of last node 
    last->next = new_node; 
    return; 
} 
*/


// Driver function 
int main() 
{ 
	int sockfd, connfd, len; 
	struct sockaddr_in servaddr, cli; 
 
  //struct UserInfo *head = NULL;
  char userInfoArray[100];
  int counter = 0;
  char* userId;
  char* password;
  char* temp;
  const char* token = ", ";
 
 
  // read in user login info
  FILE* filePointer;
  int bufferLength = 255;
  char buffer[bufferLength];
  
  filePointer = fopen("users.txt", "r");
  
  // for each line in the file, grab the username and password
  while(fgets(buffer, bufferLength, filePointer)) {

      temp = strtok(buffer, token);
      userId = temp+1;
      printf("userID: %s\n", userId);
      
      while (temp != NULL) 
      {  
        temp = strtok(NULL, " ");
        if (temp != NULL){
          password = temp;
        }
      }
      password = strtok(password, ")");
      printf("password: %s\n", password);
      
      // add user info to linked list
      //append(&head, userId, password);
      //printList(head);
/*
      //userInfoArray[counter][0] = userId[0];
 //     strcpy(userInfoArray, userId);
      counter++;
      printf("%d\n", counter);
      printf("%s %s\n", userId, userInfoArray[counter]);
      //userInfoArray[counter][0] = password[0];
      counter++;
      printf("%d\n", counter);
      //printf("%s ", userInfoArray[counter][0]);
      //userInfoArray[counter][0] = "f";
      counter++;
      printf("%d\n", counter);
      //printf("%s ", userInfoArray[counter][0]);
      
      int i=0;
      for(i=0; i<counter; i++){
        printf("%s ", userInfoArray[i]);
        
      }
      printf("\n");
      */
  }
 
  fclose(filePointer);  
  

	// socket create and verification 
	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockfd == -1) { 
		printf("socket creation failed...\n"); 
		exit(0); 
	} 
	else
		printf("Socket successfully created..\n"); 
	bzero(&servaddr, sizeof(servaddr)); 

	// assign IP, PORT 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
	servaddr.sin_port = htons(PORT); 

	// Binding newly created socket to given IP and verification 
	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) { 
		printf("socket bind failed...\n"); 
		exit(0); 
	} 
	else
		printf("Socket successfully binded..\n"); 

	// Now server is ready to listen and verification 
	if ((listen(sockfd, 5)) != 0) { 
		printf("Listen failed...\n"); 
		exit(0); 
	} 
	else
		printf("Server listening..\n"); 
	len = sizeof(cli); 

	// Accept the data packet from client and verification 
	connfd = accept(sockfd, (SA*)&cli, &len); 
	if (connfd < 0) { 
		printf("server acccept failed...\n"); 
		exit(0); 
	} 
	else
		printf("server acccept the client...\n"); 

	// Function for chatting between client and server 
	func(connfd); 

	// After chatting close the socket 
	close(sockfd); 
} 
