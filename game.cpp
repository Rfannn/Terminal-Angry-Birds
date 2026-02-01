#include <iostream>
#include <cstdlib>
#include <cmath>
#include <fstream>
#include <ctime>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <vector>

using namespace std;

const int SCREEN_W = 70; 
const int SCREEN_H = 20; 

int current_level = 1; 
int player_score = 0;
int shots_fired = 0;          
int max_shots_per_level = 25; 
bool game_active = true;
char game_screen[SCREEN_H][SCREEN_W];//game grid

// physics
double proj_x = 0; 
double proj_y = 0; 
double vel_x = 0;
double vel_y = 0; 
const double GRAVITY_FORCE = 0.3;
const double TIME_INCREMENT = 0.2; 
bool projectile_in_air = false;
int current_shot_type = 1;
bool is_explosive_shot = false;
int bounce_counter = 0;

// structure for game objects
struct gameObj {
    int pos_x;
    int pos_y;
    char display;
    bool alive;
};
//game objects
gameObj player_char;
vector<gameObj> enemy_list;
vector<gameObj> wall_list;

int level_stars[3] = {0, 0, 0};


// key press FOR LINUX
bool checkKeyPress() {
    struct termios old_term, new_term;
    int keycode;
    int old_flags;
    
    tcgetattr(STDIN_FILENO, &old_term);
    new_term = old_term;
    
    new_term.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_term);
    
    old_flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, old_flags | O_NONBLOCK);
    
    keycode = getchar();
    
    tcsetattr(STDIN_FILENO, TCSANOW, &old_term);
    fcntl(STDIN_FILENO, F_SETFL, old_flags);
        if(keycode != EOF) {
        ungetc(keycode, stdin);
        return true;
    }
    
    return false;
}

// Clear
void clearConsole() {
    cout << "\033[2J\033[1;1H";
}

// Function for setting up level
void initGameGrid() {
    for(int row = 0; row < SCREEN_H; row++) {
        for(int col = 0; col < SCREEN_W; col++) {
            game_screen[row][col] = ' ';
        }
    }
}

void drawWallStructure(int start_x, int width, int height) {
    for(int h = 0; h < height; h++) {
        for(int w = 0; w < width; w++) {
            wall_list.push_back({start_x + w, SCREEN_H - 1 - h, '#', true});
        }
    }
}

//Level 1 tilted towers
void setupLevel1() {
    cout << "\n=== LEVEL 1: TILTED TOWERS ===\n";
    usleep(700000);
    
    shots_fired = 0;
    enemy_list.clear();
    wall_list.clear();
    projectile_in_air = false;
    initGameGrid();
    player_char = {5, SCREEN_H - 2, 'P', true};
    max_shots_per_level = 25;
    
    for(int h = 0; h < 8; h++) {
        for(int w = 0; w < 5; w++) {
            wall_list.push_back({35 + w, SCREEN_H - 1 - h, '#', true});
        }
    }
    
    for(int h = 0; h < 6; h++) {
        for(int w = 0; w < 4; w++) {
            wall_list.push_back({25 + w, SCREEN_H - 1 - h, '#', true});
        }
    }
    
    for(int h = 0; h < 9; h++) {
        for(int w = 0; w < 3; w++) {
            wall_list.push_back({45 + w, SCREEN_H - 1 - h, '#', true});
        }
    }
    
    enemy_list.push_back({37, SCREEN_H - 9, 'E', true});
    enemy_list.push_back({27, SCREEN_H - 7, 'E', true});
    enemy_list.push_back({46, SCREEN_H - 10, 'E', true}); 

}

