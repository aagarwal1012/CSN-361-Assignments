/* 
 *  @author Ayush Agarwal, 17114017
*/
#include <bits/stdc++.h>
#include <string>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#define PORT 8000

using namespace std;

string copy(char *arr) {
    string a = "";
    while(*arr != '\n') {
        a += *arr;
        arr++;
    }
    return a;
}

string correct(string cword) {
    int csize = cword.length();
    int nrb = ceil(log2(csize + 1));
    cout << "Number of redundancy bits " << nrb << endl;
    int rb[nrb];
    // copying the code word to an integer array
    int cw[csize + 1];
    for(int i=1; i<=csize; i++) cw[csize-i+1] = int(cword[i-1]) - int('0');
    for(int i=csize; i>0; i--) cout << cw[i] << " ";
    cout << endl;
    int i,i1, i2, i3, i4, i5, i6, i7, a1[4], a2[4];
    int l;
    int k;
    l = csize;
    i5 = 0;
    k = 1;
    //Scan Whole frame
    for(i1=1;i1<=l;i1++)
    {       
        //Do processing only for redundancy bits
        if (i1==k)
        {
            //Get the position of 1 from binary representation
            i4 = -1; //Hold position of 1
         
            //Converting redundancy bit into binary form and get position of 1
            i7=i1;
            while (i7>0)
            {
                i4++;
                if(i7==1)
                {
                    break;
                }
                else
                {
                    i7=i7/2;
                }
            }
 
            i3=0; // Consider even parity
         
            //Calculating redundancy bit and parity
            for(i2=1;i2<=l;i2++)
            {
                //reset a1 frame
                for(i6=0;i6<4;i6++)
                {
                    a1[i6]=-1;
                }
             
                //find binary conversion
                i7=i2;
                i6=0;
                while(i7>0)
                {
                    if(i7==1)
                    {
                        a1[i6]=1;
                        break;
                    }
                    else if(i7==0)
                    {
                        a1[i6]=0;
                    }
                    else
                    {
                        a1[i6]=i7%2;
                        i7=i7/2;
                    }
                    i6++;
                }
             
                //check that the binary conversion is having bit at specific position or not
                //if yes calculate parity
             
                if(a1[i4]==1)
                {
                    if(cw[i2]==1)
                    {
                        i3++;
                    }
                }
                 
            }
 
            //Display Parity
            //Add redundancy bit in data frame
            //odd parity -> 1 :: even parity -> 0
         
            if(i3%2==0)
            {
                cw[i1]=0;
                rb[i5]=0;
            }
            else
            {
                cw[i1]=1;
                rb[i5]=1;
            }
         
            i5++;
         
            k=k*2;
        }
 
    }


    // print the redundancy bits
    cout <<"\n Redundancy bits:";
    for(i=0;i<nrb;i++)
    {
        cout<<" "<<rb[i];
    }


    //Calculate error position
    int ep=0;
    int j;
    for(i=0,j=1;i<nrb;i++)
    {
        if(rb[i]==1)
        {
            ep=ep+j;
        }
        j=j*2;
    }

    cout << "Assuming single error, the error location is " << ep << endl;
    cw[ep] = !cw[ep];
    string a="";


    // appending only the data word parts 
    for(int i=l; i>0; i--) if(log2(i) - int(log2(i)) != 0)a += char(cw[i] + int('0'));
    return a;
}
int main(int argc, char const *arguments[]) {

    // basic socket programming
    int socket_desc, /// socket_desc : server socket descriptor
    new_socket,     /// new_socket : client socket descriptor
    client_len;     /// client_len : length for storing client information
    char buffer[256]; /// buffer : for communicating with the client
    int n;          /// n : flag for checking whether the write operations are done correctly

    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_desc < 0) {
        cout << "Unable to create the sever socket" << endl;
        exit(1);
    } ///1.create the server socket, using a tcp connection (reliable, connection oriented)

    struct sockaddr_in server_address, client_address;
    
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT); ///2.change the order in which the port number is stored

    ///3.bind the port to the specified port number
    int bind_result = bind(socket_desc, (sockaddr *)&server_address, sizeof(server_address));
    if(bind_result < 0) {
        cout << "Unable to bind the socket to a port" << endl;
        exit(1);
    }
    ///4.process will wait until a client establishes a connection

    listen(socket_desc, 1);
    client_len = sizeof(client_address);
    new_socket = accept(socket_desc, (sockaddr *)&client_address, (socklen_t*)&client_len);

    if(new_socket < 0) {
        cout << "Unable to connect with the client" << endl;
        exit(1);
    }
    /// 5.empty the buffer
    bzero(buffer, 256);
    n = read(new_socket, buffer, 256); ///6.read the data into the buffer
    cout << "The message from the client is " << buffer << endl; ///7.print the buffer value on the screen
    string cword = copy(buffer);
    cout << cword.length()  << endl;

    cout << "The data word is: " << correct(cword) << endl;
    n = write(new_socket, (const char*)"Read your message\n", 17); ///8.send a message back to the client socket

    if(n < 0) {
        cout << "Unable to write to the socket" << endl;
        exit(1);
    }

    return 0; ///9.exit the program
}