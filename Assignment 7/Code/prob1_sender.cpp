/* 
 *  @author Ayush Agarwal, 17114017
*/

#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
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
    int server_fd, new_socket, valread; 
    struct sockaddr_in address; 
    int opt = 1; 
    int addrlen = sizeof(address); 
    char buffer[1024] = {0}; 
    //char *hello = "Hello from server"; 
       
    // Creating socket file descriptor 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
       
    // Forcefully attaching socket to the port 8080 
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                  &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( PORT ); 
       
    // Forcefully attaching socket to the port 8080 
    if (bind(server_fd, (struct sockaddr *)&address,  
                                 sizeof(address))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
    if (listen(server_fd, 3) < 0) 
    { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    } 
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,  
                       (socklen_t*)&addrlen))<0) 
    { 
        perror("accept"); 
        exit(EXIT_FAILURE); 
    } 

    printf("Choose which algorithm to check:\n 1. Single Parity Check\n 2.Two-dimensional Parity Check\n 3.Checksum\n 4.Cyclic Redundancy Check (CRC)\n");
    int algo;
    scanf("%d", &algo);
    send(new_socket , &algo , sizeof(algo) , 0 ); 
    
//Single parity check
    if(algo==1){
        int mlen;
        printf("Enter Message length:\n");
        scanf("%d", &mlen);
        char message[mlen+2];
        printf("Enter the message to send\n"); 
        scanf("%s", message);
        int paritysum=0;
        
        for(int i=0;i<mlen; i++){
            int x=message[i]-'0';
            if(x==1) paritysum++;
        }
        
        int paritybit=0;
        if(paritysum%2==1) paritybit=1;
        char parity=paritybit+'0';
        message[mlen]=parity;
        message[mlen+1]='\0';
        //int msglen=sizeof(message);
        //printf("%d\n", msglen);
        printf("Message after adding parity: %s\n", message);
        
        printf("1.Add an error \n2.Transmit the message\n");
        int q;
        scanf("%d", &q);
        if(q==1){
        printf("Choose how to add an error\n 1.Manually add an error\n 2.Randomly add an error\n");
        int e;
        scanf("%d", &e);
            if(e==1){
                printf("Enter number of bits to be flipped\n");
                int b;
                scanf("%d", &b);
                for(int i=0;i<b;i++){
                    printf("Enter index to flip\n");
                    int ind;
                    scanf("%d", &ind);
                    message[ind-1]=(((message[ind-1]-'0')+1)%2)+'0';
                }
            }
            else{
                printf("Enter probability of induced error\n");
                float prob;
                scanf("%f", &prob);
                for(int i=0;i<mlen+1;i++){
                    float rndno= (float)rand()/(float)RAND_MAX;
                    //printf("%f",rndno);
                    if(rndno>prob){
                        message[i]=(((message[i]-'0')+1)%2)+'0';
                    }
                }

            }
        printf("message after adding error: %s\n", message);
        }
        send(new_socket , message , strlen(message) , 0 ); 
        
    }

