#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

#include "share.h"

#define DELIMITER ","
#define TEAMINFO_AMOUNT_MAX 40

typedef struct _sTeamInfo sTeamInfo;
typedef struct _sGameInfo sGameInfo;

#pragma pack(push)
#pragma pack(1)
struct _sTeamInfo
{
    // Basic Info
    char *teamname;

    // (Home/Away) scores
    uint32_t home_scores;
    uint32_t away_scores;

    // Full Time (Home/Away) team Goals
    uint32_t home_goals;
    uint32_t away_goals;

    // Full Time Result
    uint32_t home_win;
    uint32_t away_win;
    uint32_t home_draw;
    uint32_t away_draw;
    uint32_t home_loss;
    uint32_t away_loss;

    // (Home/Away) team Shoot on Target
    uint32_t home_shoot_on_target;
    uint32_t away_shoot_on_target;

    // (Home/Away) team Shoot
    uint32_t home_shoot;
    uint32_t away_shoot;

    // (Home/Away) team Fouls committed
    uint32_t home_foul;
    uint32_t away_foul;

    // (Home/Away) team Corners
    uint32_t home_corners;
    uint32_t away_corners;

    // (Home/Away) team Yellow cards
    uint32_t home_yellow;
    uint32_t away_yellow;

    // (Home/Away) team Red cards
    uint32_t home_red;
    uint32_t away_red;
};
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
struct _sGameInfo
{
    int i;
};
#pragma pack(pop)


static char *MUST_EXIST_LABEL[] = { "Date", "HomeTeam", "AwayTeam", \
                                     "FTR",     "FTHG",     "FTAG", \
                                                 "HST",      "AST", \
                                                  "HS",       "AS", \
                                                  "HF",       "AF", \
                                                  "HC",       "AC", \
                                                  "HY",       "AY", \
                                                  "HR",       "AR" };
static int32_t MUST_EXIST_LABEL_AMOUNT = sizeof(MUST_EXIST_LABEL) / sizeof(char*);

void menu();

int32_t db_size = 0;
sTeamInfo team_db[TEAMINFO_AMOUNT_MAX] = {0};

void print_team_info(int32_t idx);

static inline void new_TeamInfo(const char *teamname)
{
    if (teamname != NULL)
    {
        team_db[db_size].teamname = strdup(teamname);
        team_db[db_size].home_scores = 0;
        team_db[db_size].away_scores = 0;
        team_db[db_size].home_goals = 0;
        team_db[db_size].away_goals = 0;
        team_db[db_size].home_win = 0;
        team_db[db_size].away_win = 0;
        team_db[db_size].home_draw = 0;
        team_db[db_size].away_draw = 0;
        team_db[db_size].home_shoot_on_target = 0;
        team_db[db_size].away_shoot_on_target = 0;
        team_db[db_size].home_shoot = 0;
        team_db[db_size].away_shoot = 0;
        team_db[db_size].home_foul = 0;
        team_db[db_size].away_foul = 0;
        team_db[db_size].home_corners = 0;
        team_db[db_size].away_corners = 0;
        team_db[db_size].home_yellow = 0;
        team_db[db_size].away_yellow = 0;
        team_db[db_size].home_red = 0;
        team_db[db_size].away_red = 0;
        db_size++;
    }

}

int cmp(const void *pA, const void *pB)
{
    sTeamInfo *teamA = (sTeamInfo*) pA;
    sTeamInfo *teamB = (sTeamInfo*) pB;
    if (teamA->home_scores + teamA->away_scores > teamB->home_scores + teamB->away_scores) return -1;
    else if (teamA->home_scores + teamA->away_scores < teamB->home_scores + teamB->away_scores) return 1;
    else return 0;
}

