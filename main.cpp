#include <simplecpp>
#include "shooter.h"
#include "bubble.h"

/* Simulation Vars */
const double STEP_TIME = 0.020;

/* Game Vars */

const int LEFT_MARGIN = 70;
const int TOP_MARGIN = 20;
const int BOTTOM_MARGIN = (PLAY_Y_HEIGHT+TOP_MARGIN);

double dist(double x1, double y1, double x2, double y2){
    //for collision check
    return sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
}

void move_bullets(vector<Bullet> &bullets){
    // move all bullets
    for(unsigned int i=0; i<bullets.size(); i++){
        if(!bullets[i].nextStep(STEP_TIME)){
            bullets.erase(bullets.begin()+i);
        }
    }
}

void move_bubbles(vector<Bubble> &bubbles){
    // move all bubbles
    for (unsigned int i=0; i < bubbles.size(); i++)
    {
        bubbles[i].nextStep(STEP_TIME);
    }
}
bool collide(Bubble &circle, Bullet &rect){//check if a particular bubble and bullet have collided
    double x1=circle.get_center_x();
    double y1=circle.get_center_y();
    double x2=rect.get_center_x();
    double y2=rect.get_center_y()+(rect.get_height())/2;
    if (dist(x1, y1, x2, y2)<=circle.get_radius()){
        return true;
    }
    return false;
}

void check_collisions(vector<Bubble> &bubbles, vector<Bullet> &bullets){
    //check collisions for all pairs for bubbles and bullets
    for (unsigned int i=0; i<bubbles.size(); i+=1){
        for (unsigned int j=0; j<bullets.size(); j+=1){
            if (collide(bubbles[i], bullets[j])){
                //if collision, both the bubble and bullets get erased
                //new bubbles may be formed depending on order of bubble
                if (bubbles[i].get_order()==1){//order 1, so no split
                    bubbles.erase(bubbles.begin()+i);
                    bullets.erase(bullets.begin()+j);
                }
                else {
                    //order is more, so 2 new bubbles formed
                    bullets.erase(bullets.begin()+j);
                    bubbles[i].split(bubbles);
                    bubbles.erase(bubbles.begin()+i);
                }
            }
        }
    }
}

bool shooter_hit(Bubble &circle, Shooter &shooter){
    //check if shooter hit by a particular bubble
    double x1=shooter.get_head_center_x();
    double y1=shooter.get_head_center_y();
    double x3=shooter.get_body_center_x();
    double y3=shooter.get_body_center_y();
    double x2=circle.get_center_x();
    double y2=circle.get_center_y();
    bool h = (dist(x1, y1, x2, y2)<shooter.get_head_radius()+circle.get_radius()); //bubbles hits head
    bool b = (dist(x3, y3, x2, y2)<circle.get_radius()); //bubbles hits body
    if (h||b) {
        return true;
    }
    return false;
}
bool check_shooter(vector<Bubble> &bubbles, Shooter &shooter){
    //check for all bubbles if they hit the shooter, but only when bubbles are below certain y level
    for (unsigned int i=0; i<bubbles.size(); i+=1){
        if (bubbles[i].get_center_y()>400){
            if (shooter_hit(bubbles[i], shooter)){
                bubbles.erase(bubbles.begin()+i); //bubble erased after it hits shooter
                return true;
            }
        }
    }
    return false;
}
void game_lost(){
    //displayed when game is lost
    Text lose(250, 250, "Game Over. You lost");
    lose.setColor(COLOR("red"));
    Text lose2(250, 270, "Click Anywhere to exit.");
    lose2.setColor(COLOR("red"));
    Rectangle losebox(250, 255, 250, 150);
    losebox.setColor(COLOR("red"));
    for (int i=0; i<15; i+=1){
        losebox.reset(250, 255, 250-4*i, 150-4*i);
        losebox.imprint();
    }

    getClick();
}

