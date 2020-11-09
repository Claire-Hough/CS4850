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
#define MAX 256 
#define PORT 18741 
#define SA struct sockaddr 

//define struct for user info linked list
struct UserInfo {
  char* userId;
  char* password;
  char* loggedIn;
};

// global variables for userInfoArray
struct UserInfo userInfoArray[100];
int counter = 0;


// newUser function
void newUser( int counter, struct UserInfo userInfoArray[], char* userId, char* password ){
  userInfoArray[counter].userId = userId;
  userInfoArray[counter].password = password;
  userInfoArray[counter].loggedIn = "false";
  counter++;
}


// login function
int login(struct UserInfo userInfoArray[], char* username, char* password){
  int i = 0;
  int response = 0;
  char* message;
  
  // check if anyone is already logged in
  for(i=0; i < (sizeof(userInfoArray)/sizeof(struct UserInfo)); i++){
  
    if( strncmp(userInfoArray[i].loggedIn, "true", 4) == 0 ){
      response = 2;
      return response;
    }
  }
  
  // check if submitted login is in array
  for(i=0; i < (sizeof(userInfoArray)/sizeof(struct UserInfo)); i++){
  
    printf("%s, %s\n", userInfoArray[i].userId, username);
    printf("%s, %s\n", userInfoArray[i].password, password);
    fflush(stdout);
  
    // if submitted userId and password are in the array, set to logged in and return confirmation
    if( (strncmp(userInfoArray[i].userId, username, strlen(username)) == 0) && (strncmp(userInfoArray[i].password, password, strlen(password)) == 0) ){
      userInfoArray[i].loggedIn = "true";
      response = 1;
      return response;
    }
    // if userId or password or both are incorrect, return error message
    else{
      response = 0;
      return response;
    }
  }
}


// Function designed for chat between client and server. 
void func(int sockfd) 
{ 
	char buff[MAX]; 
  char message[MAX];
  char *temp;
  int responsetype = 0;
  //printf("%d\n", responsetype);
  
	int n; 
	// infinite loop for chat 
	for (;;) { 
		bzero(buff, MAX); 

		// read the message from client and copy it in buffer 
		read(sockfd, buff, sizeof(buff)); 
		// print buffer which contains the client contents 
		printf("From client: %s\t To client : ", buff);  
   
   //bzero(message, MAX);
   char* username;
   char* password;
   // send message from client to login function
   if( strncmp("login", buff, 5) == 0 ){
     
    temp = strtok(buff, " ");
    //printf("\nfunction name: %s\n", temp);
    username = strtok(NULL, " ");
    //printf("userID: %s\n", username);
    password = strtok(NULL, " ");
    //printf("password: %s\n", password);
   
    //responsetype = login(userInfoArray, username, password);
    //printf("response type: %d\n", responsetype);
    
    // check if anyone is already logged in
    int i = 0;
    for(i=0; i < counter; i++){
    
      if( strncmp(userInfoArray[i].loggedIn, "true", 4) == 0 ){
        strcpy(message, "Another is currently logged in. They must be logged out before another user can log in.\n");
      }
    }    
    // check if submitted login is in array
    for(i=0; i < counter; i++){
    
      //printf("%s, %s\n", userInfoArray[i].userId, username);
     // printf("%s, %s\n", userInfoArray[i].password, password);
     // printf("i = %d\n", i);
      //fflush(stdout);
    
      // if submitted userId and password are in the array, set to logged in and return confirmation
      if( (strncmp(userInfoArray[i].userId, username, strlen(username)) == 0) && (strncmp(userInfoArray[i].password, password, strlen(password)) == 0) ){
        userInfoArray[i].loggedIn = "true";
        strcpy(message, "User is now logged in.");
        // "User " + username + " is now logged in."
        
        // snprintf
      }
      // if userId or password or both are incorrect, return error message
      else{
        //strcpy(message, "The submitted userId or password were incorrect.\n");
        message[0] = ('t');
      }
    }
   
   }
   else if( strncmp("logout", buff, 6) == 0 ){
     printf("received logout");
     fflush(stdout);
     temp = "received logout";
   }
   else{
     printf("other");
   }
   
 		//bzero(buff, MAX); 
		//n = 0;
   
		// copy server message in the buffer 
		//while ((buff[n++] = getchar()) != '\n') 
		//	; 

    //printf("%s\n", message);
    //fflush(stdout);

		// and send that buffer to client 
		write(sockfd, message, sizeof(message)); 

		// if msg contains "Exit" then server exit and chat ended. 
		if (strncmp("exit", buff, 4) == 0) { 
			printf("Server Exit...\n"); 
			break; 
		} 
	} 
} 


// Driver function 
int main() 
{ 
  // socket variables
	int sockfd, connfd, len; 
	struct sockaddr_in servaddr, cli; 
 
  // variables to process file
  char* userId;
  char* password;
  char* temp;
  const char* token = ", ";
 
  // variables to read user info file
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
    
    newUser(counter, userInfoArray, userId, password);      
    printf("%s, %s, %s\n", userInfoArray[counter].userId, userInfoArray[counter].password, userInfoArray[counter].loggedIn);     
  }
  
  printf("size of array: %d\n", sizeof(userInfoArray)/sizeof(struct UserInfo));
  printf("Length of username: %d\n", strlen(userId));
 
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
