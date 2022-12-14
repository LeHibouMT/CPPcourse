// SDL_Test.h: Includedatei für Include-Standardsystemdateien
// oder projektspezifische Includedateien.

#pragma once

#include "SDL2/include/SDL.h"
#include "SDL2/include/SDL_image.h"
#include <iostream>
#include <map>
#include <memory>
#include <vector>
#include <list>
#include <iterator>
#include <time.h>
#include <ctime>

// Defintions
constexpr double frame_rate = 60.0; // refresh rate
constexpr double frame_time = 1. / frame_rate;
constexpr unsigned frame_width = 600; // Width of window in pixel
constexpr unsigned frame_height = 600; // Height of window in pixel
// Minimal distance of animals to the border
// of the screen
constexpr unsigned frame_boundary = 100;
//std::string surfaceimage = "media/img.png";

// Helper function to initialize SDL
void init();

class animal {
private:
  SDL_Surface* window_surface_ptr_; // ptr to the surface on which we want the
                                    // animal to be drawn, also non-owning
  SDL_Surface* image_ptr_; // The texture of the sheep (the loaded image), use
                           // load_surface_for
  // todo: Attribute(s) to define its position
  unsigned x;  // representing x position
  unsigned y; // representing y position
  unsigned l; // representing length of the animal
  unsigned w; // representing width of the animal
  int ms; // movement speed of the animal
  int direction = rand() % 4; // represent a direction, 0 is north, 1 is east, 2 is south and 3 is west
  bool dead = false; // if the animal is dead, true

public:
  animal(const std::string& file_path, SDL_Surface* window_surface_ptr, int pX, int pY, int pL, int pW, int pMS);
  // todo: The constructor has to load the sdl_surface that corresponds to the
  // texture
  // so we need to load with IMG_Load in the file_path
  ~animal() = default; // todo: Use the destructor to release memory and "clean up
               // behind you"

  void draw(); // todo: Draw the animal on the screen <-> window_surface_ptr.
                 // Note that this function is not virtual, it does not depend
                 // on the static type of the instance

  virtual void move(){}; // todo: Animals move around, but in a different
                             // fashion depending on which type of animal
  // so we won't do move for animal but we will instead do a move for each animal

  int getpositionX();

  int getpositionY();

  int getms();

  int getdirection();

  void setposition(int pX, int pY);

  void changedirection(int d);

  void changems(int ms);

  bool detectanimal(animal* a);

};



// Insert here:
// class sheep, derived from animal
class sheep : public animal {
  // todo
private :
    int ismale = rand() % 2; // represent the sexe of the sheep, 1 is male, 0 is female
    time_t timer = 1000; // represent the cooldown to reproduce
    int step = rand() % 8; // represent the number of steps the sheep will take in the direction
public :
  // Ctor
    sheep(SDL_Surface* window_surface_ptr); // since we will always load the same texture sheep.png, almost same
  // Dtor
    ~sheep() = default;
  // implement functions that are purely virtual in base class
    void move(); // move according to the random step and direction

    time_t gettimer();

    int getismale();

    void changetimer(time_t t);

    void run(std::vector<animal*> v);

    void setstep(int i);

};



class wolf : public animal {
private :
    time_t timer = 0; // represent the timer before dying of starvation
public :
    // Ctor
    wolf(SDL_Surface* window_surface_ptr); // since we will always load the same texture wolf.png, almost same
    // Dtor
      // same destructor, no need for a new one
    // implement functions that are purely virtual in base class
    void findsheep(std::vector<sheep*> v); // give direction to get to the nearest move
    
    void finddog(std::vector<animal*> v); // give direction to stay away from the nearest dog

    void move(); // move in the direction of the nearest sheep

    time_t gettimer();

    void changetimer(time_t t);

};



class shepherd : public animal {
private:
    SDL_Event event; // to detect key pressed
public:
    shepherd(SDL_Surface* window_surface_ptr, SDL_Event event);
    void move();
};



class dog : public animal {
private :
    int relative = -1; // it is a number to represent the relative position of the dog, 0 is north east of the shepherd, 1 is south east and so on
    bool iscommand = false;
public :
    dog(SDL_Surface* window_surface_ptr);

    int getrealrelative(shepherd s);

    void circleshepard(shepherd s); 

    void movenocommand(); // normal move from the dog

    void movecommand(){}; // move from the dog if the player cliked on the screen

    void move();

    void changecommand(bool b);

    bool getiscommand();
};



// The "ground" on which all the animals live (like the std::vector
// in the zoo example).
class ground {
private:
  // Attention, NON-OWNING ptr, again to the screen
  SDL_Surface* window_surface_ptr_;
  int framerate = 60;
  SDL_Event event;
  // Some attribute to store all the wolves and sheep
  // here
  std::vector<sheep*> vsheep;  // we cant convert iterator type so we are using two type of vector
  std::vector<animal*> vanimalsheep;
  std::vector<wolf*> vwolf;
  std::vector<animal*> vanimalwolf;
  std::vector<dog*> vdog;
  std::vector<animal*> vanimaldog;
  shepherd player = shepherd(this->window_surface_ptr_, this->event);

public:
  ground(SDL_Surface* window_surface_ptr); // todo: Ctor
  ~ground(); // todo: Dtor, again for clean up (if necessary)
  // todo: Add an animal
  void add_animal(sheep* s); // for sheep
  void add_animal(wolf* s); // for wolf
  void add_animal(dog* dg);
  void remove_animal(sheep* s);
  void remove_animal(wolf* s);
  void starvewolf(); // wolf dying of starvation
  void eatsheep(); // wolf eating sheep
  void reproducesheep(); // reproducing sheep
  void update(); // todo: "refresh the screen": Move animals and draw them
  // Possibly other methods, depends on your implementation
  int getframerate(); // get framerate
};



// The application class, which is in charge of generating the window
class application {
private:
  // The following are OWNING ptrs
  SDL_Window* window_ptr_;
  SDL_Surface* window_surface_ptr_;
  SDL_Event event;
  // Other attributes here, for example an instance of ground
  ground* grd;

public:
  application(unsigned n_sheep, unsigned n_wolf); // Ctor
  ~application() = default;                                 // dtor

  int loop(unsigned period); // main loop of the application.
                             // this ensures that the screen is updated
                             // at the correct rate.
                             // See SDL_GetTicks() and SDL_Delay() to enforce a
                             // duration the application should terminate after
                             // 'period' seconds
};