//Two-dimensional Parity Check
    else if(algo==2){
        int mlen;
        printf("Enter Message length:\n");
        scanf("%d", &mlen);
        printf("Enter Number of segments of message\n");
        int seg;
        scanf("%d", &seg);
        
        send(new_socket , &seg , sizeof(seg) , 0 ); 
        
        int rows=seg+1;
        int cols=(mlen/seg)+1;
        
        int *parity2d[rows];
        for(int i=0; i<rows; i++){
            parity2d[i]=(int *)malloc(cols*sizeof(int));
        }
        printf("Enter %d segments of %d bits message\n", seg, mlen);
        for(int i=0; i<rows-1; i++){
            char segment[cols-1];
            scanf("%s",segment);
            int paritysum=0;
            for(int j=0; j<cols-1; j++){
                int val=segment[j]-'0';
                if(val==1) paritysum++;
                parity2d[i][j]=val;
            }
            if(paritysum%2==1) parity2d[i][cols-1]=1;
            else parity2d[i][cols-1]=0;
        }
        for(int j=0; j<cols;j++){
            int paritysum=0;
            for(int i=0; i<rows-1; i++){
                    if(parity2d[i][j]==1) paritysum++;   
            }
            if(paritysum%2==1) parity2d[rows-1][j]=1;
            else parity2d[rows-1][j]=0;
        }

        printf("Two dimensional parity matrix\n");
        for(int i=0; i<rows; i++){
            for(int j=0; j<cols;j++){
                printf("%d ", parity2d[i][j]);
            }
            printf("\n");
        }

        printf("1.Add an error \n2.Transmit the message\n");
        int q;
        scanf("%d", &q);
        if(q==1){
        printf("Choose how to add an error\n 1.Manually add an error\n 2.Randomly add an error\n");
        int e;
        scanf("%d", &e);
            if(e==1){
                printf("Enter number of bits to be flipped\n");
                int b;
                scanf("%d", &b);
                for(int i=0;i<b;i++){
                    printf("Enter the row and column of bit to flip\n");
                    int ind1, ind2;
                    scanf("%d", &ind1);
                    scanf("%d", &ind2);
                    parity2d[ind1-1][ind2-1]=(parity2d[ind1-1][ind2-1]+1)%2;
                }
            }
            else{
                printf("Enter probability of induced error\n");
                float prob;
                scanf("%f", &prob);
                for(int i=0; i<rows; i++){
                    for(int j=0; j<cols;j++){
                    float rndno= (float)rand()/(float)RAND_MAX;
                    //printf("%f",rndno);
                        if(rndno>prob){
                            parity2d[i][j]=(parity2d[i][j]+1)%2;
                        }
                    }
                }
            }
            printf("Two dimensional parity matrix after adding error\n");
            for(int i=0; i<rows; i++){
                for(int j=0; j<cols;j++){
                    printf("%d ", parity2d[i][j]);
                }
                printf("\n");
            }
        }
        char message2[rows*cols];
        for(int i=0; i<rows; i++){
            for(int j=0; j<cols;j++){
                message2[i*cols+j]=parity2d[i][j]+'0';
            }
        }
        message2[rows*cols]='\0';
        printf("Transmitted message: %s\n", message2);
        send(new_socket , message2 , strlen(message2) , 0 );
    }

//Checksum
    else if(algo==3){
        int mlen;
        printf("Enter Message length:\n");
        scanf("%d", &mlen);
        printf("Enter Number of segments of message\n");
        int seg;
        scanf("%d", &seg);
        
        send(new_socket , &seg , sizeof(seg) , 0 );
        int rows=seg+1;
        int cols=(mlen/seg);
        printf("Enter %d segments of %d bits message\n", seg, mlen);
        char *message3[rows];
        for(int i=0; i<rows; i++){
            message3[i]=(char*)malloc(cols*sizeof(char));
        }
        for(int i=0; i<rows-1; i++){
            scanf("%s", message3[i]);
        }
        char *sum= (char*)malloc(cols+1);
        for(int i=0; i<cols+1; i++){
            sum[i]='0';
        }
        sum[cols+1]='\0';
        for(int i=0; i<rows-1; i++){
            sum=addBinary(sum,message3[i]);
            if(sum[0]=='1'){
                sum[0]='0';
                sum=addBinary(sum,"1");
            }
        }
        for(int j=0; j<cols; j++){
            message3[rows-1][j]=(((sum[j+1]-'0')+1)%2)+'0';
        }
        printf("Checksum: %s\n", message3[rows-1]);
        char transmitmsg[rows*cols];
        for(int i=0; i<rows; i++){
            for(int j=0; j<cols;j++){
                transmitmsg[i*cols+j]=message3[i][j];
            }
        }
        transmitmsg[rows*cols]='\0';
    printf("Message after adding checksum: %s\n", transmitmsg);
    
    printf("1.Add an error \n2.Transmit the message\n");
    int q;
    scanf("%d", &q);
    if(q==1){
    printf("Choose how to add an error\n 1.Manually add an error\n 2.Randomly add an error\n");
    int e;
    scanf("%d", &e);
        if(e==1){
            printf("Enter number of bits to be flipped\n");
            int b;
            scanf("%d", &b);
            for(int i=0;i<b;i++){
                printf("Enter index to flip\n");
                int ind;
                scanf("%d", &ind);
                transmitmsg[ind-1]=(((transmitmsg[ind-1]-'0')+1)%2)+'0';
            }
        }
        else{
            printf("Enter probability of induced error\n");
            float prob;
            scanf("%f", &prob);
            for(int i=0;i<rows*cols;i++){
                float rndno= (float)rand()/(float)RAND_MAX;
                //printf("%f",rndno);
                if(rndno>prob){
                    transmitmsg[i]=(((transmitmsg[i]-'0')+1)%2)+'0';
                }
            }

        }
    }
    printf("Transmitted message: %s\n", transmitmsg);
    
    send(new_socket , transmitmsg , strlen(transmitmsg) , 0 );

    }