int main()
{
    char FileName[256] = {0};

    printf("Please open a season record: ");
    fgets(FileName, 256, stdin);
    remove_newline(FileName);

    if (check_file_extension(FileName, "csv") == 0)
    {
        printf("Warning: File format not recognized.\n");
        return 0;
    }

    FILE *fp = NULL;

    if ((fp = fopen(FileName, "r")) == NULL)
    {
        printf("Error: File could not be opened.\n");
        return 0;
    }

    char *line = NULL;  // line buffer
    size_t len = 0;  // buffer size
    ssize_t read;  // line len

    int32_t column_amount = 0;
    char **column_label = NULL;

    // header exist (file is not empty)
    if ((read = getline(&line, &len, fp)) == -1)
    {
        printf("Error: File is empty.\n");
        fclose(fp);
        return 0;
    }
    remove_newline(line);

    // get header (column label)
    char *c = strtok(line, DELIMITER);
    while (c != NULL)
    {
        column_label = realloc(column_label, sizeof(char*) * (column_amount + 1));
        column_label[column_amount++] = strdup(c);
        c = strtok(NULL, DELIMITER);
    }

    int32_t column_label_map[MUST_EXIST_LABEL_AMOUNT];
    memset(column_label_map, -1, sizeof(int32_t) * MUST_EXIST_LABEL_AMOUNT);

    // check header (column label) and storage pair <label, index>
    for (int32_t i = 0; i < MUST_EXIST_LABEL_AMOUNT; ++i)
    {
        int32_t flag = 1;
        for (int32_t j = 0; j < column_amount; ++j)
        {
            if (strcmp(MUST_EXIST_LABEL[i], column_label[j]) == 0)
            {
                column_label_map[i] = j;
                flag = 0;
                break;
            }
        }
        if (flag)
        {
            printf("Error: missing column: %s", MUST_EXIST_LABEL[i]);

            for (int32_t i = 0; i < column_amount; ++i)
            {
                free(column_label[i]);
            }
            free(column_label);

            free(line);
            fclose(fp);

            return 0;
        }
    }

    // read row data
    uint32_t gap = 0;
    char *value[column_amount], *most_scoring_gap_value[column_amount];
    memset(most_scoring_gap_value, 0, sizeof(char*) * column_amount);
    memset(value, 0, sizeof(char*) * column_amount);

    while ((read = getline(&line, &len, fp)) != -1)
    {
        remove_newline(line);

        // decode row data
        int32_t idx = 0;
        char *str = line, *next_str = NULL, *p = NULL;
        do
        {
            next_str = NULL;
            p = strchr(str, ',');
            if (p != NULL && *p != '\0')
            {
                next_str = (p + 1);
                *p = '\0';
            }
            // storage column value
            if (idx < column_amount)
            {
                value[idx] = str;
            }
            str = next_str;
            idx++;
        } while (next_str != NULL);

        // check file format (column amount)
        if (idx != column_amount)
        {
            printf("Error: some column missing in line data\n");

            for (int32_t i = 0; i < column_amount; ++i)
            {
                free(column_label[i]);
            }
            free(column_label);

            free(line);
            fclose(fp);

            return 0;
        }

        // debug owo
        // for (int i = 0; i < MUST_EXIST_LABEL_AMOUNT; ++i)
        // {
        //     if (column_label_map[i] != -1)
        //     {
        //         printf("%s: %s\n", MUST_EXIST_LABEL[i], value[column_label_map[i]]);
        //     }
        // }

        // update team info
        int32_t hometeam_index = -1;
        int32_t awayteam_index = -1;
        for (int32_t i = 0; i < db_size; ++i)
        {
            if (strcmp(team_db[i].teamname, value[column_label_map[1]]) == 0)
            {
                hometeam_index = i;
            }
            if (strcmp(team_db[i].teamname, value[column_label_map[2]]) == 0)
            {
                awayteam_index = i;
            }
        }
        if (hometeam_index == -1)
        {
            new_TeamInfo(value[column_label_map[1]]);
            hometeam_index = db_size-1;
        }
        if (awayteam_index == -1)
        {
            new_TeamInfo(value[column_label_map[2]]);
            awayteam_index = db_size-1;
        }

        if (value[column_label_map[3]][0] == 'H')
        {
            team_db[hometeam_index].home_scores += 3;
            team_db[hometeam_index].home_win += 1;
            team_db[awayteam_index].away_loss += 1;
        }
        else if (value[column_label_map[3]][0] == 'A')
        {
            team_db[awayteam_index].away_scores += 3;
            team_db[hometeam_index].home_loss += 1;
            team_db[awayteam_index].away_win += 1;
        }
        else
        {
            team_db[hometeam_index].home_scores += 1;
            team_db[awayteam_index].away_scores += 1;
            team_db[hometeam_index].home_draw += 1;
            team_db[awayteam_index].away_draw += 1;
        }

        team_db[hometeam_index].home_goals += atoi(value[column_label_map[4]]);
        team_db[awayteam_index].away_goals += atoi(value[column_label_map[5]]);

        if (abs(atoi(value[column_label_map[4]])-atoi(value[column_label_map[5]])) > gap)
        {
            gap = abs(atoi(value[column_label_map[4]])-atoi(value[column_label_map[5]]));
            for (int32_t i = 0; i < column_amount; ++i)
            {
                if (most_scoring_gap_value[i] != NULL) free(most_scoring_gap_value[i]);
                most_scoring_gap_value[i] = strdup(value[i]);
            }
        }

        team_db[hometeam_index].home_shoot_on_target += atoi(value[column_label_map[6]]);
        team_db[awayteam_index].away_shoot_on_target += atoi(value[column_label_map[7]]);

        team_db[hometeam_index].home_shoot += atoi(value[column_label_map[8]]);
        team_db[awayteam_index].away_shoot += atoi(value[column_label_map[9]]);

        team_db[hometeam_index].home_foul += atoi(value[column_label_map[10]]);
        team_db[awayteam_index].away_foul += atoi(value[column_label_map[11]]);

        team_db[hometeam_index].home_corners += atoi(value[column_label_map[12]]);
        team_db[awayteam_index].away_corners += atoi(value[column_label_map[13]]);

        team_db[hometeam_index].home_yellow += atoi(value[column_label_map[14]]);
        team_db[awayteam_index].away_yellow += atoi(value[column_label_map[15]]);

        team_db[hometeam_index].home_red += atoi(value[column_label_map[16]]);
        team_db[awayteam_index].away_red += atoi(value[column_label_map[17]]);
    }

    qsort(team_db, db_size, sizeof(sTeamInfo), cmp);

    menu();

    char Buffer[256] = {0};
    int32_t Choice = 0;

    while (Choice != 8)
    {
        printf("Choice (1-7, 8:exit): ");

        fgets(Buffer, 256, stdin);
        if(sscanf(Buffer, "%d", &Choice) != 1)
        {
            printf("Unrecognized command format.\n");
            continue;
        }
        uint32_t temp, flag;
        switch (Choice)
        {
            case 1:
                temp = 0, flag = 0;
                for (int32_t i = 0; i < db_size; ++i)
                {
                    if ((team_db[i].home_scores + team_db[i].away_scores) > temp)
                    {
                        temp = (team_db[i].home_scores + team_db[i].away_scores);
                    }
                }
                printf("The winner is ");
                for (int32_t i = 0; i < db_size; ++i)
                {
                    if ((team_db[i].home_scores + team_db[i].away_scores) == temp)
                    {
                        if (flag != 0) printf(", ");
                        printf("%s", team_db[i].teamname);
                        flag = 1;
                    }
                }
                printf(".\n");
                break;
            case 2:
                temp = 0, flag = 0;
                for (int32_t i = 0; i < db_size; ++i)
                {
                    if ((team_db[i].home_goals + team_db[i].away_goals) > temp)
                    {
                        temp = (team_db[i].home_goals + team_db[i].away_goals);
                    }
                }
                for (int32_t i = 0; i < db_size; ++i)
                {
                    if ((team_db[i].home_goals + team_db[i].away_goals) == temp)
                    {
                        if (flag != 0) printf(", ");
                        printf("%s", team_db[i].teamname);
                        flag = 1;
                    }
                }
                printf(", %d\n", temp);
                break;
            case 3:
                temp = 0, flag = 0;
                for (int32_t i = 0; i < db_size; ++i)
                {
                    if ((team_db[i].home_red + team_db[i].away_red) > temp)
                    {
                        temp = (team_db[i].home_red + team_db[i].away_red);
                    }
                }
                for (int32_t i = 0; i < db_size; ++i)
                {
                    if ((team_db[i].home_red + team_db[i].away_red) == temp)
                    {
                        if (flag != 0) printf(", ");
                        printf("%s", team_db[i].teamname);
                        flag = 1;
                    }
                }
                printf(", %d\n", temp);
                break;
            case 4:
                temp = 0, flag = 0;
                for (int32_t i = 0; i < db_size; ++i)
                {
                    if (team_db[i].home_win > temp)
                    {
                        temp = team_db[i].home_win;
                    }
                }
                for (int32_t i = 0; i < db_size; ++i)
                {
                    if (team_db[i].home_win == temp)
                    {
                        if (flag != 0) printf(", ");
                        printf("%s", team_db[i].teamname);
                        flag = 1;
                    }
                }
                printf(", %d\n", temp);
                break;
            case 5:
                temp = 0, flag = 0;
                for (int32_t i = 0; i < db_size; ++i)
                {
                    if (team_db[i].away_win > temp)
                    {
                        temp = team_db[i].away_win;
                    }
                }
                for (int32_t i = 0; i < db_size; ++i)
                {
                    if (team_db[i].away_win == temp)
                    {
                        if (flag != 0) printf(", ");
                        printf("%s", team_db[i].teamname);
                        flag = 1;
                    }
                }
                printf(", %d\n", temp);
                break;
            case 6:
                printf("%s, %s(%s) vs %s(%s)\n", most_scoring_gap_value[column_label_map[0]], most_scoring_gap_value[column_label_map[1]], most_scoring_gap_value[column_label_map[4]], most_scoring_gap_value[column_label_map[2]], most_scoring_gap_value[column_label_map[5]]);
                break;
            case 7:
                fgets(Buffer, 256, stdin);
                remove_newline(Buffer);
                flag = 0;
                for (int32_t i = 0; i < db_size; ++i)
                {
                    if (strcmp(team_db[i].teamname, Buffer) == 0)
                    {
                        printf("\n");
                        print_team_info(i);
                        flag = 1;
                        break;
                    }
                }
                if (flag == 0)
                {
                    printf("Error: team no exist.\n");
                }
                break;
            default:
                break;
        }
    }
    printf("Bye.\n");

    for (int32_t i = 0; i < column_amount; ++i)
    {
        free(most_scoring_gap_value[i]);
    }

    for (int32_t i = 0; i < db_size; ++i)
    {
        free(team_db[i].teamname);
    }

    for (int32_t i = 0; i < column_amount; ++i)
    {
        free(column_label[i]);
    }
    free(column_label);


    free(line);
    fclose(fp);

    return 0;
}

