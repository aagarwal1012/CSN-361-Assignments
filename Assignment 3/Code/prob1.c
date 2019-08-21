/** @file prob1.c
 *  @brief Determine class, Network and Host ID of an IPv4 address. 
 *
 *  @author Ayush Agarwal, 17114017
 *  @date August 2019
    */
#include <stdio.h>
#include <string.h>

// Function to find out the Class
char findClass(char str[])
{
  // storing first octet in arr[] variable
  char arr[4];
  int i = 0;
  while (str[i] != '.')
  {
    arr[i] = str[i];
    i++;
  }
  i--;

  // converting str[] variable into number for
  // comparison
  int ip = 0, j = 1;
  while (i >= 0)
  {
    ip = ip + (str[i] - '0') * j;
    j = j * 10;
    i--;
  }

  // Class A
  if (ip >= 1 && ip <= 126)
    return 'A';

  // Class B
  else if (ip >= 128 && ip <= 191)
    return 'B';

  // Class C
  else if (ip >= 192 && ip <= 223)
    return 'C';

  // Class D
  else if (ip >= 224 && ip <= 239)
    return 'D';

  // Class E
  else
    return 'E';
}

// Function to separate Network ID as well as
// Host ID and print them
void separate(char str[], char ipClass)
{
  // Initializing network and host array to NULL
  char network[12], host[12];
  for (int k = 0; k < 12; k++)
    network[k] = host[k] = '\0';

  // for class A, only first octet is Network ID
  // and rest are Host ID
  if (ipClass == 'A')
  {
    int i = 0, j = 0;
    while (str[j] != '.')
      network[i++] = str[j++];
    i = 0;
    j++;
    while (str[j] != '\0')
      host[i++] = str[j++];
    printf("Network ID is %s\n", network);
    printf("Host ID is %s\n", host);
  }

  // for class B, first two octet are Network ID
  // and rest are Host ID
  else if (ipClass == 'B')
  {
    int i = 0, j = 0, dotCount = 0;

    // storing in network[] up to 2nd dot
    // dotCount keeps track of number of
    // dots or octets passed
    while (dotCount < 2)
    {
      network[i++] = str[j++];
      if (str[j] == '.')
        dotCount++;
    }
    i = 0;
    j++;

    while (str[j] != '\0')
      host[i++] = str[j++];

    printf("Network ID is %s\n", network);
    printf("Host ID is %s\n", host);
  }

  // for class C, first three octet are Network ID
  // and rest are Host ID
  else if (ipClass == 'C')
  {
    int i = 0, j = 0, dotCount = 0;

    // storing in network[] up to 3rd dot
    // dotCount keeps track of number of
    // dots or octets passed
    while (dotCount < 3)
    {
      network[i++] = str[j++];
      if (str[j] == '.')
        dotCount++;
    }

    i = 0;
    j++;

    while (str[j] != '\0')
      host[i++] = str[j++];

    printf("Network ID is %s\n", network);
    printf("Host ID is %s\n", host);
  }

  // Class D and E are not divided in Network
  // and Host ID
  else
    printf("In this Class, IP address is not"
           " divided into Network and Host ID\n");
}

// Driver function is to test above function
int main()
{
  char str[100];
  printf("Enter the IPv4 address:\n");
  fgets(str, 100, stdin);
  char ipv4Class = findClass(str);
  printf("Given IP address belongs to Class %c\n", ipv4Class);
  separate(str, ipv4Class);
  return 0;
}
