#include <simplecpp>

/* Bubble Vars */
const int BUBBLE_START_X = 250;
const int BUBBLE_START_Y = 50;
const int BUBBLE_DEFAULT_RADIUS = 13;
const int BUBBLE_RADIUS_THRESHOLD = 13;
const int BUBBLE_DEFAULT_VX = 100;
const int BUBBLE_DEFAULT_VY = 0;
const int PLAY_Y_HEIGHT = 450;

class Bubble
{
private:
    Circle circle;  // the circle representing the bubble
    double vx, vy;  // velocity in x and y direction
    Color color;    // color of the bubble
    int order;      //order of bubble ie how many times it can be hit
    int level;      //level of the game
    double gravity; //gravity decided by the game level

public:
    Bubble(double cx, double cy, double r, double vx_=BUBBLE_DEFAULT_VX, double vy_=BUBBLE_DEFAULT_VY, Color color_=COLOR(0, 0, 255), int order_=2, int level_=1)
    {
        // Bubble constructor
        color = color_;
        circle = Circle(cx, cy, r);
        circle.setColor(color);
        circle.setFill(true);
        level=level_;
        vy = vy_;
        order=order_;
        //speed of bubbles is decided by level
        if (level==1){
            vx = vx_;
            gravity=50;

        }
        else if (level==2){
            vx = 1.4*vx_;
            gravity=75;

        }
        else if (level==3){
            vx = 2.0*vx_;
            gravity=100;

        }

    }

    void nextStep(double t)
    {
        // move the bubble
        double new_x = circle.getX() + vx*t;
        double new_y = circle.getY() + vy*t;
        vy+=gravity*t; //increasing vertical velocity by gravity

        if ((vx < 0 && new_x < (0 + circle.getRadius())) // bounce along X direction at left border
            ||
            (vx > 0 && new_x > (WINDOW_X - circle.getRadius()))) // bounce along X direction at right border
        {

            vx = -vx;
            new_x = circle.getX() + vx*t;

        }
        if (vy>0 && new_y+get_radius()>(PLAY_Y_HEIGHT)){ //bouncing from the bottom wall
            vy=-vy;
            new_y=circle.getY()+vy*t;
        }
        if (vy<0 && new_y<20){
            vy=0;
        }

        circle.moveTo(new_x, new_y);
    }

    void split(vector<Bubble> &bubbles){ //split bubble into 2 bubbles moving in opposite directions
        Color col;
        if (order==2){
            col=COLOR(247, 155, 7); //color decided by order of bubbles
        }
        else if (order==3){
            col=COLOR(122, 51, 255);
        }
        Bubble b1(circle.getX(), circle.getY(), 0.7*circle.getRadius(), 100.0, vy, col, order-1, level );
        Bubble b2(circle.getX(), circle.getY(), 0.7*circle.getRadius(), -100.0, vy, col, order-1, level );
        bubbles.push_back(b1); //add these new bubbles to vector bubbles
        bubbles.push_back(b2);
        return;
    }

    double get_radius()
    {
        // return the radius of the bubble
        return circle.getRadius();
    }

    double get_center_x()
    {
        // return the x coordinate of the center of the bubble
        return circle.getX();
    }

    double get_center_y()
    {
        // return the y coordinate of the center of the bubble
        return circle.getY();
    }

    double get_vx()
    {
        // return the x velocity of the bubble
        return vx;
    }
    double get_vy()
    {
        // return the y velocity of the bubble
        return vy;
    }
    int get_order(){
        return order;
    }

    Color get_color()
    {
        // return the color of the bubble
        return color;
    }
};
