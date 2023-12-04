#include <stdio.h>
  #include <stdlib.h>
  #include <time.h>

  int curr_day;
  int curr_month;
  int curr_year;
  int age;
  int bday;
  void menu();

  /*
  int pupoints (int agerange, int pushup)
  {
    int points = 0;
    int maxrep = 60 + 1 - agerange;
    int minrep = 15 + 1 - agerange;
    if (pushup < minrep)
    {
      points+=0;
    }
    else if (pushup <=minrep + 10)
    {
      points += pushup - minrep;
    }
    else
    {
      points = 15
        for (long i = minrep; i<=maxrep; i+=4)
        {
          if (pushup <= i)
          {

          }
          else
          {
            points +=
          }
                }
    }
  }


  int totalpoints(int pushup, int situp, int run)
  {
    int agegrp = 1;
    int points = 0;

    //find age grp
    for(long i = 22; i<=60; i+=2)
    {
      if (age <= i)
      {
        break;
      }
      else
      {
        agegrp+=1;
      }
    }

    points = pupoints(agegrp, pushup) + supoints(agegrp, situp) + runpoint(agegrp, run);
    return points;
  }




  */





  void editage()
  {
     system("cls");


    FILE *saveage = fopen("age.txt", "w"); //open age.txt to write

    if (saveage == NULL)
    {
      printf("File does not exist");
    }

    printf(" Enter your DOB (DDMMYYYY):");
    scanf("%d", &bday);

    //calc date month and year base off input
    int birthmonth = (bday/10000) % 100;
    int birthdate = bday/1000000;
    int birthyear = bday%10000;



    if (birthyear >= curr_year)  //in the case of an invalid year input
    {
       printf("Enter valid age");
       editage();
    }


    //age calculation
    if (curr_month >= birthmonth)
    {
      age = curr_year - birthyear;
    }
    else
    {
      age = curr_year - birthyear -1;
    }

      //saving age and bday to file
    int savedint = bday*100 + age;
    fprintf(saveage, "%d", savedint);
    fclose(saveage);
    menu();
  }



  void newworkout()
  {
    int stats[3];
    printf("Enter Pushup reps, Situp reps and 2.4km run time in seconds: \n");
    FILE *workoutlog = fopen("log.txt", "a");
    for (long i = 0; i<3; i+=1)
    {
      scanf("%d", &stats[i]);
      fprintf(workoutlog, "%d\n", stats[i]);
    }
    fclose(workoutlog);
    printf("Goodjob!");
   // printf("%d" , totalpoints(stats[0],stats[1],stats[2]));
    //stats
  }



  void choose()
  {
    int choice;
    scanf("%d", &choice);
    if (choice == 1)
    {
      editage();
    }
    else if(choice == 2)
    {
      newworkout();
        }
    else
    {
      printf("Please enter valid input: \n");
      choose();
    }
  }

  int cycledays()
  {

    int daycount =0;
    int birthmonth = (bday/10000) % 100;
    int birthdate = bday/1000000;
    int birthyear = bday%10000;
    int dayinmonth[12] = {31,28,31,30,31,30,31,31,30,31,30,31}; //days in each month from jan to dec


    //calculate days to nxt bday
    if (birthmonth == curr_month && birthdate >= curr_day)
    {
      daycount = birthdate - curr_day;
      return daycount;
    }

    if (birthmonth > curr_month)
    {
      for (int i = curr_month-1; i< birthmonth-1;i+=1)
      {
        daycount += dayinmonth[i];
      }
    }

    else
    {
      for (int i = curr_month-1; i< 12;i+=1)
      {
        daycount += dayinmonth[i];
    }
      for (int i = 0; i < birthmonth-1;i+=1)
      {
        daycount += dayinmonth[i];
      }
    }


    daycount+= birthdate;
    daycount -= curr_day;

    return daycount;
  }


  void menu()
  {
    system("cls");
    int best = 10101;
    double rank = 1;
    int cycle = cycledays();
    printf("   ###  ###  ### ### \n");
    printf("    #   # #  # #  #  \n");
    printf("    #   ###  ###  #  \n");
    printf("    #   #    #    #  \n");
    printf("   ###  #    #    #  \n");

    printf("\n 1.) Age: %d \n", age);
    printf(" 2.) New Workout \n");
    printf("\n");
    printf(" IPPT cycle ends in: %d days \n", cycle);
    printf(" Personal Best: %d points, ", best);
    printf("%lf \n", rank);
    printf("\n Select number to edit: ");
    choose();
  }



  int main()
  {
    // load current date with struct
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    curr_day = tm.tm_mday;
    curr_month = tm.tm_mon+1;
    curr_year = tm.tm_year + 1900;


    // load presaved age and bday
    FILE *readage = fopen("age.txt", "r");
    int loadage;
    fscanf(readage,"%d", &loadage);
    if (loadage == NULL)
    {
      age = 0;
      bday = 0;
    }
    fclose(readage);
    age = loadage %100;
    bday = loadage/100;
    menu();
    return 0;
  }