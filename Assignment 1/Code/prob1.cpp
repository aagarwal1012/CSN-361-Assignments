/*
 * Name - Ayush Agarwal
 * ENo. - 17114017
 */
#include <stdio.h>
#include <unistd.h>

int main()
{

  int processId = -1,
      processId1 = -1,
      processId2 = -1,
      processId11 = -1,
      processId12 = -1,
      processId21 = -1,
      processId22 = -1;

  processId1 = fork(); // first child
  processId2 = fork(); // second child

  if (processId1 > 0 and processId2 > 0) // parent
  {
    printf("Process Id of first child is %d.\n", processId1);
    printf("Process Id of second child is %d.\n", processId2);
  }

  else if (processId2 == 0 and processId1 != 0)
  {
    processId21 = fork();

    if (processId21 != 0) // second child
    {
      processId22 = fork();
      if (processId22 != 0) // second child
      {
        printf("Process Id of third grandchild is %d.\n", processId21);
        printf("Process Id of fourth grandchild is %d.\n", processId22);
        printf("Process Id of parent is %d.\n", getppid());
      }
    }
  }

  else if (processId1 == 0 and processId2 > 0) // first child
  {
    processId11 = processId2;
    processId12 = fork();
    if (processId12 != 0) // first child
    {
      printf("Process Id of first grandchild is %d.\n", processId11);
      printf("Process Id of second grandchild is %d and its parent id is %d.\n", processId12, processId2);
    }
  }
}