//Level 2 bowers castle
void setupLevel2() {
    cout << "\n=== LEVEL 2: BOWERS CASTLE ===\n";
    usleep(700000);
    
    shots_fired = 0;
    enemy_list.clear();
    wall_list.clear();
    projectile_in_air = false;
    
    initGameGrid();
    
    player_char = {5, SCREEN_H - 2, 'P', true};
    max_shots_per_level = 20;
    
    for(int h = 0; h < 7; h++) {

        wall_list.push_back({30, SCREEN_H - 1 - h, '#', true});

        wall_list.push_back({38, SCREEN_H - 1 - h, '#', true});

        if(h == 3 || h == 6) {
            for(int w = 1; w < 8; w++) {
                wall_list.push_back({30 + w, SCREEN_H - 1 - h, '#', true});
            }
        }
    }
    

    
    enemy_list.push_back({34, SCREEN_H - 8, 'E', true});
    enemy_list.push_back({32, SCREEN_H - 5, 'E', true});
    enemy_list.push_back({36, SCREEN_H - 5, 'E', true});
    enemy_list.push_back({33, SCREEN_H - 2, 'E', true}); 
    
    for(int x = 25; x < 30; x++) {
        wall_list.push_back({x, SCREEN_H - 3, '#', true});
    }
    for(int x = 38; x < 43; x++) {
        wall_list.push_back({x, SCREEN_H - 3, '#', true});
    }

    for(int x = 24; x < 44; x += 3) {
        wall_list.push_back({x, SCREEN_H - 1, '_', true});
    }
}

//Level 3 twin towers
void setupLevel3() {
    cout << "\n=== LEVEL 3: TWIN TOWERS ===\n";
    usleep(700000);
    
    shots_fired = 0;
    enemy_list.clear();
    wall_list.clear();
    projectile_in_air = false;
    
    initGameGrid();
    
    player_char = {5, SCREEN_H - 2, 'P', true};
    max_shots_per_level = 15;
     
    for(int h = 0; h < 12; h++) {
        for(int w = 0; w < 4; w++) {
            wall_list.push_back({30 + w, SCREEN_H - 1 - h, '#', true});
        }

    }
    

    for(int h = 0; h < 12; h++) {
        for(int w = 0; w < 4; w++) {
            wall_list.push_back({40 + w, SCREEN_H - 1 - h, '#', true});
        }

        if(h >= 10) {
            wall_list.push_back({42, SCREEN_H - 1 - h, '|', true});
        }
    }
    

    for(int x = 34; x < 40; x++) {
        wall_list.push_back({x, SCREEN_H - 3, '#', true});
        wall_list.push_back({x, SCREEN_H - 4, '#', true});
    }
    

    for(int h = 2; h < 10; h += 2) {
        wall_list.push_back({31, SCREEN_H - 1 - h, '+', true});
        wall_list.push_back({32, SCREEN_H - 1 - h, '+', true});
        wall_list.push_back({41, SCREEN_H - 1 - h, '+', true});
        wall_list.push_back({42, SCREEN_H - 1 - h, '+', true});
    }
    
    enemy_list.push_back({32, SCREEN_H - 6, 'E', true});
    enemy_list.push_back({33, SCREEN_H - 6, 'E', true});
    enemy_list.push_back({42, SCREEN_H - 6, 'E', true});  
    enemy_list.push_back({43, SCREEN_H - 6, 'E', true}); 
    enemy_list.push_back({37, SCREEN_H - 4, 'E', true});
    enemy_list.push_back({32, SCREEN_H - 12, 'E', true});      
    enemy_list.push_back({42, SCREEN_H - 12, 'E', true}); 
    
    for(int x = 28; x < 46; x++) {
        wall_list.push_back({x, SCREEN_H - 1, '_', true});
    }
}