void menu()
{
    printf("1) Who is the winner in this season?\n");
    printf("2) Which team gets the most scores?\n");
    printf("3) Which team gets the most red cards?\n");
    printf("4) Which team wins the most games at home?\n");
    printf("5) Which team wins the most games away from home?\n");
    printf("6) Which game has the most scoring gap?\n");
    printf("7) Team information.\n");
    printf("8) Exit\n");
}

static inline float magic(int32_t p, int32_t q)
{
    if (p == 0 || (p == 0 && q == 0)) return 0.0;
    else return ((float) p) * 100.0 / (((float) p) + ((float) q));
}

void print_team_info(int32_t idx)
{
    if (idx < db_size)
    {
        char *str = "nd";
        printf("+---------------------------------------------------------+\n");
        printf("|%*s%s%*s|\n", (57-strlen(team_db[idx].teamname))/2, "", team_db[idx].teamname, (58-strlen(team_db[idx].teamname))/2, "");  // 55
        printf("|-----------------+---------------+---------------+-------|\n");
        printf("|                 |      Home     |      Away     | Total |\n");
        printf("|-----------------+---------------+---------------+-------|\n");
        printf("| goals           | %4d (%5.1f%%) | %4d (%5.1f%%) |  %4d |\n", \
            team_db[idx].home_goals, magic(team_db[idx].home_goals, team_db[idx].away_goals), \
            team_db[idx].away_goals, magic(team_db[idx].away_goals, team_db[idx].home_goals), \
            team_db[idx].home_goals + team_db[idx].away_goals);
        printf("| shoot           | %4d (%5.1f%%) | %4d (%5.1f%%) |  %4d |\n", \
            team_db[idx].home_shoot, magic(team_db[idx].home_shoot, team_db[idx].away_shoot), \
            team_db[idx].away_shoot, magic(team_db[idx].away_shoot, team_db[idx].home_shoot), \
            team_db[idx].home_shoot + team_db[idx].away_shoot);
        printf("| shoot on target | %4d (%5.1f%%) | %4d (%5.1f%%) |  %4d |\n", \
            team_db[idx].home_shoot_on_target, magic(team_db[idx].home_shoot_on_target, team_db[idx].away_shoot_on_target), \
            team_db[idx].away_shoot_on_target, magic(team_db[idx].away_shoot_on_target, team_db[idx].home_shoot_on_target), \
            team_db[idx].home_shoot_on_target + team_db[idx].away_shoot_on_target);
        printf("| foul            | %4d (%5.1f%%) | %4d (%5.1f%%) |  %4d |\n", \
            team_db[idx].home_foul, magic(team_db[idx].home_foul, team_db[idx].away_foul), \
            team_db[idx].away_foul, magic(team_db[idx].away_foul, team_db[idx].home_foul), \
            team_db[idx].home_foul + team_db[idx].away_foul);
        printf("| corners         | %4d (%5.1f%%) | %4d (%5.1f%%) |  %4d |\n", \
            team_db[idx].home_corners, magic(team_db[idx].home_corners, team_db[idx].away_corners), \
            team_db[idx].away_corners, magic(team_db[idx].away_corners, team_db[idx].home_corners), \
            team_db[idx].home_corners + team_db[idx].away_corners);
        printf("| yellow card     | %4d (%5.1f%%) | %4d (%5.1f%%) |  %4d |\n", \
            team_db[idx].home_yellow, magic(team_db[idx].home_yellow, team_db[idx].away_yellow), \
            team_db[idx].away_yellow, magic(team_db[idx].away_yellow, team_db[idx].home_yellow), \
            team_db[idx].home_yellow + team_db[idx].away_yellow);
        printf("| red card        | %4d (%5.1f%%) | %4d (%5.1f%%) |  %4d |\n", \
            team_db[idx].home_red, magic(team_db[idx].home_red, team_db[idx].away_red), \
            team_db[idx].away_red, magic(team_db[idx].away_red, team_db[idx].home_red), \
            team_db[idx].home_red + team_db[idx].away_red);
        printf("|-----------------+---------------+---------------+-------|\n");
        printf("| win             | %4d (%5.1f%%) | %4d (%5.1f%%) |  %4d |\n", \
            team_db[idx].home_win, magic(team_db[idx].home_win, team_db[idx].away_win), \
            team_db[idx].away_win, magic(team_db[idx].away_win, team_db[idx].home_win), \
            team_db[idx].home_win + team_db[idx].away_win);
        printf("| draw            | %4d (%5.1f%%) | %4d (%5.1f%%) |  %4d |\n", \
            team_db[idx].home_draw, magic(team_db[idx].home_draw, team_db[idx].away_draw), \
            team_db[idx].away_draw, magic(team_db[idx].away_draw, team_db[idx].home_draw), \
            team_db[idx].home_draw + team_db[idx].away_draw);
        printf("| loss            | %4d (%5.1f%%) | %4d (%5.1f%%) |  %4d |\n", \
            team_db[idx].home_loss, magic(team_db[idx].home_loss, team_db[idx].away_loss), \
            team_db[idx].away_loss, magic(team_db[idx].away_loss, team_db[idx].home_loss), \
            team_db[idx].home_loss + team_db[idx].away_loss);
        printf("|-----------------+---------------+---------------+-------|\n");
        printf("| scores          | %4d (%5.1f%%) | %4d (%5.1f%%) |  %4d |\n", \
            team_db[idx].home_scores, magic(team_db[idx].home_scores, team_db[idx].away_scores), \
            team_db[idx].away_scores, magic(team_db[idx].away_scores, team_db[idx].home_scores), \
            team_db[idx].home_scores + team_db[idx].away_scores);
        printf("|-----------------+---------------------------------------|\n");
        if (idx == 0)
        {
            printf("| ranking         |                                   1st |\n");
        }
        else if (idx == 1)
        {
            printf("| ranking         |                                   2nd |\n");
        }
        else if (idx == 2)
        {
            printf("| ranking         |                                   3rd |\n");
        }
        else
        {
            printf("| ranking         |                                 %3dth |\n", idx+1);
        }
        printf("+---------------------------------------------------------+\n");
    }
}
