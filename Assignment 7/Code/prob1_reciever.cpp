/* 
 *  @author Ayush Agarwal, 17114017
*/

#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h>
#include <stdlib.h> 
#include <string.h> 
#define PORT 8080 

//add two binary string for checksum
char* addBinary(char* a, char* b) {
    int len_a = strlen(a);
    int len_b = strlen(b);
    int len = (len_a > len_b) ? (len_a) : (len_b);
    char *ans = (char *)malloc(len + 1);
    int i;
    int sum = 0;
    for (i = 0; i < len; i++) {
        if (i < len_a && i < len_b) {
            sum += (a[len_a - 1 - i] - '0') + (b[len_b - 1 - i] - '0');
        }
        else if (i < len_a) {
            sum += a[len_a - 1 - i] - '0';
        }
        else if (i < len_b) {
            sum += b[len_b - 1 - i] - '0';
        }
        ans[len - i] = sum % 2 + '0';
        sum /= 2;
    }

    ans[len + 1] = '\0';

    if (sum) {
        ans[0] = sum + '0';
        return ans;
    }
    else {
        return ans + 1;
    }
}
   
int main(int argc, char const *argv[]) 
{ 
    int sock = 0, valread; 
    struct sockaddr_in serv_addr; 
    //char *hello = "Hello from client"; 
    char buffer[1024] = {0}; 
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        return -1; 
    } 
   
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 
       
    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("\nConnection Failed \n"); 
        return -1; 
    }

    int algo;
    valread = read( sock , &algo, sizeof(algo));
//Single parity check
    if(algo==1){
        valread = read( sock , buffer, 1024); 
        printf("Recieved message: %s\n",buffer );
        int msglen=strlen(buffer);
        int paritysum=0;
        for(int i=0;i<msglen; i++){
            int x=buffer[i]-'0';
            if(x==1) paritysum++;
        }
        if(paritysum%2==1){
            printf("Error detected: Yes\n");
        }
        else{
            printf("Error detected: No\n");
            char msg_decode[msglen];
            for(int i=0;i<msglen-1; i++){
                msg_decode[i]=buffer[i];
            }
            msg_decode[msglen-1]='\0';
            printf("Decoded message: %s\n", msg_decode);
            
        }
    }
//Two-dimensional Parity Check
    else if(algo==2){
        int seg;
        valread = read( sock , &seg, sizeof(seg));
        printf("Number of segments recieved: %d\n", seg);
        valread = read( sock , buffer, 1024);
        printf("Recieved message: %s\n",buffer );
        int msglen=strlen(buffer);
        int rows=seg+1;
        int cols=(msglen/rows);
        int rowparity[rows];
        int colparity[cols];
        for(int i=0; i<rows; i++){
            int paritysum=0;
            for(int j=0; j<cols;j++){
                if(buffer[i*cols+j]=='1') paritysum++;
            }
            if(paritysum%2==1) rowparity[i]=1;
            else rowparity[i]=0;
        }
        for(int j=0; j<cols; j++){
            int paritysum=0;
            for(int i=0; i<rows;i++){
                if(buffer[i*cols+j]=='1') paritysum++;
            }
            if(paritysum%2==1) colparity[j]=1;
            else colparity[j]=0;
        }
        int errfound=0;
        for(int j=0; j<rows-1;j++){
            if(rowparity[j]==1){
                printf("Found Error at Row: %d\n", j+1);
                errfound=1;
            }
        }
        
        for(int j=0; j<cols-1;j++){
            if(colparity[j]==1){
                printf("Found Error at Column: %d\n", j+1);
                errfound=1;
            }
        }
        if(errfound==0){
            printf("Error detected: No\n");
            int origmsglen=(rows-1)*(cols-1);
            char msg_decode[origmsglen];
            int k=0;
            for(int i=0; i<rows-1; i++){
                for(int j=0; j<cols-1;j++){
                    msg_decode[k]=buffer[i*cols+j];
                    k++;
                }
            }
            msg_decode[origmsglen]='\0';
            printf("Decoded message: %s\n", msg_decode);
        }

    }