// draw the gaem screen
void drawGameScreen() {
    clearConsole();
    
    //reset grid
    initGameGrid();
    
    //walls
    for(const auto& wall : wall_list) {
        if(wall.alive && wall.pos_y >= 0 && wall.pos_y < SCREEN_H &&
           wall.pos_x >= 0 && wall.pos_x < SCREEN_W) {
            game_screen[wall.pos_y][wall.pos_x] = wall.display;
        }
    }
    
    //enemies
    for(const auto& enemy : enemy_list) {
        if(enemy.alive && enemy.pos_y >= 0 && enemy.pos_y < SCREEN_H &&
           enemy.pos_x >= 0 && enemy.pos_x < SCREEN_W) {
            game_screen[enemy.pos_y][enemy.pos_x] = enemy.display;
        }
    }
    
    //player
    if(player_char.pos_y >= 0 && player_char.pos_y < SCREEN_H &&
       player_char.pos_x >= 0 && player_char.pos_x < SCREEN_W) {
        game_screen[player_char.pos_y][player_char.pos_x] = player_char.display;
    }
    
    //projectile
    if(projectile_in_air) {
        int proj_xi = (int)proj_x;
        int proj_yi = (int)proj_y;
        if(proj_yi >= 0 && proj_yi < SCREEN_H && proj_xi >= 0 && proj_xi < SCREEN_W) {
            game_screen[proj_yi][proj_xi] = '*';
        }
    }
    
    // top border
    cout << " ";
    for(int i = 0; i < SCREEN_W + 2; i++) cout << "=";
    cout << endl;
    
    //game grid
    for(int row = 0; row < SCREEN_H; row++) {
        cout << " |";
        for(int col = 0; col < SCREEN_W; col++) {
            cout << game_screen[row][col];
        }
        cout << "|" << endl;
    }
    
    //bottom border
    cout << " ";
    for(int i = 0; i < SCREEN_W + 2; i++) cout << "=";
    cout << endl;
    
    //status
    cout << " Level: " << current_level << " | Score: " << player_score 
         << " | Shots: " << shots_fired << "/" << max_shots_per_level << endl;
    cout << " Press Q to quit current level\n";
}

// Show main menu
void showMainMenu() {
    clearConsole();
    cout << "\n\n";
    cout << "  ========================================\n";
    cout << "      ANGRY BIRDS - Console Edition\n";
    cout << "             Final Project\n";
    cout << "  ========================================\n\n";
    cout << "  1. Start Game\n";
    cout << "  2. How to Play\n";
    cout << "  3. View Progress & Scores\n";
    cout << "  4. Exit Game\n\n";
    cout << "  Enter choice (1-4): ";
}


//reset
void resetShotState() {
    projectile_in_air = false;
    is_explosive_shot = false;
    bounce_counter = 0;
}

//get input
void getPlayerShot() {
    double angle_deg, power;
    
    cout << "\n--- SHOOTING SETUP ---\n";
    cout << " Select shot type:\n";
    cout << " 1. Normal Shot\n";
    cout << " 2. Explosive Shot\n";
    cout << " 3. Bouncy Shot\n";
    cout << " Choice (1-3): ";
    cin >> current_shot_type;
    
    //input valid
    if(current_shot_type < 1 || current_shot_type > 3) {
        cout << " Invalid! Using normal shot.\n";
        current_shot_type = 1;
    }
    
    cout << " Enter angle (0-90 degrees): ";
    cin >> angle_deg;
    
    //error handling fro degree
    if(angle_deg < 0) angle_deg = 0;
    if(angle_deg > 90) angle_deg = 90;
    
    cout << " Enter power (1-15): ";
    cin >> power;
    
    //error handling for power
    if(power < 1) power = 1;
    if(power > 15) power = 15;
    
    //degree to rad
    double angle_rad = angle_deg * 3.14159 / 180.0;
    
    //x and y speed
    vel_x = power * cos(angle_rad);
    vel_y = -power * sin(angle_rad); 
    
    proj_x = player_char.pos_x + 1;
    proj_y = player_char.pos_y;
    projectile_in_air = true;
    shots_fired++;
    resetShotState();
    projectile_in_air = true;
    
    if(current_shot_type == 2) {
        is_explosive_shot = true;
        cout << " Explosive raedy\n";
    } else if(current_shot_type == 3) {
        bounce_counter = 0;
        cout << " Bouncy ready\n";
    }
    
    usleep(500000);
}
//explosion anim
void makeExplosion(int center_x, int center_y) {
    //3x3
    for(int dy = -2; dy <= 2; dy++) {
        for(int dx = -2; dx <= 2; dx++) {
            if(abs(dx) + abs(dy) <= 3) {
                int ex = center_x + dx;
                int ey = center_y + dy;
                for(auto& enemy : enemy_list) {
                    if(enemy.alive && enemy.pos_x == ex && enemy.pos_y == ey) {
                        enemy.alive = false;
                        player_score += 100;
                    }
                }
                for(auto& wall : wall_list) {
                    if(wall.alive && wall.pos_x == ex && wall.pos_y == ey) {
                        wall.alive = false;
                    }
                }
            }
        }
    }
}

