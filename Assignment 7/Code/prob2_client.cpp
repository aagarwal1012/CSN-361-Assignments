/* 
 *  @author Ayush Agarwal, 17114017
*/
#include <bits/stdc++.h>
#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#define PORT 8000

using namespace std;

int main(int argc, char const *argv[]) 
{ 
    // get the data word
    int dsize;
    cout << "Enter the size of the data word: ";
    cin >> dsize; // Get the size of the message to be entered
    int dword[dsize];
    for(int i=0; i<dsize; i++) cin >> dword[i];

    // calculate the number of redundancy bits
    int r = 1;
    while(int(pow(2, r)) - 1 - r < dsize) {
        r++;
    }
    cout << "The number of redundancy bits is: " << r << endl;



    // Decode and produce the code word
    int i, j, k;
    int temp[30];
    for (i=dsize-1,j=0;i>=0;i--,j++)
    {
        temp[j]=dword[i];
    }
    for (i=0;i<dsize;i++)
    {
        dword[i]=temp[i];
    }
 
    /* Redundancy bits */
    int nrb=0;      // Counter for number of redundancy bit
    int rb[8];      // Array to hold redundancy bits
 
    /* New Frame after introducing redundancy bits */
    int l=0;        // Length of new frame = l = m+r
    int df[30];     // New frame
 
    // Introduce redundancy bits and construct new frame

    k=1;
    for(i=1,j=0;j<dsize;i++)
    {
        //If it is location of redundancy bit
        if(i==k)
        {
            df[i]=-1;
            k=k*2;
            nrb++;
        }
        //If it is location of data bit
        else
        {
            df[i]=dword[j];
            j++;
        }
        l++;
    }
 
    // cout<<"\n";
    // cout<<"\n Frame after introducing redundancy bits :\n ";
    // for(i=l;i>0;i--)
    // {
    //     cout<<df[i]<<" ";
    // }
 
    //Some more temp variables and arrays
    int i1,i2,i3,i4,i5,i6,i7;
    int a1[4],a2[4];
     
    i5=0;       //Holds position of rb[]
 
    //Scan Whole frame and calculate all redundancy bits
    for(i1=1;i1<=l;i1++)
    {
        //check if it is redundancy bit which is to be calculated
        if(df[i1]==-1)
        {
            //Get the position of 1 from binary representation
            i4 = -1;    //Hold position of 1
             
            //Converting redundancy bit position into binary form and get position of 1
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
 
                //find binary conversion for each position
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
                    if(df[i2]==1)
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
                df[i1]=0;
                rb[i5]=0;
            }
            else
            {
                df[i1]=1;
                rb[i5]=1;
            }
            i5++;
 
            // cout<<"\n R"<<i5<<" = "<<rb[i5-1]<<"\t New Frame";
            // for(i2=l;i2>0;i2--)
            // {
            //     cout<<" "<<df[i2];
            // }
        }
     
    }
 
    cout << "The encoded code word is: ";
    for(i=l; i>0; i--) cout << df[i] << " ";
    cout << endl;


    // Introduce errors in the code word
    int err_size;
    cout << "Enter the number of errors ( we can correct only one error ): ";
    cin >> err_size;
    for(i=0; i<err_size; i++) {
        cout << "Enter the location of the erroneous bit: ";
        int loc;
        cin >> loc;
        cout << "Flipping the "  << loc << " bit" << endl;
        df[loc] = (df[loc]==0)? 1:0;
        for(j=l; j>0; j--) cout << df[j] << " ";
        cout << endl;
    } 



    // copying the integer array to a character array
    string cword = "";
    for(i=l; i>0; i--) cword += char(df[i] + int('0'));
    cword += "\n";



    // socket programming
    int sock = 0, /// sock : to store the socket descriptor
    valread;   /// valread : to check the status of reading a message from the buffer
    struct sockaddr_in serv_addr;  /// serv_addr : to store the address of the server
    char buffer[1024] = {0}; /// buffer : to read and write messages
    sock = socket(AF_INET, SOCK_STREAM, 0); ///1.create a socket of AF_INET address family
    if(sock < 0) 
    { 
        cout << "Unable to create a socket" << endl;
        exit(1);
    } 
   
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); ///2.Change the representation of the port number
       
    ///3.Convert IPv4 addresses from text to binary form 
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  
    { 
        cout << "Invalid address" << endl;
        exit(1);
    } 
    
    ///4.Connect to the server using the connect function
    int connect_status = connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (connect_status < 0) 
    { 
        cout << "Connection failed" << endl;
        exit(1);
    } 

    cout << "Client : Sending the code word\n";
    ///5. Send a message to the server
    send(sock , cword.c_str() , cword.size(), 0 ); 

    ///6.Wait till the server sends a message back.
    ///7.Store the message in the buffer.
    valread = read(sock , buffer, 1024); 
    if(valread < 0) {
        cout << "read error" << endl;
        exit(1);
    }
    ///8.Print the buffer value.
    cout << "Message from server : " << buffer << endl;
    return 0; 
} 