//Checksum
    else if(algo==3){
        int seg;
        valread = read( sock , &seg, sizeof(seg));
        printf("Number of segments recieved: %d\n", seg);
        valread = read( sock , buffer, 1024);
        printf("Recieved message: %s\n",buffer );
        int msglen=strlen(buffer);
        int rows=seg+1;
        int cols=(msglen/rows);
        char *buf_msg[rows];
        for(int i=0; i<rows; i++){
            buf_msg[i]=(char*)malloc(cols*sizeof(char));
        }
        for(int i=0; i<rows; i++){
            for(int j=0; j<cols; j++){
                buf_msg[i][j]=buffer[i*cols+j];
            }
        }
        char *sum= (char*)malloc(cols+1);
        for(int i=0; i<cols+1; i++){
            sum[i]='0';
        }
        sum[cols+1]='\0';
        for(int i=0; i<rows; i++){
            sum=addBinary(sum,buf_msg[i]);
            if(sum[0]=='1'){
                sum[0]='0';
                sum=addBinary(sum,"1");
            }
        }
        char *checksum=(char*)malloc(cols);
        for(int j=0; j<cols; j++){
            checksum[j]=(((sum[j+1]-'0')+1)%2)+'0';
        }
        checksum[cols]='\0';
        printf("Checksum: %s\n", checksum);
        int errfound=0;
        for(int j=0; j<cols;j++){
            if(checksum[j]=='1') errfound=1;
        }
        if(errfound==0){
            printf("No error found\n");
            int origmsglen=msglen-cols;
            char msg_decode[origmsglen];
            
            for(int i=0; i<origmsglen; i++){
                msg_decode[i]=buffer[i];
            }
            msg_decode[origmsglen]='\0';
            printf("Decoded message: %s\n", msg_decode);
        }
        else{
            printf("Error found\n");
        }
    }
//Cyclic Redundancy Check
    else if(algo=4){
        int divlen;
        valread = read( sock , &divlen, sizeof(divlen));
        printf("Divisor length recieved: %d\n", divlen);
        
        char divisor[divlen];
        valread = read( sock , divisor, strlen(divisor));
        divisor[divlen]='\0';
        printf("Divisor recieved: %s\n", divisor);
        valread = read( sock , buffer, 1024);
        printf("Recieved message: %s\n",buffer);
        int msglen=strlen(buffer);
        
        char temp[divlen],quot[msglen],rem[divlen],div1[divlen];
        strcpy(div1,divisor);
        for (int i=0;i<divlen;i++) temp[i]=buffer[i];
        for (int i=0;i<msglen-divlen+1;i++) {
            quot[i]=temp[0];
            if(quot[i]=='0'){
            for (int j=0;j<divlen;j++) div1[j]='0';
            } 
            else{
                for (int j=0;j<divlen;j++) div1[j]=divisor[j];
            } 
            for (int j=divlen-1;j>0;j--) {
                if(temp[j]==div1[j])
                rem[j-1]='0'; else
                rem[j-1]='1';
            }
            rem[divlen-1]=buffer[i+divlen];
            strcpy(temp,rem);
        }
        strcpy(rem,temp);
        rem[divlen-1]='\0';
        
        printf("Remainder: %s\n",rem);
        int errfound=0;
        for(int j=0; j<divlen-1;j++){
            if(rem[j]=='1') errfound=1;
        }
        if(errfound==0){
            printf("No error found\n");
            int origmsglen=msglen-divlen+1;
            char msg_decode[origmsglen];
            for(int i=0; i<origmsglen; i++){
                msg_decode[i]=buffer[i];
            }
            msg_decode[origmsglen]='\0';
            printf("Decoded message: %s\n", msg_decode);
        }
        else{
            printf("Error found\n");
        }

    }
    return 0; 
}