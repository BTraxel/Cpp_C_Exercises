#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <algorithm>

using namespace std;

// Function to simulate a runner running
void run(int teamNumber, int runnerNumber, int timeTaken, vector<int>& teamRuntimes, vector<int>& runnersFinished) {
    cout << "Runner " << runnerNumber << " from Team " << teamNumber << " is running." << endl;
    // Simulate the time taken by the runner
    this_thread::sleep_for(chrono::seconds(timeTaken));
    cout << "Runner " << runnerNumber << " from Team " << teamNumber << " has finished running." << endl;

    // Update the team's total run time
    teamRuntimes[teamNumber - 1] += timeTaken;
    
    // Mark the runner as finished
    runnersFinished[teamNumber - 1]++;

    // Check if all runners of the team have finished running
    if (runnersFinished[teamNumber - 1] == teamRuntimes.size()) {
        cout << "Team " << teamNumber << " has finished running." << endl;
    }
}

int main() {
    int numTeams, runnersPerTeam, bellTime;

    cout << "Enter the number of teams: ";
    cin >> numTeams;

    cout << "Enter the number of runners per team: ";
    cin >> runnersPerTeam;

    // Ask for the bell time (the time when the race starts)
    cout << "Enter the bell time (seconds) to start the race: ";
    cin >> bellTime;

    // Create a vector of threads to represent each team
    vector<thread> teams(numTeams);

    // Create a vector to store the time taken by each runner
    vector<vector<int>> runnerTimes(numTeams, vector<int>(runnersPerTeam));

    // Create a vector to store each team's total run time
    vector<int> teamRuntimes(numTeams, 0);

    // Create a vector to keep track of the number of runners finished for each team
    vector<int> runnersFinished(numTeams, 0);

    // Ask for the time taken by each runner
    for (int teamNumber = 1; teamNumber <= numTeams; ++teamNumber) {
        for (int runnerNumber = 1; runnerNumber <= runnersPerTeam; ++runnerNumber) {
            cout << "Enter the time taken by Runner " << runnerNumber << " from Team " << teamNumber << ": ";
            cin >> runnerTimes[teamNumber - 1][runnerNumber - 1];
        }
    }

    // Start the race after the bell rings
    this_thread::sleep_for(chrono::seconds(bellTime));

    // Simulate the race for each team
    for (int teamNumber = 1; teamNumber <= numTeams; ++teamNumber) {
        teams[teamNumber - 1] = thread([teamNumber, runnersPerTeam, &runnerTimes, &teamRuntimes, &runnersFinished]() {
            for (int runnerNumber = 1; runnerNumber <= runnersPerTeam; ++runnerNumber) {
                run(teamNumber, runnerNumber, runnerTimes[teamNumber - 1][runnerNumber - 1], teamRuntimes, runnersFinished);
            }
        });
    }

    // Wait for all team threads to finish
    for (int teamNumber = 1; teamNumber <= numTeams; ++teamNumber) {
        teams[teamNumber - 1].join();
    }

    // Calculate and display the ranking of teams based on their total run times
    vector<pair<int, int>> ranking; // (team number, total run time)
    for (int i = 0; i < numTeams; ++i) {
        ranking.push_back({i + 1, teamRuntimes[i]});
    }

    // Sort the teams based on run times (lower time is better)
    sort(ranking.begin(), ranking.end(), [](const pair<int, int>& a, const pair<int, int>& b) {
        return a.second < b.second;
    });

    cout << "\nRace Results (Ranking):\n";
    for (int i = 0; i < numTeams; ++i) {
        int teamNumber = ranking[i].first;
        int totalTime = ranking[i].second;
        cout << "Team " << teamNumber << ": Total Run Time = " << totalTime << " seconds (Rank " << (i + 1) << ")\n";
    }

    return 0;
}