//check if hit
bool checkCollision(int x, int y) {
    for(auto& enemy : enemy_list) {
        if(enemy.alive && enemy.pos_x == x && enemy.pos_y == y) {
            enemy.alive = false;
            player_score += 100;
            
            if(is_explosive_shot) {
                makeExplosion(x, y);
            }
            
            return true;
        }
    }
    
    for(auto& wall : wall_list) {
        if(wall.alive && wall.pos_x == x && wall.pos_y == y) {
            if(is_explosive_shot) {
                makeExplosion(x, y);
                return true;
            }
            
            //check for bouncy shot
            if(current_shot_type == 3 && bounce_counter < 3) {
                vel_x = -vel_x * 0.7;//lose power
                vel_y = -vel_y * 0.7;
                bounce_counter++;
                return false;
            }
            
            return true;
        }
    }
    
    return false;
}

void updateProjectilePhysics() {
    if(!projectile_in_air) return;
    
    double prev_x = proj_x;
    double prev_y = proj_y;
    
    proj_x += vel_x * TIME_INCREMENT;
    proj_y += vel_y * TIME_INCREMENT;
    
    //add gravity
    vel_y += GRAVITY_FORCE;
    
    if(proj_x < 0 || proj_x >= SCREEN_W || proj_y < 0 || proj_y >= SCREEN_H) {
        projectile_in_air = false;
        return;
    }
    
    int curr_xi = (int)proj_x;
    int curr_yi = (int)proj_y;
    
    if(checkCollision(curr_xi, curr_yi)) {
        projectile_in_air = false;
        return;
    }
    int mid_x = (int)((prev_x + proj_x) / 2);
    int mid_y = (int)((prev_y + proj_y) / 2);
    if(checkCollision(mid_x, mid_y)) {
        projectile_in_air = false;
        return;
    }
}

int calculateStarRating(int shots_used, int target_shots) {
    if(shots_used <= target_shots) return 3;      // Excellent
    else if(shots_used <= target_shots + 2) return 2; // Good
    else if(shots_used <= target_shots + 4) return 1; // Okay
    else return 0;                                // Just completed
}


//save progress and points
void saveGameProgress() {
    ofstream save_file("game_save_data.txt");
    if(save_file.is_open()) {
        //
        save_file << current_level << endl;
        save_file << player_score << endl;
        save_file << level_stars[0] << endl;
        save_file << level_stars[1] << endl;
        save_file << level_stars[2] << endl;
        save_file.close();
    }
}

//load progerss
void loadGameProgress() {
    ifstream save_file("game_save_data.txt");
    if(save_file.is_open()) {
        save_file >> current_level;
        save_file >> player_score;
        save_file >> level_stars[0];
        save_file >> level_stars[1];
        save_file >> level_stars[2];
        save_file.close();
    } else {
        current_level = 1;
        player_score = 0;
        level_stars[0] = 0;
        level_stars[1] = 0;
        level_stars[2] = 0;
    }
}

void showInstructions() {
    clearConsole();
    cout << "\n ========== GAME INSTRUCTIONS ==========\n\n";
    cout << " OBJECTIVE:\n";
    cout << " Destroy all enemies (E)\n";
    
    cout << " CONTROLS:\n";
    cout << " 1. Choose shot type (1-3)\n";
    cout << " 2. Enter angle (0-90 degrees)\n";
    cout << " 3. Enter power (1-15)\n";
    cout << " 5. Press Q to quit a level\n\n";
    
    cout << " SHOT TYPES:\n";
    cout << " 1. NORMAL - basic\n";
    cout << " 2. EXPLOSIVE - blows up\n";
    cout << " 3. BOUNCY - bounces 3 times\n\n";
    
    cout << " LEVELS:\n";
    cout << " 1. Tilted Towers\n";
    cout << " 2. Bowers Castle\n";
    cout << " 3. Twin Towers \n\n";
    
    cout << " SCORING:\n";
    cout << " - Each enemy: 100 points\n";
    cout << " - Less shots = more stars (0-3)\n";
    cout << " - Save/Load system tracks progress\n\n";
    
    cout << " Press Enter to return to menu...";
    cin.ignore();
    cin.get();
}