//Cyclic Redundancy Check(CRC)    
    else if(algo==4){

        int divlen;
        printf("Enter length of Divisor:\n");
        scanf("%d", &divlen);

        send(new_socket , &divlen , sizeof(divlen) , 0 );

        char divisor[divlen];
        printf("Enter Divisor:\n");
	    scanf("%s", divisor);
        divisor[divlen]='\0';
        send(new_socket , divisor , strlen(divisor) , 0 );

        int mlen;
        printf("Enter Message length:\n");
        scanf("%d", &mlen);
        char message4[mlen+divlen-1];
        printf("Enter the message to send\n"); 
        scanf("%s", message4);
        for(int i=mlen; i<mlen+divlen-1; i++){
            message4[i]='0';
        }
        message4[mlen+divlen-1]='\0';
        char temp[divlen],quot[mlen+divlen-1],rem[divlen],div1[divlen];
        strcpy(div1,divisor);
        for (int i=0;i<divlen;i++) temp[i]=message4[i];
        for (int i=0;i<mlen;i++) {
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
            rem[divlen-1]=message4[i+divlen];
            strcpy(temp,rem);
        }
        strcpy(rem,temp);
        rem[divlen-1]='\0';
        
        printf("Remainder: %s\n",rem);
        char *transmitmsg=(char*)malloc(mlen+divlen-1);;
        transmitmsg=addBinary(message4,rem);
        
        printf("Message after CRC: %s\n", transmitmsg);
    
        printf("1.Add an error \n2.Transmit the message\n");
        int q;
        scanf("%d", &q);
        if(q==1){
        printf("Choose how to add an error\n 1.Manually add an error\n 2.Randomly add an error\n");
        int e;
        scanf("%d", &e);
            if(e==1){
                printf("Enter number of bits to be flipped\n");
                int b;
                scanf("%d", &b);
                for(int i=0;i<b;i++){
                    printf("Enter index to flip\n");
                    int ind;
                    scanf("%d", &ind);
                    transmitmsg[ind-1]=(((transmitmsg[ind-1]-'0')+1)%2)+'0';
                }
            }
            else{
                printf("Enter probability of induced error\n");
                float prob;
                scanf("%f", &prob);
                for(int i=0;i<mlen+divlen-1;i++){
                    float rndno= (float)rand()/(float)RAND_MAX;
                    //printf("%f",rndno);
                    if(rndno>prob){
                        transmitmsg[i]=(((transmitmsg[i]-'0')+1)%2)+'0';
                    }
                }
            }
        }
        printf("Transmitted message: %s\n", transmitmsg);
        
        send(new_socket , transmitmsg , strlen(transmitmsg) , 0 );

    }
    return 0; 
}