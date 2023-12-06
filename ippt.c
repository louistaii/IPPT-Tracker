  #include <stdio.h>
  #include <stdlib.h>
  #include <time.h>


  // declaring global variables
  int curr_day; int curr_month; int curr_year;
  int age;      int bday;       int agegrp;  int loadage;
  int pbpu;     int pbsu;       int pbrun;   int pbpoints; 
  
  void menu();


  void update_savefile()
  {
    FILE *newpb = fopen("data/savefile.txt", "w");
    fprintf(newpb, "%d\n", pbpu);
    fprintf(newpb, "%d\n", pbsu);
    fprintf(newpb, "%d\n", pbrun);
    fprintf(newpb, "%d", loadage);
    fclose(newpb);
 }


  // Returns gold/silver/pass based of points achieved
  char *calctier (int points)
  {
    if (points >= 85)
    {
      return "Gold";
    }
    if (points >= 75)
    {
      return "Silver";
    }
    if (points >= 61)
    {
      return "Pass w Incentive";
    }
    if (points >= 51)
    {
      return "Pass";
    }
    return "Fail";
  }


  //opens and read pushup/ situp score chart
  int readchart(int rep, char* table)
  {
    FILE *chart = fopen(table, "r");
    int count = 0;
    if (chart == NULL)
    {
      printf("Chart does not exist");
      return 0;
    }
    char line[300];
    rep = 60-rep;
    while (fgets(line,sizeof line, chart)!= NULL)
    {
      if (count == rep)
      {
        //printf("%s", line);
        int tens = (line[agegrp * 3] -'0');
        int ones =(line[ 1 + agegrp * 3] -'0');
        //printf("%d", tens);
        //printf("%d", ones);
        return tens *10 + ones;
      }
      count +=1;
    }

    fclose(chart);
  }




  // calculate pushup points by comparing reps to chart
  int pupoints (int rep)
  {
    int points;
    int maxrep = 60 + 1 - agegrp;   //rep that gives full points
    int minrep = 15 + 1 - agegrp;   // rep that gives least points
    if (rep >= maxrep)
    {
      return 25;
       }
    if (rep < minrep)
    {
      return 0;
    }

    points = readchart(rep, "data/pushuptable.txt");
    return points;
  }




  // calculate situp points by comparing reps to chart
  int supoints (int rep)
  {
    int points;
    int maxrep = 60 + 1 - agegrp;
    int minrep = 15 + 1 - agegrp;
    if (rep >= maxrep)
    {
      return 25;
    }
    if (rep < minrep)
    {
      return 0;
    }
    
    points = readchart(rep, "data/situptable.txt");
    return points;
  }



  //calculate 2.4 points
  int runpoints(int time)
  {
    if (time <= 510)
    {
      return 50;
    }
    if (time > 1100)
    {
      return 0;
    }

    // find row of table where timing lies in
    int row;
    if (time%10 != 0)
    {
      row = (time -500)/10;
    }
    else
    {
      row = (time-500)/10 - 1;
    }

    //open table
    FILE *chart = fopen("data/runtable.txt", "r");
    int count = 0;
    if (chart == NULL)
    {
      printf("Chart does not exist");
      return 0;
    }
    char line[300];
    while (fgets(line,sizeof line, chart)!= NULL)
    {
      if (count == row)
      {
        int tens = (line[2+agegrp * 3] -'0');    //finds the two columns containing the points
        int ones = (line[ 3 + agegrp * 3] -'0');
        return tens *10 + ones;
      }
      count +=1;
    }

    fclose(chart);
  }



  //finds age group for scoring using given age
  void set_age_grp()
  {
    agegrp = 1;
    for(long i = 22; i<=60; i+=2)
    {
      if (age <= i)
      {
        break;
      }
      agegrp += 1;
    }
  }



  void editage()
  {
    system("cls");

    printf(" Enter your D.O.B (DDMMYYYY):");
    scanf("%d", &bday);

    if (bday == NULL)
    {
      menu();
      return;
    }

    //calc date month and year base off input
    int birthmonth = (bday/10000) % 100;
    int birthdate = bday/1000000;
    int birthyear = bday%10000;



    if (birthyear >= curr_year)  //in the case of an invalid year input
    {
      printf(" Enter valid age");
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
    loadage = bday*100 + age;
    update_savefile();
    set_age_grp();
    menu();
  }


  void calcstat(int initial, int pb)
  {
    if (pb - initial > 0)
    {
      printf(" (%d reps to PB: %d)\n", pb-initial, pb);
      return;
    }
    printf(" (PERSONAL BEST)\n");
    return;
  }


  void newworkout()
  {
    system("cls");
    int stats[3];
    printf(" Enter Pushup reps, Situp reps and 2.4km run time in seconds: \n");
    //FILE *workoutlog = fopen("log.txt", "a");
    for (long i = 0; i<3; i+=1)
    {
      scanf("%d", &stats[i]);
      //fprintf(workoutlog, "%d\n", stats[i]);
    }
    //fclose(workoutlog);



    int pushup = pupoints(stats[0]);
    int situp = supoints(stats[1]);
    int run = runpoints(stats[2]);
    int total = pushup + situp+ run;

    //in the case of new PB
    if (total > pbpoints)
    {
      pbpu = stats[0];
      pbsu = stats[1];
      pbrun = stats[2]; 
      update_savefile();
      printf("\n NEW PERSONAL BEST!");
    }
  
    printf(" GOODJOB! \n Pushups: %d Points",pushup);
    calcstat(stats[0], pbpu); //edit to points
    printf(" Situps: %d Points", situp);
    calcstat(stats[1],pbsu);
    printf(" 2.4km run: %d Points", run);
    if (stats[2]>pbrun)
    {
      printf(" (%ds to PB)", stats[2]-pbrun);
    }
    else
    {
      printf(" PERSONAL BEST");
    }
    printf("\n Total: %d Points, ", total);
    printf("%s\n", calctier(pushup+situp+run));
    
    printf("\n Enter any key to return to menu");
    char next;
    scanf(" %c",&next);
    menu();
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

    int daycount = 0;
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


  void menu()  //main menu
  {
    system("cls");
    pbpoints = pupoints(pbpu) + supoints(pbsu) + runpoints(pbrun); //calculate ppersonal best points base off reps
    char* tier = calctier(pbpoints);   //calculating tier for best PB score
    int cycle = cycledays();       // days left until next birthday = end of ippt cycle

    printf("   ###  ###  ###  ### \n");
    printf("    #   # #  # #   #  \n");
    printf("    #   ###  ###   #  \n");
    printf("    #   #    #     #  \n");
    printf("   ###  #    #     #  \n");

    printf("\n 1. Age: %d \n", age);
    printf(" 2. New Workout \n");
    printf("\n");
    printf(" Age group for scoring: %d \n", agegrp);
    printf(" IPPT cycle ends in: %d days \n", cycle);
    printf("\n PERSONAL BEST: \n %d points, ", pbpoints);
    printf("%s \n", tier);
    printf(" Pushups: %d reps\n", pbpu);
    printf(" Situps: %d reps\n", pbsu);
    printf(" 2.4km run: %ds\n", pbrun);
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

    //load PB , birthday and age data from savefile.txt
    FILE *chart = fopen("data/savefile.txt", "r");
    int count = 0;
    if (chart == NULL)
    {
      printf("savefile does not exist");
      return 1;
    }
    char line[100];
    while (fgets(line,sizeof line, chart)!= NULL)
    {
      if (count == 0)
      {
        pbpu = atoi(line);
      }
      else if (count == 1)
      {
        pbsu = atoi(line);
      }
      else if (count == 2)
      {
        pbrun = atoi(line);
      }
      else
      {
        loadage = atoi(line);
      }
      count +=1;
    }
    age = loadage %100;
    bday = loadage/100;

    // calculate and set age grp for ippt scoring
    set_age_grp(); 

    menu();
    return 0;

   

  }