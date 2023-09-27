
#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

int nb_task;
float accessibilityRM = 0;
float accessibilityDM = 0;
float accessibilityEDF = 0;
int nbimaxacDM;
int nbimaxacRM;
int mnRM = 0;
int mnDM = 0;
int RMpossible = 0;
int DMpossible = 0;
int b = 0;
int diviseurcomm;
int a;
float ACCESScond;


int main()
{
    cout << "Number of tasks?: " << endl;
    cin >> nb_task;

    vector<int> priotriRM(nb_task);
    vector<int> priotriDM(nb_task);
    vector<float> tpriotriRM(nb_task);
    vector<float> tpriotriDM(nb_task);

    vector<vector<float>> tableau(nb_task, vector<float>(8));
    for(int i = 0; i < nb_task; i++)
    {
        cout << "Task number " << i << ":" << endl;
        cout << "Arrival time?: " << endl;
        cin >> tableau[i][0];
        cout << "Period?: " << endl;
        cin >> tableau[i][1];
        cout << "Deadline?: " << endl;
        cin >> tableau[i][2];
        cout << "Capacity?: " << endl;
        cin >> tableau[i][3];

        tableau[i][4]=1/tableau[i][1];
        tableau[i][5]=tableau[i][3]/tableau[i][1];

        tableau[i][6]=1/tableau[i][2];
        tableau[i][7]=tableau[i][3]/tableau[i][2];
    }

    ACCESScond = nb_task*(pow(2,(1/float(nb_task)))-1);

    for(int i = 0; i < nb_task; i++)
    {
        accessibilityRM += tableau[i][5];
    }

    if(accessibilityRM <= ACCESScond)
    {
        cout << "Tasks can be scheduled using RM" << endl;
        RMpossible = 1;
    }

    else
    {
        cout << accessibilityRM << " > " << ACCESScond<< endl;
        cout << "Tasks can't be scheduled using RM" << endl;
    }



    for(int i = 0; i < nb_task; i++)
    {
        accessibilityDM += tableau[i][7];
    }

    if(accessibilityDM <= ACCESScond)
    {
        cout << "Tasks can be scheduled using DM" << endl;
        DMpossible = 1;
    }

    else
    {
        cout << accessibilityDM << " > " << ACCESScond << endl;
        cout << "Tasks can't be scheduled using DM" << endl;
    }

    if(DMpossible == 1 || RMpossible == 1)
    {
        for(int i = 0; i < nb_task; i++)
        {
            tpriotriRM[i] = 0;
            tpriotriDM[i] = 0;
            priotriRM[i] = 0;
            priotriDM[i] = 0;
        }
    }


    for(int i = 0; i < nb_task; i++)
    {
        if(RMpossible == 1)
        {
            for(int j = 0; j < nb_task; j++)
            {
                if(tableau[j][4]>tpriotriRM[i])
                {
                    tpriotriRM[i] = tableau[j][4];
                    nbimaxacRM = j;
                }
            }
            priotriRM[i]=nbimaxacRM;
            tableau[nbimaxacRM][4]=0;
        }



        if(DMpossible == 1)
        {
            for(int j = 0; j < nb_task; j++)
            {
                if(tableau[j][6]>tpriotriRM[i])
                {
                    tpriotriDM[i] = tableau[j][6];
                    nbimaxacDM = j;
                }
            }

            priotriDM[i]=nbimaxacDM;
            tableau[nbimaxacDM][6]=0;
        }
    }



    if(RMpossible == 1 || DMpossible == 1)
    {
        cout << "Searching for the total cycle period" << endl;
        int periodmax = 0;
        for(int j = 0; j < nb_task; j++)
        {
            if(periodmax<tableau[priotriDM[j]][1])
            {
                periodmax = tableau[priotriDM[j]][1];
            }
        }


        // Find the least common multiple
        diviseurcomm = periodmax;
        a = 1;
        cout << "Maximum period found: " << periodmax <<endl;

        while(a!=0)
        {
            a=0;
            for(int j = 0; j < nb_task; j++)
            {
                a += diviseurcomm%int(tableau[priotriDM[j]][1]);
            }
            if(a!=0)
            {
                diviseurcomm += 1;
            }
        }

        cout << "Total cycle period: " << diviseurcomm <<endl;
    }


    vector<int> activeDM(diviseurcomm);
    vector<int> activeRM(diviseurcomm);

    for(int i = 0;i < diviseurcomm;i++)
    {
        activeDM[i] = 101;
        activeRM[i] = 101;
    }

    vector<int> dispRM(nb_task);
    vector<int> dispDM(nb_task);

    a = 0;



    for(int i = 0;i < nb_task;i++)
    {
        dispDM[i] = 0;
        dispRM[i] = 0;
    }


    vector<int>copy_capacityDM(nb_task);
    vector<int>copy_capacityRM(nb_task);

    if(DMpossible == 1)
    {
        for(int i = 0;i < nb_task;i++)
        {
            copy_capacityDM[i] = tableau[priotriDM[i]][3];
        }
    }

    if(RMpossible == 1)
    {
        for(int i = 0;i < nb_task;i++)
        {
            copy_capacityRM[i] = tableau[priotriRM[i]][3];
        }
    }

    vector<int>nbperiodRM(nb_task);
    vector<int>nbperiodDM(nb_task);

    for(int i = 0;i < nb_task;i++)
    {
        nbperiodRM[i] = 1;
        nbperiodDM[i] = 1;
    }

    cout << "Creating the schedule loop" << endl;
    for(int j = 0;j < diviseurcomm;j++)
    {
        for(int i = 0;i < nb_task;i++)
        {
            if(DMpossible == 1)
            {
                if(j>=tableau[priotriDM[i]][1]*nbperiodDM[i])
                {
                    // Reset the capacity for tasks whose period has ended
                    tableau[priotriDM[i]][3] = copy_capacityDM[i];
                    nbperiodDM[i] += 1;
                }
            }

            if(RMpossible == 1)
            {
                if(j>=tableau[priotriRM[i]][1]*nbperiodRM[i])
                {
                    cout << j << " is a multiple of " << tableau[priotriRM[i]][1] << ", resetting capacity" << endl;
                    // Reset the capacity for tasks whose period has ended
                    tableau[priotriRM[i]][3] = copy_capacityRM[i];
                    nbperiodRM[i] += 1;
                }
            }
        }

        cout << "Pass 1" << endl;


        for(int i = 0;i < nb_task;i++)
        {
            if(DMpossible == 1)
            {
                if(tableau[priotriDM[i]][0]<=j)
                {
                    dispDM[i] = 1;
                }
            }

            if(RMpossible == 1)
            {
                if(tableau[priotriRM[i]][0]<=j)
                {
                    dispRM[i] = 1;
                }
            }
        }

        cout << "Pass 2" << endl;

        if(DMpossible == 1)
        {
            while((a<nb_task) && (tableau[priotriDM[a]][3]<=0 || dispDM[a]==0))
            {
                a += 1;
            }


            if(a < nb_task)
            {
                activeDM[j] = priotriDM[a];
                tableau[priotriDM[a]][3] -= 1;
            }
            a=0;
        }

        if(RMpossible == 1)
        {
            while((b<nb_task) && (tableau[priotriRM[b]][3]<=0 || dispRM[b]==0))
            {
                b += 1;
            }

            if(b < nb_task)
            {
                activeRM[j] = priotriRM[b];
                tableau[priotriRM[b]][3] -= 1;
            }
            b = 0;
        }

        cout << j << " assigned time" << endl;
    }

    if(DMpossible == 1)
    {
        cout << "--Schedule DM--" << endl;
        for(int i = 0; i < diviseurcomm; i++)
        {
            if(activeDM[i]==101)
            {
                cout << "-     REST     -";
            }
            else
            {
                cout << "- Activity #" << activeDM[i] << " -";
            }
        }
        cout << endl;
    }

    if(RMpossible == 1)
    {
        cout << "--Schedule RM--" << endl;
        for(int i = 0; i < diviseurcomm; i++)
        {
            if(activeRM[i]==101)
            {
                cout << "-   REST   -";
            }
            else
            {
                cout << "- Activity #" << activeRM[i] << " -";
            }
        }
        cout << endl;
    }

    return 0;
}
