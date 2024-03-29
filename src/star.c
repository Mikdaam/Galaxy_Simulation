#include "star.h"

Point *create_point(double x, double y) {
    Point *p = malloc(sizeof(Point));
    p->x = x;
    p->y = y;
    return p;
}

double distance(Point *p1, Point *p2) {
    double dx = p1->x - p2->x;
    double dy = p1->y - p2->y;
    return sqrt(dx*dx + dy*dy);
}

Star *create_star(double x, double y, double vx, double vy, double mass) {
    Star *s = malloc(sizeof(Star));
    s->position = create_point(x, y);
    s->velocity = create_point(vx, vy);
    s->acceleration = create_point(0.0, 0.0);
    s->mass = mass;
    s->radius = DEFAULT_RADIUS;
    s->color = RANDOM_COLOR;
    return s;
}

void draw_star(Star *star, double scale) {
    int window_x = WINDOW_WIDTH * (0.5 + (star->position->x/scale));
    int window_y = WINDOW_HEIGHT * (0.5 + (star->position->y/scale));

    double radius_factor = log10(star->mass) - log10(DEFAULT_MASS);
    double radius = DEFAULT_RADIUS + radius_factor;

    MLV_draw_filled_circle(window_x, window_y, radius, star->color);
}

void init_acceleration(Star **star) {
    (*star)->acceleration->x = 0.0;
    (*star)->acceleration->y = 0.0;
}

void update_star_acceleration(Star **star, Point *other_star_position, double other_star_mass) {
    double distance_between_stars = distance(other_star_position, (*star)->position);

    double dx = other_star_position->x - (*star)->position->x;
    double dy = other_star_position->y - (*star)->position->y;

    (*star)->acceleration->x += (G * other_star_mass / (distance_between_stars * distance_between_stars + soomthing_length * soomthing_length)) * (dx / distance_between_stars);
    (*star)->acceleration->y += (G * other_star_mass / (distance_between_stars * distance_between_stars + soomthing_length * soomthing_length)) * (dy / distance_between_stars);
}

void update_star_velocity(Star **star) {
    (*star)->velocity->x += (*star)->acceleration->x * dt;
    (*star)->velocity->y += (*star)->acceleration->y * dt;
}

void update_star_position(Star **star) {
    (*star)->position->x += (*star)->velocity->x * dt;
    (*star)->position->y += (*star)->velocity->y * dt;
}

void update_star(Star **star, Star *other_star) {
    //update_star_acceleration(star, other_star);
    update_star_velocity(star);
    update_star_position(star);
}

void merge_star(Star **star, Star **other_star) {
    int star_window_x = WINDOW_WIDTH * (0.5 + ((*star)->position->x/2.83800e+06));
    int star_window_y = WINDOW_HEIGHT * (0.5 + ((*star)->position->y/2.83800e+06));

    int other_star_window_x = WINDOW_WIDTH * (0.5 + (((*other_star)->position->x/2.83800e+06)));
    int other_star_window_y = WINDOW_HEIGHT * (0.5 + (((*other_star)->position->y/2.83800e+06)));

    MLV_draw_line(star_window_x, star_window_y, other_star_window_x, other_star_window_y, MLV_COLOR_ROSY_BROWN);

    (*star)->mass += (*other_star)->mass;

    (*other_star)->mass = 0.0;
    //(*other_star)->color = MLV_COLOR_BLACK; /* cheat: to gave the impression that the star is gone */

    (*other_star) = NULL;
}

void free_star(Star *star) {
    free(star->position);
    free(star->velocity);
    free(star->acceleration);
    free(star);
}