// Show progress screen
void showProgressScreen() {
    clearConsole();
    cout << "\n ========== YOUR PROGRESS ==========\n\n";
    cout << " Total Score: " << player_score << "\n";
    cout << " Current Level: " << current_level << "\n\n";
    
    string level_names[3] = {"Tilted Towers", "Bowers Castle", "Twin Towers"};
    
    for(int i = 0; i < 3; i++) {
        cout << " Level " << (i+1) << " - " << level_names[i] << ": ";
        
        if(level_stars[i] == 3) cout << "★★★ (Perfect!)";
        else if(level_stars[i] == 2) cout << "★★☆ (Good)";
        else if(level_stars[i] == 1) cout << "★☆☆ (Completed)";
        else cout << "Not completed";
        
        cout << endl;
    }

    cout << " Press Enter to continue...";
    cin.ignore();
    cin.get();
}


void playGameLevel(int level_num) {
    current_level = level_num;
    
    if(level_num == 1) setupLevel1();
    else if(level_num == 2) setupLevel2();
    else if(level_num == 3) setupLevel3();
    
    bool level_running = true;
    int target_shots;
    if(level_num == 1) target_shots = 9;
    else if(level_num == 2) target_shots = 7;
    else target_shots = 5;
    
    while(level_running) {
        drawGameScreen();

        bool enemies_remaining = false;
        for(const auto& enemy : enemy_list) {
            if(enemy.alive) {
                enemies_remaining = true;
                break;
            }
        }
        
        if(!enemies_remaining) {
            cout << "\n ╔══════════════════════════╗\n";
            cout << " ║     LEVEL COMPLETE!     ║\n";
            cout << " ╚══════════════════════════╝\n";
            
            int earned_stars = calculateStarRating(shots_fired, target_shots);
            cout << " Shots used: " << shots_fired << endl;
            cout << " Stars earned: " << earned_stars << " ";
            
            for(int i = 0; i < earned_stars; i++) cout << "★";
            for(int i = earned_stars; i < 3; i++) cout << "☆";
            cout << endl;
            
            player_score += earned_stars * 50;
            if(earned_stars > level_stars[level_num-1]) {
                level_stars[level_num-1] = earned_stars;
            }
            
            saveGameProgress(); //autosave
            cout << "\n Press Enter to continue...";
            cin.ignore();
            cin.get();
            level_running = false;
            continue;
        }
        
        if(shots_fired >= max_shots_per_level && !projectile_in_air) {
            cout << "\n ╔══════════════════════════╗\n";
            cout << " ║    OUT OF AMMO!         ║\n";
            cout << " ║    Level Failed         ║\n";
            cout << " ╚══════════════════════════╝\n";
            cout << " Press Enter to retry...";
            cin.ignore();
            cin.get();
            level_running = false;
            continue;
        }
        if(!projectile_in_air) {
            getPlayerShot();
        } else {
            updateProjectilePhysics();
            if(checkKeyPress()) {
                char key = getchar();
                if(key == 'q' || key == 'Q') {
                    level_running = false;
                    cout << " Quitting level...\n";
                    usleep(500000);
                }
            }
        }
        usleep(40000);
    }
}

int main() {
    //random
    srand(time(0));
    loadGameProgress();
    while(game_active) {
        showMainMenu();
        
        char user_choice;
        cin >> user_choice;
        cin.ignore();
        
        switch(user_choice) {
            case '1': {
                cout << "\n Select level to play (1-3):\n";
                cout << " 1. Tilted Towers (Easy)\n";
                cout << " 2. Bowers Castle (Medium)\n";
                cout << " 3. Twin Towers (Hard)\n";
                cout << " Your choice: ";
                
                int selected_level;
                cin >> selected_level;
                cin.ignore();
                
                if(selected_level >= 1 && selected_level <= 3) {
                    playGameLevel(selected_level);
                } else {
                    cout << " Invalid level selection!\n";
                    usleep(1000000);
                }
                break;
            }
            case '2':
                showInstructions();
                break;
            case '3':
                showProgressScreen();
                break;
            case '4':
                game_active = false;
                cout << "\n Bye!\n";
                cout << " Final Score: " << player_score << endl;
                break;
            default:
                cout << " Invalid option. Please choose 1-4.\n";
                usleep(1000000);
        }
    }
    return 0;
}