vector<Bubble> create_bubbles(int level)
{
    // create initial bubbles in the game
    Color col;
    if (level==1){
        col = COLOR(255, 105, 180);
    }
    else if (level==2){
        col = COLOR(0, 160, 3);
    }
    else if (level==3){
        col = COLOR(71, 148, 255);
    }
    double rad=BUBBLE_DEFAULT_RADIUS;
    if (level>1){
        rad*=(1.3+0.3*(level==3)); //radius of larger bubbles for higher levels
    }
    vector<Bubble> bubbles;
    //order of bubbles is 2 for levels 1 and 2, order is 3 for level 3
    //more initial bubbles for higher rounds
    bubbles.push_back(Bubble(WINDOW_X/2.0, BUBBLE_START_Y, rad, -BUBBLE_DEFAULT_VX, 0, col, 2+(level==3), level));
    bubbles.push_back(Bubble(WINDOW_X/4.0, BUBBLE_START_Y, rad, BUBBLE_DEFAULT_VX, 0, col, 2+(level==3), level));
    if (level>=2){
        bubbles.push_back(Bubble(2.0*WINDOW_X/3.0, BUBBLE_START_Y, rad, -BUBBLE_DEFAULT_VX, 0, col, 2+(level==3), level));
    }
    if (level>=3){
        bubbles.push_back(Bubble(4.0*WINDOW_X/5.0, BUBBLE_START_Y, rad, -BUBBLE_DEFAULT_VX, 0, col, 2+(level==3), level));
    }
    return bubbles;
}
void advance(){
    //displayed when a level is cleared
    Text adv(250, 250, "YAY! You won this round!. You progress to the next!");
    adv.setColor(COLOR(0, 204, 119));
    Text adv2(250, 270, "Click Anywhere to start next round.");
    adv2.setColor(COLOR(0, 204, 119));
    Rectangle advbox(250, 255, 400, 150);
    advbox.setColor(COLOR(0, 204, 119));
    for (int i=0; i<15; i+=1){
        advbox.reset(250, 255, 400-4*i, 150-4*i);
        wait(0.05);
    }

    getClick();
}
void victory(){
    //displayed when game is won, that is all 3 levels cleared
    Text win(250, 250, "YOU WON THIS GAME.");
    win.setColor(COLOR(0, 199, 126));
    Text win2(250, 270, "CONGRATULATIONS!!");
    win2.setColor(COLOR(0, 199, 126));
    Rectangle winbox(250, 255, 250, 150);
    winbox.setColor(COLOR(0, 199, 126));
    for (int i=0; i<15; i+=1){
        winbox.reset(250, 255, 250-4*i, 150-4*i);
        winbox.imprint();
    }
    getClick();

}


bool game(int level){
    //the main game function, takes argument as the level
    //type is bool, returns true if level cleared. Returns false if game lost.
    int health=level; //initial health
    Text healthtext(180, 475, "HEALTH - ");
    string msg_cmd("Cmd: _");
    Text charPressed(LEFT_MARGIN, BOTTOM_MARGIN, msg_cmd);

    // Intialize the shooter
    Shooter shooter(SHOOTER_START_X, SHOOTER_START_Y, SHOOTER_VX);
    // Initialize the bubbles
    vector<Bubble> bubbles = create_bubbles(level);

    // Initialize the bullets (empty)
    vector<Bullet> bullets;

    XEvent event;
    Text h(210, 475, health); //display the health
    // Main game loop
    while (true)
    {
        bool pendingEvent = checkEvent(event);
        if (pendingEvent)
        {
            // Get the key pressed
            char c = charFromEvent(event);
            msg_cmd[msg_cmd.length() - 1] = c;
            charPressed.setMessage(msg_cmd);

            // Update the shooter
            if(c == 'a')
                shooter.move(STEP_TIME, true);
            else if(c == 'd')
                shooter.move(STEP_TIME, false);
            else if(c == 'w')
                bullets.push_back(shooter.shoot());
            else if(c == 'q')
                return 0;
            else if (c=='c'){
                //cheat code
                //allows you to skip levels, mainly for showing and testing features

                return true;
            }
        }

        // Update the bubbles
        move_bubbles(bubbles);

        // Update the bullets
        move_bullets(bullets);
        check_collisions(bubbles, bullets);
        if (check_shooter(bubbles, shooter)){
            //if one of bubbles has hit shooter
            shooter.hurt();
            health-=1;
            h.reset(210, 475, health); //update health displayed
            if (health==0){
                game_lost();
                return false;
            }
        }
        if (bubbles.size()==0){
            //no bubbles left, level cleared
            advance();
            return true;
        }
        int clutter = (bubbles.size()+bullets.size())/4; //no. of objects on screen
        if (clutter<3){
            //only wait if less objects, otherwise game becomes slow due to already high processing time
            wait(STEP_TIME/(clutter+2));
        }


    }
}

int main()
{
    initCanvas("Bubble Trouble", WINDOW_X, WINDOW_Y);

    Line b1(0, PLAY_Y_HEIGHT, WINDOW_X, PLAY_Y_HEIGHT);
    b1.setColor(COLOR(0, 0, 255));

    int level=1;
    //if game() function returns true, level is increased and game function runs again.
    //if game() function returns false, then return 0; is executed, and the game ends.
    if (game(level)){
        level+=1;
        if (game(level)){
            level+=1;
            if (game(level)){
                //final victory, level 3 is won
                victory();
                return 0;
            }

            return 0;
        }

        return 0;
    }

    return 0;

}
